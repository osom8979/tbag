/**
 * @file   LogLua.hpp
 * @brief  LogLua class prototype.
 * @author zer0
 * @date   2019-05-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_LOGLUA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_LOGLUA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/script/LuaExtension.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

TBAG_API bool luaopen_log(lua_State * L);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_LOGLUA_HPP__

