/**
 * @file   MainArguments.cpp
 * @brief  MainArguments class implementation.
 * @author zer0
 * @date   2019-05-22
 */

#include <libtbag/string/MainArguments.hpp>
#include <cassert>
#include <cassert>
#include <cassert>
#include <cassert>
#include <cassert>
#include <cassert>
#include <cassert>
#include <cassert>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

MainArguments::MainArguments()
{
    // EMPTY.
}

MainArguments::MainArguments(std::string const & program)
{
    setArgs(program);
    updateCache();
}

MainArguments::MainArguments(std::vector<std::string> const & args)
{
    setArgs(args);
    updateCache();
}

MainArguments::MainArguments(std::string const & program,
                             std::vector<std::string> const & args)
{
    setArgs(program, args);
    updateCache();
}

MainArguments::MainArguments(std::vector<std::string> const & args,
                             std::vector<std::string> const & envs)
{
    setArgs(args);
    setEnvs(envs);
    updateCache();
}

MainArguments::MainArguments(std::string const & program,
                             std::vector<std::string> const & args,
                             std::vector<std::string> const & envs)
{
    setArgs(program, args);
    setEnvs(envs);
    updateCache();
}

MainArguments::~MainArguments()
{
    // EMPTY.
}

void MainArguments::setArgs(std::string const & program)
{
    _args.clear();
    _args.push_back(program);
}

void MainArguments::setArgs(std::vector<std::string> const & args)
{
    _args = args;
}

void MainArguments::setArgs(std::string const & program, std::vector<std::string> const & args)
{
    _args.clear();
    _args.push_back(program);
    _args.insert(_args.end(), args.begin(), args.end());
}

void MainArguments::setEnvs(std::vector<std::string> const & args)
{
    _envs = args;
}

void MainArguments::updateCache()
{
    _cache.args_ptrs.clear();
    for (auto & arg : _args) {
        _cache.args_ptrs.push_back(&(arg[0]));
    }
    _cache.args_ptrs.push_back(nullptr);
    assert(!_cache.args_ptrs.empty());

    _cache.envs_ptrs.clear();
    for (auto & arg : _envs) {
        _cache.envs_ptrs.push_back(&(arg[0]));
    }
    _cache.envs_ptrs.push_back(nullptr);
    assert(!_cache.envs_ptrs.empty());
}

int MainArguments::argc() const
{
    auto const c = static_cast<int>(_cache.args_ptrs.size());
    return c >= 1 ? c - 1 : 0;
}

char ** MainArguments::argv()
{
    return (char**)_cache.args_ptrs.data();
}

int MainArguments::envc() const
{
    auto const c = static_cast<int>(_cache.envs_ptrs.size());
    return c >= 1 ? c - 1 : 0;
}

char ** MainArguments::envs()
{
    return (char**)_cache.envs_ptrs.data();
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

