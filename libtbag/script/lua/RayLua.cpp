/**
 * @file   RayLua.cpp
 * @brief  RayLua class implementation.
 * @author zer0
 * @date   2019-04-27
 */

#include <libtbag/script/lua/RayLua.hpp>
#include <libtbag/ray/RayBypass.hpp>
#include <libtbag/ray/RayGuiBypass.hpp>

#include <cstring>
#include <cstdlib>
#include <cassert>

#include <vector>
#include <string>

// #include <libtbag/string/Format.hpp> // [WARNING] Don't use this header.
#if defined(TBAG_PLATFORM_WINDOWS) && defined(_WINGDI_)
#error The Rectangle symbol in wingdi.h conflicts.
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray;

static void luaL_register_metatable(lua_State * L, char const * name, luaL_Reg const * l)
{
    luaL_newmetatable(L, name);
    {
        luaL_register(L, nullptr, l);

        lua_pushliteral(L, "__index");
        lua_pushvalue(L, -2); // Duplicate metatable.
        lua_rawset(L, -3);
    }
    lua_pop(L, 1);
}

static std::vector<int> luaL_checkinteger_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<int> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkinteger(L, -1);
    }
    return result;
}

static std::vector<std::string> luaL_checkstring_array(lua_State * L, int arg_num)
{
    auto size = lua_objlen(L, arg_num);
    std::vector<std::string> result(size);
    for (int i = 0; i < size; ++i) {
        lua_rawgeti(L, arg_num, i+1);
        result[i] = luaL_checkstring(L, -1);
    }
    return result;
}

