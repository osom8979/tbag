/**
 * @file   RayLua.cpp
 * @brief  RayLua class implementation.
 * @author zer0
 * @date   2019-04-27
 */

#include <libtbag/script/lua/RayLua.hpp>

#include <cstring>
#include <cstdlib>
#include <cassert>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray;

# /***********/
# /* Vector2 */
# /***********/

void luaE_pushvector2(lua_State * L, Vector2 const & vec)
{
    lua_createtable(L, 0, 2);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
}

Vector2 luaE_checkvector2(lua_State * L, int num_arg)
{
    Vector2 result = {0,};
    if (lua_objlen(L, num_arg) >= 2) {
        lua_rawgeti(L, num_arg, 1);
        result.x = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 2);
        result.y = luaL_checknumber(L, -1);
        lua_pop(L, 2);
    } else {
        lua_getfield(L, num_arg, "x");
        result.x = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "y");
        result.y = luaL_checknumber(L, -1);
        lua_pop(L, 2);
    }
    return result;
}

Vector2 luaE_optvector2(lua_State * L, int num_arg, Vector2 const & def)
{
    if (lua_istable(L, num_arg)) {
        return luaE_checkvector2(L, num_arg);
    }
    return def;
}

std::vector<Vector2> luaE_checkvector2_array(lua_State * L, int num_arg)
{
    auto const length = lua_objlen(L, num_arg);
    std::vector<Vector2> result(length);
    for (auto i = 0; i < length; ++i) {
        lua_rawgeti(L, num_arg, 1);
        result[i] = luaE_checkvector2(L, lua_absindex(L, -1));
    }
    lua_pop(L, length);
    return result;
}

# /***********/
# /* Vector3 */
# /***********/

void luaE_pushvector3(lua_State * L, Vector3 const & vec)
{
    lua_createtable(L, 0, 3);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, vec.z);
    lua_setfield(L, -2, "z");
}

Vector3 luaE_checkvector3(lua_State * L, int num_arg)
{
    Vector3 result = {0,};
    if (lua_objlen(L, num_arg) >= 3) {
        lua_rawgeti(L, num_arg, 1);
        result.x = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 2);
        result.y = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 3);
        result.z = luaL_checknumber(L, -1);
        lua_pop(L, 3);
    } else {
        lua_getfield(L, num_arg, "x");
        result.x = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "y");
        result.y = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "z");
        result.z = luaL_checknumber(L, -1);
        lua_pop(L, 3);
    }
    return result;
}

Vector3 luaE_optvector3(lua_State * L, int num_arg, Vector3 const & def)
{
    if (lua_istable(L, num_arg)) {
        return luaE_checkvector3(L, num_arg);
    }
    return def;
}

# /************************/
# /* Vector4 (Quaternion) */
# /************************/

void luaE_pushvector4(lua_State * L, Vector4 const & vec)
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

Vector4 luaE_checkvector4(lua_State * L, int num_arg)
{
    Vector4 result = {0,};
    if (lua_objlen(L, num_arg) >= 4) {
        lua_rawgeti(L, num_arg, 1);
        result.x = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 2);
        result.y = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 3);
        result.z = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 4);
        result.w = luaL_checknumber(L, -1);
        lua_pop(L, 4);
    } else {
        lua_getfield(L, num_arg, "x");
        result.x = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "y");
        result.y = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "z");
        result.z = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "w");
        result.w = luaL_checknumber(L, -1);
        lua_pop(L, 4);
    }
    return result;
}

Vector4 luaE_optvector4(lua_State * L, int num_arg, Vector4 const & def)
{
    if (lua_istable(L, num_arg)) {
        return luaE_checkvector4(L, num_arg);
    }
    return def;
}

# /**********/
# /* Matrix */
# /**********/

void luaE_pushmatrix(lua_State * L, Matrix const & mat)
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

Matrix luaE_checkmatrix(lua_State * L, int num_arg)
{
    Matrix result = {0,};
    if (lua_objlen(L, num_arg) >= 16) {
        lua_rawgeti(L, num_arg, 1);
        result.m0 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 2);
        result.m4 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 3);
        result.m8 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 4);
        result.m12 = luaL_checknumber(L, -1);

        lua_rawgeti(L, num_arg, 5);
        result.m1 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 6);
        result.m5 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 7);
        result.m9 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 8);
        result.m13 = luaL_checknumber(L, -1);

        lua_rawgeti(L, num_arg, 9);
        result.m2 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 10);
        result.m6 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 11);
        result.m10 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 12);
        result.m14 = luaL_checknumber(L, -1);

        lua_rawgeti(L, num_arg, 13);
        result.m3 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 14);
        result.m7 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 15);
        result.m11 = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 16);
        result.m15 = luaL_checknumber(L, -1);
        lua_pop(L, 16);
    }
    return result;
}

# /*********/
# /* Color */
# /*********/

void luaE_pushcolor(lua_State * L, Color const & color)
{
    lua_createtable(L, 0, 4);
    lua_pushinteger(L, color.r);
    lua_setfield(L, -2, "r");
    lua_pushinteger(L, color.g);
    lua_setfield(L, -2, "g");
    lua_pushinteger(L, color.b);
    lua_setfield(L, -2, "b");
    lua_pushinteger(L, color.a);
    lua_setfield(L, -2, "a");
}

Color luaE_checkcolor(lua_State * L, int num_arg)
{
    Color result = {0,};
    auto const length = lua_objlen(L, num_arg);
    if (length >= 3) {
        lua_rawgeti(L, num_arg, 1);
        result.r = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 2);
        result.g = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 3);
        result.b = luaL_checkinteger(L, -1);
        if (length >= 4) {
            lua_rawgeti(L, num_arg, 4);
            result.a = luaL_checkinteger(L, -1);
            lua_pop(L, 4);
        } else {
            result.a = 255;
            lua_pop(L, 3);
        }
    } else {
        lua_getfield(L, num_arg, "r");
        result.r = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "g");
        result.g = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "b");
        result.b = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "a");
        if (lua_type(L, -1) == LUA_TNUMBER) {
            result.a = luaL_checkinteger(L, -1);
        } else {
            result.a = 255;
        }
        lua_pop(L, 4);
    }
    return result;
}

std::vector<Color> luaE_checkcolor_array(lua_State * L, int num_arg)
{
    auto const length = lua_objlen(L, num_arg);
    std::vector<Color> result(length);
    for (auto i = 0; i < length; ++i) {
        lua_rawgeti(L, num_arg, 1);
        result[i] = luaE_checkcolor(L, lua_absindex(L, -1));
    }
    lua_pop(L, length);
    return result;
}

# /**************/
# /* Rectangle2 */
# /**************/

void luaE_pushrectangle(lua_State * L, Rectangle2 const & rect)
{
    lua_createtable(L, 0, 4);
    lua_pushnumber(L, rect.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, rect.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, rect.width);
    lua_setfield(L, -2, "width");
    lua_pushnumber(L, rect.height);
    lua_setfield(L, -2, "height");
}

Rectangle2 luaE_checkrectangle(lua_State * L, int num_arg)
{
    Rectangle2 result = {0,};
    if (lua_objlen(L, num_arg) >= 4) {
        lua_rawgeti(L, num_arg, 1);
        result.x = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 2);
        result.y = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 3);
        result.width = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 4);
        result.height = luaL_checknumber(L, -1);
        lua_pop(L, 4);
    } else {
        lua_getfield(L, num_arg, "x");
        result.x = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "y");
        result.y = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "width");
        result.width = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "height");
        result.height = luaL_checknumber(L, -1);
        lua_pop(L, 4);
    }
    return result;
}

# /*********/
# /* Image */
# /*********/

TBAG_LUA_USERDATA_DEFAULT_IMPL(Image, IMAGE, image)

# /***************************************/
# /* Texture2D (Texture, TextureCubemap) */
# /***************************************/

void luaE_pushtexture2d(lua_State * L, Texture2D const & tex)
{
    lua_createtable(L, 0, 5);
    lua_pushinteger(L, tex.id);
    lua_setfield(L, -2, "id");
    lua_pushinteger(L, tex.width);
    lua_setfield(L, -2, "width");
    lua_pushinteger(L, tex.height);
    lua_setfield(L, -2, "height");
    lua_pushinteger(L, tex.mipmaps);
    lua_setfield(L, -2, "mipmaps");
    lua_pushinteger(L, tex.format);
    lua_setfield(L, -2, "format");
}

Texture2D luaE_checktexture2d(lua_State * L, int num_arg)
{
    Texture2D result = {0,};
    if (lua_objlen(L, num_arg) >= 5) {
        lua_rawgeti(L, num_arg, 1);
        result.id = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 2);
        result.width = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 3);
        result.height = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 4);
        result.mipmaps = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 5);
        result.format = luaL_checkinteger(L, -1);
        lua_pop(L, 5);
    } else {
        lua_getfield(L, num_arg, "id");
        result.id = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "width");
        result.width = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "height");
        result.height = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "mipmaps");
        result.mipmaps = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "format");
        result.format = luaL_checkinteger(L, -1);
        lua_pop(L, 5);
    }
    return result;
}

# /***********************************/
# /* RenderTexture2D (RenderTexture) */
# /***********************************/

void luaE_pushrendertexture2d(lua_State * L, RenderTexture2D const & tex)
{
    lua_createtable(L, 0, 5);
    lua_pushinteger(L, tex.id);
    lua_setfield(L, -2, "id");
    luaE_pushtexture2d(L, tex.texture);
    lua_setfield(L, -2, "texture");
    luaE_pushtexture2d(L, tex.depth);
    lua_setfield(L, -2, "depth");
    lua_pushboolean(L, tex.depthTexture?1:0);
    lua_setfield(L, -2, "depthTexture");
}

RenderTexture2D luaE_checkrendertexture2d(lua_State * L, int num_arg)
{
    RenderTexture2D result = {0,};
    lua_getfield(L, num_arg, "id");
    result.id = luaL_checkinteger(L, -1);
    lua_getfield(L, num_arg, "texture");
    result.texture = luaE_checktexture2d(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "depth");
    result.depth = luaE_checktexture2d(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "depthTexture");
    result.depthTexture = (lua_toboolean(L, -1) != 0);
    lua_pop(L, 4);
    return result;
}

# /**************/
# /* NPatchInfo */
# /**************/

void luaE_pushnpatchinfo(lua_State * L, NPatchInfo const & npatch)
{
    lua_createtable(L, 0, 6);
    luaE_pushrectangle(L, npatch.sourceRec);
    lua_setfield(L, -2, "sourceRec");
    lua_pushinteger(L, npatch.left);
    lua_setfield(L, -2, "left");
    lua_pushinteger(L, npatch.top);
    lua_setfield(L, -2, "top");
    lua_pushinteger(L, npatch.right);
    lua_setfield(L, -2, "right");
    lua_pushinteger(L, npatch.bottom);
    lua_setfield(L, -2, "bottom");
    lua_pushinteger(L, npatch.type);
    lua_setfield(L, -2, "type");
}

NPatchInfo luaE_checknpatchinfo(lua_State * L, int num_arg)
{
    NPatchInfo result = {0,};
    if (lua_objlen(L, num_arg) >= 6) {
        lua_rawgeti(L, num_arg, 1);
        result.sourceRec = luaE_checkrectangle(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.left = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 3);
        result.top = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 4);
        result.right = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 5);
        result.bottom = luaL_checkinteger(L, -1);
        lua_rawgeti(L, num_arg, 6);
        result.type = luaL_checkinteger(L, -1);
        lua_pop(L, 6);
    } else {
        lua_getfield(L, num_arg, "sourceRec");
        result.sourceRec = luaE_checkrectangle(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "left");
        result.left = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "top");
        result.top = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "right");
        result.right = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "bottom");
        result.bottom = luaL_checkinteger(L, -1);
        lua_getfield(L, num_arg, "type");
        result.type = luaL_checkinteger(L, -1);
        lua_pop(L, 6);
    }
    return result;
}

# /************/
# /* CharInfo */
# /************/

TBAG_LUA_USERDATA_DEFAULT_IMPL(CharInfo, CHARINFO, charinfo)

void luaE_pushcharinfo_array(lua_State * L, CharInfo * char_infos, int size)
{
    assert(char_infos != nullptr);
    assert(size >= 1);

    lua_createtable(L, size, 0);
    for (int i = 0; i < size; ++i) {
        luaE_pushcharinfo(L, char_infos+i);
        lua_rawseti(L, -2, i+1);
    }
}

std::vector<CharInfo> luaE_checkcharinfo_array(lua_State * L, int num_arg)
{
    auto const length = lua_objlen(L, num_arg);
    std::vector<CharInfo> result(length);
    for (auto i = 0; i < length; ++i) {
        lua_rawgeti(L, num_arg, 1);
        result[i] = *luaE_checkcharinfo(L, lua_absindex(L, -1));
    }
    lua_pop(L, length);
    return result;
}

std::vector<int> luaE_checkchars_array(lua_State * L, int num_arg)
{
    auto const length = lua_objlen(L, num_arg);
    std::vector<int> result(length);
    for (auto i = 0; i < length; ++i) {
        lua_rawgeti(L, num_arg, 1);
        result[i] = luaL_checkinteger(L, lua_absindex(L, -1));
    }
    lua_pop(L, length);
    return result;
}

# /*********************/
# /* Font (SpriteFont) */
# /*********************/

TBAG_LUA_USERDATA_DEFAULT_IMPL(Font, FONT, font)

# /*********************/
# /* Camera3D (Camera) */
# /*********************/

void luaE_pushcamera3d(lua_State * L, Camera3D const & cam)
{
    lua_createtable(L, 0, 5);
    luaE_pushvector3(L, cam.position);
    lua_setfield(L, -2, "position");
    luaE_pushvector3(L, cam.target);
    lua_setfield(L, -2, "target");
    luaE_pushvector3(L, cam.up);
    lua_setfield(L, -2, "up");
    lua_pushnumber(L, cam.fovy);
    lua_setfield(L, -2, "fovy");
    lua_pushinteger(L, cam.type);
    lua_setfield(L, -2, "type");
}

Camera3D luaE_checkcamera3d(lua_State * L, int num_arg)
{
    Camera3D result = {0,};
    if (lua_objlen(L, num_arg) >= 5) {
        lua_rawgeti(L, num_arg, 1);
        result.position = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.target = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 3);
        result.up = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 4);
        result.fovy = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 5);
        result.type = luaL_checkinteger(L, -1);
        lua_pop(L, 5);
    } else {
        lua_getfield(L, num_arg, "position");
        result.position = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "target");
        result.target = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "up");
        result.up = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "fovy");
        result.fovy = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "type");
        result.type = luaL_checkinteger(L, -1);
        lua_pop(L, 5);
    }
    return result;
}

# /************/
# /* Camera2D */
# /************/

void luaE_pushcamera2d(lua_State * L, Camera2D const & cam)
{
    lua_createtable(L, 0, 4);
    luaE_pushvector2(L, cam.offset);
    lua_setfield(L, -2, "offset");
    luaE_pushvector2(L, cam.target);
    lua_setfield(L, -2, "target");
    lua_pushnumber(L, cam.rotation);
    lua_setfield(L, -2, "rotation");
    lua_pushnumber(L, cam.zoom);
    lua_setfield(L, -2, "zoom");
}

Camera2D luaE_checkcamera2d(lua_State * L, int num_arg)
{
    Camera2D result = {0,};
    if (lua_objlen(L, num_arg) >= 4) {
        lua_rawgeti(L, num_arg, 1);
        result.offset = luaE_checkvector2(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.target = luaE_checkvector2(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 3);
        result.rotation = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 4);
        result.zoom = luaL_checknumber(L, -1);
        lua_pop(L, 4);
    } else {
        lua_getfield(L, num_arg, "offset");
        result.offset = luaE_checkvector2(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "target");
        result.target = luaE_checkvector2(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "rotation");
        result.rotation = luaL_checknumber(L, -1);
        lua_getfield(L, num_arg, "zoom");
        result.zoom = luaL_checknumber(L, -1);
        lua_pop(L, 4);
    }
    return result;
}

# /***************/
# /* BoundingBox */
# /***************/

void luaE_pushboundingbox(lua_State * L, BoundingBox const & bbox)
{
    lua_createtable(L, 0, 2);
    luaE_pushvector3(L, bbox.min);
    lua_setfield(L, -2, "min");
    luaE_pushvector3(L, bbox.max);
    lua_setfield(L, -2, "max");
}

BoundingBox luaE_checkboundingbox(lua_State * L, int num_arg)
{
    BoundingBox result = {0,};
    if (lua_objlen(L, num_arg) >= 2) {
        lua_rawgeti(L, num_arg, 1);
        result.min = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.max = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    } else {
        lua_getfield(L, num_arg, "position");
        result.min = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "direction");
        result.max = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    }
    return result;
}

# /********/
# /* Mesh */
# /********/

TBAG_LUA_USERDATA_DEFAULT_IMPL(Mesh, MESH, mesh)

# /**********/
# /* Shader */
# /**********/

