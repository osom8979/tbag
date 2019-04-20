/**
 * @file   RayLuaBinding.hpp
 * @brief  RayLuaBinding class prototype.
 * @author zer0
 * @date   2019-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYLUABINDING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYLUABINDING_HPP__

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

namespace ray {

TBAG_API bool bindRayForLua(lua_State * L);

} // namespace ray

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RAY_RAYLUABINDING_HPP__

