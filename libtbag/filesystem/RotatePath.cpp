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
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

RotatePath::RotatePath()
        : path(), checker(), updater(), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(std::string const & arguments)
        : RotatePath(createParams(arguments))
{
    // EMPTY.
}

RotatePath::RotatePath(InitParams const & params)
        : path(), checker(params.checker), updater(params.updater), cleaner(params.cleaner)
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p, InitParams const & params)
        : path(p), checker(params.checker), updater(params.updater), cleaner(params.cleaner)
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p)
        : path(p), checker(), updater(), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p, SharedChecker const & k, SharedUpdater const & u)
        : path(p), checker(k), updater(u), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(SharedChecker const & k, SharedUpdater const & u)
        : path(), checker(k), updater(u), cleaner()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & p, SharedChecker const & k, SharedUpdater const & u, SharedCleaner const & c)
        : path(p), checker(k), updater(u), cleaner(c)
{
    // EMPTY.
}

RotatePath::RotatePath(SharedChecker const & k, SharedUpdater const & u, SharedCleaner const & c)
        : path(), checker(k), updater(u), cleaner(c)
{
    // EMPTY.
}

RotatePath::InitParams RotatePath::createParams(std::string const & arguments, Environments const & envs)
{
    InitParams result;
    using namespace libtbag::string;
    auto cmd = Commander("", Commander::DEFAULT_DELIMITER);
    cmd.insert(CHECKER_KEY_SIZE, [&](Arguments const & args){
        if (args.empty()) {
            result.checker = std::make_shared<SizeChecker>();
        } else {
            result.checker = std::make_shared<SizeChecker>(toByteSize(args.get(0)));
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
    checker = params.checker;
    updater = params.updater;
    cleaner = params.cleaner;
}

void RotatePath::init(std::string const & args)
{
    auto params = createParams(args);
    checker = params.checker;
    updater = params.updater;
    cleaner = params.cleaner;
}

bool RotatePath::update()
{
    if (!updater) {
        return false;
    }

    Path next = updater->update(path);
    if (!next.empty()) {
        path.swap(next);
        return true;
    }
    return false;
}

bool RotatePath::testIfRead(Path const & prev) const
{
    return checker && checker->test(prev, nullptr, 0);
}

bool RotatePath::testIfRead() const
{
    return testIfRead(path);
}

bool RotatePath::testIfWrite(Path const & prev, char const * buffer, std::size_t size) const
{
    return checker && checker->test(prev, buffer, size);
}

bool RotatePath::testIfWrite(char const * buffer, std::size_t size) const
{
    return testIfWrite(path, buffer, size);
}

bool RotatePath::next(char const * buffer, std::size_t size)
{
    if (!testIfWrite(buffer, size)) {
        return false;
    }
    if (!cleaner) {
        return update();
    }
    Path const PREV_PATH = path;
    return update() && cleaner->clean(PREV_PATH);
}

bool RotatePath::next()
{
    if (!testIfRead()) {
        return false;
    }
    if (!cleaner) {
        return update();
    }
    Path const PREV_PATH = path;
    return update() && cleaner->clean(PREV_PATH);
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

