/**
 * @file   StringLua.hpp
 * @brief  StringLua class prototype.
 * @author zer0
 * @date   2019-05-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_STRINGLUA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_STRINGLUA_HPP__

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

TBAG_API std::string luaE_checkfformat(lua_State * L, int greater_equals_index);

TBAG_API bool luaE_open_string(lua_State * L);

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUA_STRINGLUA_HPP__

