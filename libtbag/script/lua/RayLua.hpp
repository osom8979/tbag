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
 *   ray.InitWindow(width, height, 'Basic window')
 *   gui.InitRayGui()
 *   ray.SetTargetFPS(60)
 *
 *   gradient = ray.GenImageGradientV(400, 400, ray.RED, ray.BLUE);
 *   texture = ray.LoadTextureFromImage(gradient);
 *   ray.UnloadImage(gradient);
 *   gradient = nil
 *
 *   while not ray.WindowShouldClose() do
 *       gui.UpdateRayGui()
 *       if gui.Begin('Window') then
 *           gui.Text('Label')
 *       end
 *       gui.End()
 *
 *       ray.BeginDrawing()
 *       ray.ClearBackground({30, 30, 30})
 *       ray.DrawTexture(texture, 0, 0, ray.WHITE);
 *       ray.DrawCircleV({width/2, height/2}, 50, ray.RED);
 *       ray.DrawText('Basic window!', 10, 20, 20, {200, 200, 200});
 *       ray.EndDrawing()
 *   end
 *
 *   ray.UnloadTexture(texture);
 *   texture = nil
 *
 *   gui.ShutdownRayGui()
 *   ray.CloseWindow()
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

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

TBAG_CONSTEXPR char const * const lua_ray_name() TBAG_NOEXCEPT
{
    return "ray";
}

TBAG_API void luaL_pushvector2(lua_State * L, Vector2 const & vec);
TBAG_API Vector2 luaL_checkvector2(lua_State * L, int num_arg);
TBAG_API Vector2 luaL_optvector2(lua_State * L, int num_arg, Vector2 const & def);
TBAG_API std::vector<Vector2> luaL_checkvector2_array(lua_State * L, int num_arg);

TBAG_API void luaL_pushvector3(lua_State * L, Vector3 const & vec);
TBAG_API Vector3 luaL_checkvector3(lua_State * L, int num_arg);

TBAG_API void luaL_pushvector4(lua_State * L, Vector4 const & vec);
TBAG_API Vector4 luaL_checkvector4(lua_State * L, int num_arg);

TBAG_API void luaL_pushmatrix(lua_State * L, Matrix const & mat);
TBAG_API Matrix luaL_checkmatrix(lua_State * L, int num_arg);

TBAG_API void luaL_pushcolor(lua_State * L, Color const & color);
TBAG_API Color luaL_checkcolor(lua_State * L, int num_arg);
TBAG_API std::vector<Color> luaL_checkcolor_array(lua_State * L, int num_arg);

TBAG_API void luaL_pushrectangle(lua_State * L, Rectangle2 const & rect);
TBAG_API Rectangle2 luaL_checkrectangle(lua_State * L, int num_arg);

TBAG_API Image * luaL_pushimage(lua_State * L, Image const * src = nullptr);
TBAG_API Image * luaL_checkimage(lua_State * L, int num_arg);

TBAG_API void luaL_pushtexture2d(lua_State * L, Texture2D const & tex);
TBAG_API Texture2D luaL_checktexture2d(lua_State * L, int num_arg);

TBAG_API void luaL_pushrendertexture2d(lua_State * L, RenderTexture2D const & tex);
TBAG_API RenderTexture2D luaL_checkrendertexture2d(lua_State * L, int num_arg);

TBAG_API void luaL_pushnpatchinfo(lua_State * L, NPatchInfo const & npatch);
TBAG_API NPatchInfo luaL_checknpatchinfo(lua_State * L, int num_arg);

TBAG_API CharInfo * luaL_pushcharinfo(lua_State * L, CharInfo const * src = nullptr);
TBAG_API CharInfo * luaL_checkcharinfo(lua_State * L, int num_arg);
TBAG_API void luaL_pushcharinfo_array(lua_State * L, CharInfo * char_infos, int size);
TBAG_API std::vector<CharInfo> luaL_checkcharinfo_array(lua_State * L, int num_arg);
TBAG_API std::vector<int> luaL_checkchars_array(lua_State * L, int num_arg);

TBAG_API Font * luaL_pushfont(lua_State * L, Font const * src = nullptr);
TBAG_API Font * luaL_checkfont(lua_State * L, int num_arg);

TBAG_API void luaL_pushcamera3d(lua_State * L, Camera3D const & cam);
TBAG_API Camera3D luaL_checkcamera3d(lua_State * L, int num_arg);

TBAG_API void luaL_pushcamera2d(lua_State * L, Camera2D const & cam);
TBAG_API Camera2D luaL_checkcamera2d(lua_State * L, int num_arg);

TBAG_API void luaL_pushboundingbox(lua_State * L, BoundingBox const & bbox);
TBAG_API BoundingBox luaL_checkboundingbox(lua_State * L, int num_arg);

TBAG_API Mesh * luaL_pushmesh(lua_State * L, Mesh const * src = nullptr);
TBAG_API Mesh * luaL_checkmesh(lua_State * L, int num_arg);

TBAG_API void luaL_pushshader(lua_State * L, Shader const & shader);
TBAG_API Shader luaL_checkshader(lua_State * L, int num_arg);

TBAG_API void luaL_pushmaterialmap(lua_State * L, MaterialMap const & materials);
TBAG_API MaterialMap luaL_checkmaterialmap(lua_State * L, int num_arg);

TBAG_API Material * luaL_pushmaterial(lua_State * L, Material const * src = nullptr);
TBAG_API Material * luaL_checkmaterial(lua_State * L, int num_arg);

TBAG_API Model * luaL_pushmodel(lua_State * L, Model const * src = nullptr);
TBAG_API Model * luaL_checkmodel(lua_State * L, int num_arg);

TBAG_API ModelAnimation * luaL_pushmodelanimation(lua_State * L, ModelAnimation const * src = nullptr);
TBAG_API ModelAnimation * luaL_checkmodelanimation(lua_State * L, int num_arg);

TBAG_API void luaL_pushray(lua_State * L, Ray const & ray);
TBAG_API Ray luaL_checkray(lua_State * L, int num_arg);

TBAG_API void luaL_pushrayhitinfo(lua_State * L, RayHitInfo const & info);
TBAG_API RayHitInfo luaL_checkrayhitinfo(lua_State * L, int num_arg);

TBAG_API Wave * luaL_pushwave(lua_State * L, Wave const * src = nullptr);
TBAG_API Wave * luaL_checkwave(lua_State * L, int num_arg);

TBAG_API Sound * luaL_pushsound(lua_State * L, Sound const * src = nullptr);
TBAG_API Sound * luaL_checksound(lua_State * L, int num_arg);

struct MusicWrapper
{
    Music music;
};

TBAG_API MusicWrapper * luaL_pushmusic(lua_State * L, MusicWrapper const * src = nullptr);
TBAG_API MusicWrapper * luaL_checkmusic(lua_State * L, int num_arg);

TBAG_API AudioStream * luaL_pushaudiostream(lua_State * L, AudioStream const * src = nullptr);
TBAG_API AudioStream * luaL_checkaudiostream(lua_State * L, int num_arg);

TBAG_API void luaL_pushvrdeviceinfo(lua_State * L, VrDeviceInfo const & info);
TBAG_API VrDeviceInfo luaL_checkvrdeviceinfo(lua_State * L, int num_arg);

TBAG_API bool luaopen_ray(lua_State * L);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUA_HPP__

