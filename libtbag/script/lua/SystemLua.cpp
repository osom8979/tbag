/**
 * @file   SystemLua.cpp
 * @brief  SystemLua class implementation.
 * @author zer0
 * @date   2019-10-20
 */

#include <libtbag/script/lua/SystemLua.hpp>
#include <libtbag/system/ResourceUsage.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

static int _getMemoryUsage(lua_State * L)
{
    lua_pushinteger(L, libtbag::system::getMemoryUsage(luaL_checkinteger(L, 1)));
    return 1;
}

#ifndef SYSTEM_REGISTER
#define SYSTEM_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_system[] = {
        SYSTEM_REGISTER(getMemoryUsage),
        { nullptr, nullptr }
};

bool luaE_open_system(lua_State * L)
{
    luaL_register(L, lua_tbag_name(), __lua_system);
    {
    }
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

