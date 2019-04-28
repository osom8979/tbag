/**
 * @file   RayLuaCore.hpp
 * @brief  RayLuaCore class prototype.
 * @author zer0
 * @date   2019-04-28
 *
 * @remarks
 *  Example:
 *  @code{.lua}
 *   -- Core - Basic window
 *   ray.InitWindow(800, 450, "Basic window")
 *   ray.SetTargetFPS(60);
 *   while not ray.WindowShouldClose() do
 *       ray.BeginDrawing()
 *       ray.ClearBackground({r=30, g=30, b=30, a=255})
 *       ray.EndDrawing()
 *   end
 *   ray.CloseWindow()
 *  @endcode
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUACORE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUACORE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/script/lua/RayLuaCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray;

TBAG_API bool luaopen_ray_core(lua_State * L);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUACORE_HPP__

