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
        : path(), checker(), updater()
{
    // EMPTY.
}

RotatePath::RotatePath(std::string const & arguments)
        : RotatePath(createParams(arguments))
{
    // EMPTY.
}

RotatePath::RotatePath(InitParams const & params)
        : path(), checker(params.first), updater(params.second)
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path)
        : path(path), checker(), updater()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path, SharedChecker const & checker, SharedUpdater const & updater)
        : path(path), checker(checker), updater(updater)
{
    // EMPTY.
}

RotatePath::RotatePath(SharedChecker const & checker, SharedUpdater const & updater)
        : path(), checker(checker), updater(updater)
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
            result.first = std::make_shared<SizeChecker>();
        } else {
            result.first = std::make_shared<SizeChecker>(toByteSize(args.get(0)));
        }
    });
    cmd.insert(UPDATER_KEY_TIME, [&](Arguments const & args){
        if (args.empty()) {
            result.second = std::make_shared<TimeFormatUpdater>();
        } else {
            result.second = std::make_shared<TimeFormatUpdater>(args.get(0));
        }
    });
    cmd.insert(UPDATER_KEY_COUNTER, [&](Arguments const & args){
        auto const ARGS_SIZE = args.size();
        if (ARGS_SIZE == 0U) {
            result.second = std::make_shared<CounterUpdater>();
        } else if (ARGS_SIZE == 1U) {
            result.second = std::make_shared<CounterUpdater>(args.get(0));
        } else if (ARGS_SIZE == 2U) {
            result.second = std::make_shared<CounterUpdater>(args.get(0), args.get(1));
        } else {
            unsigned long counter = 0;
            args.opt(2, &counter);
            result.second = std::make_shared<CounterUpdater>(args.get(0), args.get(1), counter);
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
    checker = params.first;
    updater = params.second;
}

void RotatePath::init(std::string const & args)
{
    auto params = createParams(args);
    checker = params.first;
    updater = params.second;
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
    if (testIfWrite(buffer, size)) {
        return update();
    }
    return false;
}

bool RotatePath::next()
{
    if (testIfRead()) {
        return update();
    }
    return false;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

