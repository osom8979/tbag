/**
 * @file   RotatePath.cpp
 * @brief  RotatePath class implementation.
 * @author zer0
 * @date   2017-07-31
 */

#include <libtbag/filesystem/RotatePath.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Commander.hpp>

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
    if (cleaner) {
        cleaner->clean(path);
    }
    writer.reset();
    cleaner.reset();
    updater.reset();
}

RotatePath::InitParams RotatePath::createParams(std::string const & arguments, Environments const & envs)
{
    InitParams result;
    using namespace libtbag::string;
    auto cmd = Commander("", Commander::DEFAULT_DELIMITER);
    cmd.insert(WRITER_KEY_SIZE, [&](Arguments const & args){
        if (args.empty()) {
            result.writer = std::make_shared<MaxSizeWriter>();
        } else {
            result.writer = std::make_shared<MaxSizeWriter>(toByteSize(args.get(0)));
        }
    });
    cmd.insert(UPDATER_KEY_TIME, [&](Arguments const & args){
        if (args.empty()) {
            result.updater = std::make_shared<TimeFormatUpdater>();
        } else {
            result.updater = std::make_shared<TimeFormatUpdater>(args.get(0));
        }
    });
    cmd.insert(UPDATER_KEY_COUNTER, [&](Arguments const & args){
        auto const ARGS_SIZE = args.size();
        if (ARGS_SIZE == 0U) {
            result.updater = std::make_shared<CounterUpdater>();
        } else if (ARGS_SIZE == 1U) {
            result.updater = std::make_shared<CounterUpdater>(args.get(0));
        } else if (ARGS_SIZE == 2U) {
            result.updater = std::make_shared<CounterUpdater>(args.get(0), args.get(1));
        } else {
            unsigned long counter = 0;
            args.opt(2, &counter);
            result.updater = std::make_shared<CounterUpdater>(args.get(0), args.get(1), counter);
        }
    });
    cmd.insert(CLEANER_KEY_ARCHIVE, [&](Arguments const & args){
        auto const ARGS_SIZE = args.size();
        if (ARGS_SIZE == 0U) {
            result.cleaner = std::make_shared<ArchiveCleaner>();
        } else if (ARGS_SIZE == 1U) {
            result.cleaner = std::make_shared<ArchiveCleaner>(args.get(0));
        } else {
            bool remove = true;
            args.opt(1, &remove);
            result.cleaner = std::make_shared<ArchiveCleaner>(args.get(0), remove);
        }
    });
    cmd.request(envs.convert(arguments));
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
            return E_EINTERNAL;
        }
    }

    assert(!path.empty());
    if (!writer->ready()) {
        if (!writer->open(path.c_str())) {
            return E_EOPEN;
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
            return E_EOPEN;
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