void luaE_pushshader(lua_State * L, Shader const & shader)
{
    lua_createtable(L, 0, 2);
    lua_pushinteger(L, shader.id);
    lua_setfield(L, -2, "id");

    lua_createtable(L, MAX_SHADER_LOCATIONS, 0);
    for (int i = 0; i < MAX_SHADER_LOCATIONS; ++i) {
        lua_pushinteger(L, shader.locs[i]);
        lua_rawseti(L, -2, i+1);
    }
    lua_setfield(L, -2, "locs");
}

Shader luaE_checkshader(lua_State * L, int num_arg)
{
    Shader result = {0,};
    lua_getfield(L, num_arg, "id");
    result.id = luaL_checkinteger(L, lua_absindex(L, -1));

    lua_getfield(L, num_arg, "locs");
    auto locs = luaE_checkinteger_array(L, lua_absindex(L, -1));
    auto min_size = locs.size();
    if (min_size > MAX_SHADER_LOCATIONS) {
        min_size = MAX_SHADER_LOCATIONS;
    }
    if (min_size >= 1) {
        for (int i = 0; i < min_size; ++i) {
            result.locs[i] = locs[i];
        }
    }

    lua_pop(L, 2);
    return result;
}

# /***************/
# /* MaterialMap */
# /***************/

void luaE_pushmaterialmap(lua_State * L, MaterialMap const & materials)
{
    lua_createtable(L, 0, 3);
    luaE_pushtexture2d(L, materials.texture);
    lua_setfield(L, -2, "texture");
    luaE_pushcolor(L, materials.color);
    lua_setfield(L, -2, "color");
    lua_pushnumber(L, materials.value);
    lua_setfield(L, -2, "value");
}

MaterialMap luaE_checkmaterialmap(lua_State * L, int num_arg)
{
    MaterialMap result = {0,};
    lua_getfield(L, num_arg, "texture");
    result.texture = luaE_checktexture2d(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "color");
    result.color = luaE_checkcolor(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "value");
    result.value = luaL_checknumber(L, lua_absindex(L, -1));
    lua_pop(L, 3);
    return result;
}

# /************/
# /* Material */
# /************/

TBAG_LUA_USERDATA_DEFAULT_IMPL(Material, MATERIAL, material)

# /*************/
# /* Transform */
# /*************/

//typedef struct Transform {
//    Vector3 translation;
//    Quaternion rotation;
//    Vector3 scale;
//} Transform;

# /************/
# /* BoneInfo */
# /************/

//typedef struct BoneInfo {
//    char name[32];
//    int parent;
//} BoneInfo;

# /*********/
# /* Model */
# /*********/

TBAG_LUA_USERDATA_DEFAULT_IMPL(Model, MODEL, model)

# /******************/
# /* ModelAnimation */
# /******************/

TBAG_LUA_USERDATA_DEFAULT_IMPL(ModelAnimation, MODELANIMATION, modelanimation)

# /*******/
# /* Ray */
# /*******/

void luaE_pushray(lua_State * L, Ray const & ray)
{
    lua_createtable(L, 0, 2);
    luaE_pushvector3(L, ray.position);
    lua_setfield(L, -2, "position");
    luaE_pushvector3(L, ray.direction);
    lua_setfield(L, -2, "direction");
}

Ray luaE_checkray(lua_State * L, int num_arg)
{
    Ray result = {0,};
    if (lua_objlen(L, num_arg) >= 2) {
        lua_rawgeti(L, num_arg, 1);
        result.position = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.direction = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    } else {
        lua_getfield(L, num_arg, "position");
        result.position = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "direction");
        result.direction = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    }
    return result;
}

# /**************/
# /* RayHitInfo */
# /**************/

void luaE_pushrayhitinfo(lua_State * L, RayHitInfo const & info)
{
    lua_createtable(L, 0, 4);
    lua_pushboolean(L, info.hit);
    lua_setfield(L, -2, "hit");
    lua_pushnumber(L, info.distance);
    lua_setfield(L, -2, "distance");
    luaE_pushvector3(L, info.position);
    lua_setfield(L, -2, "position");
    luaE_pushvector3(L, info.normal);
    lua_setfield(L, -2, "normal");
}

RayHitInfo luaE_checkrayhitinfo(lua_State * L, int num_arg)
{
    RayHitInfo result = {0,};
    if (lua_objlen(L, num_arg) >= 4) {
        lua_rawgeti(L, num_arg, 1);
        result.hit = lua_toboolean(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.distance = luaL_checknumber(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 3);
        result.position = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 4);
        result.normal = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 4);
    } else {
        lua_getfield(L, num_arg, "hit");
        result.hit = lua_toboolean(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "distance");
        result.distance = luaL_checknumber(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "position");
        result.position = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "normal");
        result.normal = luaE_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 4);
    }
    return result;
}

# /********/
# /* Wave */
# /********/

TBAG_LUA_USERDATA_DEFAULT_IMPL(Wave, WAVE, wave)

# /*********/
# /* Sound */
# /*********/

TBAG_LUA_USERDATA_DEFAULT_IMPL(Sound, SOUND, sound)

# /*********/
# /* Music */
# /*********/

TBAG_LUA_USERDATA_IMPL(MusicWrapper, Music, MUSIC, music, nullptr)

# /***************/
# /* AudioStream */
# /***************/

TBAG_LUA_USERDATA_DEFAULT_IMPL(AudioStream, AUDIOSTREAM, audiostream)

# /****************/
# /* VrDeviceInfo */
# /****************/

void luaE_pushvrdeviceinfo(lua_State * L, VrDeviceInfo const & info)
{
    lua_createtable(L, 0, 10);
    lua_pushinteger(L, info.hResolution);
    lua_setfield(L, -2, "hResolution");
    lua_pushinteger(L, info.vResolution);
    lua_setfield(L, -2, "vResolution");
    lua_pushnumber(L, info.hScreenSize);
    lua_setfield(L, -2, "hScreenSize");
    lua_pushnumber(L, info.vScreenSize);
    lua_setfield(L, -2, "vScreenSize");
    lua_pushnumber(L, info.vScreenCenter);
    lua_setfield(L, -2, "vScreenCenter");
    lua_pushnumber(L, info.eyeToScreenDistance);
    lua_setfield(L, -2, "eyeToScreenDistance");
    lua_pushnumber(L, info.lensSeparationDistance);
    lua_setfield(L, -2, "lensSeparationDistance");
    lua_pushnumber(L, info.interpupillaryDistance);
    lua_setfield(L, -2, "interpupillaryDistance");

    lua_createtable(L, 4, 0);
    lua_pushnumber(L, info.lensDistortionValues[0]);
    lua_rawseti(L, -2, 1);
    lua_pushnumber(L, info.lensDistortionValues[1]);
    lua_rawseti(L, -2, 2);
    lua_pushnumber(L, info.lensDistortionValues[2]);
    lua_rawseti(L, -2, 3);
    lua_pushnumber(L, info.lensDistortionValues[3]);
    lua_rawseti(L, -2, 4);
    lua_setfield(L, -2, "lensDistortionValues");

    lua_createtable(L, 4, 0);
    lua_pushnumber(L, info.chromaAbCorrection[0]);
    lua_rawseti(L, -2, 1);
    lua_pushnumber(L, info.chromaAbCorrection[1]);
    lua_rawseti(L, -2, 2);
    lua_pushnumber(L, info.chromaAbCorrection[2]);
    lua_rawseti(L, -2, 3);
    lua_pushnumber(L, info.chromaAbCorrection[3]);
    lua_rawseti(L, -2, 4);
    lua_setfield(L, -2, "chromaAbCorrection");
}

VrDeviceInfo luaE_checkvrdeviceinfo(lua_State * L, int num_arg)
{
    VrDeviceInfo result = {0,};
    lua_getfield(L, num_arg, "hResolution");
    result.hResolution = luaL_checkinteger(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "vResolution");
    result.vResolution = luaL_checkinteger(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "hScreenSize");
    result.hScreenSize = luaL_checknumber(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "vScreenSize");
    result.vScreenSize = luaL_checknumber(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "vScreenCenter");
    result.vScreenCenter = luaL_checknumber(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "eyeToScreenDistance");
    result.eyeToScreenDistance = luaL_checknumber(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "lensSeparationDistance");
    result.lensSeparationDistance = luaL_checknumber(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "interpupillaryDistance");
    result.interpupillaryDistance = luaL_checknumber(L, lua_absindex(L, -1));

    lua_getfield(L, num_arg, "lensDistortionValues");
    auto lens_distortion_values = luaE_checknumber_array(L, lua_absindex(L, -1));
    auto lens_distortion_values_size = lens_distortion_values.size();
    if (lens_distortion_values_size > 4) {
        lens_distortion_values_size = 4;
    }
    if (lens_distortion_values_size >= 1) {
        for (int i = 0; i < lens_distortion_values_size; ++i) {
            result.lensDistortionValues[i] = lens_distortion_values[i];
        }
    }

    lua_getfield(L, num_arg, "chromaAbCorrection");
    auto chroma_ab_correction = luaE_checknumber_array(L, lua_absindex(L, -1));
    auto chroma_ab_correction_size = chroma_ab_correction.size();
    if (chroma_ab_correction_size > 4) {
        chroma_ab_correction_size = 4;
    }
    if (chroma_ab_correction_size >= 1) {
        for (int i = 0; i < chroma_ab_correction_size; ++i) {
            result.chromaAbCorrection[i] = chroma_ab_correction[i];
        }
    }

    lua_pop(L, 10);
    return result;
}

# /******************/
# /* VrStereoConfig */
# /******************/

//typedef struct VrStereoConfig {
//    RenderTexture2D stereoFbo;
//    Shader distortionShader;
//    Matrix eyesProjection[2];
//    Matrix eyesViewOffset[2];
//    int eyeViewportRight[4];
//    int eyeViewportLeft[4];
//} VrStereoConfig;

# /*****************/
# /* [CORE MODULE] */
# /*****************/

static int _InitWindow(lua_State * L)
{
    InitWindow(luaL_checkinteger(L, 1),
               luaL_checkinteger(L, 2),
               luaL_checkstring(L, 3));
    return 0;
}

static int _WindowShouldClose(lua_State * L)
{
    bool result = WindowShouldClose();
    lua_pushboolean(L, result);
    return 1;
}

static int _CloseWindow(lua_State * L)
{
    CloseWindow();
    return 0;
}

static int _IsWindowReady(lua_State * L)
{
    lua_pushboolean(L, IsWindowReady()?1:0);
    return 1;
}

static int _IsWindowMinimized(lua_State * L)
{
    lua_pushboolean(L, IsWindowMinimized()?1:0);
    return 1;
}

static int _IsWindowResized(lua_State * L)
{
    lua_pushboolean(L, IsWindowResized()?1:0);
    return 1;
}

static int _IsWindowHidden(lua_State * L)
{
    lua_pushboolean(L, IsWindowHidden()?1:0);
    return 1;
}

static int _ToggleFullscreen(lua_State * L)
{
    ToggleFullscreen();
    return 0;
}

static int _UnhideWindow(lua_State * L)
{
    UnhideWindow();
    return 0;
}

static int _HideWindow(lua_State * L)
{
    HideWindow();
    return 0;
}

static int _SetWindowIcon(lua_State * L)
{
    SetWindowIcon(*luaE_checkimage(L, 1));
    return 0;
}

static int _SetWindowTitle(lua_State * L)
{
    SetWindowTitle(luaL_checkstring(L, 1));
    return 0;
}

static int _SetWindowPosition(lua_State * L)
{
    SetWindowPosition(luaL_checkinteger(L, 1),
                      luaL_checkinteger(L, 2));
    return 0;
}

static int _SetWindowMonitor(lua_State * L)
{
    SetWindowMonitor(luaL_checkinteger(L, 1));
    return 0;
}

static int _SetWindowMinSize(lua_State * L)
{
    SetWindowMinSize(luaL_checkinteger(L, 1),
                     luaL_checkinteger(L, 2));
    return 0;
}

static int _SetWindowSize(lua_State * L)
{
    SetWindowSize(luaL_checkinteger(L, 1),
                  luaL_checkinteger(L, 2));
    return 0;
}

static int _GetScreenWidth(lua_State * L)
{
    lua_pushinteger(L, GetScreenWidth());
    return 1;
}

static int _GetScreenHeight(lua_State * L)
{
    lua_pushinteger(L, GetScreenHeight());
    return 1;
}

static int _GetMonitorCount(lua_State * L)
{
    lua_pushinteger(L, GetMonitorCount());
    return 1;
}