#ifndef RAY_USERDATA
#define RAY_USERDATA(type, upper, lower, more_regs)                             \
    TBAG_CONSTEXPR static char const * const METATABLE_##upper = "#name";       \
    static type * luaL_push##lower(lua_State * L, type const * src = nullptr)   \
    {                                                                           \
        auto * result = (type*)lua_newuserdata(L, sizeof(type));                \
        assert(result != nullptr);                                              \
        if (src) {                                                              \
            memcpy((void*)result, (void const *)src, sizeof(type));             \
        } else {                                                                \
            memset((void*)result, 0x00, sizeof(type));                          \
        }                                                                       \
        luaL_getmetatable(L, METATABLE_##upper);                                \
        lua_setmetatable(L, -2);                                                \
        return result;                                                          \
    }                                                                           \
    static type * luaL_check##lower(lua_State * L, int num_arg)                 \
    {                                                                           \
        auto * result = (type*)luaL_checkudata(L, num_arg, METATABLE_##upper);  \
        if (result == nullptr) {                                                \
            luaL_typerror(L, num_arg, METATABLE_##upper);                       \
            return nullptr;                                                     \
        }                                                                       \
        return result;                                                          \
    }                                                                           \
    static int _##type(lua_State * L)                                           \
    {                                                                           \
        luaL_push##lower(L);                                                    \
        return 1;                                                               \
    }                                                                           \
    static int _##type##_gc(lua_State * L)                                      \
    {                                                                           \
        return 0;                                                               \
    }                                                                           \
    static int _##type##_tostring(lua_State * L)                                \
    {                                                                           \
        lua_pushstring(L, METATABLE_##upper);                                   \
        return 1;                                                               \
    }                                                                           \
    static luaL_Reg const __lua_lay_##lower[] = {                               \
            { "__gc", _##type##_gc },                                           \
            { "__tostring", _##type##_tostring },                               \
            { nullptr, nullptr }                                                \
    }; /* -- END -- */
#endif

# /***********/
# /* Vector2 */
# /***********/

static void luaL_pushvector2(lua_State * L, Vector2 const & vec)
{
    lua_createtable(L, 0, 2);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
}

static Vector2 luaL_checkvector2(lua_State * L, int num_arg)
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

static std::vector<Vector2> luaL_checkvector2_array(lua_State * L, int num_arg)
{
    auto const length = lua_objlen(L, num_arg);
    std::vector<Vector2> result(length);
    for (auto i = 0; i < length; ++i) {
        lua_rawgeti(L, num_arg, 1);
        result[i] = luaL_checkvector2(L, lua_absindex(L, -1));
    }
    lua_pop(L, length);
    return result;
}

# /***********/
# /* Vector3 */
# /***********/

static void luaL_pushvector3(lua_State * L, Vector3 const & vec)
{
    lua_createtable(L, 0, 3);
    lua_pushnumber(L, vec.x);
    lua_setfield(L, -2, "x");
    lua_pushnumber(L, vec.y);
    lua_setfield(L, -2, "y");
    lua_pushnumber(L, vec.z);
    lua_setfield(L, -2, "z");
}

static Vector3 luaL_checkvector3(lua_State * L, int num_arg)
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

# /************************/
# /* Vector4 (Quaternion) */
# /************************/

static void luaL_pushvector4(lua_State * L, Vector4 const & vec)
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

static Vector4 luaL_checkvector4(lua_State * L, int num_arg)
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

# /**********/
# /* Matrix */
# /**********/

static void luaL_pushmatrix(lua_State * L, Matrix const & mat)
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

static Matrix luaL_checkmatrix(lua_State * L, int num_arg)
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

static void luaL_pushcolor(lua_State * L, Color const & color)
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

static Color luaL_checkcolor(lua_State * L, int num_arg)
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

static std::vector<Color> luaL_checkcolor_array(lua_State * L, int num_arg)
{
    auto const length = lua_objlen(L, num_arg);
    std::vector<Color> result(length);
    for (auto i = 0; i < length; ++i) {
        lua_rawgeti(L, num_arg, 1);
        result[i] = luaL_checkcolor(L, lua_absindex(L, -1));
    }
    lua_pop(L, length);
    return result;
}

# /*************/
# /* Rectangle */
# /*************/

static void luaL_pushrectangle(lua_State * L, Rectangle const & rect)
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

static Rectangle luaL_checkrectangle(lua_State * L, int num_arg)
{
    Rectangle result = {0,};
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

RAY_USERDATA(Image, IMAGE, image,)

# /***************************************/
# /* Texture2D (Texture, TextureCubemap) */
# /***************************************/

static void luaL_pushtexture2d(lua_State * L, Texture2D const & tex)
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

static Texture2D luaL_checktexture2d(lua_State * L, int num_arg)
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

static void luaL_pushrendertexture2d(lua_State * L, RenderTexture2D const & tex)
{
    lua_createtable(L, 0, 5);
    lua_pushinteger(L, tex.id);
    lua_setfield(L, -2, "id");
    luaL_pushtexture2d(L, tex.texture);
    lua_setfield(L, -2, "texture");
    luaL_pushtexture2d(L, tex.depth);
    lua_setfield(L, -2, "depth");
    lua_pushboolean(L, tex.depthTexture?1:0);
    lua_setfield(L, -2, "depthTexture");
}

static RenderTexture2D luaL_checkrendertexture2d(lua_State * L, int num_arg)
{
    RenderTexture2D result = {0,};
    lua_getfield(L, num_arg, "id");
    result.id = luaL_checkinteger(L, -1);
    lua_getfield(L, num_arg, "texture");
    result.texture = luaL_checktexture2d(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "depth");
    result.depth = luaL_checktexture2d(L, lua_absindex(L, -1));
    lua_getfield(L, num_arg, "depthTexture");
    result.depthTexture = (lua_toboolean(L, -1) != 0);
    lua_pop(L, 4);
    return result;
}

# /**************/
# /* NPatchInfo */
# /**************/

static void luaL_pushnpatchinfo(lua_State * L, NPatchInfo const & npatch)
{
    lua_createtable(L, 0, 6);
    luaL_pushrectangle(L, npatch.sourceRec);
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

static NPatchInfo luaL_checknpatchinfo(lua_State * L, int num_arg)
{
    NPatchInfo result = {0,};
    if (lua_objlen(L, num_arg) >= 6) {
        lua_rawgeti(L, num_arg, 1);
        result.sourceRec = luaL_checkrectangle(L, lua_absindex(L, -1));
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
        result.sourceRec = luaL_checkrectangle(L, lua_absindex(L, -1));
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

RAY_USERDATA(CharInfo, CHARINFO, charinfo,)

static void luaL_pushcharinfo_array(lua_State * L, CharInfo * char_infos, int size)
{
    assert(char_infos != nullptr);
    assert(size >= 1);

    lua_createtable(L, size, 0);
    for (int i = 0; i < size; ++i) {
        luaL_pushcharinfo(L, char_infos+i);
        lua_rawseti(L, -2, i+1);
    }
}

static std::vector<CharInfo> luaL_checkcharinfo_array(lua_State * L, int num_arg)
{
    auto const length = lua_objlen(L, num_arg);
    std::vector<CharInfo> result(length);
    for (auto i = 0; i < length; ++i) {
        lua_rawgeti(L, num_arg, 1);
        result[i] = *luaL_checkcharinfo(L, lua_absindex(L, -1));
    }
    lua_pop(L, length);
    return result;
}

static std::vector<int> luaL_checkchars_array(lua_State * L, int num_arg)
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

RAY_USERDATA(Font, FONT, font,)

# /*********************/
# /* Camera3D (Camera) */
# /*********************/

static void luaL_pushcamera3d(lua_State * L, Camera3D const & cam)
{
    lua_createtable(L, 0, 5);
    luaL_pushvector3(L, cam.position);
    lua_setfield(L, -2, "position");
    luaL_pushvector3(L, cam.target);
    lua_setfield(L, -2, "target");
    luaL_pushvector3(L, cam.up);
    lua_setfield(L, -2, "up");
    lua_pushnumber(L, cam.fovy);
    lua_setfield(L, -2, "fovy");
    lua_pushinteger(L, cam.type);
    lua_setfield(L, -2, "type");
}

static Camera3D luaL_checkcamera3d(lua_State * L, int num_arg)
{
    Camera3D result = {0,};
    if (lua_objlen(L, num_arg) >= 5) {
        lua_rawgeti(L, num_arg, 1);
        result.position = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.target = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 3);
        result.up = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 4);
        result.fovy = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 5);
        result.type = luaL_checkinteger(L, -1);
        lua_pop(L, 5);
    } else {
        lua_getfield(L, num_arg, "position");
        result.position = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "target");
        result.target = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "up");
        result.up = luaL_checkvector3(L, lua_absindex(L, -1));
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

static void luaL_pushcamera2d(lua_State * L, Camera2D const & cam)
{
    lua_createtable(L, 0, 4);
    luaL_pushvector2(L, cam.offset);
    lua_setfield(L, -2, "offset");
    luaL_pushvector2(L, cam.target);
    lua_setfield(L, -2, "target");
    lua_pushnumber(L, cam.rotation);
    lua_setfield(L, -2, "rotation");
    lua_pushnumber(L, cam.zoom);
    lua_setfield(L, -2, "zoom");
}

static Camera2D luaL_checkcamera2d(lua_State * L, int num_arg)
{
    Camera2D result = {0,};
    if (lua_objlen(L, num_arg) >= 4) {
        lua_rawgeti(L, num_arg, 1);
        result.offset = luaL_checkvector2(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.target = luaL_checkvector2(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 3);
        result.rotation = luaL_checknumber(L, -1);
        lua_rawgeti(L, num_arg, 4);
        result.zoom = luaL_checknumber(L, -1);
        lua_pop(L, 4);
    } else {
        lua_getfield(L, num_arg, "offset");
        result.offset = luaL_checkvector2(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "target");
        result.target = luaL_checkvector2(L, lua_absindex(L, -1));
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

static void luaL_pushboundingbox(lua_State * L, BoundingBox const & bbox)
{
    lua_createtable(L, 0, 2);
    luaL_pushvector3(L, bbox.min);
    lua_setfield(L, -2, "min");
    luaL_pushvector3(L, bbox.max);
    lua_setfield(L, -2, "max");
}

static BoundingBox luaL_checkboundingbox(lua_State * L, int num_arg)
{
    BoundingBox result = {0,};
    if (lua_objlen(L, num_arg) >= 2) {
        lua_rawgeti(L, num_arg, 1);
        result.min = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.max = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    } else {
        lua_getfield(L, num_arg, "position");
        result.min = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "direction");
        result.max = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    }
    return result;
}

# /********/
# /* Mesh */
# /********/

RAY_USERDATA(Mesh, MESH, mesh,)

# /**********/
# /* Shader */
# /**********/

//typedef struct Shader {
//    unsigned int id;
//    int locs[MAX_SHADER_LOCATIONS];
//} Shader;

# /***************/
# /* MaterialMap */
# /***************/

//typedef struct MaterialMap {
//    Texture2D texture;
//    Color color;
//    float value;
//} MaterialMap;

# /************/
# /* Material */
# /************/

RAY_USERDATA(Material, MATERIAL, material,)

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

RAY_USERDATA(Model, MODEL, model,)

# /******************/
# /* ModelAnimation */
# /******************/

RAY_USERDATA(ModelAnimation, MODELANIMATION, modelanimation,)

# /*******/
# /* Ray */
# /*******/

static void luaL_pushray(lua_State * L, Ray const & ray)
{
    lua_createtable(L, 0, 2);
    luaL_pushvector3(L, ray.position);
    lua_setfield(L, -2, "position");
    luaL_pushvector3(L, ray.direction);
    lua_setfield(L, -2, "direction");
}

static Ray luaL_checkray(lua_State * L, int num_arg)
{
    Ray result = {0,};
    if (lua_objlen(L, num_arg) >= 2) {
        lua_rawgeti(L, num_arg, 1);
        result.position = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_rawgeti(L, num_arg, 2);
        result.direction = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    } else {
        lua_getfield(L, num_arg, "position");
        result.position = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_getfield(L, num_arg, "direction");
        result.direction = luaL_checkvector3(L, lua_absindex(L, -1));
        lua_pop(L, 2);
    }
    return result;
}

# /**************/
# /* RayHitInfo */
# /**************/

//typedef struct RayHitInfo {
//    bool hit;
//    float distance;
//    Vector3 position;
//    Vector3 normal;
//} RayHitInfo;

# /********/
# /* Wave */
# /********/

RAY_USERDATA(Wave, WAVE, wave,)

# /*********/
# /* Sound */
# /*********/

RAY_USERDATA(Sound, SOUND, sound,)

# /***************/
# /* AudioStream */
# /***************/

RAY_USERDATA(AudioStream, AUDIOSTREAM, audiostream,)

# /******************/
# /* VrDeviceInfo */
# /******************/

//typedef struct VrDeviceInfo {
//    int hResolution;
//    int vResolution;
//    float hScreenSize;
//    float vScreenSize;
//    float vScreenCenter;
//    float eyeToScreenDistance;
//    float lensSeparationDistance;
//    float interpupillaryDistance;
//    float lensDistortionValues[4];
//    float chromaAbCorrection[4];
//} VrDeviceInfo;

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
    SetWindowIcon(*luaL_checkimage(L, 1));
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
    ClearBackground(luaL_checkcolor(L, 1));
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
    BeginMode2D(luaL_checkcamera2d(L, 1));
    return 0;
}

static int _EndMode2D(lua_State * L)
{
    EndMode2D();
    return 0;
}

static int _BeginMode3D(lua_State * L)
{
    BeginMode3D(luaL_checkcamera3d(L, 1));
    return 0;
}

static int _EndMode3D(lua_State * L)
{
    EndMode3D();
    return 0;
}

static int _BeginTextureMode(lua_State * L)
{
    BeginTextureMode(luaL_checkrendertexture2d(L, 1));
    return 0;
}

static int _EndTextureMode(lua_State * L)
{
    EndTextureMode();
    return 0;
}

static int _GetMouseRay(lua_State * L)
{
    luaL_pushray(L, GetMouseRay(luaL_checkvector2(L, 1), luaL_checkcamera3d(L, 2)));
    return 1;
}

static int _GetWorldToScreen(lua_State * L)
{
    luaL_pushvector2(L, GetWorldToScreen(luaL_checkvector3(L, 1), luaL_checkcamera3d(L, 2)));
    return 1;
}

static int _GetCameraMatrix(lua_State * L)
{
    luaL_pushmatrix(L, GetCameraMatrix(luaL_checkcamera3d(L, 1)));
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
    lua_pushnumber(L, ColorToInt(luaL_checkcolor(L, 1)));
    return 1;
}

static int _ColorNormalize(lua_State * L)
{
    luaL_pushvector4(L, ColorNormalize(luaL_checkcolor(L, 1)));
    return 1;
}

static int _ColorToHSV(lua_State * L)
{
    luaL_pushvector3(L, ColorToHSV(luaL_checkcolor(L, 1)));
    return 1;
}

static int _ColorFromHSV(lua_State * L)
{
    luaL_pushcolor(L, ColorFromHSV(luaL_checkvector3(L, 1)));
    return 1;
}

static int _GetColor(lua_State * L)
{
    luaL_pushcolor(L, GetColor(luaL_checkinteger(L, 1)));
    return 1;
}

static int _Fade(lua_State * L)
{
    luaL_pushcolor(L, Fade(luaL_checkcolor(L, 1), luaL_checknumber(L, 2)));
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
    luaL_error(L, "Unsupported operation error");
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
    luaL_pushvector2(L, GetMousePosition());
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
    luaL_pushvector2(L, GetTouchPosition(luaL_checkinteger(L, 1)));
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
    luaL_pushvector2(L, GetGestureDragVector());
    return 1;
}

static int _GetGestureDragAngle(lua_State * L)
{
    lua_pushnumber(L, GetGestureDragAngle());
    return 1;
}

static int _GetGesturePinchVector(lua_State * L)
{
    luaL_pushvector2(L, GetGesturePinchVector());
    return 1;
}

static int _GetGesturePinchAngle(lua_State * L)
{
    lua_pushnumber(L, GetGesturePinchAngle());
    return 1;
}

static int _SetCameraMode(lua_State * L)
{
    SetCameraMode(luaL_checkcamera3d(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _UpdateCamera(lua_State * L)
{
    auto cam = luaL_checkcamera3d(L, 1);
    UpdateCamera(&cam);
    luaL_pushcamera3d(L, cam);
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
              luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawPixelV(lua_State * L)
{
    DrawPixelV(luaL_checkvector2(L, 1),
               luaL_checkcolor(L, 2));
    return 0;
}

static int _DrawLine(lua_State * L)
{
    DrawLine(luaL_checkinteger(L, 1),
             luaL_checkinteger(L, 2),
             luaL_checkinteger(L, 3),
             luaL_checkinteger(L, 4),
             luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawLineV(lua_State * L)
{
    DrawLineV(luaL_checkvector2(L, 1),
              luaL_checkvector2(L, 2),
              luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawLineEx(lua_State * L)
{
    DrawLineEx(luaL_checkvector2(L, 1),
               luaL_checkvector2(L, 2),
               luaL_checknumber(L, 3),
               luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawLineBezier(lua_State * L)
{
    DrawLineBezier(luaL_checkvector2(L, 1),
                   luaL_checkvector2(L, 2),
                   luaL_checknumber(L, 3),
                   luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawCircle(lua_State * L)
{
    DrawCircle(luaL_checkinteger(L, 1),
               luaL_checkinteger(L, 2),
               luaL_checknumber(L, 3),
               luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawCircleSector(lua_State * L)
{
    DrawCircleSector(luaL_checkvector2(L, 1),
                     luaL_checknumber(L, 2),
                     luaL_checkinteger(L, 3),
                     luaL_checkinteger(L, 4),
                     luaL_checkinteger(L, 5),
                     luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawCircleSectorLines(lua_State * L)
{
    DrawCircleSectorLines(luaL_checkvector2(L, 1),
                          luaL_checknumber(L, 2),
                          luaL_checkinteger(L, 3),
                          luaL_checkinteger(L, 4),
                          luaL_checkinteger(L, 5),
                          luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawCircleGradient(lua_State * L)
{
    DrawCircleGradient(luaL_checkinteger(L, 1),
                       luaL_checkinteger(L, 2),
                       luaL_checknumber(L, 3),
                       luaL_checkcolor(L, 4),
                       luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawCircleV(lua_State * L)
{
    DrawCircleV(luaL_checkvector2(L, 1),
                luaL_checknumber(L, 2),
                luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawCircleLines(lua_State * L)
{
    DrawCircleLines(luaL_checkinteger(L, 1),
                    luaL_checkinteger(L, 2),
                    luaL_checknumber(L, 3),
                    luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawRing(lua_State * L)
{
    DrawRing(luaL_checkvector2(L, 1),
             luaL_checknumber(L, 2),
             luaL_checknumber(L, 3),
             luaL_checkinteger(L, 4),
             luaL_checkinteger(L, 5),
             luaL_checkinteger(L, 6),
             luaL_checkcolor(L, 7));
    return 0;
}

static int _DrawRingLines(lua_State * L)
{
    DrawRingLines(luaL_checkvector2(L, 1),
                  luaL_checknumber(L, 2),
                  luaL_checknumber(L, 3),
                  luaL_checkinteger(L, 4),
                  luaL_checkinteger(L, 5),
                  luaL_checkinteger(L, 6),
                  luaL_checkcolor(L, 7));
    return 0;
}

static int _DrawRectangle(lua_State * L)
{
    DrawRectangle(luaL_checkinteger(L, 1),
                  luaL_checkinteger(L, 2),
                  luaL_checkinteger(L, 3),
                  luaL_checkinteger(L, 4),
                  luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawRectangleV(lua_State * L)
{
    DrawRectangleV(luaL_checkvector2(L, 1),
                   luaL_checkvector2(L, 2),
                   luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawRectangleRec(lua_State * L)
{
    DrawRectangleRec(luaL_checkrectangle(L, 1),
                     luaL_checkcolor(L, 2));
    return 0;
}

static int _DrawRectanglePro(lua_State * L)
{
    DrawRectanglePro(luaL_checkrectangle(L, 1),
                     luaL_checkvector2(L, 2),
                     luaL_checknumber(L, 3),
                     luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawRectangleGradientV(lua_State * L)
{
    DrawRectangleGradientV(luaL_checkinteger(L, 1),
                           luaL_checkinteger(L, 2),
                           luaL_checkinteger(L, 3),
                           luaL_checkinteger(L, 4),
                           luaL_checkcolor(L, 5),
                           luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawRectangleGradientH(lua_State * L)
{
    DrawRectangleGradientH(luaL_checkinteger(L, 1),
                           luaL_checkinteger(L, 2),
                           luaL_checkinteger(L, 3),
                           luaL_checkinteger(L, 4),
                           luaL_checkcolor(L, 5),
                           luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawRectangleGradientEx(lua_State * L)
{
    DrawRectangleGradientEx(luaL_checkrectangle(L, 1),
                            luaL_checkcolor(L, 2),
                            luaL_checkcolor(L, 3),
                            luaL_checkcolor(L, 4),
                            luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawRectangleLines(lua_State * L)
{
    DrawRectangleLines(luaL_checkinteger(L, 1),
                       luaL_checkinteger(L, 2),
                       luaL_checkinteger(L, 3),
                       luaL_checkinteger(L, 4),
                       luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawRectangleLinesEx(lua_State * L)
{
    DrawRectangleLinesEx(luaL_checkrectangle(L, 1),
                         luaL_checkinteger(L, 2),
                         luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawRectangleRounded(lua_State * L)
{
    DrawRectangleRounded(luaL_checkrectangle(L, 1),
                         luaL_checknumber(L, 2),
                         luaL_checkinteger(L, 3),
                         luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawRectangleRoundedLines(lua_State * L)
{
    DrawRectangleRoundedLines(luaL_checkrectangle(L, 1),
                              luaL_checknumber(L, 2),
                              luaL_checkinteger(L, 3),
                              luaL_checkinteger(L, 4),
                              luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawTriangle(lua_State * L)
{
    DrawTriangle(luaL_checkvector2(L, 1),
                 luaL_checkvector2(L, 2),
                 luaL_checkvector2(L, 3),
                 luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawTriangleLines(lua_State * L)
{
    DrawTriangleLines(luaL_checkvector2(L, 1),
                      luaL_checkvector2(L, 2),
                      luaL_checkvector2(L, 3),
                      luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawPoly(lua_State * L)
{
    DrawPoly(luaL_checkvector2(L, 1),
             luaL_checkinteger(L, 2),
             luaL_checknumber(L, 3),
             luaL_checknumber(L, 4),
             luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawPolyEx(lua_State * L)
{
    DrawPolyEx(luaL_checkvector2_array(L, 1).data(),
               luaL_checkinteger(L, 2),
               luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawPolyExLines(lua_State * L)
{
    DrawPolyExLines(luaL_checkvector2_array(L, 1).data(),
                    luaL_checkinteger(L, 2),
                    luaL_checkcolor(L, 3));
    return 0;
}

static int _SetShapesTexture(lua_State * L)
{
    SetShapesTexture(luaL_checktexture2d(L, 1),
                     luaL_checkrectangle(L, 2));
    return 0;
}

static int _CheckCollisionRecs(lua_State * L)
{
    auto const result = CheckCollisionRecs(luaL_checkrectangle(L, 1),
                                           luaL_checkrectangle(L, 2));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionCircles(lua_State * L)
{
    auto const result = CheckCollisionCircles(luaL_checkvector2(L, 1),
                                              luaL_checknumber(L, 2),
                                              luaL_checkvector2(L, 3),
                                              luaL_checknumber(L, 4));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionCircleRec(lua_State * L)
{
    auto const result = CheckCollisionCircleRec(luaL_checkvector2(L, 1),
                                                luaL_checknumber(L, 2),
                                                luaL_checkrectangle(L, 3));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _GetCollisionRec(lua_State * L)
{
    auto const result = GetCollisionRec(luaL_checkrectangle(L, 1),
                                        luaL_checkrectangle(L, 2));
    luaL_pushrectangle(L, result);
    return 1;
}

static int _CheckCollisionPointRec(lua_State * L)
{
    auto const result = CheckCollisionPointRec(luaL_checkvector2(L, 1),
                                               luaL_checkrectangle(L, 2));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionPointCircle(lua_State * L)
{
    auto const result = CheckCollisionPointCircle(luaL_checkvector2(L, 1),
                                                  luaL_checkvector2(L, 2),
                                                  luaL_checknumber(L, 3));
    lua_pushboolean(L, result?1:0);
    return 1;
}

static int _CheckCollisionPointTriangle(lua_State * L)
{
    auto const result = CheckCollisionPointTriangle(luaL_checkvector2(L, 1),
                                                    luaL_checkvector2(L, 2),
                                                    luaL_checkvector2(L, 3),
                                                    luaL_checkvector2(L, 4));
    lua_pushboolean(L, result?1:0);
    return 1;
}

# /*********************/
# /* [TEXTURES MODULE] */
# /*********************/

static int _LoadImage(lua_State * L)
{
    auto const result = LoadImage(luaL_checkstring(L, 1));
    luaL_pushimage(L, &result);
    return 1;
}

static int _LoadImageEx(lua_State * L)
{
    auto colors = luaL_checkcolor_array(L, 1);
    auto const result = LoadImageEx(colors.data(), luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    luaL_pushimage(L, &result);
    return 1;
}

static int _LoadImagePro(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _LoadImageRaw(lua_State * L)
{
    auto const result = LoadImageRaw(luaL_checkstring(L, 1),
                                     luaL_checkinteger(L, 2),
                                     luaL_checkinteger(L, 3),
                                     luaL_checkinteger(L, 4),
                                     luaL_checkinteger(L, 5));
    luaL_pushimage(L, &result);
    return 1;
}

static int _ExportImage(lua_State * L)
{
    ExportImage(*luaL_checkimage(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _ExportImageAsCode(lua_State * L)
{
    ExportImageAsCode(*luaL_checkimage(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _LoadTexture(lua_State * L)
{
    auto const result = LoadTexture(luaL_checkstring(L, 1));
    luaL_pushtexture2d(L, result);
    return 1;
}

static int _LoadTextureFromImage(lua_State * L)
{
    luaL_pushtexture2d(L, LoadTextureFromImage(*luaL_checkimage(L, 1)));
    return 1;
}

static int _LoadTextureCubemap(lua_State * L)
{
    auto const tex = LoadTextureCubemap(*luaL_checkimage(L, 1), luaL_checkinteger(L, 2));
    luaL_pushtexture2d(L, tex);
    return 1;
}

static int _LoadRenderTexture(lua_State * L)
{
    auto tex = LoadRenderTexture(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2));
    luaL_pushrendertexture2d(L, tex);
    return 1;
}

static int _UnloadImage(lua_State * L)
{
    UnloadImage(*luaL_checkimage(L, 1));
    return 0;
}

static int _UnloadTexture(lua_State * L)
{
    UnloadTexture(luaL_checktexture2d(L, 1));
    return 0;
}

static int _UnloadRenderTexture(lua_State * L)
{
    UnloadRenderTexture(luaL_checkrendertexture2d(L, 1));
    return 0;
}

static int _GetImageData(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    assert(image != nullptr);

    auto const SIZE = image->width * image->height;

    Color * pixels = GetImageData(*image);
    assert(pixels != nullptr);

    lua_createtable(L, SIZE, 0);
    for (int i = 0; i < SIZE; ++i) {
        luaL_pushcolor(L, pixels[i]);
        lua_rawseti(L, -2, i+1);
    }
    free(pixels);
    return 1;
}

static int _GetImageDataNormalized(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    assert(image != nullptr);

    auto const SIZE = image->width * image->height;

    Vector4 * pixels = GetImageDataNormalized(*image);
    assert(pixels != nullptr);

    lua_createtable(L, SIZE, 0);
    for (int i = 0; i < SIZE; ++i) {
        luaL_pushvector4(L, pixels[i]);
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
    auto const image = GetTextureData(luaL_checktexture2d(L, 1));
    luaL_pushimage(L, &image);
    return 1;
}

static int _GetScreenData(lua_State * L)
{
    auto const image = GetScreenData();
    luaL_pushimage(L, &image);
    return 1;
}

static int _UpdateTexture(lua_State * L)
{
    auto tex = luaL_checktexture2d(L, 1);
    auto colors = luaL_checkcolor_array(L, 2);
    UpdateTexture(tex, (void const *)colors.data());
    return 0;
}

static int _ImageCopy(lua_State * L)
{
    auto image = ImageCopy(*luaL_checkimage(L, 1));
    luaL_pushimage(L, &image);
    return 1;
}

static int _ImageToPOT(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageToPOT(image, luaL_checkcolor(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageFormat(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageFormat(image, luaL_checkinteger(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageAlphaMask(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageAlphaMask(image, *luaL_checkimage(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageAlphaClear(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageAlphaClear(image, luaL_checkcolor(L, 2), luaL_checknumber(L, 3));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageAlphaCrop(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageAlphaCrop(image, luaL_checknumber(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageAlphaPremultiply(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageAlphaPremultiply(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageCrop(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageCrop(image, luaL_checkrectangle(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageResize(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageResize(image, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageResizeNN(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageResizeNN(image, luaL_checkinteger(L, 2), luaL_checkinteger(L, 3));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageResizeCanvas(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageResizeCanvas(image,
                      luaL_checkinteger(L, 2),
                      luaL_checkinteger(L, 3),
                      luaL_checkinteger(L, 4),
                      luaL_checkinteger(L, 5),
                      luaL_checkcolor(L, 6));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageMipmaps(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageMipmaps(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageDither(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageDither(image,
                luaL_checkinteger(L, 2),
                luaL_checkinteger(L, 3),
                luaL_checkinteger(L, 4),
                luaL_checkinteger(L, 5));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageExtractPalette(lua_State * L)
{
    int extract_count = 0;
    Color * pixels = ImageExtractPalette(*luaL_checkimage(L, 1), luaL_checkinteger(L, 2), &extract_count);
    assert(pixels != nullptr);

    lua_createtable(L, extract_count, 0);
    for (int i = 0; i < extract_count; ++i) {
        luaL_pushcolor(L, pixels[i]);
        lua_rawseti(L, -2, i+1);
    }
    free(pixels);
    return 1;
}

static int _ImageText(lua_State * L)
{
    auto image = ImageText(luaL_checkstring(L, 1), luaL_checkinteger(L, 2), luaL_checkcolor(L, 3));
    luaL_pushimage(L, &image);
    return 1;
}

static int _ImageTextEx(lua_State * L)
{
    auto image = ImageTextEx(*luaL_checkfont(L, 1),
                             luaL_checkstring(L, 2),
                             luaL_checknumber(L, 3),
                             luaL_checknumber(L, 4),
                             luaL_checkcolor(L, 5));
    luaL_pushimage(L, &image);
    return 1;
}

static int _ImageDraw(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageDraw(image,
              *luaL_checkimage(L, 2),
              luaL_checkrectangle(L, 3),
              luaL_checkrectangle(L, 4));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageDrawRectangle(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageDrawRectangle(image,
                       luaL_checkrectangle(L, 2),
                       luaL_checkcolor(L, 3));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageDrawRectangleLines(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageDrawRectangleLines(image,
                            luaL_checkrectangle(L, 2),
                            luaL_checkinteger(L, 3),
                            luaL_checkcolor(L, 4));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageDrawText(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageDrawText(image,
                  luaL_checkvector2(L, 2),
                  luaL_checkstring(L, 3),
                  luaL_checkinteger(L, 4),
                  luaL_checkcolor(L, 5));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageDrawTextEx(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageDrawTextEx(image,
                    luaL_checkvector2(L, 2),
                    *luaL_checkfont(L, 3),
                    luaL_checkstring(L, 4),
                    luaL_checknumber(L, 5),
                    luaL_checknumber(L, 6),
                    luaL_checkcolor(L, 7));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageFlipVertical(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageFlipVertical(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageFlipHorizontal(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageFlipHorizontal(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageRotateCW(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageRotateCW(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageRotateCCW(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageRotateCCW(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageColorTint(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageColorTint(image, luaL_checkcolor(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageColorInvert(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageColorInvert(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageColorGrayscale(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageColorGrayscale(image);
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageColorContrast(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageColorContrast(image, luaL_checknumber(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageColorBrightness(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageColorBrightness(image, luaL_checkinteger(L, 2));
    luaL_pushimage(L, image);
    return 1;
}

static int _ImageColorReplace(lua_State * L)
{
    auto * image = luaL_checkimage(L, 1);
    ImageColorReplace(image, luaL_checkcolor(L, 2), luaL_checkcolor(L, 3));
    luaL_pushimage(L, image);
    return 1;
}

static int _GenImageColor(lua_State * L)
{
    auto const result = GenImageColor(luaL_checkinteger(L, 1),
                                      luaL_checkinteger(L, 2),
                                      luaL_checkcolor(L, 3));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenImageGradientV(lua_State * L)
{
    auto const result = GenImageGradientV(luaL_checkinteger(L, 1),
                                          luaL_checkinteger(L, 2),
                                          luaL_checkcolor(L, 3),
                                          luaL_checkcolor(L, 4));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenImageGradientH(lua_State * L)
{
    auto const result = GenImageGradientH(luaL_checkinteger(L, 1),
                                          luaL_checkinteger(L, 2),
                                          luaL_checkcolor(L, 3),
                                          luaL_checkcolor(L, 4));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenImageGradientRadial(lua_State * L)
{
    auto const result = GenImageGradientRadial(luaL_checkinteger(L, 1),
                                               luaL_checkinteger(L, 2),
                                               luaL_checknumber(L, 3),
                                               luaL_checkcolor(L, 4),
                                               luaL_checkcolor(L, 5));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenImageChecked(lua_State * L)
{
    auto const result = GenImageChecked(luaL_checkinteger(L, 1),
                                        luaL_checkinteger(L, 2),
                                        luaL_checkinteger(L, 3),
                                        luaL_checkinteger(L, 4),
                                        luaL_checkcolor(L, 5),
                                        luaL_checkcolor(L, 6));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenImageWhiteNoise(lua_State * L)
{
    auto const result = GenImageWhiteNoise(luaL_checkinteger(L, 1),
                                           luaL_checkinteger(L, 2),
                                           luaL_checknumber(L, 3));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenImagePerlinNoise(lua_State * L)
{
    auto const result = GenImagePerlinNoise(luaL_checkinteger(L, 1),
                                            luaL_checkinteger(L, 2),
                                            luaL_checkinteger(L, 3),
                                            luaL_checkinteger(L, 4),
                                            luaL_checknumber(L, 5));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenImageCellular(lua_State * L)
{
    auto const result = GenImageCellular(luaL_checkinteger(L, 1),
                                         luaL_checkinteger(L, 2),
                                         luaL_checkinteger(L, 3));
    luaL_pushimage(L, &result);
    return 1;
}

static int _GenTextureMipmaps(lua_State * L)
{
    //void GenTextureMipmaps(Texture2D * texture);
    return 0;
}

static int _SetTextureFilter(lua_State * L)
{
    SetTextureFilter(luaL_checktexture2d(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _SetTextureWrap(lua_State * L)
{
    SetTextureWrap(luaL_checktexture2d(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _DrawTexture(lua_State * L)
{
    DrawTexture(luaL_checktexture2d(L, 1),
                luaL_checkinteger(L, 2),
                luaL_checkinteger(L, 3),
                luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawTextureV(lua_State * L)
{
    DrawTextureV(luaL_checktexture2d(L, 1),
                 luaL_checkvector2(L, 2),
                 luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawTextureEx(lua_State * L)
{
    DrawTextureEx(luaL_checktexture2d(L, 1),
                  luaL_checkvector2(L, 2),
                  luaL_checknumber(L, 3),
                  luaL_checknumber(L, 4),
                  luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawTextureRec(lua_State * L)
{
    DrawTextureRec(luaL_checktexture2d(L, 1),
                   luaL_checkrectangle(L, 2),
                   luaL_checkvector2(L, 3),
                   luaL_checkcolor(L, 4));
    return 0;
}

static int _DrawTextureQuad(lua_State * L)
{
    DrawTextureQuad(luaL_checktexture2d(L, 1),
                    luaL_checkvector2(L, 2),
                    luaL_checkvector2(L, 3),
                    luaL_checkrectangle(L, 4),
                    luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawTexturePro(lua_State * L)
{
    DrawTexturePro(luaL_checktexture2d(L, 1),
                   luaL_checkrectangle(L, 2),
                   luaL_checkrectangle(L, 3),
                   luaL_checkvector2(L, 4),
                   luaL_checknumber(L, 5),
                   luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawTextureNPatch(lua_State * L)
{
    DrawTextureNPatch(luaL_checktexture2d(L, 1),
                      luaL_checknpatchinfo(L, 2),
                      luaL_checkrectangle(L, 3),
                      luaL_checkvector2(L, 4),
                      luaL_checknumber(L, 5),
                      luaL_checkcolor(L, 6));
    return 0;
}

# /*****************/
# /* [TEXT MODULE] */
# /*****************/

static int _GetFontDefault(lua_State * L)
{
    auto result = GetFontDefault();
    luaL_pushfont(L, &result);
    return 1;
}

static int _LoadFont(lua_State * L)
{
    auto result = LoadFont(luaL_checkstring(L, 1));
    luaL_pushfont(L, &result);
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
        chars = luaL_checkchars_array(L, 3);
        if (!chars.empty()) {
            font_chars = chars.data();
            chars_count = chars.size();
        }
    }

    auto result = LoadFontEx(file_name, font_size, font_chars, chars_count);
    luaL_pushfont(L, &result);
    return 1;
}

static int _LoadFontFromImage(lua_State * L)
{
    auto result = LoadFontFromImage(*luaL_checkimage(L, 1), luaL_checkcolor(L, 2), luaL_checkinteger(L, 3));
    luaL_pushfont(L, &result);
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
        chars = luaL_checkchars_array(L, 3);
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

    luaL_pushcharinfo_array(L, infos, chars_count);
    return 1;
}

static int _GenImageFontAtlas(lua_State * L)
{
    auto chars = luaL_checkcharinfo_array(L, 1);
    auto image = GenImageFontAtlas(chars.data(), chars.size(),
                                   luaL_checkinteger(L, 2),
                                   luaL_checkinteger(L, 3),
                                   luaL_checkinteger(L, 4));
    luaL_pushimage(L, &image);
    return 1;
}

static int _UnloadFont(lua_State * L)
{
    UnloadFont(*luaL_checkfont(L, 1));
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
             luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawTextEx(lua_State * L)
{
    DrawTextEx(*luaL_checkfont(L, 1),
               luaL_checkstring(L, 2),
               luaL_checkvector2(L, 3),
               luaL_checknumber(L, 4),
               luaL_checknumber(L, 5),
               luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawTextRec(lua_State * L)
{
    DrawTextRec(*luaL_checkfont(L, 1),
                luaL_checkstring(L, 2),
                luaL_checkrectangle(L, 3),
                luaL_checknumber(L, 4),
                luaL_checknumber(L, 5),
                lua_toboolean(L, 6),
                luaL_checkcolor(L, 7));
    return 0;
}

static int _DrawTextRecEx(lua_State * L)
{
    DrawTextRecEx(*luaL_checkfont(L, 1),
                  luaL_checkstring(L, 2),
                  luaL_checkrectangle(L, 3),
                  luaL_checknumber(L, 4),
                  luaL_checknumber(L, 5),
                  lua_toboolean(L, 6),
                  luaL_checkcolor(L, 7),
                  luaL_checkinteger(L, 8),
                  luaL_checkinteger(L, 9),
                  luaL_checkcolor(L, 10),
                  luaL_checkcolor(L, 11));
    return 0;
}

static int _MeasureText(lua_State * L)
{
    lua_pushinteger(L, MeasureText(luaL_checkstring(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _MeasureTextEx(lua_State * L)
{
    luaL_pushvector2(L, MeasureTextEx(*luaL_checkfont(L, 1),
                                      luaL_checkstring(L, 2),
                                      luaL_checknumber(L, 3),
                                      luaL_checknumber(L, 4)));
    return 1;
}

static int _GetGlyphIndex(lua_State * L)
{
    lua_pushinteger(L, GetGlyphIndex(*luaL_checkfont(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _TextIsEqual(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextLength(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextSubtext(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextReplace(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextInsert(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextJoin(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextSplit(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextAppend(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextFindIndex(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextToUpper(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextToLower(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextToPascal(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

static int _TextToInteger(lua_State * L)
{
    luaL_error(L, "Unsupported operation error");
    return 0;
}

# /*******************/
# /* [MODELS MODULE] */
# /*******************/

static int _DrawLine3D(lua_State * L)
{
    DrawLine3D(luaL_checkvector3(L, 1),
               luaL_checkvector3(L, 2),
               luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawCircle3D(lua_State * L)
{
    DrawCircle3D(luaL_checkvector3(L, 1),
                 luaL_checknumber(L, 2),
                 luaL_checkvector3(L, 3),
                 luaL_checknumber(L, 4),
                 luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawCube(lua_State * L)
{
    DrawCube(luaL_checkvector3(L, 1),
             luaL_checknumber(L, 2),
             luaL_checknumber(L, 3),
             luaL_checknumber(L, 4),
             luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawCubeV(lua_State * L)
{
    DrawCubeV(luaL_checkvector3(L, 1),
              luaL_checkvector3(L, 2),
              luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawCubeWires(lua_State * L)
{
    DrawCubeWires(luaL_checkvector3(L, 1),
                  luaL_checknumber(L, 2),
                  luaL_checknumber(L, 3),
                  luaL_checknumber(L, 4),
                  luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawCubeWiresV(lua_State * L)
{
    DrawCubeWiresV(luaL_checkvector3(L, 1),
                   luaL_checkvector3(L, 2),
                   luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawCubeTexture(lua_State * L)
{
    DrawCubeTexture(luaL_checktexture2d(L, 1),
                    luaL_checkvector3(L, 2),
                    luaL_checknumber(L, 3),
                    luaL_checknumber(L, 4),
                    luaL_checknumber(L, 5),
                    luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawSphere(lua_State * L)
{
    DrawSphere(luaL_checkvector3(L, 1),
               luaL_checknumber(L, 2),
               luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawSphereEx(lua_State * L)
{
    DrawSphereEx(luaL_checkvector3(L, 1),
                 luaL_checknumber(L, 2),
                 luaL_checkinteger(L, 3),
                 luaL_checkinteger(L, 4),
                 luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawSphereWires(lua_State * L)
{
    DrawSphereWires(luaL_checkvector3(L, 1),
                    luaL_checknumber(L, 2),
                    luaL_checkinteger(L, 3),
                    luaL_checkinteger(L, 4),
                    luaL_checkcolor(L, 5));
    return 0;
}

static int _DrawCylinder(lua_State * L)
{
    DrawCylinder(luaL_checkvector3(L, 1),
                 luaL_checknumber(L, 2),
                 luaL_checknumber(L, 3),
                 luaL_checknumber(L, 4),
                 luaL_checkinteger(L, 5),
                 luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawCylinderWires(lua_State * L)
{
    DrawCylinderWires(luaL_checkvector3(L, 1),
                      luaL_checknumber(L, 2),
                      luaL_checknumber(L, 3),
                      luaL_checknumber(L, 4),
                      luaL_checkinteger(L, 5),
                      luaL_checkcolor(L, 6));
    return 0;
}

static int _DrawPlane(lua_State * L)
{
    DrawPlane(luaL_checkvector3(L, 1),
              luaL_checkvector2(L, 2),
              luaL_checkcolor(L, 3));
    return 0;
}

static int _DrawRay(lua_State * L)
{
    DrawRay(luaL_checkray(L, 1),
            luaL_checkcolor(L, 2));
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
    DrawGizmo(luaL_checkvector3(L, 1));
    return 0;
}

static int _LoadModel(lua_State * L)
{
    auto mesh = LoadModel(luaL_checkstring(L, 1));
    luaL_pushmodel(L, &mesh);
    return 1;
}

static int _LoadModelFromMesh(lua_State * L)
{
    auto mesh = LoadModelFromMesh(*luaL_checkmesh(L, 1));
    luaL_pushmodel(L, &mesh);
    return 1;
}

static int _UnloadModel(lua_State * L)
{
    UnloadModel(*luaL_checkmodel(L, 1));
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
//void DrawBillboardRec(Camera camera, Texture2D texture, Rectangle source_rec, Vector3 center, float size, Color tint);
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

//char * LoadText(char const * file_name);
//Shader LoadShader(char const * vs_file_name, char const * fs_file_name);
//Shader LoadShaderCode(char * vs_code, char * fs_code);
//void UnloadShader(Shader shader);
//
//Shader GetShaderDefault();
//Texture2D GetTextureDefault();
//
//int GetShaderLocation(Shader shader, char const * uniform_name);
//void SetShaderValue(Shader shader, int uniform_loc, void const * value, int uniform_type);
//void SetShaderValueV(Shader shader, int uniform_loc, void const * value, int uniform_type, int count);
//void SetShaderValueMatrix(Shader shader, int uniform_loc, Matrix mat);
//void SetShaderValueTexture(Shader shader, int uniform_loc, Texture2D texture);
//void SetMatrixProjection(Matrix proj);
//void SetMatrixModelview(Matrix view);
//Matrix GetMatrixModelview();
//
//Texture2D GenTextureCubemap(Shader shader, Texture2D sky_hdr, int size);
//Texture2D GenTextureIrradiance(Shader shader, Texture2D cubemap, int size);
//Texture2D GenTexturePrefilter(Shader shader, Texture2D cubemap, int size);
//Texture2D GenTextureBRDF(Shader shader, int size);
//
//void BeginShaderMode(Shader shader);
//void EndShaderMode();
//void BeginBlendMode(int mode);
//void EndBlendMode();
//void BeginScissorMode(int x, int y, int width, int height);
//void EndScissorMode();
//
//VrDeviceInfo GetVrDeviceInfo(int vr_device_type);
//void InitVrSimulator(VrDeviceInfo info);
//void UpdateVrTracking(Camera * camera);
//void CloseVrSimulator();
//bool IsVrSimulatorReady();
//void ToggleVrMode();
//void BeginVrDrawing();
//void EndVrDrawing();

# /******************/
# /* [AUDIO MODULE] */
# /******************/

//void InitAudioDevice();
//void CloseAudioDevice();
//bool IsAudioDeviceReady();
//void SetMasterVolume(float volume);
//
//Wave LoadWave(char const * file_name);
//Wave LoadWaveEx(void * data, int sample_count, int sample_rate, int sample_size, int channels);
//Sound LoadSound(char const * file_name);
//Sound LoadSoundFromWave(Wave wave);
//void UpdateSound(Sound sound, void const * data, int samples_count);
//void UnloadWave(Wave wave);
//void UnloadSound(Sound sound);
//void ExportWave(Wave wave, char const * file_name);
//void ExportWaveAsCode(Wave wave, char const * file_name);
//
//void PlaySound(Sound sound);
//void PauseSound(Sound sound);
//void ResumeSound(Sound sound);
//void StopSound(Sound sound);
//bool IsSoundPlaying(Sound sound);
//void SetSoundVolume(Sound sound, float volume);
//void SetSoundPitch(Sound sound, float pitch);
//void WaveFormat(Wave * wave, int sample_rate, int sample_size, int channels);
//Wave WaveCopy(Wave wave);
//void WaveCrop(Wave * wave, int init_sample, int final_sample);
//float * GetWaveData(Wave wave);
//
//Music LoadMusicStream(char const * file_name);
//void UnloadMusicStream(Music music);
//void PlayMusicStream(Music music);
//void UpdateMusicStream(Music music);
//void StopMusicStream(Music music);
//void PauseMusicStream(Music music);
//void ResumeMusicStream(Music music);
//bool IsMusicPlaying(Music music);
//void SetMusicVolume(Music music, float volume);
//void SetMusicPitch(Music music, float pitch);
//void SetMusicLoopCount(Music music, int count);
//float GetMusicTimeLength(Music music);
//float GetMusicTimePlayed(Music music);
//
//AudioStream InitAudioStream(unsigned int sample_rate, unsigned int sample_size, unsigned int channels);
//void UpdateAudioStream(AudioStream stream, void const * data, int samples_count);
//void CloseAudioStream(AudioStream stream);
//bool IsAudioBufferProcessed(AudioStream stream);
//void PlayAudioStream(AudioStream stream);
//void PauseAudioStream(AudioStream stream);
//void ResumeAudioStream(AudioStream stream);
//bool IsAudioStreamPlaying(AudioStream stream);
//void StopAudioStream(AudioStream stream);
//void SetAudioStreamVolume(AudioStream stream, float volume);
//void SetAudioStreamPitch(AudioStream stream, float pitch);

# /****************/
# /* [GUI MODULE] */
# /****************/

static int _GuiEnable(lua_State * L)
{
    GuiEnable();
    return 0;
}

static int _GuiDisable(lua_State * L)
{
    GuiDisable();
    return 0;
}

static int _GuiLock(lua_State * L)
{
    GuiLock();
    return 0;
}

static int _GuiUnlock(lua_State * L)
{
    GuiUnlock();
    return 0;
}

static int _GuiState(lua_State * L)
{
    GuiState(luaL_checkinteger(L, 1));
    return 0;
}

static int _GuiFont(lua_State * L)
{
    GuiFont(*luaL_checkfont(L, 1));
    return 0;
}

static int _GuiFade(lua_State * L)
{
    GuiFade(luaL_checknumber(L, 1));
    return 0;
}

static int _GuiSetStyle(lua_State * L)
{
    GuiSetStyle(luaL_checkinteger(L, 1),
                luaL_checkinteger(L, 2),
                luaL_checkinteger(L, 3));
    return 0;
}

static int _GuiGetStyle(lua_State * L)
{
    lua_pushinteger(L, GuiGetStyle(luaL_checkinteger(L, 1), luaL_checkinteger(L, 2)));
    return 1;
}

static int _GuiWindowBox(lua_State * L)
{
    lua_pushboolean(L, GuiWindowBox(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

static int _GuiGroupBox(lua_State * L)
{
    GuiGroupBox(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiLine(lua_State * L)
{
    GuiLine(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiPanel(lua_State * L)
{
    GuiPanel(luaL_checkrectangle(L, 1));
    return 0;
}

static int _GuiScrollPanel(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    Rectangle content = luaL_checkrectangle(L, 2);
    Vector2 scroll = luaL_checkvector2(L, 3);
    luaL_pushrectangle(L, GuiScrollPanel(bounds, content, &scroll));
    luaL_pushvector2(L, scroll);
    return 2;
}

static int _GuiLabel(lua_State * L)
{
    GuiLabel(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiButton(lua_State * L)
{
    lua_pushboolean(L, GuiButton(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

static int _GuiLabelButton(lua_State * L)
{
    lua_pushboolean(L, GuiLabelButton(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2)));
    return 1;
}

static int _GuiImageButton(lua_State * L)
{
    lua_pushboolean(L, GuiImageButton(luaL_checkrectangle(L, 1), luaL_checktexture2d(L, 2)));
    return 1;
}

static int _GuiImageButtonEx(lua_State * L)
{
    lua_pushboolean(L, GuiImageButtonEx(luaL_checkrectangle(L, 1),
                                        luaL_checktexture2d(L, 2),
                                        luaL_checkrectangle(L, 3),
                                        luaL_checkstring(L, 4)));
    return 1;
}

static int _GuiToggle(lua_State * L)
{
    lua_pushboolean(L, GuiToggle(luaL_checkrectangle(L, 1),
                                 luaL_checkstring(L, 2),
                                 lua_toboolean(L, 3)));
    return 1;
}

static int _GuiToggleGroup(lua_State * L)
{
    lua_pushinteger(L, GuiToggleGroup(luaL_checkrectangle(L, 1),
                                      luaL_checkstring(L, 2),
                                      luaL_checkinteger(L, 3)));
    return 1;
}

static int _GuiCheckBox(lua_State * L)
{
    lua_pushboolean(L, GuiCheckBox(luaL_checkrectangle(L, 1),
                                   luaL_checkstring(L, 2),
                                   luaL_checkinteger(L, 3)));
    return 1;
}

static int _GuiComboBox(lua_State * L)
{
    lua_pushinteger(L, GuiComboBox(luaL_checkrectangle(L, 1),
                                   luaL_checkstring(L, 2),
                                   luaL_checkinteger(L, 3)));
    return 1;
}

static int _GuiDropdownBox(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    char const * text = luaL_checkstring(L, 2);
    int active = luaL_checkinteger(L, 3);
    bool edit_mode = lua_toboolean(L, 4);
    lua_pushboolean(L, GuiDropdownBox(bounds, text, &active, edit_mode));
    lua_pushinteger(L, active);
    return 2;
}

static int _GuiSpinner(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    int value = luaL_checkinteger(L, 2);
    int min_value = luaL_checkinteger(L, 3);
    int max_value = luaL_checkinteger(L, 4);
    bool edit_mode = lua_toboolean(L, 5);
    lua_pushboolean(L, GuiSpinner(bounds, &value, min_value, max_value, edit_mode));
    lua_pushinteger(L, value);
    return 2;
}

static int _GuiValueBox(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    int value = luaL_checkinteger(L, 2);
    int min_value = luaL_checkinteger(L, 3);
    int max_value = luaL_checkinteger(L, 4);
    bool edit_mode = lua_toboolean(L, 5);
    lua_pushboolean(L, GuiValueBox(bounds, &value, min_value, max_value, edit_mode));
    lua_pushinteger(L, value);
    return 2;
}

static int _GuiTextBox(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    char const * text = luaL_checkstring(L, 2);
    int text_size = luaL_checkinteger(L, 3);
    bool edit_mode = lua_toboolean(L, 4);

    char buffer[MAX_TEXT_BOX_LENGTH] = {0,};
    strncpy(buffer, text, MAX_TEXT_BOX_LENGTH);

    lua_pushboolean(L, GuiTextBox(bounds, buffer, text_size, edit_mode));
    lua_pushstring(L, buffer);
    return 2;
}

static int _GuiTextBoxMulti(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    char const * text = luaL_checkstring(L, 2);
    int text_size = luaL_checkinteger(L, 3);
    bool edit_mode = lua_toboolean(L, 4);

    char buffer[MAX_TEXT_BOX_LENGTH] = {0,};
    strncpy(buffer, text, MAX_TEXT_BOX_LENGTH);

    lua_pushboolean(L, GuiTextBoxMulti(bounds, buffer, text_size, edit_mode));
    lua_pushstring(L, buffer);
    return 2;
}

static int _GuiSlider(lua_State * L)
{
    lua_pushnumber(L, GuiSlider(luaL_checkrectangle(L, 1),
                                luaL_checkstring(L, 2),
                                luaL_checknumber(L, 3),
                                luaL_checknumber(L, 4),
                                luaL_checknumber(L, 5),
                                lua_toboolean(L, 6)));
    return 1;
}

static int _GuiSliderBar(lua_State * L)
{
    lua_pushnumber(L, GuiSliderBar(luaL_checkrectangle(L, 1),
                                   luaL_checkstring(L, 2),
                                   luaL_checknumber(L, 3),
                                   luaL_checknumber(L, 4),
                                   luaL_checknumber(L, 5),
                                   lua_toboolean(L, 6)));
    return 1;
}

static int _GuiProgressBar(lua_State * L)
{
    lua_pushnumber(L, GuiProgressBar(luaL_checkrectangle(L, 1),
                                     luaL_checkstring(L, 2),
                                     luaL_checknumber(L, 3),
                                     luaL_checknumber(L, 4),
                                     luaL_checknumber(L, 5),
                                     lua_toboolean(L, 6)));
    return 1;
}

static int _GuiStatusBar(lua_State * L)
{
    GuiStatusBar(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiDummyRec(lua_State * L)
{
    GuiDummyRec(luaL_checkrectangle(L, 1), luaL_checkstring(L, 2));
    return 0;
}

static int _GuiScrollBar(lua_State * L)
{
    lua_pushinteger(L, GuiScrollBar(luaL_checkrectangle(L, 1),
                                    luaL_checkinteger(L, 2),
                                    luaL_checkinteger(L, 3),
                                    luaL_checkinteger(L, 4)));
    return 1;
}

static int _GuiListView(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    char const * text = luaL_checkstring(L, 2);
    int active = luaL_checkinteger(L, 3);
    int scroll_index = luaL_checkinteger(L, 4);
    bool edit_mode = lua_toboolean(L, 5);

    lua_pushboolean(L, GuiListView(bounds, text, &active, &scroll_index, edit_mode));
    lua_pushinteger(L, active);
    lua_pushinteger(L, scroll_index);
    return 3;
}

static int _GuiListViewEx(lua_State * L)
{
    Rectangle bounds = luaL_checkrectangle(L, 1);
    auto text = luaL_checkstring_array(L, 2);
    int count = static_cast<int>(text.size());
    auto enabled = luaL_checkinteger_array(L, 3);
    int active = luaL_checkinteger(L, 4);
    int focus = 0;
    int scroll_index = luaL_checkinteger(L, 5);
    bool edit_mode = lua_toboolean(L, 6);

    std::vector<char const *> text_list(count);
    for (int i = 0; i < count; ++i) {
        text_list[i] = text[i].data();
    }

    lua_pushboolean(L, GuiListViewEx(bounds, text_list.data(), count, enabled.data(),
                                     &active, &focus, &scroll_index, edit_mode));
    lua_pushinteger(L, active);
    lua_pushinteger(L, focus);
    lua_pushinteger(L, scroll_index);
    return 4;
}

static int _GuiMessageBox(lua_State * L)
{
    lua_pushinteger(L, GuiMessageBox(luaL_checkrectangle(L, 1),
                                     luaL_checkstring(L, 2),
                                     luaL_checkstring(L, 3),
                                     luaL_checkstring(L, 4)));
    return 1;
}

static int _GuiColorPicker(lua_State * L)
{
    luaL_pushcolor(L, GuiColorPicker(luaL_checkrectangle(L, 1),
                                     luaL_checkcolor(L, 2)));
    return 1;
}

static int _GuiGrid(lua_State * L)
{
    luaL_pushvector2(L, GuiGrid(luaL_checkrectangle(L, 1),
                                luaL_checknumber(L, 2),
                                luaL_checkinteger(L, 3)));
    return 1;
}

static int _GuiLoadStyle(lua_State * L)
{
    GuiLoadStyle(luaL_checkstring(L, 1));
    return 0;
}

static int _GuiLoadStyleProps(lua_State * L)
{
    auto props = luaL_checkinteger_array(L, 1);
    GuiLoadStyleProps(props.data(), props.size());
    return 0;
}

static int _GuiLoadStyleDefault(lua_State * L)
{
    GuiLoadStyleDefault();
    return 0;
}

static int _GuiUpdateStyleComplete(lua_State * L)
{
    GuiUpdateStyleComplete();
    return 0;
}

#ifndef RAY_REGISTER
#define RAY_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_lay_core[] = {
        { METATABLE_IMAGE, _Image },
        { METATABLE_CHARINFO, _CharInfo },
        { METATABLE_FONT, _Font },
        { METATABLE_MESH, _Mesh },
        { METATABLE_MATERIAL, _Material },
        { METATABLE_MODEL, _Model },
        { METATABLE_MODELANIMATION, _ModelAnimation },
        { METATABLE_WAVE, _Wave },
        { METATABLE_SOUND, _Sound },
        { METATABLE_AUDIOSTREAM, _AudioStream },

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
        // RAY_REGISTER(LoadText),
        // RAY_REGISTER(LoadShader),
        // RAY_REGISTER(LoadShaderCode),
        // RAY_REGISTER(UnloadShader),
        // RAY_REGISTER(GetShaderDefault),
        // RAY_REGISTER(GetTextureDefault),

        // [SHADERS] Shader configuration functions
        // RAY_REGISTER(GetShaderLocation),
        // RAY_REGISTER(SetShaderValue),
        // RAY_REGISTER(SetShaderValueV),
        // RAY_REGISTER(SetShaderValueMatrix),
        // RAY_REGISTER(SetShaderValueTexture),
        // RAY_REGISTER(SetMatrixProjection),
        // RAY_REGISTER(SetMatrixModelview),
        // RAY_REGISTER(GetMatrixModelview),

        // [SHADERS] Shading begin/end functions
        // RAY_REGISTER(BeginShaderMode),
        // RAY_REGISTER(EndShaderMode),
        // RAY_REGISTER(BeginBlendMode),
        // RAY_REGISTER(EndBlendMode),
        // RAY_REGISTER(BeginScissorMode),
        // RAY_REGISTER(EndScissorMode),

        // [SHADERS] VR control functions
        // RAY_REGISTER(InitVrSimulator),
        // RAY_REGISTER(CloseVrSimulator),
        // RAY_REGISTER(UpdateVrTracking),
        // RAY_REGISTER(SetVrConfiguration),
        // RAY_REGISTER(IsVrSimulatorReady),
        // RAY_REGISTER(ToggleVrMode),
        // RAY_REGISTER(BeginVrDrawing),
        // RAY_REGISTER(EndVrDrawing),

        // [AUDIO] Audio device management functions
        // RAY_REGISTER(InitAudioDevice),
        // RAY_REGISTER(CloseAudioDevice),
        // RAY_REGISTER(IsAudioDeviceReady),
        // RAY_REGISTER(SetMasterVolume),

        // [AUDIO] Wave/Sound loading/unloading functions
        // RAY_REGISTER(LoadWave),
        // RAY_REGISTER(LoadWaveEx),
        // RAY_REGISTER(LoadSound),
        // RAY_REGISTER(LoadSoundFromWave),
        // RAY_REGISTER(UpdateSound),
        // RAY_REGISTER(UnloadWave),
        // RAY_REGISTER(UnloadSound),
        // RAY_REGISTER(ExportWave),
        // RAY_REGISTER(ExportWaveAsCode),

        // [AUDIO] Wave/Sound management functions
        // RAY_REGISTER(PlaySound),
        // RAY_REGISTER(PauseSound),
        // RAY_REGISTER(ResumeSound),
        // RAY_REGISTER(StopSound),
        // RAY_REGISTER(IsSoundPlaying),
        // RAY_REGISTER(SetSoundVolume),
        // RAY_REGISTER(SetSoundPitch),
        // RAY_REGISTER(WaveFormat),
        // RAY_REGISTER(WaveCopy),
        // RAY_REGISTER(WaveCrop),
        // RAY_REGISTER(GetWaveData),

        // [AUDIO] Music management functions
        // RAY_REGISTER(LoadMusicStream),
        // RAY_REGISTER(UnloadMusicStream),
        // RAY_REGISTER(PlayMusicStream),
        // RAY_REGISTER(UpdateMusicStream),
        // RAY_REGISTER(StopMusicStream),
        // RAY_REGISTER(PauseMusicStream),
        // RAY_REGISTER(ResumeMusicStream),
        // RAY_REGISTER(IsMusicPlaying),
        // RAY_REGISTER(SetMusicVolume),
        // RAY_REGISTER(SetMusicPitch),
        // RAY_REGISTER(SetMusicLoopCount),
        // RAY_REGISTER(GetMusicTimeLength),
        // RAY_REGISTER(GetMusicTimePlayed),

        // [AUDIO] AudioStream management functions
        // RAY_REGISTER(InitAudioStream),
        // RAY_REGISTER(UpdateAudioStream),
        // RAY_REGISTER(CloseAudioStream),
        // RAY_REGISTER(IsAudioBufferProcessed),
        // RAY_REGISTER(PlayAudioStream),
        // RAY_REGISTER(PauseAudioStream),
        // RAY_REGISTER(ResumeAudioStream),
        // RAY_REGISTER(IsAudioStreamPlaying),
        // RAY_REGISTER(StopAudioStream),
        // RAY_REGISTER(SetAudioStreamVolume),
        // RAY_REGISTER(SetAudioStreamPitch),

        // [GUI] raygui methods.
        RAY_REGISTER(GuiEnable),
        RAY_REGISTER(GuiDisable),
        RAY_REGISTER(GuiLock),
        RAY_REGISTER(GuiUnlock),
        RAY_REGISTER(GuiState),
        RAY_REGISTER(GuiFont),
        RAY_REGISTER(GuiFade),
        RAY_REGISTER(GuiSetStyle),
        RAY_REGISTER(GuiGetStyle),
        RAY_REGISTER(GuiWindowBox),
        RAY_REGISTER(GuiGroupBox),
        RAY_REGISTER(GuiLine),
        RAY_REGISTER(GuiPanel),
        RAY_REGISTER(GuiScrollPanel),
        RAY_REGISTER(GuiLabel),
        RAY_REGISTER(GuiButton),
        RAY_REGISTER(GuiLabelButton),
        RAY_REGISTER(GuiImageButton),
        RAY_REGISTER(GuiImageButtonEx),
        RAY_REGISTER(GuiToggle),
        RAY_REGISTER(GuiToggleGroup),
        RAY_REGISTER(GuiCheckBox),
        RAY_REGISTER(GuiComboBox),
        RAY_REGISTER(GuiDropdownBox),
        RAY_REGISTER(GuiSpinner),
        RAY_REGISTER(GuiValueBox),
        RAY_REGISTER(GuiTextBox),
        RAY_REGISTER(GuiTextBoxMulti),
        RAY_REGISTER(GuiSlider),
        RAY_REGISTER(GuiSliderBar),
        RAY_REGISTER(GuiProgressBar),
        RAY_REGISTER(GuiStatusBar),
        RAY_REGISTER(GuiDummyRec),
        RAY_REGISTER(GuiScrollBar),
        RAY_REGISTER(GuiListView),
        RAY_REGISTER(GuiListViewEx),
        RAY_REGISTER(GuiMessageBox),
        RAY_REGISTER(GuiColorPicker),
        RAY_REGISTER(GuiGrid),
        RAY_REGISTER(GuiLoadStyle),
        RAY_REGISTER(GuiLoadStyleProps),
        RAY_REGISTER(GuiLoadStyleDefault),
        RAY_REGISTER(GuiUpdateStyleComplete),

        { nullptr, nullptr }
};

bool luaopen_ray(lua_State * L)
{
    luaL_register(L, lua_ray_name(), __lua_lay_core);
    {
        // clang-format off
        luaL_register_metatable(L, METATABLE_IMAGE,          __lua_lay_image);
        luaL_register_metatable(L, METATABLE_CHARINFO,       __lua_lay_charinfo);
        luaL_register_metatable(L, METATABLE_FONT,           __lua_lay_font);
        luaL_register_metatable(L, METATABLE_MESH,           __lua_lay_mesh);
        luaL_register_metatable(L, METATABLE_MATERIAL,       __lua_lay_material);
        luaL_register_metatable(L, METATABLE_MODEL,          __lua_lay_model);
        luaL_register_metatable(L, METATABLE_MODELANIMATION, __lua_lay_modelanimation);
        luaL_register_metatable(L, METATABLE_WAVE,           __lua_lay_wave);
        luaL_register_metatable(L, METATABLE_SOUND,          __lua_lay_sound);
        luaL_register_metatable(L, METATABLE_AUDIOSTREAM,    __lua_lay_audiostream);
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

