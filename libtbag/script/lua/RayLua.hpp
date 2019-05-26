/**
 * @file   RayLua.hpp
 * @brief  RayLua class prototype.
 * @author zer0
 * @date   2019-04-27
 *
 * @remarks
 *  Example:
 *  @code{.lua}
 *   -- Core - Basic window
 *
 *   width = 800
 *   height = 450
 *
 *   tbag.InitWindow(width, height, 'Basic window')
 *   tbag.GuiInitRay()
 *   tbag.SetTargetFPS(60)
 *
 *   gradient = tbag.GenImageGradientV(400, 400, tbag.RED, tbag.BLUE);
 *   texture = tbag.LoadTextureFromImage(gradient);
 *   tbag.UnloadImage(gradient);
 *   gradient = nil
 *
 *   while not tbag.WindowShouldClose() do
 *       tbag.GuiUpdateRay()
 *       if tbag.GuiBegin('Window') then
 *           tbag.GuiText('Label')
 *       end
 *       tbag.GuiEnd()
 *
 *       tbag.GuiShowDemoWindow()
 *
 *       tbag.BeginDrawing()
 *       tbag.ClearBackground({30, 30, 30})
 *       tbag.DrawTexture(texture, 0, 0, tbag.WHITE);
 *       tbag.DrawCircleV({width/2, height/2}, 50, tbag.RED);
 *       tbag.DrawText('Basic window!', 10, 20, 20, {200, 200, 200});
 *       tbag.EndDrawing()
 *   end
 *
 *   tbag.UnloadTexture(texture);
 *   texture = nil
 *
 *   tbag.GuiShutdownRay()
 *   tbag.CloseWindow()
 *  @endcode
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ray/RayBypass.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/script/LuaExtension.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

TBAG_API void luaE_pushvector2(lua_State * L, Vector2 const & vec);
TBAG_API Vector2 luaE_checkvector2(lua_State * L, int num_arg);
TBAG_API Vector2 luaE_optvector2(lua_State * L, int num_arg, Vector2 const & def);
TBAG_API std::vector<Vector2> luaE_checkvector2_array(lua_State * L, int num_arg);

TBAG_API void luaE_pushvector3(lua_State * L, Vector3 const & vec);
TBAG_API Vector3 luaE_checkvector3(lua_State * L, int num_arg);
TBAG_API Vector3 luaE_optvector3(lua_State * L, int num_arg, Vector3 const & def);

TBAG_API void luaE_pushvector4(lua_State * L, Vector4 const & vec);
TBAG_API Vector4 luaE_checkvector4(lua_State * L, int num_arg);
TBAG_API Vector4 luaE_optvector4(lua_State * L, int num_arg, Vector4 const & def);

TBAG_API void luaE_pushmatrix(lua_State * L, Matrix const & mat);
TBAG_API Matrix luaE_checkmatrix(lua_State * L, int num_arg);

TBAG_API void luaE_pushcolor(lua_State * L, Color const & color);
TBAG_API Color luaE_checkcolor(lua_State * L, int num_arg);
TBAG_API std::vector<Color> luaE_checkcolor_array(lua_State * L, int num_arg);

TBAG_API void luaE_pushrectangle(lua_State * L, Rectangle2 const & rect);
TBAG_API Rectangle2 luaE_checkrectangle(lua_State * L, int num_arg);

TBAG_LUA_USERDATA_PROTO(Image, image, TBAG_API)

TBAG_API void luaE_pushtexture2d(lua_State * L, Texture2D const & tex);
TBAG_API Texture2D luaE_checktexture2d(lua_State * L, int num_arg);

TBAG_API void luaE_pushrendertexture2d(lua_State * L, RenderTexture2D const & tex);
TBAG_API RenderTexture2D luaE_checkrendertexture2d(lua_State * L, int num_arg);

TBAG_API void luaE_pushnpatchinfo(lua_State * L, NPatchInfo const & npatch);
TBAG_API NPatchInfo luaE_checknpatchinfo(lua_State * L, int num_arg);

TBAG_LUA_USERDATA_PROTO(CharInfo, charinfo, TBAG_API)

TBAG_API void luaE_pushcharinfo_array(lua_State * L, CharInfo * char_infos, int size);
TBAG_API std::vector<CharInfo> luaE_checkcharinfo_array(lua_State * L, int num_arg);
TBAG_API std::vector<int> luaE_checkchars_array(lua_State * L, int num_arg);

TBAG_LUA_USERDATA_PROTO(Font, font, TBAG_API)

TBAG_API void luaE_pushcamera3d(lua_State * L, Camera3D const & cam);
TBAG_API Camera3D luaE_checkcamera3d(lua_State * L, int num_arg);

TBAG_API void luaE_pushcamera2d(lua_State * L, Camera2D const & cam);
TBAG_API Camera2D luaE_checkcamera2d(lua_State * L, int num_arg);

TBAG_API void luaE_pushboundingbox(lua_State * L, BoundingBox const & bbox);
TBAG_API BoundingBox luaE_checkboundingbox(lua_State * L, int num_arg);

TBAG_LUA_USERDATA_PROTO(Mesh, mesh, TBAG_API)

TBAG_API void luaE_pushshader(lua_State * L, Shader const & shader);
TBAG_API Shader luaE_checkshader(lua_State * L, int num_arg);

TBAG_API void luaE_pushmaterialmap(lua_State * L, MaterialMap const & materials);
TBAG_API MaterialMap luaE_checkmaterialmap(lua_State * L, int num_arg);

TBAG_LUA_USERDATA_PROTO(Material, material, TBAG_API)
TBAG_LUA_USERDATA_PROTO(Model, model, TBAG_API)
TBAG_LUA_USERDATA_PROTO(ModelAnimation, modelanimation, TBAG_API)

TBAG_API void luaE_pushray(lua_State * L, Ray const & ray);
TBAG_API Ray luaE_checkray(lua_State * L, int num_arg);

TBAG_API void luaE_pushrayhitinfo(lua_State * L, RayHitInfo const & info);
TBAG_API RayHitInfo luaE_checkrayhitinfo(lua_State * L, int num_arg);

TBAG_LUA_USERDATA_PROTO(Wave, wave, TBAG_API)
TBAG_LUA_USERDATA_PROTO(Sound, sound, TBAG_API)

struct MusicWrapper
{
    Music music;
};

TBAG_LUA_USERDATA_PROTO(MusicWrapper, music, TBAG_API)
TBAG_LUA_USERDATA_PROTO(AudioStream, audiostream, TBAG_API)

TBAG_API void luaE_pushvrdeviceinfo(lua_State * L, VrDeviceInfo const & info);
TBAG_API VrDeviceInfo luaE_checkvrdeviceinfo(lua_State * L, int num_arg);

TBAG_API bool luaE_open_ray(lua_State * L);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUA_HPP__