static int _GetMonitorWidth(lua_State * L)
{
    lua_pushinteger(L, GetMonitorWidth(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetMonitorHeight(lua_State * L)
{
    lua_pushinteger(L, GetMonitorHeight(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetMonitorPhysicalWidth(lua_State * L)
{
    lua_pushinteger(L, GetMonitorPhysicalWidth(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetMonitorPhysicalHeight(lua_State * L)
{
    lua_pushinteger(L, GetMonitorPhysicalHeight(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetMonitorName(lua_State * L)
{
    lua_pushstring(L, GetMonitorName(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetClipboardText(lua_State * L)
{
    lua_pushstring(L, GetClipboardText());
    return 1;
}

static int _SetClipboardText(lua_State * L)
{
    SetClipboardText(luaL_checkstring(L, 1));
    return 0;
}

static int _ShowCursor(lua_State * L)
{
    ShowCursor();
    return 0;
}

static int _HideCursor(lua_State * L)
{
    HideCursor();
    return 0;
}

static int _IsCursorHidden(lua_State * L)
{
    lua_pushboolean(L, IsCursorHidden()?1:0);
    return 1;
}

static int _EnableCursor(lua_State * L)
{
    EnableCursor();
    return 0;
}

static int _DisableCursor(lua_State * L)
{
    DisableCursor();
    return 0;
}

static int _ClearBackground(lua_State * L)
{
    ClearBackground(luaE_checkcolor(L, 1));
    return 0;
}

static int _BeginDrawing(lua_State * L)
{
    BeginDrawing();
    return 0;
}

static int _EndDrawing(lua_State * L)
{
    EndDrawing();
    return 0;
}

static int _BeginMode2D(lua_State * L)
{
    BeginMode2D(luaE_checkcamera2d(L, 1));
    return 0;
}

static int _EndMode2D(lua_State * L)
{
    EndMode2D();
    return 0;
}

static int _BeginMode3D(lua_State * L)
{
    BeginMode3D(luaE_checkcamera3d(L, 1));
    return 0;
}

static int _EndMode3D(lua_State * L)
{
    EndMode3D();
    return 0;
}

static int _BeginTextureMode(lua_State * L)
{
    BeginTextureMode(luaE_checkrendertexture2d(L, 1));
    return 0;
}

static int _EndTextureMode(lua_State * L)
{
    EndTextureMode();
    return 0;
}

static int _GetMouseRay(lua_State * L)
{
    luaE_pushray(L, GetMouseRay(luaE_checkvector2(L, 1), luaE_checkcamera3d(L, 2)));
    return 1;
}

static int _GetWorldToScreen(lua_State * L)
{
    luaE_pushvector2(L, GetWorldToScreen(luaE_checkvector3(L, 1), luaE_checkcamera3d(L, 2)));
    return 1;
}

static int _GetCameraMatrix(lua_State * L)
{
    luaE_pushmatrix(L, GetCameraMatrix(luaE_checkcamera3d(L, 1)));
    return 1;
}

static int _SetTargetFPS(lua_State * L)
{
    SetTargetFPS(luaL_checkinteger(L, 1));
    return 0;
}

static int _GetFPS(lua_State * L)
{
    lua_pushinteger(L, GetFPS());
    return 1;
}

static int _GetFrameTime(lua_State * L)
{
    lua_pushnumber(L, GetFrameTime());
    return 1;
}

static int _GetTime(lua_State * L)
{
    lua_pushnumber(L, GetTime());
    return 1;
}

static int _ColorToInt(lua_State * L)
{
    lua_pushnumber(L, ColorToInt(luaE_checkcolor(L, 1)));
    return 1;
}

static int _ColorNormalize(lua_State * L)
{
    luaE_pushvector4(L, ColorNormalize(luaE_checkcolor(L, 1)));
    return 1;
}

static int _ColorToHSV(lua_State * L)
{
    luaE_pushvector3(L, ColorToHSV(luaE_checkcolor(L, 1)));
    return 1;
}

static int _ColorFromHSV(lua_State * L)
{
    luaE_pushcolor(L, ColorFromHSV(luaE_checkvector3(L, 1)));
    return 1;
}

static int _GetColor(lua_State * L)
{
    luaE_pushcolor(L, GetColor(luaL_checkinteger(L, 1)));
    return 1;
}

static int _Fade(lua_State * L)
{
    luaE_pushcolor(L, Fade(luaE_checkcolor(L, 1), luaL_checknumber(L, 2)));
    return 1;
}

static int _SetConfigFlags(lua_State * L)
{
    SetConfigFlags(luaL_checkinteger(L, 1));
    return 0;
}

static int _SetTraceLogLevel(lua_State * L)
{
    SetTraceLogLevel(luaL_checkinteger(L, 1));
    return 0;
}

static int _SetTraceLogExit(lua_State * L)
{
    SetTraceLogExit(luaL_checkinteger(L, 1));
    return 0;
}

static int _SetTraceLogCallback(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TraceLog(lua_State * L)
{
    TraceLog(luaL_checkinteger(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _TakeScreenshot(lua_State * L)
{
    TakeScreenshot(luaL_checkstring(L, 1));
    return 0;
}

static int _GetRandomValue(lua_State * L)
{
    lua_pushinteger(L, GetRandomValue(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _FileExists(lua_State * L)
{
    lua_pushboolean(L, FileExists(luaL_checkstring(L, 1)));
    return 1;
}

static int _IsFileExtension(lua_State * L)
{
    lua_pushboolean(L, IsFileExtension(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

static int _GetExtension(lua_State * L)
{
    lua_pushstring(L, GetExtension(luaL_checkstring(L, 1)));
    return 1;
}

static int _GetFileName(lua_State * L)
{
    lua_pushstring(L, GetFileName(luaL_checkstring(L, 1)));
    return 1;
}

static int _GetFileNameWithoutExt(lua_State * L)
{
    lua_pushstring(L, GetFileNameWithoutExt(luaL_checkstring(L, 1)));
    return 1;
}

static int _GetDirectoryPath(lua_State * L)
{
    lua_pushstring(L, GetDirectoryPath(luaL_checkstring(L, 1)));
    return 1;
}

static int _GetWorkingDirectory(lua_State * L)
{
    lua_pushstring(L, GetWorkingDirectory());
    return 1;
}

static int _GetDirectoryFiles(lua_State * L)
{
    int count = 0;
    char ** result = GetDirectoryFiles(luaL_checkstring(L, 1), &count);
    lua_createtable(L, count, 0);
    for (int i = 0; i < count; ++i) {
        lua_pushstring(L, result[i]);
        lua_rawseti(L, -2, i+1);
    }
    return 1;
}

static int _ClearDirectoryFiles(lua_State * L)
{
    ClearDirectoryFiles();
    return 0;
}

static int _ChangeDirectory(lua_State * L)
{
    lua_pushboolean(L, ChangeDirectory(luaL_checkstring(L, 1)));
    return 1;
}

static int _IsFileDropped(lua_State * L)
{
    lua_pushboolean(L, IsFileDropped());
    return 1;
}

static int _GetDroppedFiles(lua_State * L)
{
    int count = 0;
    char ** result = GetDroppedFiles(&count);
    lua_createtable(L, count, 0);
    for (int i = 0; i < count; ++i) {
        lua_pushstring(L, result[i]);
        lua_rawseti(L, -2, i+1);
    }
    return 1;
}

static int _ClearDroppedFiles(lua_State * L)
{
    ClearDroppedFiles();
    return 0;
}

static int _GetFileModTime(lua_State * L)
{
    lua_pushinteger(L, GetFileModTime(luaL_checkstring(L, 1)));
    return 1;
}

static int _StorageSaveValue(lua_State * L)
{
    StorageSaveValue(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _StorageLoadValue(lua_State * L)
{
    lua_pushinteger(L, StorageLoadValue(luaL_checkinteger(L, 1)));
    return 1;
}

static int _OpenURL(lua_State * L)
{
    OpenURL(luaL_checkstring(L, 1));
    return 0;
}

static int _IsKeyPressed(lua_State * L)
{
    lua_pushboolean(L, IsKeyPressed(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsKeyDown(lua_State * L)
{
    lua_pushboolean(L, IsKeyDown(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsKeyReleased(lua_State * L)
{
    lua_pushboolean(L, IsKeyReleased(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsKeyUp(lua_State * L)
{
    lua_pushboolean(L, IsKeyUp(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetKeyPressed(lua_State * L)
{
    lua_pushinteger(L, GetKeyPressed());
    return 1;
}

static int _SetExitKey(lua_State * L)
{
    SetExitKey(luaL_checkinteger(L, 1));
    return 0;
}

static int _IsGamepadAvailable(lua_State * L)
{
    lua_pushboolean(L, IsGamepadAvailable(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsGamepadName(lua_State * L)
{
    lua_pushboolean(L, IsGamepadName(luaL_checkinteger(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

static int _GetGamepadName(lua_State * L)
{
    lua_pushstring(L, GetGamepadName(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsGamepadButtonPressed(lua_State * L)
{
    lua_pushboolean(L, IsGamepadButtonPressed(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _IsGamepadButtonDown(lua_State * L)
{
    lua_pushboolean(L, IsGamepadButtonDown(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _IsGamepadButtonReleased(lua_State * L)
{
    lua_pushboolean(L, IsGamepadButtonReleased(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _IsGamepadButtonUp(lua_State * L)
{
    lua_pushboolean(L, IsGamepadButtonUp(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _GetGamepadButtonPressed(lua_State * L)
{
    lua_pushinteger(L, GetGamepadButtonPressed());
    return 1;
}

static int _GetGamepadAxisCount(lua_State * L)
{
    lua_pushinteger(L, GetGamepadAxisCount(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetGamepadAxisMovement(lua_State * L)
{
    lua_pushnumber(L, GetGamepadAxisMovement(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _IsMouseButtonPressed(lua_State * L)
{
    lua_pushboolean(L, IsMouseButtonPressed(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsMouseButtonDown(lua_State * L)
{
    lua_pushboolean(L, IsMouseButtonDown(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsMouseButtonReleased(lua_State * L)
{
    lua_pushboolean(L, IsMouseButtonReleased(luaL_checkinteger(L, 1)));
    return 1;
}

static int _IsMouseButtonUp(lua_State * L)
{
    lua_pushboolean(L, IsMouseButtonUp(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetMouseX(lua_State * L)
{
    lua_pushinteger(L, GetMouseX());
    return 1;
}

static int _GetMouseY(lua_State * L)
{
    lua_pushinteger(L, GetMouseY());
    return 1;
}

static int _GetMousePosition(lua_State * L)
{
    luaE_pushvector2(L, GetMousePosition());
    return 1;
}

static int _SetMousePosition(lua_State * L)
{
    SetMousePosition(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _SetMouseOffset(lua_State * L)
{
    SetMouseOffset(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _SetMouseScale(lua_State * L)
{
    SetMouseScale(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
    return 0;
}

static int _GetMouseWheelMove(lua_State * L)
{
    lua_pushinteger(L, GetMouseWheelMove());
    return 1;
}

static int _GetTouchX(lua_State * L)
{
    lua_pushinteger(L, GetTouchX());
    return 1;
}

static int _GetTouchY(lua_State * L)
{
    lua_pushinteger(L, GetTouchY());
    return 1;
}

static int _GetTouchPosition(lua_State * L)
{
    luaE_pushvector2(L, GetTouchPosition(luaL_checkinteger(L, 1)));
    return 1;
}

static int _SetGesturesEnabled(lua_State * L)
{
    SetGesturesEnabled(luaL_checkinteger(L, 1));
    return 0;
}

static int _IsGestureDetected(lua_State * L)
{
    lua_pushboolean(L, IsGestureDetected(luaL_checkinteger(L, 1)));
    return 1;
}

static int _GetGestureDetected(lua_State * L)
{
    lua_pushinteger(L, GetGestureDetected());
    return 1;
}

static int _GetTouchPointsCount(lua_State * L)
{
    lua_pushinteger(L, GetTouchPointsCount());
    return 1;
}

static int _GetGestureHoldDuration(lua_State * L)
{
    lua_pushnumber(L, GetGestureHoldDuration());
    return 1;
}

static int _GetGestureDragVector(lua_State * L)
{
    luaE_pushvector2(L, GetGestureDragVector());
    return 1;
}

static int _GetGestureDragAngle(lua_State * L)
{
    lua_pushnumber(L, GetGestureDragAngle());
    return 1;
}

static int _GetGesturePinchVector(lua_State * L)
{
    luaE_pushvector2(L, GetGesturePinchVector());
    return 1;
}

static int _GetGesturePinchAngle(lua_State * L)
{
    lua_pushnumber(L, GetGesturePinchAngle());
    return 1;
}

static int _SetCameraMode(lua_State * L)
{
    SetCameraMode(luaE_checkcamera3d(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _UpdateCamera(lua_State * L)
{
    auto cam = luaE_checkcamera3d(L, 1);
    UpdateCamera(&cam);
    luaE_pushcamera3d(L, cam);
    return 1;
}

static int _SetCameraPanControl(lua_State * L)
{
    SetCameraPanControl(luaL_checkinteger(L, 1));
    return 0;
}

static int _SetCameraAltControl(lua_State * L)
{
    SetCameraAltControl(luaL_checkinteger(L, 1));
    return 0;
}

static int _SetCameraSmoothZoomControl(lua_State * L)
{
    SetCameraSmoothZoomControl(luaL_checkinteger(L, 1));
    return 0;
}

static int _SetCameraMoveControls(lua_State * L)
{
    SetCameraMoveControls(luaL_checkinteger(L, 1),
                          luaL_checkinteger(L, 2),
                          luaL_checkinteger(L, 3),
                          luaL_checkinteger(L, 4),
                          luaL_checkinteger(L, 5),
                          luaL_checkinteger(L, 6));
    return 0;
}

# /*******************/
# /* [SHAPES MODULE] */
# /*******************/

static int _DrawPixel(lua_State * L)
{
    DrawPixel(luaL_checkinteger(L, 1),
              luaL_checkinteger(L, 2),
              luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawPixelV(lua_State * L)
{
    DrawPixelV(luaE_checkvector2(L, 1),
               luaE_checkcolor(L, 2));
    return 0;
}

static int _DrawLine(lua_State * L)
{
    DrawLine(luaL_checkinteger(L, 1),
             luaL_checkinteger(L, 2),
             luaL_checkinteger(L, 3),
             luaL_checkinteger(L, 4),
             luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawLineV(lua_State * L)
{
    DrawLineV(luaE_checkvector2(L, 1),
              luaE_checkvector2(L, 2),
              luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawLineEx(lua_State * L)
{
    DrawLineEx(luaE_checkvector2(L, 1),
               luaE_checkvector2(L, 2),
               luaL_checknumber(L, 3),
               luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawLineBezier(lua_State * L)
{
    DrawLineBezier(luaE_checkvector2(L, 1),
                   luaE_checkvector2(L, 2),
                   luaL_checknumber(L, 3),
                   luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawCircle(lua_State * L)
{
    DrawCircle(luaL_checkinteger(L, 1),
               luaL_checkinteger(L, 2),
               luaL_checknumber(L, 3),
               luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawCircleSector(lua_State * L)
{
    DrawCircleSector(luaE_checkvector2(L, 1),
                     luaL_checknumber(L, 2),
                     luaL_checkinteger(L, 3),
                     luaL_checkinteger(L, 4),
                     luaL_checkinteger(L, 5),
                     luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawCircleSectorLines(lua_State * L)
{
    DrawCircleSectorLines(luaE_checkvector2(L, 1),
                          luaL_checknumber(L, 2),
                          luaL_checkinteger(L, 3),
                          luaL_checkinteger(L, 4),
                          luaL_checkinteger(L, 5),
                          luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawCircleGradient(lua_State * L)
{
    DrawCircleGradient(luaL_checkinteger(L, 1),
                       luaL_checkinteger(L, 2),
                       luaL_checknumber(L, 3),
                       luaE_checkcolor(L, 4),
                       luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawCircleV(lua_State * L)
{
    DrawCircleV(luaE_checkvector2(L, 1),
                luaL_checknumber(L, 2),
                luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawCircleLines(lua_State * L)
{
    DrawCircleLines(luaL_checkinteger(L, 1),
                    luaL_checkinteger(L, 2),
                    luaL_checknumber(L, 3),
                    luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawRing(lua_State * L)
{
    DrawRing(luaE_checkvector2(L, 1),
             luaL_checknumber(L, 2),
             luaL_checknumber(L, 3),
             luaL_checkinteger(L, 4),
             luaL_checkinteger(L, 5),
             luaL_checkinteger(L, 6),
             luaE_checkcolor(L, 7));
    return 0;
}

static int _DrawRingLines(lua_State * L)
{
    DrawRingLines(luaE_checkvector2(L, 1),
                  luaL_checknumber(L, 2),
                  luaL_checknumber(L, 3),
                  luaL_checkinteger(L, 4),
                  luaL_checkinteger(L, 5),
                  luaL_checkinteger(L, 6),
                  luaE_checkcolor(L, 7));
    return 0;
}

static int _DrawRectangle(lua_State * L)
{
    DrawRectangle(luaL_checkinteger(L, 1),
                  luaL_checkinteger(L, 2),
                  luaL_checkinteger(L, 3),
                  luaL_checkinteger(L, 4),
                  luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawRectangleV(lua_State * L)
{
    DrawRectangleV(luaE_checkvector2(L, 1),
                   luaE_checkvector2(L, 2),
                   luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawRectangleRec(lua_State * L)
{
    DrawRectangleRec(luaE_checkrectangle(L, 1),
                     luaE_checkcolor(L, 2));
    return 0;
}

static int _DrawRectanglePro(lua_State * L)
{
    DrawRectanglePro(luaE_checkrectangle(L, 1),
                     luaE_checkvector2(L, 2),
                     luaL_checknumber(L, 3),
                     luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawRectangleGradientV(lua_State * L)
{
    DrawRectangleGradientV(luaL_checkinteger(L, 1),
                           luaL_checkinteger(L, 2),
                           luaL_checkinteger(L, 3),
                           luaL_checkinteger(L, 4),
                           luaE_checkcolor(L, 5),
                           luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawRectangleGradientH(lua_State * L)
{
    DrawRectangleGradientH(luaL_checkinteger(L, 1),
                           luaL_checkinteger(L, 2),
                           luaL_checkinteger(L, 3),
                           luaL_checkinteger(L, 4),
                           luaE_checkcolor(L, 5),
                           luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawRectangleGradientEx(lua_State * L)
{
    DrawRectangleGradientEx(luaE_checkrectangle(L, 1),
                            luaE_checkcolor(L, 2),
                            luaE_checkcolor(L, 3),
                            luaE_checkcolor(L, 4),
                            luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawRectangleLines(lua_State * L)
{
    DrawRectangleLines(luaL_checkinteger(L, 1),
                       luaL_checkinteger(L, 2),
                       luaL_checkinteger(L, 3),
                       luaL_checkinteger(L, 4),
                       luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawRectangleLinesEx(lua_State * L)
{
    DrawRectangleLinesEx(luaE_checkrectangle(L, 1),
                         luaL_checkinteger(L, 2),
                         luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawRectangleRounded(lua_State * L)
{
    DrawRectangleRounded(luaE_checkrectangle(L, 1),
                         luaL_checknumber(L, 2),
                         luaL_checkinteger(L, 3),
                         luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawRectangleRoundedLines(lua_State * L)
{
    DrawRectangleRoundedLines(luaE_checkrectangle(L, 1),
                              luaL_checknumber(L, 2),
                              luaL_checkinteger(L, 3),
                              luaL_checkinteger(L, 4),
                              luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawTriangle(lua_State * L)
{
    DrawTriangle(luaE_checkvector2(L, 1),
                 luaE_checkvector2(L, 2),
                 luaE_checkvector2(L, 3),
                 luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawTriangleLines(lua_State * L)
{
    DrawTriangleLines(luaE_checkvector2(L, 1),
                      luaE_checkvector2(L, 2),
                      luaE_checkvector2(L, 3),
                      luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawPoly(lua_State * L)
{
    DrawPoly(luaE_checkvector2(L, 1),
             luaL_checkinteger(L, 2),
             luaL_checknumber(L, 3),
             luaL_checknumber(L, 4),
             luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawPolyEx(lua_State * L)
{
    DrawPolyEx(luaE_checkvector2_array(L, 1).data(),
               luaL_checkinteger(L, 2),
               luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawPolyExLines(lua_State * L)
{
    DrawPolyExLines(luaE_checkvector2_array(L, 1).data(),
                    luaL_checkinteger(L, 2),
                    luaE_checkcolor(L, 3));
    return 0;
}

static int _SetShapesTexture(lua_State * L)
{
    SetShapesTexture(luaE_checktexture2d(L, 1),
                     luaE_checkrectangle(L, 2));
    return 0;
}

static int _CheckCollisionRecs(lua_State * L)
{
    auto const result = CheckCollisionRecs(luaE_checkrectangle(L, 1),
                                           luaE_checkrectangle(L, 2));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionCircles(lua_State * L)
{
    auto const result = CheckCollisionCircles(luaE_checkvector2(L, 1),
                                              luaL_checknumber(L, 2),
                                              luaE_checkvector2(L, 3),
                                              luaL_checknumber(L, 4));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionCircleRec(lua_State * L)
{
    auto const result = CheckCollisionCircleRec(luaE_checkvector2(L, 1),
                                                luaL_checknumber(L, 2),
                                                luaE_checkrectangle(L, 3));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _GetCollisionRec(lua_State * L)
{
    auto const result = GetCollisionRec(luaE_checkrectangle(L, 1),
                                        luaE_checkrectangle(L, 2));
    luaE_pushrectangle(L, result);
    return 1;
}

static int _CheckCollisionPointRec(lua_State * L)
{
    auto const result = CheckCollisionPointRec(luaE_checkvector2(L, 1),
                                               luaE_checkrectangle(L, 2));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionPointCircle(lua_State * L)
{
    auto const result = CheckCollisionPointCircle(luaE_checkvector2(L, 1),
                                                  luaE_checkvector2(L, 2),
                                                  luaL_checknumber(L, 3));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionPointTriangle(lua_State * L)
{
    auto const result = CheckCollisionPointTriangle(luaE_checkvector2(L, 1),
                                                    luaE_checkvector2(L, 2),
                                                    luaE_checkvector2(L, 3),
                                                    luaE_checkvector2(L, 4));
    lua_pushboolean(L, result?1:0);
    return 1;
}

# /*********************/
# /* [TEXTURES MODULE] */
# /*********************/

static int _LoadImage(lua_State * L)
{
    auto const result = LoadImage(luaL_checkstring(L, 1));
    luaE_pushimage(L, &result);
    return 1;
}

static int _LoadImageEx(lua_State * L)
{
    auto colors = luaE_checkcolor_array(L, 1);
    auto const result = LoadImageEx(colors.data(), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    luaE_pushimage(L, &result);
    return 1;
}

static int _LoadImagePro(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _LoadImageRaw(lua_State * L)
{
    auto const result = LoadImageRaw(luaL_checkstring(L, 1),
                                     luaL_checkinteger(L, 2),
                                     luaL_checkinteger(L, 3),
                                     luaL_checkinteger(L, 4),
                                     luaL_checkinteger(L, 5));
    luaE_pushimage(L, &result);
    return 1;
}

static int _ExportImage(lua_State * L)
{
    ExportImage(*luaE_checkimage(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _ExportImageAsCode(lua_State * L)
{
    ExportImageAsCode(*luaE_checkimage(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _LoadTexture(lua_State * L)
{
    auto const result = LoadTexture(luaL_checkstring(L, 1));
    luaE_pushtexture2d(L, result);
    return 1;
}

static int _LoadTextureFromImage(lua_State * L)
{
    luaE_pushtexture2d(L, LoadTextureFromImage(*luaE_checkimage(L, 1)));
    return 1;
}

static int _LoadTextureCubemap(lua_State * L)
{
    auto const tex = LoadTextureCubemap(*luaE_checkimage(L, 1), luaL_checkinteger(L, 2));
    luaE_pushtexture2d(L, tex);
    return 1;
}

static int _LoadRenderTexture(lua_State * L)
{
    auto tex = LoadRenderTexture(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    luaE_pushrendertexture2d(L, tex);
    return 1;
}

static int _UnloadImage(lua_State * L)
{
    UnloadImage(*luaE_checkimage(L, 1));
    return 0;
}

static int _UnloadTexture(lua_State * L)
{
    UnloadTexture(luaE_checktexture2d(L, 1));
    return 0;
}

static int _UnloadRenderTexture(lua_State * L)
{
    UnloadRenderTexture(luaE_checkrendertexture2d(L, 1));
    return 0;
}

static int _GetImageData(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    assert(image != nullptr);

    auto const SIZE = image->width * image->height;

    Color * pixels = GetImageData(*image);
    assert(pixels != nullptr);

    lua_createtable(L, SIZE, 0);
    for (int i = 0; i < SIZE; ++i) {
        luaE_pushcolor(L, pixels[i]);
        lua_rawseti(L, -2, i+1);
    }
    free(pixels);
    return 1;
}

static int _GetImageDataNormalized(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    assert(image != nullptr);

    auto const SIZE = image->width * image->height;

    Vector4 * pixels = GetImageDataNormalized(*image);
    assert(pixels != nullptr);

    lua_createtable(L, SIZE, 0);
    for (int i = 0; i < SIZE; ++i) {
        luaE_pushvector4(L, pixels[i]);
        lua_rawseti(L, -2, i+1);
    }
    free(pixels);
    return 1;
}

static int _GetPixelDataSize(lua_State * L)
{
    lua_pushinteger(L, GetPixelDataSize(luaL_checkinteger(L, 1),
                                        luaL_checkinteger(L, 2),
                                        luaL_checkinteger(L, 3)));
    return 1;
}

static int _GetTextureData(lua_State * L)
{
    auto const image = GetTextureData(luaE_checktexture2d(L, 1));
    luaE_pushimage(L, &image);
    return 1;
}

static int _GetScreenData(lua_State * L)
{
    auto const image = GetScreenData();
    luaE_pushimage(L, &image);
    return 1;
}

static int _UpdateTexture(lua_State * L)
{
    auto tex = luaE_checktexture2d(L, 1);
    auto colors = luaE_checkcolor_array(L, 2);
    UpdateTexture(tex, (void const *)colors.data());
    return 0;
}

static int _ImageCopy(lua_State * L)
{
    auto image = ImageCopy(*luaE_checkimage(L, 1));
    luaE_pushimage(L, &image);
    return 1;
}

static int _ImageToPOT(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageToPOT(image, luaE_checkcolor(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageFormat(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageFormat(image, luaL_checkinteger(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageAlphaMask(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageAlphaMask(image, *luaE_checkimage(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageAlphaClear(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageAlphaClear(image, luaE_checkcolor(L, 2), luaL_checknumber(L, 3));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageAlphaCrop(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageAlphaCrop(image, luaL_checknumber(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageAlphaPremultiply(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageAlphaPremultiply(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageCrop(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageCrop(image, luaE_checkrectangle(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageResize(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageResize(image, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageResizeNN(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageResizeNN(image, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageResizeCanvas(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageResizeCanvas(image,
                      luaL_checkinteger(L, 2),
                      luaL_checkinteger(L, 3),
                      luaL_checkinteger(L, 4),
                      luaL_checkinteger(L, 5),
                      luaE_checkcolor(L, 6));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageMipmaps(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageMipmaps(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageDither(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageDither(image,
                luaL_checkinteger(L, 2),
                luaL_checkinteger(L, 3),
                luaL_checkinteger(L, 4),
                luaL_checkinteger(L, 5));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageExtractPalette(lua_State * L)
{
    int extract_count = 0;
    Color * pixels = ImageExtractPalette(*luaE_checkimage(L, 1), luaL_checkinteger(L, 2), &extract_count);
    assert(pixels != nullptr);

    lua_createtable(L, extract_count, 0);
    for (int i = 0; i < extract_count; ++i) {
        luaE_pushcolor(L, pixels[i]);
        lua_rawseti(L, -2, i+1);
    }
    free(pixels);
    return 1;
}

static int _ImageText(lua_State * L)
{
    auto image = ImageText(luaL_checkstring(L, 1), luaL_checkinteger(L, 2), luaE_checkcolor(L, 3));
    luaE_pushimage(L, &image);
    return 1;
}

static int _ImageTextEx(lua_State * L)
{
    auto image = ImageTextEx(*luaE_checkfont(L, 1),
                             luaL_checkstring(L, 2),
                             luaL_checknumber(L, 3),
                             luaL_checknumber(L, 4),
                             luaE_checkcolor(L, 5));
    luaE_pushimage(L, &image);
    return 1;
}

static int _ImageDraw(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageDraw(image,
              *luaE_checkimage(L, 2),
              luaE_checkrectangle(L, 3),
              luaE_checkrectangle(L, 4));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageDrawRectangle(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageDrawRectangle(image,
                       luaE_checkrectangle(L, 2),
                       luaE_checkcolor(L, 3));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageDrawRectangleLines(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageDrawRectangleLines(image,
                            luaE_checkrectangle(L, 2),
                            luaL_checkinteger(L, 3),
                            luaE_checkcolor(L, 4));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageDrawText(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageDrawText(image,
                  luaE_checkvector2(L, 2),
                  luaL_checkstring(L, 3),
                  luaL_checkinteger(L, 4),
                  luaE_checkcolor(L, 5));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageDrawTextEx(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageDrawTextEx(image,
                    luaE_checkvector2(L, 2),
                    *luaE_checkfont(L, 3),
                    luaL_checkstring(L, 4),
                    luaL_checknumber(L, 5),
                    luaL_checknumber(L, 6),
                    luaE_checkcolor(L, 7));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageFlipVertical(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageFlipVertical(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageFlipHorizontal(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageFlipHorizontal(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageRotateCW(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageRotateCW(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageRotateCCW(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageRotateCCW(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageColorTint(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageColorTint(image, luaE_checkcolor(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageColorInvert(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageColorInvert(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageColorGrayscale(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageColorGrayscale(image);
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageColorContrast(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageColorContrast(image, luaL_checknumber(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageColorBrightness(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageColorBrightness(image, luaL_checkinteger(L, 2));
    luaE_pushimage(L, image);
    return 1;
}

static int _ImageColorReplace(lua_State * L)
{
    auto * image = luaE_checkimage(L, 1);
    ImageColorReplace(image, luaE_checkcolor(L, 2), luaE_checkcolor(L, 3));
    luaE_pushimage(L, image);
    return 1;
}

static int _GenImageColor(lua_State * L)
{
    auto const result = GenImageColor(luaL_checkinteger(L, 1),
                                      luaL_checkinteger(L, 2),
                                      luaE_checkcolor(L, 3));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenImageGradientV(lua_State * L)
{
    auto const result = GenImageGradientV(luaL_checkinteger(L, 1),
                                          luaL_checkinteger(L, 2),
                                          luaE_checkcolor(L, 3),
                                          luaE_checkcolor(L, 4));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenImageGradientH(lua_State * L)
{
    auto const result = GenImageGradientH(luaL_checkinteger(L, 1),
                                          luaL_checkinteger(L, 2),
                                          luaE_checkcolor(L, 3),
                                          luaE_checkcolor(L, 4));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenImageGradientRadial(lua_State * L)
{
    auto const result = GenImageGradientRadial(luaL_checkinteger(L, 1),
                                               luaL_checkinteger(L, 2),
                                               luaL_checknumber(L, 3),
                                               luaE_checkcolor(L, 4),
                                               luaE_checkcolor(L, 5));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenImageChecked(lua_State * L)
{
    auto const result = GenImageChecked(luaL_checkinteger(L, 1),
                                        luaL_checkinteger(L, 2),
                                        luaL_checkinteger(L, 3),
                                        luaL_checkinteger(L, 4),
                                        luaE_checkcolor(L, 5),
                                        luaE_checkcolor(L, 6));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenImageWhiteNoise(lua_State * L)
{
    auto const result = GenImageWhiteNoise(luaL_checkinteger(L, 1),
                                           luaL_checkinteger(L, 2),
                                           luaL_checknumber(L, 3));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenImagePerlinNoise(lua_State * L)
{
    auto const result = GenImagePerlinNoise(luaL_checkinteger(L, 1),
                                            luaL_checkinteger(L, 2),
                                            luaL_checkinteger(L, 3),
                                            luaL_checkinteger(L, 4),
                                            luaL_checknumber(L, 5));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenImageCellular(lua_State * L)
{
    auto const result = GenImageCellular(luaL_checkinteger(L, 1),
                                         luaL_checkinteger(L, 2),
                                         luaL_checkinteger(L, 3));
    luaE_pushimage(L, &result);
    return 1;
}

static int _GenTextureMipmaps(lua_State * L)
{
    //void GenTextureMipmaps(Texture2D * texture);
    return 0;
}

static int _SetTextureFilter(lua_State * L)
{
    SetTextureFilter(luaE_checktexture2d(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _SetTextureWrap(lua_State * L)
{
    SetTextureWrap(luaE_checktexture2d(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _DrawTexture(lua_State * L)
{
    DrawTexture(luaE_checktexture2d(L, 1),
                luaL_checkinteger(L, 2),
                luaL_checkinteger(L, 3),
                luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawTextureV(lua_State * L)
{
    DrawTextureV(luaE_checktexture2d(L, 1),
                 luaE_checkvector2(L, 2),
                 luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawTextureEx(lua_State * L)
{
    DrawTextureEx(luaE_checktexture2d(L, 1),
                  luaE_checkvector2(L, 2),
                  luaL_checknumber(L, 3),
                  luaL_checknumber(L, 4),
                  luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawTextureRec(lua_State * L)
{
    DrawTextureRec(luaE_checktexture2d(L, 1),
                   luaE_checkrectangle(L, 2),
                   luaE_checkvector2(L, 3),
                   luaE_checkcolor(L, 4));
    return 0;
}

static int _DrawTextureQuad(lua_State * L)
{
    DrawTextureQuad(luaE_checktexture2d(L, 1),
                    luaE_checkvector2(L, 2),
                    luaE_checkvector2(L, 3),
                    luaE_checkrectangle(L, 4),
                    luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawTexturePro(lua_State * L)
{
    DrawTexturePro(luaE_checktexture2d(L, 1),
                   luaE_checkrectangle(L, 2),
                   luaE_checkrectangle(L, 3),
                   luaE_checkvector2(L, 4),
                   luaL_checknumber(L, 5),
                   luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawTextureNPatch(lua_State * L)
{
    DrawTextureNPatch(luaE_checktexture2d(L, 1),
                      luaE_checknpatchinfo(L, 2),
                      luaE_checkrectangle(L, 3),
                      luaE_checkvector2(L, 4),
                      luaL_checknumber(L, 5),
                      luaE_checkcolor(L, 6));
    return 0;
}

# /*****************/
# /* [TEXT MODULE] */
# /*****************/

static int _GetFontDefault(lua_State * L)
{
    auto result = GetFontDefault();
    luaE_pushfont(L, &result);
    return 1;
}

static int _LoadFont(lua_State * L)
{
    auto result = LoadFont(luaL_checkstring(L, 1));
    luaE_pushfont(L, &result);
    return 1;
}

static int _LoadFontEx(lua_State * L)
{
    auto file_name = luaL_checkstring(L, 1);
    auto font_size = luaL_checkinteger(L, 2);
    int * font_chars = nullptr;
    int chars_count = 0;

    std::vector<int> chars;
    if (lua_type(L, 3) == LUA_TTABLE) {
        chars = luaE_checkchars_array(L, 3);
        if (!chars.empty()) {
            font_chars = chars.data();
            chars_count = chars.size();
        }
    }

    auto result = LoadFontEx(file_name, font_size, font_chars, chars_count);
    luaE_pushfont(L, &result);
    return 1;
}

static int _LoadFontFromImage(lua_State * L)
{
    auto result = LoadFontFromImage(*luaE_checkimage(L, 1), luaE_checkcolor(L, 2), luaL_checkinteger(L, 3));
    luaE_pushfont(L, &result);
    return 1;
}

static int _LoadFontData(lua_State * L)
{
    char const * file_name = luaL_checkstring(L, 1);
    int font_size = luaL_checkinteger(L, 2);
    int * font_chars = nullptr;
    int chars_count = 0;
    int type = 0;

    std::vector<int> chars;
    int third_argument_type = lua_type(L, 3);
    if (third_argument_type == LUA_TTABLE) {
        chars = luaE_checkchars_array(L, 3);
        if (!chars.empty()) {
            font_chars = chars.data();
            chars_count = chars.size();
        }
        type = luaL_checkinteger(L, 4);
    } else if (third_argument_type == LUA_TNUMBER) {
        type = luaL_checkinteger(L, 3);
    } else {
        luaL_error(L, "Argument error");
        return 0;
    }

    CharInfo * infos = LoadFontData(file_name, font_size, font_chars, chars_count, type);
    if (chars_count == 0) {
        return 0;
    }

    luaE_pushcharinfo_array(L, infos, chars_count);
    return 1;
}

static int _GenImageFontAtlas(lua_State * L)
{
    auto chars = luaE_checkcharinfo_array(L, 1);
    auto image = GenImageFontAtlas(chars.data(), chars.size(),
                                   luaL_checkinteger(L, 2),
                                   luaL_checkinteger(L, 3),
                                   luaL_checkinteger(L, 4));
    luaE_pushimage(L, &image);
    return 1;
}

static int _UnloadFont(lua_State * L)
{
    UnloadFont(*luaE_checkfont(L, 1));
    return 0;
}

static int _DrawFPS(lua_State * L)
{
    DrawFPS(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _DrawText(lua_State * L)
{
    DrawText(luaL_checkstring(L, 1),
             luaL_checkinteger(L, 2),
             luaL_checkinteger(L, 3),
             luaL_checkinteger(L, 4),
             luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawTextEx(lua_State * L)
{
    DrawTextEx(*luaE_checkfont(L, 1),
               luaL_checkstring(L, 2),
               luaE_checkvector2(L, 3),
               luaL_checknumber(L, 4),
               luaL_checknumber(L, 5),
               luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawTextRec(lua_State * L)
{
    DrawTextRec(*luaE_checkfont(L, 1),
                luaL_checkstring(L, 2),
                luaE_checkrectangle(L, 3),
                luaL_checknumber(L, 4),
                luaL_checknumber(L, 5),
                lua_toboolean(L, 6),
                luaE_checkcolor(L, 7));
    return 0;
}

static int _DrawTextRecEx(lua_State * L)
{
    DrawTextRecEx(*luaE_checkfont(L, 1),
                  luaL_checkstring(L, 2),
                  luaE_checkrectangle(L, 3),
                  luaL_checknumber(L, 4),
                  luaL_checknumber(L, 5),
                  lua_toboolean(L, 6),
                  luaE_checkcolor(L, 7),
                  luaL_checkinteger(L, 8),
                  luaL_checkinteger(L, 9),
                  luaE_checkcolor(L, 10),
                  luaE_checkcolor(L, 11));
    return 0;
}

static int _MeasureText(lua_State * L)
{
    lua_pushinteger(L, MeasureText(luaL_checkstring(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _MeasureTextEx(lua_State * L)
{
    luaE_pushvector2(L, MeasureTextEx(*luaE_checkfont(L, 1),
                                      luaL_checkstring(L, 2),
                                      luaL_checknumber(L, 3),
                                      luaL_checknumber(L, 4)));
    return 1;
}

static int _GetGlyphIndex(lua_State * L)
{
    lua_pushinteger(L, GetGlyphIndex(*luaE_checkfont(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _TextIsEqual(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextLength(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextSubtext(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextReplace(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextInsert(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextJoin(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextSplit(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextAppend(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextFindIndex(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextToUpper(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextToLower(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextToPascal(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _TextToInteger(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

# /*******************/
# /* [MODELS MODULE] */
# /*******************/

static int _DrawLine3D(lua_State * L)
{
    DrawLine3D(luaE_checkvector3(L, 1),
               luaE_checkvector3(L, 2),
               luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawCircle3D(lua_State * L)
{
    DrawCircle3D(luaE_checkvector3(L, 1),
                 luaL_checknumber(L, 2),
                 luaE_checkvector3(L, 3),
                 luaL_checknumber(L, 4),
                 luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawCube(lua_State * L)
{
    DrawCube(luaE_checkvector3(L, 1),
             luaL_checknumber(L, 2),
             luaL_checknumber(L, 3),
             luaL_checknumber(L, 4),
             luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawCubeV(lua_State * L)
{
    DrawCubeV(luaE_checkvector3(L, 1),
              luaE_checkvector3(L, 2),
              luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawCubeWires(lua_State * L)
{
    DrawCubeWires(luaE_checkvector3(L, 1),
                  luaL_checknumber(L, 2),
                  luaL_checknumber(L, 3),
                  luaL_checknumber(L, 4),
                  luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawCubeWiresV(lua_State * L)
{
    DrawCubeWiresV(luaE_checkvector3(L, 1),
                   luaE_checkvector3(L, 2),
                   luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawCubeTexture(lua_State * L)
{
    DrawCubeTexture(luaE_checktexture2d(L, 1),
                    luaE_checkvector3(L, 2),
                    luaL_checknumber(L, 3),
                    luaL_checknumber(L, 4),
                    luaL_checknumber(L, 5),
                    luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawSphere(lua_State * L)
{
    DrawSphere(luaE_checkvector3(L, 1),
               luaL_checknumber(L, 2),
               luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawSphereEx(lua_State * L)
{
    DrawSphereEx(luaE_checkvector3(L, 1),
                 luaL_checknumber(L, 2),
                 luaL_checkinteger(L, 3),
                 luaL_checkinteger(L, 4),
                 luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawSphereWires(lua_State * L)
{
    DrawSphereWires(luaE_checkvector3(L, 1),
                    luaL_checknumber(L, 2),
                    luaL_checkinteger(L, 3),
                    luaL_checkinteger(L, 4),
                    luaE_checkcolor(L, 5));
    return 0;
}

static int _DrawCylinder(lua_State * L)
{
    DrawCylinder(luaE_checkvector3(L, 1),
                 luaL_checknumber(L, 2),
                 luaL_checknumber(L, 3),
                 luaL_checknumber(L, 4),
                 luaL_checkinteger(L, 5),
                 luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawCylinderWires(lua_State * L)
{
    DrawCylinderWires(luaE_checkvector3(L, 1),
                      luaL_checknumber(L, 2),
                      luaL_checknumber(L, 3),
                      luaL_checknumber(L, 4),
                      luaL_checkinteger(L, 5),
                      luaE_checkcolor(L, 6));
    return 0;
}

static int _DrawPlane(lua_State * L)
{
    DrawPlane(luaE_checkvector3(L, 1),
              luaE_checkvector2(L, 2),
              luaE_checkcolor(L, 3));
    return 0;
}

static int _DrawRay(lua_State * L)
{
    DrawRay(luaE_checkray(L, 1),
            luaE_checkcolor(L, 2));
    return 0;
}

static int _DrawGrid(lua_State * L)
{
    DrawGrid(luaL_checkinteger(L, 1),
             luaL_checknumber(L, 2));
    return 0;
}

static int _DrawGizmo(lua_State * L)
{
    DrawGizmo(luaE_checkvector3(L, 1));
    return 0;
}

static int _LoadModel(lua_State * L)
{
    auto mesh = LoadModel(luaL_checkstring(L, 1));
    luaE_pushmodel(L, &mesh);
    return 1;
}

static int _LoadModelFromMesh(lua_State * L)
{
    auto mesh = LoadModelFromMesh(*luaE_checkmesh(L, 1));
    luaE_pushmodel(L, &mesh);
    return 1;
}

static int _UnloadModel(lua_State * L)
{
    UnloadModel(*luaE_checkmodel(L, 1));
    return 0;
}

//Mesh * LoadMeshes(char const * file_name, int * mesh_count);
//void ExportMesh(Mesh mesh, char const * file_name);
//void UnloadMesh(Mesh * mesh);

//Material * LoadMaterials(char const * file_name, int * material_count);
//Material LoadMaterialDefault();
//void UnloadMaterial(Material material);
//void SetMaterialTexture(Material * material, int map_type, Texture2D texture);
//void SetModelMeshMaterial(Model * model, int meshId, int materialId);
//
//ModelAnimation * LoadModelAnimations(char const * file_name, int * anims_count);
//void UpdateModelAnimation(Model model, ModelAnimation anim, int frame);
//void UnloadModelAnimation(ModelAnimation anim);
//bool IsModelAnimationValid(Model model, ModelAnimation anim);
//
//Mesh GenMeshPoly(int sides, float radius);
//Mesh GenMeshPlane(float width, float length, int res_x, int res_z);
//Mesh GenMeshCube(float width, float height, float length);
//Mesh GenMeshSphere(float radius, int rings, int slices);
//Mesh GenMeshHemiSphere(float radius, int rings, int slices);
//Mesh GenMeshCylinder(float radius, float height, int slices);
//Mesh GenMeshTorus(float radius, float size, int rad_seg, int sides);
//Mesh GenMeshKnot(float radius, float size, int rad_seg, int sides);
//Mesh GenMeshHeightmap(Image heightmap, Vector3 size);
//Mesh GenMeshCubicmap(Image cubicmap, Vector3 cube_size);
//
//BoundingBox MeshBoundingBox(Mesh mesh);
//void MeshTangents(Mesh * mesh);
//void MeshBinormals(Mesh * mesh);
//
//void DrawModel(Model model, Vector3 position, float scale, Color tint);
//void DrawModelEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle, Vector3 scale, Color tint);
//void DrawModelWires(Model model, Vector3 position, float scale, Color tint);
//void DrawModelWiresEx(Model model, Vector3 position, Vector3 rotation_axis, float rotation_angle, Vector3 scale, Color tint);
//void DrawBoundingBox(BoundingBox box, Color color);
//void DrawBillboard(Camera camera, Texture2D texture, Vector3 center, float size, Color tint);
//void DrawBillboardRec(Camera camera, Texture2D texture, Rectangle2 source_rec, Vector3 center, float size, Color tint);
//
//bool CheckCollisionSpheres(Vector3 center_a, float radius_a, Vector3 center_b, float radius_b);
//bool CheckCollisionBoxes(BoundingBox box1, BoundingBox box2);
//bool CheckCollisionBoxSphere(BoundingBox box, Vector3 center_sphere, float radius_sphere);
//bool CheckCollisionRaySphere(Ray ray, Vector3 sphere_position, float sphere_radius);
//bool CheckCollisionRaySphereEx(Ray ray, Vector3 sphere_position, float sphere_radius, Vector3 * collision_point);
//bool CheckCollisionRayBox(Ray ray, BoundingBox box);
//RayHitInfo GetCollisionRayModel(Ray ray, Model * model);
//RayHitInfo GetCollisionRayTriangle(Ray ray, Vector3 p1, Vector3 p2, Vector3 p3);
//RayHitInfo GetCollisionRayGround(Ray ray, float ground_height);

# /********************/
# /* [SHADERS MODULE] */
# /********************/

static int _LoadText(lua_State * L)
{
    lua_pushstring(L, LoadText(luaL_checkstring(L, 1)));
    return 1;
}

static int _LoadShader(lua_State * L)
{
    luaE_pushshader(L, LoadShader(luaL_checkstring(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

static int _LoadShaderCode(lua_State * L)
{
    std::string vs_code = luaL_checkstring(L, 1);
    std::string fs_code = luaL_checkstring(L, 2);
    luaE_pushshader(L, LoadShaderCode(&vs_code[0], &fs_code[0]));
    return 1;
}

static int _UnloadShader(lua_State * L)
{
    UnloadShader(luaE_checkshader(L, 1));
    return 0;
}

static int _GetShaderDefault(lua_State * L)
{
    luaE_pushshader(L, GetShaderDefault());
    return 1;
}

static int _GetTextureDefault(lua_State * L)
{
    luaE_pushtexture2d(L, GetTextureDefault());
    return 1;
}

static int _GetShaderLocation(lua_State * L)
{
    lua_pushinteger(L, GetShaderLocation(luaE_checkshader(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

static int _SetShaderValue(lua_State * L)
{
    // TODO: Implement
    return 0;
}

static int _SetShaderValueV(lua_State * L)
{
    // TODO: Implement
    return 0;
}

static int _SetShaderValueMatrix(lua_State * L)
{
    SetShaderValueMatrix(luaE_checkshader(L, 1),
                         luaL_checkinteger(L, 2),
                         luaE_checkmatrix(L, 3));
    return 0;
}

static int _SetShaderValueTexture(lua_State * L)
{
    SetShaderValueTexture(luaE_checkshader(L, 1),
                          luaL_checkinteger(L, 2),
                          luaE_checktexture2d(L, 3));
    return 0;
}

static int _SetMatrixProjection(lua_State * L)
{
    SetMatrixProjection(luaE_checkmatrix(L, 1));
    return 0;
}

static int _SetMatrixModelview(lua_State * L)
{
    SetMatrixModelview(luaE_checkmatrix(L, 1));
    return 0;
}

static int _GetMatrixModelview(lua_State * L)
{
    luaE_pushmatrix(L, GetMatrixModelview());
    return 1;
}

static int _GenTextureCubemap(lua_State * L)
{
    luaE_pushtexture2d(L, GenTextureCubemap(luaE_checkshader(L, 1),
                                            luaE_checktexture2d(L, 2),
                                            luaL_checkinteger(L, 3)));
    return 1;
}

static int _GenTextureIrradiance(lua_State * L)
{
    luaE_pushtexture2d(L, GenTextureIrradiance(luaE_checkshader(L, 1),
                                               luaE_checktexture2d(L, 2),
                                               luaL_checkinteger(L, 3)));
    return 1;
}

static int _GenTexturePrefilter(lua_State * L)
{
    luaE_pushtexture2d(L, GenTexturePrefilter(luaE_checkshader(L, 1),
                                              luaE_checktexture2d(L, 2),
                                              luaL_checkinteger(L, 3)));
    return 1;
}

static int _GenTextureBRDF(lua_State * L)
{
    luaE_pushtexture2d(L, GenTextureBRDF(luaE_checkshader(L, 1),
                                         luaL_checkinteger(L, 2)));
    return 1;
}

static int _BeginShaderMode(lua_State * L)
{
    BeginShaderMode(luaE_checkshader(L, 1));
    return 0;
}

static int _EndShaderMode(lua_State * L)
{
    EndShaderMode();
    return 0;
}

static int _BeginBlendMode(lua_State * L)
{
    BeginBlendMode(luaL_checkinteger(L, 1));
    return 0;
}

static int _EndBlendMode(lua_State * L)
{
    EndBlendMode();
    return 0;
}

static int _BeginScissorMode(lua_State * L)
{
    BeginScissorMode(luaL_checkinteger(L, 1),
                     luaL_checkinteger(L, 2),
                     luaL_checkinteger(L, 3),
                     luaL_checkinteger(L, 4));
    return 0;
}

static int _EndScissorMode(lua_State * L)
{
    EndScissorMode();
    return 0;
}

static int _GetVrDeviceInfo(lua_State * L)
{
    luaE_pushvrdeviceinfo(L, GetVrDeviceInfo(luaL_checkinteger(L, 1)));
    return 1;
}

static int _InitVrSimulator(lua_State * L)
{
    InitVrSimulator(luaE_checkvrdeviceinfo(L, 1));
    return 0;
}

static int _UpdateVrTracking(lua_State * L)
{
    auto cam = luaE_checkcamera3d(L, 1);
    UpdateVrTracking(&cam);
    luaE_pushcamera3d(L, cam);
    return 1;
}

static int _SetVrConfiguration(lua_State * L)
{
    // SetVrConfiguration(VrDeviceInfo info, Shader distortion)
    luaE_unsupport(L);
    return 0;
}

static int _CloseVrSimulator(lua_State * L)
{
    CloseVrSimulator();
    return 0;
}

static int _IsVrSimulatorReady(lua_State * L)
{
    lua_pushboolean(L, IsVrSimulatorReady());
    return 1;
}

static int _ToggleVrMode(lua_State * L)
{
    ToggleVrMode();
    return 0;
}

static int _BeginVrDrawing(lua_State * L)
{
    BeginVrDrawing();
    return 0;
}

static int _EndVrDrawing(lua_State * L)
{
    EndVrDrawing();
    return 0;
}

# /******************/
# /* [AUDIO MODULE] */
# /******************/

static int _InitAudioDevice(lua_State * L)
{
    InitAudioDevice();
    return 0;
}

static int _CloseAudioDevice(lua_State * L)
{
    CloseAudioDevice();
    return 0;
}

static int _IsAudioDeviceReady(lua_State * L)
{
    lua_pushboolean(L, IsAudioDeviceReady());
    return 1;
}

static int _SetMasterVolume(lua_State * L)
{
    SetMasterVolume(luaL_checknumber(L, 1));
    return 0;
}

static int _LoadWave(lua_State * L)
{
    auto result = LoadWave(luaL_checkstring(L, 1));
    luaE_pushwave(L, &result);
    return 1;
}

static int _LoadWaveEx(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _LoadSound(lua_State * L)
{
    auto result = LoadSound(luaL_checkstring(L, 1));
    luaE_pushsound(L, &result);
    return 1;
}

static int _LoadSoundFromWave(lua_State * L)
{
    auto result = LoadSoundFromWave(*luaE_checkwave(L, 1));
    luaE_pushsound(L, &result);
    return 1;
}

static int _UpdateSound(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _UnloadWave(lua_State * L)
{
    UnloadWave(*luaE_checkwave(L, 1));
    return 0;
}

static int _UnloadSound(lua_State * L)
{
    UnloadSound(*luaE_checksound(L, 1));
    return 0;
}

static int _ExportWave(lua_State * L)
{
    ExportWave(*luaE_checkwave(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _ExportWaveAsCode(lua_State * L)
{
    ExportWaveAsCode(*luaE_checkwave(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _PlaySound(lua_State * L)
{
    PlaySound(*luaE_checksound(L, 1));
    return 0;
}

static int _PauseSound(lua_State * L)
{
    PauseSound(*luaE_checksound(L, 1));
    return 0;
}

static int _ResumeSound(lua_State * L)
{
    ResumeSound(*luaE_checksound(L, 1));
    return 0;
}

static int _StopSound(lua_State * L)
{
    StopSound(*luaE_checksound(L, 1));
    return 0;
}

static int _IsSoundPlaying(lua_State * L)
{
    lua_pushboolean(L, IsSoundPlaying(*luaE_checksound(L, 1)));
    return 1;
}

static int _SetSoundVolume(lua_State * L)
{
    SetSoundVolume(*luaE_checksound(L, 1), luaL_checknumber(L, 2));
    return 0;
}

static int _SetSoundPitch(lua_State * L)
{
    SetSoundPitch(*luaE_checksound(L, 1), luaL_checknumber(L, 2));
    return 0;
}

static int _WaveFormat(lua_State * L)
{
    WaveFormat(luaE_checkwave(L, 1),
               luaL_checkinteger(L, 2),
               luaL_checkinteger(L, 3),
               luaL_checkinteger(L, 4));
    return 0;
}

static int _WaveCopy(lua_State * L)
{
    auto result = WaveCopy(*luaE_checkwave(L, 1));
    luaE_pushwave(L, &result);
    return 1;
}

static int _WaveCrop(lua_State * L)
{
    WaveCrop(luaE_checkwave(L, 1),
             luaL_checkinteger(L, 2),
             luaL_checkinteger(L, 3));
    return 0;
}

static int _GetWaveData(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _LoadMusicStream(lua_State * L)
{
    auto result = LoadMusicStream(luaL_checkstring(L, 1));
    MusicWrapper wrapper = {0,};
    wrapper.music = result;
    luaE_pushmusic(L, &wrapper);
    return 1;
}

static int _UnloadMusicStream(lua_State * L)
{
    UnloadMusicStream(luaE_checkmusic(L, 1)->music);
    return 0;
}

static int _PlayMusicStream(lua_State * L)
{
    PlayMusicStream(luaE_checkmusic(L, 1)->music);
    return 0;
}

static int _UpdateMusicStream(lua_State * L)
{
    UpdateMusicStream(luaE_checkmusic(L, 1)->music);
    return 0;
}

static int _StopMusicStream(lua_State * L)
{
    StopMusicStream(luaE_checkmusic(L, 1)->music);
    return 0;
}

static int _PauseMusicStream(lua_State * L)
{
    PauseMusicStream(luaE_checkmusic(L, 1)->music);
    return 0;
}

static int _ResumeMusicStream(lua_State * L)
{
    ResumeMusicStream(luaE_checkmusic(L, 1)->music);
    return 0;
}

static int _IsMusicPlaying(lua_State * L)
{
    lua_pushboolean(L, IsMusicPlaying(luaE_checkmusic(L, 1)->music)?1:0);
    return 1;
}

static int _SetMusicVolume(lua_State * L)
{
    SetMusicVolume(luaE_checkmusic(L, 1)->music, luaL_checknumber(L, 2));
    return 0;
}

static int _SetMusicPitch(lua_State * L)
{
    SetMusicPitch(luaE_checkmusic(L, 1)->music, luaL_checknumber(L, 2));
    return 0;
}

static int _SetMusicLoopCount(lua_State * L)
{
    SetMusicLoopCount(luaE_checkmusic(L, 1)->music, luaL_checkinteger(L, 2));
    return 0;
}

static int _GetMusicTimeLength(lua_State * L)
{
    lua_pushnumber(L, GetMusicTimeLength(luaE_checkmusic(L, 1)->music));
    return 1;
}

static int _GetMusicTimePlayed(lua_State * L)
{
    lua_pushnumber(L, GetMusicTimePlayed(luaE_checkmusic(L, 1)->music));
    return 1;
}

static int _InitAudioStream(lua_State * L)
{
    auto result = InitAudioStream(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    luaE_pushaudiostream(L, &result);
    return 1;
}

static int _UpdateAudioStream(lua_State * L)
{
    luaE_unsupport(L);
    return 0;
}

static int _CloseAudioStream(lua_State * L)
{
    CloseAudioStream(*luaE_checkaudiostream(L, 1));
    return 0;
}

static int _IsAudioBufferProcessed(lua_State * L)
{
    lua_pushboolean(L, IsAudioBufferProcessed(*luaE_checkaudiostream(L, 1))?1:0);
    return 1;
}

static int _PlayAudioStream(lua_State * L)
{
    PlayAudioStream(*luaE_checkaudiostream(L, 1));
    return 0;
}

static int _PauseAudioStream(lua_State * L)
{
    PauseAudioStream(*luaE_checkaudiostream(L, 1));
    return 0;
}

static int _ResumeAudioStream(lua_State * L)
{
    ResumeAudioStream(*luaE_checkaudiostream(L, 1));
    return 0;
}

static int _IsAudioStreamPlaying(lua_State * L)
{
    lua_pushboolean(L, IsAudioStreamPlaying(*luaE_checkaudiostream(L, 1))?1:0);
    return 1;
}

static int _StopAudioStream(lua_State * L)
{
    StopAudioStream(*luaE_checkaudiostream(L, 1));
    return 0;
}

static int _SetAudioStreamVolume(lua_State * L)
{
    SetAudioStreamVolume(*luaE_checkaudiostream(L, 1), luaL_checknumber(L, 2));
    return 0;
}

static int _SetAudioStreamPitch(lua_State * L)
{
    SetAudioStreamPitch(*luaE_checkaudiostream(L, 1), luaL_checknumber(L, 2));
    return 0;
}

#ifndef RAY_REGISTER
#define RAY_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_lay[] = {
        // clang-format off
        { METATABLE_IMAGE         , _Image          },
        { METATABLE_CHARINFO      , _CharInfo       },
        { METATABLE_FONT          , _Font           },
        { METATABLE_MESH          , _Mesh           },
        { METATABLE_MATERIAL      , _Material       },
        { METATABLE_MODEL         , _Model          },
        { METATABLE_MODELANIMATION, _ModelAnimation },
        { METATABLE_WAVE          , _Wave           },
        { METATABLE_SOUND         , _Sound          },
        { METATABLE_AUDIOSTREAM   , _AudioStream    },
        // clang-format on

        // [CORE] Window-related functions
        RAY_REGISTER(InitWindow),
        RAY_REGISTER(WindowShouldClose),
        RAY_REGISTER(CloseWindow),
        RAY_REGISTER(IsWindowReady),
        RAY_REGISTER(IsWindowMinimized),
        RAY_REGISTER(IsWindowResized),
        RAY_REGISTER(IsWindowHidden),
        RAY_REGISTER(ToggleFullscreen),
        RAY_REGISTER(UnhideWindow),
        RAY_REGISTER(HideWindow),
        RAY_REGISTER(SetWindowIcon),
        RAY_REGISTER(SetWindowTitle),
        RAY_REGISTER(SetWindowPosition),
        RAY_REGISTER(SetWindowMonitor),
        RAY_REGISTER(SetWindowMinSize),
        RAY_REGISTER(SetWindowSize),
        RAY_REGISTER(GetScreenWidth),
        RAY_REGISTER(GetScreenHeight),
        RAY_REGISTER(GetMonitorCount),
        RAY_REGISTER(GetMonitorWidth),
        RAY_REGISTER(GetMonitorHeight),
        RAY_REGISTER(GetMonitorPhysicalWidth),
        RAY_REGISTER(GetMonitorPhysicalHeight),
        RAY_REGISTER(GetMonitorName),
        RAY_REGISTER(GetClipboardText),
        RAY_REGISTER(SetClipboardText),

        // [CORE] Cursor-related functions
        RAY_REGISTER(ShowCursor),
        RAY_REGISTER(HideCursor),
        RAY_REGISTER(IsCursorHidden),
        RAY_REGISTER(EnableCursor),
        RAY_REGISTER(DisableCursor),

        // [CORE] Drawing-related functions
        RAY_REGISTER(ClearBackground),
        RAY_REGISTER(BeginDrawing),
        RAY_REGISTER(EndDrawing),
        RAY_REGISTER(BeginMode2D),
        RAY_REGISTER(EndMode2D),
        RAY_REGISTER(BeginMode3D),
        RAY_REGISTER(EndMode3D),
        RAY_REGISTER(BeginTextureMode),
        RAY_REGISTER(EndTextureMode),

        // [CORE] Screen-space-related functions
        RAY_REGISTER(GetMouseRay),
        RAY_REGISTER(GetWorldToScreen),
        RAY_REGISTER(GetCameraMatrix),

        // [CORE] Timing-related functions
        RAY_REGISTER(SetTargetFPS),
        RAY_REGISTER(GetFPS),
        RAY_REGISTER(GetFrameTime),
        RAY_REGISTER(GetTime),

        // [CORE] Color-related functions
        RAY_REGISTER(ColorToInt),
        RAY_REGISTER(ColorNormalize),
        RAY_REGISTER(ColorToHSV),
        RAY_REGISTER(ColorFromHSV),
        RAY_REGISTER(GetColor),
        RAY_REGISTER(Fade),

        // [CORE] Misc. functions
        RAY_REGISTER(SetConfigFlags),
        RAY_REGISTER(SetTraceLogLevel),
        RAY_REGISTER(SetTraceLogExit),
        RAY_REGISTER(SetTraceLogCallback), // Not implementation.
        RAY_REGISTER(TraceLog),
        RAY_REGISTER(TakeScreenshot),
        RAY_REGISTER(GetRandomValue),

        // [CORE] Files management functions
        RAY_REGISTER(FileExists),
        RAY_REGISTER(IsFileExtension),
        RAY_REGISTER(GetExtension),
        RAY_REGISTER(GetFileName),
        RAY_REGISTER(GetFileNameWithoutExt),
        RAY_REGISTER(GetDirectoryPath),
        RAY_REGISTER(GetWorkingDirectory),
        RAY_REGISTER(GetDirectoryFiles),
        RAY_REGISTER(ClearDirectoryFiles),
        RAY_REGISTER(ChangeDirectory),
        RAY_REGISTER(IsFileDropped),
        RAY_REGISTER(GetDroppedFiles),
        RAY_REGISTER(ClearDroppedFiles),
        RAY_REGISTER(GetFileModTime),

        // [CORE] Persistent storage management
        RAY_REGISTER(StorageSaveValue),
        RAY_REGISTER(StorageLoadValue),

        // [CORE] Web browser
        RAY_REGISTER(OpenURL),

        // [CORE] Input-related functions: keyboard
        RAY_REGISTER(IsKeyPressed),
        RAY_REGISTER(IsKeyDown),
        RAY_REGISTER(IsKeyReleased),
        RAY_REGISTER(IsKeyUp),
        RAY_REGISTER(GetKeyPressed),
        RAY_REGISTER(SetExitKey),

        // [CORE] Input-related functions: gamepads
        RAY_REGISTER(IsGamepadAvailable),
        RAY_REGISTER(IsGamepadName),
        RAY_REGISTER(GetGamepadName),
        RAY_REGISTER(IsGamepadButtonPressed),
        RAY_REGISTER(IsGamepadButtonDown),
        RAY_REGISTER(IsGamepadButtonReleased),
        RAY_REGISTER(IsGamepadButtonUp),
        RAY_REGISTER(GetGamepadButtonPressed),
        RAY_REGISTER(GetGamepadAxisCount),
        RAY_REGISTER(GetGamepadAxisMovement),

        // [CORE] Input-related functions: mouse
        RAY_REGISTER(IsMouseButtonPressed),
        RAY_REGISTER(IsMouseButtonDown),
        RAY_REGISTER(IsMouseButtonReleased),
        RAY_REGISTER(IsMouseButtonUp),
        RAY_REGISTER(GetMouseX),
        RAY_REGISTER(GetMouseY),
        RAY_REGISTER(GetMousePosition),
        RAY_REGISTER(SetMousePosition),
        RAY_REGISTER(SetMouseOffset),
        RAY_REGISTER(SetMouseScale),
        RAY_REGISTER(GetMouseWheelMove),

        // [CORE] Input-related functions: touch
        RAY_REGISTER(GetTouchX),
        RAY_REGISTER(GetTouchY),
        RAY_REGISTER(GetTouchPosition),

        // [CORE] Gestures and Touch Handling Functions (Module: gestures)
        RAY_REGISTER(SetGesturesEnabled),
        RAY_REGISTER(IsGestureDetected),
        RAY_REGISTER(GetGestureDetected),
        RAY_REGISTER(GetTouchPointsCount),
        RAY_REGISTER(GetGestureHoldDuration),
        RAY_REGISTER(GetGestureDragVector),
        RAY_REGISTER(GetGestureDragAngle),
        RAY_REGISTER(GetGesturePinchVector),
        RAY_REGISTER(GetGesturePinchAngle),

        // [CORE] Camera System Functions (Module: camera)
        RAY_REGISTER(SetCameraMode),
        RAY_REGISTER(UpdateCamera),
        RAY_REGISTER(SetCameraPanControl),
        RAY_REGISTER(SetCameraAltControl),
        RAY_REGISTER(SetCameraSmoothZoomControl),
        RAY_REGISTER(SetCameraMoveControls),

        // [SHAPES] Basic shapes drawing functions
        RAY_REGISTER(DrawPixel),
        RAY_REGISTER(DrawPixelV),
        RAY_REGISTER(DrawLine),
        RAY_REGISTER(DrawLineV),
        RAY_REGISTER(DrawLineEx),
        RAY_REGISTER(DrawLineBezier),
        RAY_REGISTER(DrawCircle),
        RAY_REGISTER(DrawCircleSector),
        RAY_REGISTER(DrawCircleSectorLines),
        RAY_REGISTER(DrawCircleGradient),
        RAY_REGISTER(DrawCircleV),
        RAY_REGISTER(DrawCircleLines),
        RAY_REGISTER(DrawRing),
        RAY_REGISTER(DrawRingLines),
        RAY_REGISTER(DrawRectangle),
        RAY_REGISTER(DrawRectangleV),
        RAY_REGISTER(DrawRectangleRec),
        RAY_REGISTER(DrawRectanglePro),
        RAY_REGISTER(DrawRectangleGradientV),
        RAY_REGISTER(DrawRectangleGradientH),
        RAY_REGISTER(DrawRectangleGradientEx),
        RAY_REGISTER(DrawRectangleLines),
        RAY_REGISTER(DrawRectangleLinesEx),
        RAY_REGISTER(DrawRectangleRounded),
        RAY_REGISTER(DrawRectangleRoundedLines),
        RAY_REGISTER(DrawTriangle),
        RAY_REGISTER(DrawTriangleLines),
        RAY_REGISTER(DrawPoly),
        RAY_REGISTER(DrawPolyEx),
        RAY_REGISTER(DrawPolyExLines),
        RAY_REGISTER(SetShapesTexture),

        // [SHAPES] Basic shapes collision detection functions
        RAY_REGISTER(CheckCollisionRecs),
        RAY_REGISTER(CheckCollisionCircles),
        RAY_REGISTER(CheckCollisionCircleRec),
        RAY_REGISTER(GetCollisionRec),
        RAY_REGISTER(CheckCollisionPointRec),
        RAY_REGISTER(CheckCollisionPointCircle),
        RAY_REGISTER(CheckCollisionPointTriangle),

        // [TEXTURES] Image/Texture2D data loading/unloading/saving functions
        RAY_REGISTER(LoadImage),
        RAY_REGISTER(LoadImageEx),
        RAY_REGISTER(LoadImagePro), // Not implementation.
        RAY_REGISTER(LoadImageRaw),
        RAY_REGISTER(ExportImage),
        RAY_REGISTER(ExportImageAsCode),
        RAY_REGISTER(LoadTexture),
        RAY_REGISTER(LoadTextureFromImage),
        RAY_REGISTER(LoadTextureCubemap),
        RAY_REGISTER(LoadRenderTexture),
        RAY_REGISTER(UnloadImage),
        RAY_REGISTER(UnloadTexture),
        RAY_REGISTER(UnloadRenderTexture),
        RAY_REGISTER(GetImageData),
        RAY_REGISTER(GetImageDataNormalized),
        RAY_REGISTER(GetPixelDataSize),
        RAY_REGISTER(GetTextureData),
        RAY_REGISTER(GetScreenData),
        RAY_REGISTER(UpdateTexture),

        // [TEXTURES] Image manipulation functions
        RAY_REGISTER(ImageCopy),
        RAY_REGISTER(ImageToPOT),
        RAY_REGISTER(ImageFormat),
        RAY_REGISTER(ImageAlphaMask),
        RAY_REGISTER(ImageAlphaClear),
        RAY_REGISTER(ImageAlphaCrop),
        RAY_REGISTER(ImageAlphaPremultiply),
        RAY_REGISTER(ImageCrop),
        RAY_REGISTER(ImageResize),
        RAY_REGISTER(ImageResizeNN),
        RAY_REGISTER(ImageResizeCanvas),
        RAY_REGISTER(ImageMipmaps),
        RAY_REGISTER(ImageDither),
        RAY_REGISTER(ImageExtractPalette),
        RAY_REGISTER(ImageText),
        RAY_REGISTER(ImageTextEx),
        RAY_REGISTER(ImageDraw),
        RAY_REGISTER(ImageDrawRectangle),
        RAY_REGISTER(ImageDrawRectangleLines),
        RAY_REGISTER(ImageDrawText),
        RAY_REGISTER(ImageDrawTextEx),
        RAY_REGISTER(ImageFlipVertical),
        RAY_REGISTER(ImageFlipHorizontal),
        RAY_REGISTER(ImageRotateCW),
        RAY_REGISTER(ImageRotateCCW),
        RAY_REGISTER(ImageColorTint),
        RAY_REGISTER(ImageColorInvert),
        RAY_REGISTER(ImageColorGrayscale),
        RAY_REGISTER(ImageColorContrast),
        RAY_REGISTER(ImageColorBrightness),
        RAY_REGISTER(ImageColorReplace),

        // [TEXTURES] Image generation functions
        RAY_REGISTER(GenImageColor),
        RAY_REGISTER(GenImageGradientV),
        RAY_REGISTER(GenImageGradientH),
        RAY_REGISTER(GenImageGradientRadial),
        RAY_REGISTER(GenImageChecked),
        RAY_REGISTER(GenImageWhiteNoise),
        RAY_REGISTER(GenImagePerlinNoise),
        RAY_REGISTER(GenImageCellular),

        // [TEXTURES] Texture2D configuration functions
        RAY_REGISTER(GenTextureMipmaps),
        RAY_REGISTER(SetTextureFilter),
        RAY_REGISTER(SetTextureWrap),

        // [TEXTURES] Texture2D drawing functions
        RAY_REGISTER(DrawTexture),
        RAY_REGISTER(DrawTextureV),
        RAY_REGISTER(DrawTextureEx),
        RAY_REGISTER(DrawTextureRec),
        RAY_REGISTER(DrawTextureQuad),
        RAY_REGISTER(DrawTexturePro),
        RAY_REGISTER(DrawTextureNPatch),

        // [TEXT] Font loading/unloading functions
        RAY_REGISTER(GetFontDefault),
        RAY_REGISTER(LoadFont),
        RAY_REGISTER(LoadFontEx),
        RAY_REGISTER(LoadFontFromImage),
        RAY_REGISTER(LoadFontData),
        RAY_REGISTER(GenImageFontAtlas),
        RAY_REGISTER(UnloadFont),

        // [TEXT] Text drawing functions
        RAY_REGISTER(DrawFPS),
        RAY_REGISTER(DrawText),
        RAY_REGISTER(DrawTextEx),
        RAY_REGISTER(DrawTextRec),
        RAY_REGISTER(DrawTextRecEx),

        // [TEXT] Text misc. functions
        RAY_REGISTER(MeasureText),
        RAY_REGISTER(MeasureTextEx),
        RAY_REGISTER(GetGlyphIndex),

        // [TEXT] Text strings management functions - (NOT IMPLEMENTATION)
        RAY_REGISTER(TextIsEqual),
        RAY_REGISTER(TextLength),
        RAY_REGISTER(TextSubtext),
        RAY_REGISTER(TextReplace),
        RAY_REGISTER(TextInsert),
        RAY_REGISTER(TextJoin),
        RAY_REGISTER(TextSplit),
        RAY_REGISTER(TextAppend),
        RAY_REGISTER(TextFindIndex),
        RAY_REGISTER(TextToUpper),
        RAY_REGISTER(TextToLower),
        RAY_REGISTER(TextToPascal),
        RAY_REGISTER(TextToInteger),

        // [MODELS] Basic geometric 3D shapes drawing functions
        RAY_REGISTER(DrawLine3D),
        RAY_REGISTER(DrawCircle3D),
        RAY_REGISTER(DrawCube),
        RAY_REGISTER(DrawCubeV),
        RAY_REGISTER(DrawCubeWires),
        RAY_REGISTER(DrawCubeWiresV),
        RAY_REGISTER(DrawCubeTexture),
        RAY_REGISTER(DrawSphere),
        RAY_REGISTER(DrawSphereEx),
        RAY_REGISTER(DrawSphereWires),
        RAY_REGISTER(DrawCylinder),
        RAY_REGISTER(DrawCylinderWires),
        RAY_REGISTER(DrawPlane),
        RAY_REGISTER(DrawRay),
        RAY_REGISTER(DrawGrid),
        RAY_REGISTER(DrawGizmo),

        // [MODELS] Model loading/unloading functions
        RAY_REGISTER(LoadModel),
        RAY_REGISTER(LoadModelFromMesh),
        RAY_REGISTER(UnloadModel),

        // [MODELS] Mesh loading/unloading functions
        // RAY_REGISTER(LoadMeshes),
        // RAY_REGISTER(ExportMesh),
        // RAY_REGISTER(UnloadMesh),

        // [MODELS] Material loading/unloading functions
        // RAY_REGISTER(LoadMaterials),
        // RAY_REGISTER(LoadMaterialDefault),
        // RAY_REGISTER(UnloadMaterial),
        // RAY_REGISTER(SetMaterialTexture),
        // RAY_REGISTER(SetModelMeshMaterial),

        // [MODELS] Model animations loading/unloading functions
        // RAY_REGISTER(LoadModelAnimations),
        // RAY_REGISTER(UpdateModelAnimation),
        // RAY_REGISTER(UnloadModelAnimation),
        // RAY_REGISTER(IsModelAnimationValid),

        // [MODELS] Mesh generation functions
        // RAY_REGISTER(GenMeshPoly),
        // RAY_REGISTER(GenMeshPlane),
        // RAY_REGISTER(GenMeshCube),
        // RAY_REGISTER(GenMeshSphere),
        // RAY_REGISTER(GenMeshHemiSphere),
        // RAY_REGISTER(GenMeshCylinder),
        // RAY_REGISTER(GenMeshTorus),
        // RAY_REGISTER(GenMeshKnot),
        // RAY_REGISTER(GenMeshHeightmap),
        // RAY_REGISTER(GenMeshCubicmap),

        // [MODELS] Mesh manipulation functions
        // RAY_REGISTER(MeshBoundingBox),
        // RAY_REGISTER(MeshTangents),
        // RAY_REGISTER(MeshBinormals),

        // [MODELS] Model drawing functions
        // RAY_REGISTER(DrawModel),
        // RAY_REGISTER(DrawModelEx),
        // RAY_REGISTER(DrawModelWires),
        // RAY_REGISTER(DrawModelWiresEx),
        // RAY_REGISTER(DrawBoundingBox),
        // RAY_REGISTER(DrawBillboard),
        // RAY_REGISTER(DrawBillboardRec),

        // [MODELS] Collision detection functions
        // RAY_REGISTER(CheckCollisionSpheres),
        // RAY_REGISTER(CheckCollisionBoxes),
        // RAY_REGISTER(CheckCollisionBoxSphere),
        // RAY_REGISTER(CheckCollisionRaySphere),
        // RAY_REGISTER(CheckCollisionRaySphereEx),
        // RAY_REGISTER(CheckCollisionRayBox),
        // RAY_REGISTER(GetCollisionRayModel),
        // RAY_REGISTER(GetCollisionRayTriangle),
        // RAY_REGISTER(GetCollisionRayGround),

        // [SHADERS] Shader loading/unloading functions
        RAY_REGISTER(LoadText),
        RAY_REGISTER(LoadShader),
        RAY_REGISTER(LoadShaderCode),
        RAY_REGISTER(UnloadShader),
        RAY_REGISTER(GetShaderDefault),
        RAY_REGISTER(GetTextureDefault),

        // [SHADERS] Shader configuration functions
        RAY_REGISTER(GetShaderLocation),
        RAY_REGISTER(SetShaderValue),
        RAY_REGISTER(SetShaderValueV),
        RAY_REGISTER(SetShaderValueMatrix),
        RAY_REGISTER(SetShaderValueTexture),
        RAY_REGISTER(SetMatrixProjection),
        RAY_REGISTER(SetMatrixModelview),
        RAY_REGISTER(GetMatrixModelview),

        // [SHADERS] Shading begin/end functions
        RAY_REGISTER(BeginShaderMode),
        RAY_REGISTER(EndShaderMode),
        RAY_REGISTER(BeginBlendMode),
        RAY_REGISTER(EndBlendMode),
        RAY_REGISTER(BeginScissorMode),
        RAY_REGISTER(EndScissorMode),

        // [SHADERS] VR control functions
        RAY_REGISTER(GetVrDeviceInfo), // [DEPRECATED]
        RAY_REGISTER(InitVrSimulator),
        RAY_REGISTER(CloseVrSimulator),
        RAY_REGISTER(UpdateVrTracking),
        RAY_REGISTER(SetVrConfiguration),
        RAY_REGISTER(IsVrSimulatorReady),
        RAY_REGISTER(ToggleVrMode),
        RAY_REGISTER(BeginVrDrawing),
        RAY_REGISTER(EndVrDrawing),

        // [AUDIO] Audio device management functions
        RAY_REGISTER(InitAudioDevice),
        RAY_REGISTER(CloseAudioDevice),
        RAY_REGISTER(IsAudioDeviceReady),
        RAY_REGISTER(SetMasterVolume),

        // [AUDIO] Wave/Sound loading/unloading functions
        RAY_REGISTER(LoadWave),
        RAY_REGISTER(LoadWaveEx),
        RAY_REGISTER(LoadSound),
        RAY_REGISTER(LoadSoundFromWave),
        RAY_REGISTER(UpdateSound),
        RAY_REGISTER(UnloadWave),
        RAY_REGISTER(UnloadSound),
        RAY_REGISTER(ExportWave),
        RAY_REGISTER(ExportWaveAsCode),

        // [AUDIO] Wave/Sound management functions
        RAY_REGISTER(PlaySound),
        RAY_REGISTER(PauseSound),
        RAY_REGISTER(ResumeSound),
        RAY_REGISTER(StopSound),
        RAY_REGISTER(IsSoundPlaying),
        RAY_REGISTER(SetSoundVolume),
        RAY_REGISTER(SetSoundPitch),
        RAY_REGISTER(WaveFormat),
        RAY_REGISTER(WaveCopy),
        RAY_REGISTER(WaveCrop),
        RAY_REGISTER(GetWaveData),

        // [AUDIO] Music management functions
        RAY_REGISTER(LoadMusicStream),
        RAY_REGISTER(UnloadMusicStream),
        RAY_REGISTER(PlayMusicStream),
        RAY_REGISTER(UpdateMusicStream),
        RAY_REGISTER(StopMusicStream),
        RAY_REGISTER(PauseMusicStream),
        RAY_REGISTER(ResumeMusicStream),
        RAY_REGISTER(IsMusicPlaying),
        RAY_REGISTER(SetMusicVolume),
        RAY_REGISTER(SetMusicPitch),
        RAY_REGISTER(SetMusicLoopCount),
        RAY_REGISTER(GetMusicTimeLength),
        RAY_REGISTER(GetMusicTimePlayed),

        // [AUDIO] AudioStream management functions
        RAY_REGISTER(InitAudioStream),
        RAY_REGISTER(UpdateAudioStream),
        RAY_REGISTER(CloseAudioStream),
        RAY_REGISTER(IsAudioBufferProcessed),
        RAY_REGISTER(PlayAudioStream),
        RAY_REGISTER(PauseAudioStream),
        RAY_REGISTER(ResumeAudioStream),
        RAY_REGISTER(IsAudioStreamPlaying),
        RAY_REGISTER(StopAudioStream),
        RAY_REGISTER(SetAudioStreamVolume),
        RAY_REGISTER(SetAudioStreamPitch),

        { nullptr, nullptr }
};

#ifndef RAY_INT_SYMBOL
#define RAY_INT_SYMBOL(L, s)        \
    do {                            \
        lua_pushinteger(L, s);      \
        lua_setfield(L, -2, #s);    \
    } while (false) /* -- END -- */
#endif

#ifndef RAY_FLOAT_SYMBOL
#define RAY_FLOAT_SYMBOL(L, s)      \
    do {                            \
        lua_pushnumber(L, s);       \
        lua_setfield(L, -2, #s);    \
    } while (false) /* -- END -- */
#endif

#ifndef RAY_COLOR_SYMBOL
#define RAY_COLOR_SYMBOL(L, s, r, g, b, a)      \
    do {                                        \
        luaE_pushcolor(L, Color{r, g, b, a});   \
        lua_setfield(L, -2, #s);                \
    } while (false) /* -- END -- */
#endif

static void luaE_push_raylib_symbols(lua_State * L)
{
    RAY_FLOAT_SYMBOL(L, PI);
    RAY_FLOAT_SYMBOL(L, DEG2RAD);
    RAY_FLOAT_SYMBOL(L, RAD2DEG);

    RAY_INT_SYMBOL(L, MAX_TOUCH_POINTS);
    RAY_INT_SYMBOL(L, MAX_SHADER_LOCATIONS);
    RAY_INT_SYMBOL(L, MAX_MATERIAL_MAPS);

    RAY_COLOR_SYMBOL(L, LIGHTGRAY , 200, 200, 200, 255);
    RAY_COLOR_SYMBOL(L, GRAY      , 130, 130, 130, 255);
    RAY_COLOR_SYMBOL(L, DARKGRAY  ,  80,  80,  80, 255);
    RAY_COLOR_SYMBOL(L, YELLOW    , 253, 249,   0, 255);
    RAY_COLOR_SYMBOL(L, GOLD      , 255, 203,   0, 255);
    RAY_COLOR_SYMBOL(L, ORANGE    , 255, 161,   0, 255);
    RAY_COLOR_SYMBOL(L, PINK      , 255, 109, 194, 255);
    RAY_COLOR_SYMBOL(L, RED       , 230,  41,  55, 255);
    RAY_COLOR_SYMBOL(L, MAROON    , 190,  33,  55, 255);
    RAY_COLOR_SYMBOL(L, GREEN     ,   0, 228,  48, 255);
    RAY_COLOR_SYMBOL(L, LIME      ,   0, 158,  47, 255);
    RAY_COLOR_SYMBOL(L, DARKGREEN ,   0, 117,  44, 255);
    RAY_COLOR_SYMBOL(L, SKYBLUE   , 102, 191, 255, 255);
    RAY_COLOR_SYMBOL(L, BLUE      ,   0, 121, 241, 255);
    RAY_COLOR_SYMBOL(L, DARKBLUE  ,   0,  82, 172, 255);
    RAY_COLOR_SYMBOL(L, PURPLE    , 200, 122, 255, 255);
    RAY_COLOR_SYMBOL(L, VIOLET    , 135,  60, 190, 255);
    RAY_COLOR_SYMBOL(L, DARKPURPLE, 112,  31, 126, 255);
    RAY_COLOR_SYMBOL(L, BEIGE     , 211, 176, 131, 255);
    RAY_COLOR_SYMBOL(L, BROWN     , 127, 106,  79, 255);
    RAY_COLOR_SYMBOL(L, DARKBROWN ,  76,  63,  47, 255);
    RAY_COLOR_SYMBOL(L, WHITE     , 255, 255, 255, 255);
    RAY_COLOR_SYMBOL(L, BLACK     ,   0,   0,   0, 255);
    RAY_COLOR_SYMBOL(L, BLANK     ,   0,   0,   0,   0);
    RAY_COLOR_SYMBOL(L, MAGENTA   , 255,   0, 255, 255);
    RAY_COLOR_SYMBOL(L, RAYWHITE  , 245, 245, 245, 255);

    RAY_INT_SYMBOL(L, FLAG_SHOW_LOGO);
    RAY_INT_SYMBOL(L, FLAG_FULLSCREEN_MODE);
    RAY_INT_SYMBOL(L, FLAG_WINDOW_RESIZABLE);
    RAY_INT_SYMBOL(L, FLAG_WINDOW_UNDECORATED);
    RAY_INT_SYMBOL(L, FLAG_WINDOW_TRANSPARENT);
    RAY_INT_SYMBOL(L, FLAG_WINDOW_HIDDEN);
    RAY_INT_SYMBOL(L, FLAG_MSAA_4X_HINT);
    RAY_INT_SYMBOL(L, FLAG_VSYNC_HINT);
    RAY_INT_SYMBOL(L, LOG_ALL);
    RAY_INT_SYMBOL(L, LOG_TRACE);
    RAY_INT_SYMBOL(L, LOG_DEBUG);
    RAY_INT_SYMBOL(L, LOG_INFO);
    RAY_INT_SYMBOL(L, LOG_WARNING);
    RAY_INT_SYMBOL(L, LOG_ERROR);
    RAY_INT_SYMBOL(L, LOG_FATAL);
    RAY_INT_SYMBOL(L, LOG_NONE);
    RAY_INT_SYMBOL(L, KEY_APOSTROPHE);
    RAY_INT_SYMBOL(L, KEY_COMMA);
    RAY_INT_SYMBOL(L, KEY_MINUS);
    RAY_INT_SYMBOL(L, KEY_PERIOD);
    RAY_INT_SYMBOL(L, KEY_SLASH);
    RAY_INT_SYMBOL(L, KEY_ZERO);
    RAY_INT_SYMBOL(L, KEY_ONE);
    RAY_INT_SYMBOL(L, KEY_TWO);
    RAY_INT_SYMBOL(L, KEY_THREE);
    RAY_INT_SYMBOL(L, KEY_FOUR);
    RAY_INT_SYMBOL(L, KEY_FIVE);
    RAY_INT_SYMBOL(L, KEY_SIX);
    RAY_INT_SYMBOL(L, KEY_SEVEN);
    RAY_INT_SYMBOL(L, KEY_EIGHT);
    RAY_INT_SYMBOL(L, KEY_NINE);
    RAY_INT_SYMBOL(L, KEY_SEMICOLON);
    RAY_INT_SYMBOL(L, KEY_EQUAL);
    RAY_INT_SYMBOL(L, KEY_A);
    RAY_INT_SYMBOL(L, KEY_B);
    RAY_INT_SYMBOL(L, KEY_C);
    RAY_INT_SYMBOL(L, KEY_D);
    RAY_INT_SYMBOL(L, KEY_E);
    RAY_INT_SYMBOL(L, KEY_F);
    RAY_INT_SYMBOL(L, KEY_G);
    RAY_INT_SYMBOL(L, KEY_H);
    RAY_INT_SYMBOL(L, KEY_I);
    RAY_INT_SYMBOL(L, KEY_J);
    RAY_INT_SYMBOL(L, KEY_K);
    RAY_INT_SYMBOL(L, KEY_L);
    RAY_INT_SYMBOL(L, KEY_M);
    RAY_INT_SYMBOL(L, KEY_N);
    RAY_INT_SYMBOL(L, KEY_O);
    RAY_INT_SYMBOL(L, KEY_P);
    RAY_INT_SYMBOL(L, KEY_Q);
    RAY_INT_SYMBOL(L, KEY_R);
    RAY_INT_SYMBOL(L, KEY_S);
    RAY_INT_SYMBOL(L, KEY_T);
    RAY_INT_SYMBOL(L, KEY_U);
    RAY_INT_SYMBOL(L, KEY_V);
    RAY_INT_SYMBOL(L, KEY_W);
    RAY_INT_SYMBOL(L, KEY_X);
    RAY_INT_SYMBOL(L, KEY_Y);
    RAY_INT_SYMBOL(L, KEY_Z);
    RAY_INT_SYMBOL(L, KEY_SPACE);
    RAY_INT_SYMBOL(L, KEY_ESCAPE);
    RAY_INT_SYMBOL(L, KEY_ENTER);
    RAY_INT_SYMBOL(L, KEY_TAB);
    RAY_INT_SYMBOL(L, KEY_BACKSPACE);
    RAY_INT_SYMBOL(L, KEY_INSERT);
    RAY_INT_SYMBOL(L, KEY_DELETE);
    RAY_INT_SYMBOL(L, KEY_RIGHT);
    RAY_INT_SYMBOL(L, KEY_LEFT);
    RAY_INT_SYMBOL(L, KEY_DOWN);
    RAY_INT_SYMBOL(L, KEY_UP);
    RAY_INT_SYMBOL(L, KEY_PAGE_UP);
    RAY_INT_SYMBOL(L, KEY_PAGE_DOWN);
    RAY_INT_SYMBOL(L, KEY_HOME);
    RAY_INT_SYMBOL(L, KEY_END);
    RAY_INT_SYMBOL(L, KEY_CAPS_LOCK);
    RAY_INT_SYMBOL(L, KEY_SCROLL_LOCK);
    RAY_INT_SYMBOL(L, KEY_NUM_LOCK);
    RAY_INT_SYMBOL(L, KEY_PRINT_SCREEN);
    RAY_INT_SYMBOL(L, KEY_PAUSE);
    RAY_INT_SYMBOL(L, KEY_F1);
    RAY_INT_SYMBOL(L, KEY_F2);
    RAY_INT_SYMBOL(L, KEY_F3);
    RAY_INT_SYMBOL(L, KEY_F4);
    RAY_INT_SYMBOL(L, KEY_F5);
    RAY_INT_SYMBOL(L, KEY_F6);
    RAY_INT_SYMBOL(L, KEY_F7);
    RAY_INT_SYMBOL(L, KEY_F8);
    RAY_INT_SYMBOL(L, KEY_F9);
    RAY_INT_SYMBOL(L, KEY_F10);
    RAY_INT_SYMBOL(L, KEY_F11);
    RAY_INT_SYMBOL(L, KEY_F12);
    RAY_INT_SYMBOL(L, KEY_LEFT_SHIFT);
    RAY_INT_SYMBOL(L, KEY_LEFT_CONTROL);
    RAY_INT_SYMBOL(L, KEY_LEFT_ALT);
    RAY_INT_SYMBOL(L, KEY_LEFT_SUPER);
    RAY_INT_SYMBOL(L, KEY_RIGHT_SHIFT);
    RAY_INT_SYMBOL(L, KEY_RIGHT_CONTROL);
    RAY_INT_SYMBOL(L, KEY_RIGHT_ALT);
    RAY_INT_SYMBOL(L, KEY_RIGHT_SUPER);
    RAY_INT_SYMBOL(L, KEY_KB_MENU);
    RAY_INT_SYMBOL(L, KEY_LEFT_BRACKET);
    RAY_INT_SYMBOL(L, KEY_BACKSLASH);
    RAY_INT_SYMBOL(L, KEY_RIGHT_BRACKET);
    RAY_INT_SYMBOL(L, KEY_GRAVE);
    RAY_INT_SYMBOL(L, KEY_KP_0);
    RAY_INT_SYMBOL(L, KEY_KP_1);
    RAY_INT_SYMBOL(L, KEY_KP_2);
    RAY_INT_SYMBOL(L, KEY_KP_3);
    RAY_INT_SYMBOL(L, KEY_KP_4);
    RAY_INT_SYMBOL(L, KEY_KP_5);
    RAY_INT_SYMBOL(L, KEY_KP_6);
    RAY_INT_SYMBOL(L, KEY_KP_7);
    RAY_INT_SYMBOL(L, KEY_KP_8);
    RAY_INT_SYMBOL(L, KEY_KP_9);
    RAY_INT_SYMBOL(L, KEY_KP_DECIMAL);
    RAY_INT_SYMBOL(L, KEY_KP_DIVIDE);
    RAY_INT_SYMBOL(L, KEY_KP_MULTIPLY);
    RAY_INT_SYMBOL(L, KEY_KP_SUBTRACT);
    RAY_INT_SYMBOL(L, KEY_KP_ADD);
    RAY_INT_SYMBOL(L, KEY_KP_ENTER);
    RAY_INT_SYMBOL(L, KEY_KP_EQUAL);
    RAY_INT_SYMBOL(L, KEY_BACK);
    RAY_INT_SYMBOL(L, KEY_MENU);
    RAY_INT_SYMBOL(L, KEY_VOLUME_UP);
    RAY_INT_SYMBOL(L, KEY_VOLUME_DOWN);
    RAY_INT_SYMBOL(L, MOUSE_LEFT_BUTTON);
    RAY_INT_SYMBOL(L, MOUSE_RIGHT_BUTTON);
    RAY_INT_SYMBOL(L, MOUSE_MIDDLE_BUTTON);
    RAY_INT_SYMBOL(L, GAMEPAD_PLAYER1);
    RAY_INT_SYMBOL(L, GAMEPAD_PLAYER2);
    RAY_INT_SYMBOL(L, GAMEPAD_PLAYER3);
    RAY_INT_SYMBOL(L, GAMEPAD_PLAYER4);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_TRIANGLE);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_CIRCLE);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_CROSS);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_SQUARE);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_L1);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_R1);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_L2);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_R2);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_START);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_SELECT);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_PS);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_UP);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_RIGHT);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_DOWN);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_BUTTON_LEFT);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_AXIS_LEFT_X);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_AXIS_LEFT_Y);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_AXIS_RIGHT_X);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_AXIS_RIGHT_Y);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_AXIS_L2);
    RAY_INT_SYMBOL(L, GAMEPAD_PS3_AXIS_R2);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_A);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_B);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_X);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_Y);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_LB);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_RB);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_SELECT);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_START);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_HOME);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_UP);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_RIGHT);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_DOWN);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_BUTTON_LEFT);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_AXIS_LEFT_X);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_AXIS_LEFT_Y);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_AXIS_RIGHT_X);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_AXIS_RIGHT_Y);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_AXIS_LT);
    RAY_INT_SYMBOL(L, GAMEPAD_XBOX_AXIS_RT);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_DPAD_UP);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_DPAD_DOWN);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_DPAD_LEFT);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_DPAD_RIGHT);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_DPAD_CENTER);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_A);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_B);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_C);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_X);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_Y);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_Z);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_L1);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_R1);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_L2);
    RAY_INT_SYMBOL(L, GAMEPAD_ANDROID_BUTTON_R2);
    RAY_INT_SYMBOL(L, LOC_VERTEX_POSITION);
    RAY_INT_SYMBOL(L, LOC_VERTEX_TEXCOORD01);
    RAY_INT_SYMBOL(L, LOC_VERTEX_TEXCOORD02);
    RAY_INT_SYMBOL(L, LOC_VERTEX_NORMAL);
    RAY_INT_SYMBOL(L, LOC_VERTEX_TANGENT);
    RAY_INT_SYMBOL(L, LOC_VERTEX_COLOR);
    RAY_INT_SYMBOL(L, LOC_MATRIX_MVP);
    RAY_INT_SYMBOL(L, LOC_MATRIX_MODEL);
    RAY_INT_SYMBOL(L, LOC_MATRIX_VIEW);
    RAY_INT_SYMBOL(L, LOC_MATRIX_PROJECTION);
    RAY_INT_SYMBOL(L, LOC_VECTOR_VIEW);
    RAY_INT_SYMBOL(L, LOC_COLOR_DIFFUSE);
    RAY_INT_SYMBOL(L, LOC_COLOR_SPECULAR);
    RAY_INT_SYMBOL(L, LOC_COLOR_AMBIENT);
    RAY_INT_SYMBOL(L, LOC_MAP_ALBEDO);
    RAY_INT_SYMBOL(L, LOC_MAP_METALNESS);
    RAY_INT_SYMBOL(L, LOC_MAP_NORMAL);
    RAY_INT_SYMBOL(L, LOC_MAP_ROUGHNESS);
    RAY_INT_SYMBOL(L, LOC_MAP_OCCLUSION);
    RAY_INT_SYMBOL(L, LOC_MAP_EMISSION);
    RAY_INT_SYMBOL(L, LOC_MAP_HEIGHT);
    RAY_INT_SYMBOL(L, LOC_MAP_CUBEMAP);
    RAY_INT_SYMBOL(L, LOC_MAP_IRRADIANCE);
    RAY_INT_SYMBOL(L, LOC_MAP_PREFILTER);
    RAY_INT_SYMBOL(L, LOC_MAP_BRDF);
    RAY_INT_SYMBOL(L, LOC_MAP_DIFFUSE);
    RAY_INT_SYMBOL(L, LOC_MAP_SPECULAR);
    RAY_INT_SYMBOL(L, UNIFORM_FLOAT);
    RAY_INT_SYMBOL(L, UNIFORM_VEC2);
    RAY_INT_SYMBOL(L, UNIFORM_VEC3);
    RAY_INT_SYMBOL(L, UNIFORM_VEC4);
    RAY_INT_SYMBOL(L, UNIFORM_INT);
    RAY_INT_SYMBOL(L, UNIFORM_IVEC2);
    RAY_INT_SYMBOL(L, UNIFORM_IVEC3);
    RAY_INT_SYMBOL(L, UNIFORM_IVEC4);
    RAY_INT_SYMBOL(L, UNIFORM_SAMPLER2D);
    RAY_INT_SYMBOL(L, MAP_ALBEDO);
    RAY_INT_SYMBOL(L, MAP_METALNESS);
    RAY_INT_SYMBOL(L, MAP_NORMAL);
    RAY_INT_SYMBOL(L, MAP_ROUGHNESS);
    RAY_INT_SYMBOL(L, MAP_OCCLUSION);
    RAY_INT_SYMBOL(L, MAP_EMISSION);
    RAY_INT_SYMBOL(L, MAP_HEIGHT);
    RAY_INT_SYMBOL(L, MAP_CUBEMAP);
    RAY_INT_SYMBOL(L, MAP_IRRADIANCE);
    RAY_INT_SYMBOL(L, MAP_PREFILTER);
    RAY_INT_SYMBOL(L, MAP_BRDF);
    RAY_INT_SYMBOL(L, MAP_DIFFUSE);
    RAY_INT_SYMBOL(L, MAP_SPECULAR);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_GRAYSCALE);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_GRAY_ALPHA);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R5G6B5);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R8G8B8);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R5G5B5A1);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R4G4B4A4);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R8G8B8A8);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R32);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R32G32B32);
    RAY_INT_SYMBOL(L, UNCOMPRESSED_R32G32B32A32);
    RAY_INT_SYMBOL(L, COMPRESSED_DXT1_RGB);
    RAY_INT_SYMBOL(L, COMPRESSED_DXT1_RGBA);
    RAY_INT_SYMBOL(L, COMPRESSED_DXT3_RGBA);
    RAY_INT_SYMBOL(L, COMPRESSED_DXT5_RGBA);
    RAY_INT_SYMBOL(L, COMPRESSED_ETC1_RGB);
    RAY_INT_SYMBOL(L, COMPRESSED_ETC2_RGB);
    RAY_INT_SYMBOL(L, COMPRESSED_ETC2_EAC_RGBA);
    RAY_INT_SYMBOL(L, COMPRESSED_PVRT_RGB);
    RAY_INT_SYMBOL(L, COMPRESSED_PVRT_RGBA);
    RAY_INT_SYMBOL(L, COMPRESSED_ASTC_4x4_RGBA);
    RAY_INT_SYMBOL(L, COMPRESSED_ASTC_8x8_RGBA);
    RAY_INT_SYMBOL(L, FILTER_POINT);
    RAY_INT_SYMBOL(L, FILTER_BILINEAR);
    RAY_INT_SYMBOL(L, FILTER_TRILINEAR);
    RAY_INT_SYMBOL(L, FILTER_ANISOTROPIC_4X);
    RAY_INT_SYMBOL(L, FILTER_ANISOTROPIC_8X);
    RAY_INT_SYMBOL(L, FILTER_ANISOTROPIC_16X);
    RAY_INT_SYMBOL(L, CUBEMAP_AUTO_DETECT);
    RAY_INT_SYMBOL(L, CUBEMAP_LINE_VERTICAL);
    RAY_INT_SYMBOL(L, CUBEMAP_LINE_HORIZONTAL);
    RAY_INT_SYMBOL(L, CUBEMAP_CROSS_THREE_BY_FOUR);
    RAY_INT_SYMBOL(L, CUBEMAP_CROSS_FOUR_BY_THREE);
    RAY_INT_SYMBOL(L, CUBEMAP_PANORAMA);
    RAY_INT_SYMBOL(L, WRAP_REPEAT);
    RAY_INT_SYMBOL(L, WRAP_CLAMP);
    RAY_INT_SYMBOL(L, WRAP_MIRROR_REPEAT);
    RAY_INT_SYMBOL(L, WRAP_MIRROR_CLAMP);
    RAY_INT_SYMBOL(L, FONT_DEFAULT);
    RAY_INT_SYMBOL(L, FONT_BITMAP);
    RAY_INT_SYMBOL(L, FONT_SDF);
    RAY_INT_SYMBOL(L, BLEND_ALPHA);
    RAY_INT_SYMBOL(L, BLEND_ADDITIVE);
    RAY_INT_SYMBOL(L, BLEND_MULTIPLIED);
    RAY_INT_SYMBOL(L, GESTURE_NONE);
    RAY_INT_SYMBOL(L, GESTURE_TAP);
    RAY_INT_SYMBOL(L, GESTURE_DOUBLETAP);
    RAY_INT_SYMBOL(L, GESTURE_HOLD);
    RAY_INT_SYMBOL(L, GESTURE_DRAG);
    RAY_INT_SYMBOL(L, GESTURE_SWIPE_RIGHT);
    RAY_INT_SYMBOL(L, GESTURE_SWIPE_LEFT);
    RAY_INT_SYMBOL(L, GESTURE_SWIPE_UP);
    RAY_INT_SYMBOL(L, GESTURE_SWIPE_DOWN);
    RAY_INT_SYMBOL(L, GESTURE_PINCH_IN);
    RAY_INT_SYMBOL(L, GESTURE_PINCH_OUT);
    RAY_INT_SYMBOL(L, CAMERA_CUSTOM);
    RAY_INT_SYMBOL(L, CAMERA_FREE);
    RAY_INT_SYMBOL(L, CAMERA_ORBITAL);
    RAY_INT_SYMBOL(L, CAMERA_FIRST_PERSON);
    RAY_INT_SYMBOL(L, CAMERA_THIRD_PERSON);
    RAY_INT_SYMBOL(L, CAMERA_PERSPECTIVE);
    RAY_INT_SYMBOL(L, CAMERA_ORTHOGRAPHIC);
    RAY_INT_SYMBOL(L, HMD_DEFAULT_DEVICE);
    RAY_INT_SYMBOL(L, HMD_OCULUS_RIFT_DK2);
    RAY_INT_SYMBOL(L, HMD_OCULUS_RIFT_CV1);
    RAY_INT_SYMBOL(L, HMD_OCULUS_GO);
    RAY_INT_SYMBOL(L, HMD_VALVE_HTC_VIVE);
    RAY_INT_SYMBOL(L, HMD_SONY_PSVR);
    RAY_INT_SYMBOL(L, NPT_9PATCH);
    RAY_INT_SYMBOL(L, NPT_3PATCH_VERTICAL);
    RAY_INT_SYMBOL(L, NPT_3PATCH_HORIZONTAL);
}

bool luaopen_ray(lua_State * L)
{
    luaL_register(L, lua_tbag_name(), __lua_lay);
    {
        luaE_push_raylib_symbols(L);

        // clang-format off
        luaE_register_image(L);
        luaE_register_charinfo(L);
        luaE_register_font(L);
        luaE_register_mesh(L);
        luaE_register_material(L);
        luaE_register_model(L);
        luaE_register_modelanimation(L);
        luaE_register_wave(L);
        luaE_register_sound(L);
        luaE_register_audiostream(L);
        // clang-format on
    }
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

