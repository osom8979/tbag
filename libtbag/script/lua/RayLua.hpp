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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

TBAG_API bool bindRayLua(lua_State * lua);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_RAYLUA_HPP__

