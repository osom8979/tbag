/**
 * @file   RayLua.cpp
 * @brief  RayLua class implementation.
 * @author zer0
 * @date   2019-04-27
 */

#include <libtbag/script/lua/RayLua.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

void lua_ray_pushvector2(lua_State * lua, Vector2 const & vec)
{
    lua_createtable(lua, 0, 2);
    lua_pushnumber(lua, vec.x);
    lua_setfield(lua, -2, "x");
    lua_pushnumber(lua, vec.y);
    lua_setfield(lua, -2, "y");
}

void lua_ray_pushvector3(lua_State * lua, Vector3 const & vec)
{
    lua_createtable(lua, 0, 3);
    lua_pushnumber(lua, vec.x);
    lua_setfield(lua, -2, "x");
    lua_pushnumber(lua, vec.y);
    lua_setfield(lua, -2, "y");
    lua_pushnumber(lua, vec.z);
    lua_setfield(lua, -2, "z");
}

void lua_ray_pushvector4(lua_State * lua, Vector4 const & vec)
{
    lua_createtable(lua, 0, 4);
    lua_pushnumber(lua, vec.x);
    lua_setfield(lua, -2, "x");
    lua_pushnumber(lua, vec.y);
    lua_setfield(lua, -2, "y");
    lua_pushnumber(lua, vec.z);
    lua_setfield(lua, -2, "z");
    lua_pushnumber(lua, vec.w);
    lua_setfield(lua, -2, "w");
}

void lua_ray_pushquaternion(lua_State * lua, Quaternion const & vec)
{
    lua_createtable(lua, 0, 4);
    lua_pushnumber(lua, vec.x);
    lua_setfield(lua, -2, "x");
    lua_pushnumber(lua, vec.y);
    lua_setfield(lua, -2, "y");
    lua_pushnumber(lua, vec.z);
    lua_setfield(lua, -2, "z");
    lua_pushnumber(lua, vec.w);
    lua_setfield(lua, -2, "w");
}

void lua_ray_pushmatrix(lua_State * lua, Matrix const & mat)
{
    lua_createtable(lua, 16, 0);

    lua_pushnumber(lua, mat.m0);
    lua_rawseti(lua, -2, 1);
    lua_pushnumber(lua, mat.m4);
    lua_rawseti(lua, -2, 2);
    lua_pushnumber(lua, mat.m8);
    lua_rawseti(lua, -2, 3);
    lua_pushnumber(lua, mat.m12);
    lua_rawseti(lua, -2, 4);

    lua_pushnumber(lua, mat.m1);
    lua_rawseti(lua, -2, 5);
    lua_pushnumber(lua, mat.m5);
    lua_rawseti(lua, -2, 6);
    lua_pushnumber(lua, mat.m9);
    lua_rawseti(lua, -2, 7);
    lua_pushnumber(lua, mat.m13);
    lua_rawseti(lua, -2, 8);

    lua_pushnumber(lua, mat.m2);
    lua_rawseti(lua, -2, 9);
    lua_pushnumber(lua, mat.m6);
    lua_rawseti(lua, -2, 10);
    lua_pushnumber(lua, mat.m10);
    lua_rawseti(lua, -2, 11);
    lua_pushnumber(lua, mat.m14);
    lua_rawseti(lua, -2, 12);

    lua_pushnumber(lua, mat.m3);
    lua_rawseti(lua, -2, 13);
    lua_pushnumber(lua, mat.m7);
    lua_rawseti(lua, -2, 14);
    lua_pushnumber(lua, mat.m11);
    lua_rawseti(lua, -2, 15);
    lua_pushnumber(lua, mat.m15);
    lua_rawseti(lua, -2, 16);
}

bool bindRayLua(lua_State * lua)
{
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

