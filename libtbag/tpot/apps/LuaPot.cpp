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

int LuaPot::onMain()
{
    auto arg0 = libtbag::filesystem::Path::getExePath().toString();
    auto original_args = args();
    auto const ORIGINAL_ARGS_SIZE = original_args.size();

    assert(!arg0.empty());
    assert(ORIGINAL_ARGS_SIZE >= 1);

    std::vector<char*> argv;
    argv.push_back(&(arg0[0]));
    for (std::size_t i = 1; i < ORIGINAL_ARGS_SIZE; ++i) {
        argv.push_back(&(original_args[i][0]));
    }
    argv.push_back(nullptr);
    assert(argv.size() >= 2);

    return libtbag::script::luac(static_cast<int>(argv.size()), argv.data());
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

