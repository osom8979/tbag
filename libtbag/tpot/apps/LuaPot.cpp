/**
 * @file   LuaPot.cpp
 * @brief  LuaPot class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#include <libtbag/tpot/apps/LuaPot.hpp>
#include <libtbag/log/Log.hpp>

#include <iostream>

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
    std::cout << "lua\n";
    return 0;
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

