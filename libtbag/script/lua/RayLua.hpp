/**
 * @file   RayLua.hpp
 * @brief  RayLua class prototype.
 * @author zer0
 * @date   2019-04-27
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
#include <libtbag/ray/RayBypass.hpp>
#include <libtbag/ray/RayGuiBypass.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::ray;

TBAG_API void lua_ray_pushvector2(lua_State * L, Vector2 const & vec);
TBAG_API void lua_ray_pushvector3(lua_State * L, Vector3 const & vec);
TBAG_API void lua_ray_pushvector4(lua_State * L, Vector4 const & vec);
TBAG_API void lua_ray_pushquaternion(lua_State * L, Quaternion const & vec);
TBAG_API void lua_ray_pushmatrix(lua_State * L, Matrix const & mat);

TBAG_API Color lua_ray_getcolor(lua_State * L, int num_arg);

TBAG_API bool luaopen_raylib(lua_State * L);

TBAG_API char const * const get_basic_window_lua(lua_State * L) TBAG_NOEXCEPT;

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUA_HPP__

