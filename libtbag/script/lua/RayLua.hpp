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
 *   ray.SetTargetFPS(60)
 *
 *   gradient = ray.GenImageGradientV(400, 400, ray.RED, ray.BLUE);
 *   texture = ray.LoadTextureFromImage(gradient);
 *   ray.UnloadImage(gradient);
 *   gradient = nil
 *
 *   while not ray.WindowShouldClose() do
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
#include <libtbag/script/LuaBypass.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

TBAG_CONSTEXPR int const MAX_TEXT_BOX_LENGTH = 2048;

TBAG_CONSTEXPR char const * const lua_ray_name() TBAG_NOEXCEPT
{
    return "ray";
}

TBAG_API bool luaopen_ray(lua_State * L);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUA_HPP__

