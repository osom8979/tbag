/**
 * @file   RotatePath.cpp
 * @brief  RotatePath class implementation.
 * @author zer0
 * @date   2017-07-31
 */

#include <libtbag/filesystem/RotatePath.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Arguments.hpp>
#include <libtbag/util/ByteString.hpp>

#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

RotatePath::RotatePath()
        : path(), writer(), updater(), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(std::string const & arguments)
        : RotatePath(createParams(arguments))
{
    // EMPTY.
}

RotatePath::RotatePath(InitParams const & params)
        : path(), writer(params.writer), updater(params.updater), cleaner(params.cleaner)
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p, InitParams const & params)
        : path(p), writer(params.writer), updater(params.updater), cleaner(params.cleaner)
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p)
        : path(p), writer(), updater(), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p, SharedWriter const & w, SharedUpdater const & u)
        : path(p), writer(w), updater(u), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(SharedWriter const & w, SharedUpdater const & u)
        : path(), writer(w), updater(u), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p, SharedWriter const & w, SharedUpdater const & u, SharedCleaner const & c)
        : path(p), writer(w), updater(u), cleaner(c)
{
    // EMPTY.
}

RotatePath::RotatePath(SharedWriter const & w, SharedUpdater const & u, SharedCleaner const & c)
        : path(), writer(w), updater(u), cleaner(c)
{
    // EMPTY.
}

RotatePath::~RotatePath()
{
    if (writer) {
        if (writer->ready()) {
            writer->flush();
            writer->close();
        }
    }
    if (cleaner && !path.empty()) {
        cleaner->clean(path);
    }
    writer.reset();
    cleaner.reset();
    updater.reset();
}

RotatePath::InitParams RotatePath::createParams(std::string const & arguments, Environments const & envs)
{
    TBAG_CONSTEXPR static char const * const DELIMITER = " ";
    TBAG_CONSTEXPR static char const * const KEY_VAL_DELIMITER = "=";

    InitParams result;

    using namespace libtbag::string;
    for (auto const & token : splitTokens(envs.convert(arguments), DELIMITER)) {
        auto const key_val = splitTokens(trim(token), KEY_VAL_DELIMITER);
        if (key_val.empty()) {
            continue;
        }

        Arguments args;
        if (key_val.size() >= 2u) {
            args.parse(key_val[1]);
        }

        auto const command = trim(key_val[0]);
        if (command == WRITER_KEY_SIZE) {
            if (args.empty()) {
                result.writer = std::make_shared<MaxSizeWriter>(0);
            } else {
                auto const byte_size_result = libtbag::util::parseByteSize(args.at(0));
                if (byte_size_result) {
                    result.writer = std::make_shared<MaxSizeWriter>(byte_size_result.value);
                } else {
                    result.writer = std::make_shared<MaxSizeWriter>(0);
                }
            }

        } else if (command == UPDATER_KEY_TIME) {
            if (args.empty()) {
                result.updater = std::make_shared<TimeFormatUpdater>();
            } else {
                result.updater = std::make_shared<TimeFormatUpdater>(args.at(0));
            }

        } else if (command == UPDATER_KEY_COUNTER) {
            auto const ARGS_SIZE = args.size();
            if (ARGS_SIZE == 0U) {
                result.updater = std::make_shared<CounterUpdater>();
            } else if (ARGS_SIZE == 1U) {
                result.updater = std::make_shared<CounterUpdater>(args.at(0));
            } else if (ARGS_SIZE == 2U) {
                result.updater = std::make_shared<CounterUpdater>(args.at(0), args.at(1));
            } else {
                unsigned long counter = 0;
                args.opt(2, &counter);
                result.updater = std::make_shared<CounterUpdater>(args.at(0), args.at(1), counter);
            }

        } else if (command == CLEANER_KEY_ARCHIVE) {
            auto const ARGS_SIZE = args.size();
            if (ARGS_SIZE == 0U) {
                result.cleaner = std::make_shared<ArchiveCleaner>();
            } else if (ARGS_SIZE == 1U) {
                result.cleaner = std::make_shared<ArchiveCleaner>(args.at(0));
            } else {
                bool remove = true;
                args.opt(1, &remove);
                result.cleaner = std::make_shared<ArchiveCleaner>(args.at(0), remove);
            }
        }
    }
    return result;
}

RotatePath::InitParams RotatePath::createParams(std::string const & arguments)
{
    return createParams(arguments, Environments::createDefaultEnvironments());
}

void RotatePath::init(std::string const & args, Environments const & envs)
{
    auto params = createParams(args, envs);
    writer = params.writer;
    updater = params.updater;
    cleaner = params.cleaner;
}

void RotatePath::init(std::string const & args)
{
    auto params = createParams(args);
    writer = params.writer;
    updater = params.updater;
    cleaner = params.cleaner;
}

void RotatePath::update()
{
    if (updater) {
        path = updater->update(path);
    }
}

void RotatePath::flush()
{
    if (writer) {
        writer->flush();
    }
}

Err RotatePath::write(char const * buffer, std::size_t size)
{
    if (!isReady()) {
        return E_ALREADY;
    }

    assert(writer);
    assert(updater);

    if (path.empty()) {
        path = updater->update(path);
        if (path.empty()) {
            return E_INTERNAL;
        }
    }

    assert(!path.empty());
    if (!writer->ready()) {
        if (!writer->open(path.c_str())) {
            return E_OPEN;
        }
    }

    Err code;
    int written;

    assert(writer->ready());
    while (true) {
        code = writer->write(buffer, static_cast<int>(size), &written);
        if (code == E_SMALLBUF) {
            // SKIP.
        } else if (code == E_CONTINUE) {
            // SKIP.
        } else {
            return code;
        }

        writer->flush();
        writer->close();

        if (cleaner) {
            cleaner->clean(path);
        }

        path = updater->update(path);
        assert(!path.empty());

        if (!writer->open(path.c_str())) {
            return E_OPEN;
        }

        if (code == E_CONTINUE) {
            assert(size - written == 0);
            return E_SUCCESS;
        } else {
            buffer += written;
            size -= written;
            assert(size >= 1);
        }
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return E_UNKNOWN;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

