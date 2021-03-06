/**
 * @file   SystemLua.hpp
 * @brief  SystemLua class prototype.
 * @author zer0
 * @date   2019-10-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_SYSTEMLUA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_SYSTEMLUA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/script/LuaExtension.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

TBAG_API bool luaE_open_system(lua_State * L);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_SYSTEMLUA_HPP__

