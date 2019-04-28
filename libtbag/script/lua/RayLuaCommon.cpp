/**
 * @file   RayLuaCommon.cpp
 * @brief  RayLuaCommon class implementation.
 * @author zer0
 * @date   2019-04-28
 */

#include <libtbag/script/lua/RayLuaCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

void lua_ray_pushvector2(lua_State * L, Vector2 const & vec)
{
    lua_createtable(L, 0, 2);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
}

void lua_ray_pushvector3(lua_State * L, Vector3 const & vec)
{
    lua_createtable(L, 0, 3);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, vec.z);
    lua_setfield(L, -2, "z");
}

void lua_ray_pushvector4(lua_State * L, Vector4 const & vec)
{
    lua_createtable(L, 0, 4);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, vec.z);
    lua_setfield(L, -2, "z");
    lua_pushnumber(L, vec.w);
    lua_setfield(L, -2, "w");
}

void lua_ray_pushquaternion(lua_State * L, Quaternion const & vec)
{
    lua_createtable(L, 0, 4);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, vec.z);
    lua_setfield(L, -2, "z");
    lua_pushnumber(L, vec.w);
    lua_setfield(L, -2, "w");
}

void lua_ray_pushmatrix(lua_State * L, Matrix const & mat)
{
    lua_createtable(L, 16, 0);

    lua_pushnumber(L, mat.m0);
    lua_rawseti(L, -2, 1);
    lua_pushnumber(L, mat.m4);
    lua_rawseti(L, -2, 2);
    lua_pushnumber(L, mat.m8);
    lua_rawseti(L, -2, 3);
    lua_pushnumber(L, mat.m12);
    lua_rawseti(L, -2, 4);

    lua_pushnumber(L, mat.m1);
    lua_rawseti(L, -2, 5);
    lua_pushnumber(L, mat.m5);
    lua_rawseti(L, -2, 6);
    lua_pushnumber(L, mat.m9);
    lua_rawseti(L, -2, 7);
    lua_pushnumber(L, mat.m13);
    lua_rawseti(L, -2, 8);

    lua_pushnumber(L, mat.m2);
    lua_rawseti(L, -2, 9);
    lua_pushnumber(L, mat.m6);
    lua_rawseti(L, -2, 10);
    lua_pushnumber(L, mat.m10);
    lua_rawseti(L, -2, 11);
    lua_pushnumber(L, mat.m14);
    lua_rawseti(L, -2, 12);

    lua_pushnumber(L, mat.m3);
    lua_rawseti(L, -2, 13);
    lua_pushnumber(L, mat.m7);
    lua_rawseti(L, -2, 14);
    lua_pushnumber(L, mat.m11);
    lua_rawseti(L, -2, 15);
    lua_pushnumber(L, mat.m15);
    lua_rawseti(L, -2, 16);
}

Color lua_ray_getcolor(lua_State * L, int num_arg)
{
    Color result = {0,};
    lua_getfield(L, num_arg, "r");
    result.r = luaL_checkinteger(L, -1);
    lua_getfield(L, num_arg, "g");
    result.g = luaL_checkinteger(L, -1);
    lua_getfield(L, num_arg, "b");
    result.b = luaL_checkinteger(L, -1);
    lua_getfield(L, num_arg, "a");
    result.a = luaL_checkinteger(L, -1);
    lua_pop(L, 4);
    return result;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

