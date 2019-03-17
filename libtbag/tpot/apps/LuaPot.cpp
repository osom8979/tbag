/**
 * @file   LuaPot.cpp
 * @brief  LuaPot class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#include <libtbag/tpot/apps/LuaPot.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <iostream>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

LuaPot::LuaPot()
{
    // EMPTY.
}

LuaPot::~LuaPot()
{
    // EMPTY.
}

char const * const LuaPot::getName() const TBAG_NOEXCEPT
{
    return "lua";
}

char const * const LuaPot::getRemarks() const TBAG_NOEXCEPT
{
    return "Lua Interpreter";
}

char const * const LuaPot::getHelp() const TBAG_NOEXCEPT
{
    return "usage: %s lua [options]... [script [args]...].\n"
           "Available options are:\n"
           "  -e chunk  Execute string 'chunk' .\n"
           "  -l name   Require library 'name' .\n"
           "  -b ...    Save or list bytecode.\n"
           "  -j cmd    Perform LuaJIT control command.\n"
           "  -O[opt]   Control LuaJIT optimizations.\n"
           "  -i        Enter interactive mode after executing 'script'.\n"
           "  -v        Show version information.\n"
           "  -E        Ignore environment variables.\n"
           "  --        Stop handling options.\n"
           "  -         Execute stdin and stop handling options.\n";
}

int LuaPot::onMain()
{
    auto arg0 = libtbag::filesystem::Path::getExePath().toString();
    assert(!arg0.empty());

    auto original_args = args();
    if (!original_args.empty() && original_args[0] == getName()) {
        // If the 'lua' mode name argument is exists, erase it.
        original_args.erase(original_args.begin());
    }
    auto const ARGS_SIZE = original_args.size();

    std::vector<char*> argv;
    argv.push_back(&(arg0[0]));
    for (std::size_t i = 0; i < ARGS_SIZE; ++i) {
        argv.push_back(&(original_args[i][0]));
    }
    argv.push_back(nullptr);
    assert(!argv.empty());

    tDLogIfD(params().verbose && !original_args.empty(), "LuaPot::onMain() Passed arguments: {}",
             libtbag::string::mergeTokens(original_args));
    return libtbag::script::luac(static_cast<int>(argv.size()), argv.data());
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

