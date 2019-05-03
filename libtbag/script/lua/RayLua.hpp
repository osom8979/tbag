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

