/**
 * @file   RayLuaCommon.cpp
 * @brief  RayLuaCommon class implementation.
 * @author zer0
 * @date   2019-04-28
 */

#include <libtbag/script/lua/RayLuaCommon.hpp>
#include <iostream>

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

TBAG_CONSTEXPR static char const * const METATABLE_IMAGE = "Image";

static int __lua_ray_Image_new(lua_State * L)
{
    auto * image = (Image*)lua_newuserdata(L, sizeof(Image));
    memset(image, 0x00, sizeof(Image));
    luaL_setmetatable(L, METATABLE_IMAGE);
    return 1;
}

static luaL_Reg const __lua_lay_image[] = {
        { "new", __lua_ray_Image_new },
        { nullptr, nullptr }
};

void lua_ray_register_image(lua_State * L)
{
    std::cout << "lua_ray_register_image(1):\n" << getPrintableStackInformation(L);
    luaL_newmetatable(L, METATABLE_IMAGE);
    std::cout << "lua_ray_register_image(2):\n" << getPrintableStackInformation(L);
    luaL_register(L, lua_ray_name(), __lua_lay_image);
    std::cout << "lua_ray_register_image(3):\n" << getPrintableStackInformation(L);
    lua_setfield(L, -1, "__index");
    std::cout << "lua_ray_register_image(4):\n" << getPrintableStackInformation(L);
}

bool luaopen_ray_common(lua_State * L)
{
    lua_ray_register_image(L);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

