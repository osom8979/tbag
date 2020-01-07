/**
 * @file   RayTiledMapLua.cpp
 * @brief  RayTiledMapLua class implementation.
 * @author zer0
 * @date   2020-01-07
 */

#include <libtbag/script/lua/RayTiledMapLua.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

static luaL_Reg const __lua_laytiledmap[] = {
        { nullptr, nullptr }
};

bool luaE_open_raytiledmap(lua_State * L)
{
    luaL_register(L, lua_tbag_name(), __lua_laytiledmap);
    {
        // EMPTY.
    }
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

