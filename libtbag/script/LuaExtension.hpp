/**
 * @file   LuaExtension.hpp
 * @brief  LuaExtension class prototype.
 * @author zer0
 * @date   2019-05-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAEXTENSION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAEXTENSION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/LuaBypass.hpp>

#include <cassert>
#include <cstring>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

TBAG_API int luaL_unsupport(lua_State * L);
TBAG_API void luaL_register_metatable(lua_State * L, char const * name, luaL_Reg const * l);
TBAG_API void luadebug_printstack(lua_State * L);

// clang-format off
TBAG_API std::vector<lua_Integer> luaL_checkinteger_array(lua_State * L, int arg_num);
TBAG_API std::vector<lua_Number>  luaL_checknumber_array (lua_State * L, int arg_num);
TBAG_API std::vector<std::string> luaL_checkstring_array (lua_State * L, int arg_num);
// clang-format on

#ifndef TBAG_LUA_USERDATA_PROTO
#define TBAG_LUA_USERDATA_PROTO(type, lower, api)                               \
    api type * luaL_push##lower(lua_State * L, type const * src = nullptr);     \
    api type * luaL_check##lower(lua_State * L, int num_arg);                   \
    api type * luaL_opt##lower(lua_State * L, int num_arg, type * def);         \
    /* -- END -- */
#endif

#ifndef TBAG_LUA_USERDATA_IMPL
#define TBAG_LUA_USERDATA_IMPL(type, name, upper, lower, api, more_regs)        \
    TBAG_CONSTEXPR static char const * const METATABLE_##upper = #name;         \
    api type * luaL_push##lower(lua_State * L, type const * src)                \
    {                                                                           \
        auto * result = (type*)lua_newuserdata(L, sizeof(type));                \
        assert(result != nullptr);                                              \
        if (src) {                                                              \
            memcpy((void*)result, (void const *)src, sizeof(type));             \
        } else {                                                                \
            memset((void*)result, 0x00, sizeof(type));                          \
        }                                                                       \
        luaL_getmetatable(L, METATABLE_##upper);                                \
        lua_setmetatable(L, -2);                                                \
        return result;                                                          \
    }                                                                           \
    api type * luaL_check##lower(lua_State * L, int num_arg)                    \
    {                                                                           \
        auto * result = (type*)luaL_checkudata(L, num_arg, METATABLE_##upper);  \
        if (result == nullptr) {                                                \
            luaL_typerror(L, num_arg, METATABLE_##upper);                       \
            return nullptr;                                                     \
        }                                                                       \
        return result;                                                          \
    }                                                                           \
    api type * luaL_opt##lower(lua_State * L, int num_arg, type * def)          \
    {                                                                           \
        if (lua_isuserdata(L, num_arg)) {                                       \
            return luaL_check##lower(L, num_arg);                               \
        }                                                                       \
        return def;                                                             \
    }                                                                           \
    static int _##name(lua_State * L)                                           \
    {                                                                           \
        luaL_push##lower(L);                                                    \
        return 1;                                                               \
    }                                                                           \
    static int _##name##_gc(lua_State * L)                                      \
    {                                                                           \
        return 0;                                                               \
    }                                                                           \
    static int _##name##_tostring(lua_State * L)                                \
    {                                                                           \
        lua_pushstring(L, METATABLE_##upper);                                   \
        return 1;                                                               \
    }                                                                           \
    static luaL_Reg const __lua_reg_##lower[] = {                               \
            { "__gc", _##name##_gc },                                           \
            { "__tostring", _##name##_tostring },                               \
            more_regs                                                           \
            { nullptr, nullptr }                                                \
    }; /* -- END -- */
#endif

#ifndef TBAG_LUA_USERDATA_DEFAULT_IMPL
#define TBAG_LUA_USERDATA_DEFAULT_IMPL(type, upper, lower) \
    TBAG_LUA_USERDATA_IMPL(type, type, upper, lower,,)
#endif

template <typename T>
struct PodWrapper
{
    T value;
};

using BooleanWrapper  = PodWrapper<bool>;
using IntegerWrapper  = PodWrapper<int>;
using UnsignedWrapper = PodWrapper<unsigned int>;
using FloatWrapper    = PodWrapper<float>;
using DoubleWrapper   = PodWrapper<double>;

TBAG_LUA_USERDATA_PROTO(BooleanWrapper , booleanwrapper , TBAG_API)
TBAG_LUA_USERDATA_PROTO(IntegerWrapper , integerwrapper , TBAG_API)
TBAG_LUA_USERDATA_PROTO(UnsignedWrapper, unsignedwrapper, TBAG_API)
TBAG_LUA_USERDATA_PROTO(FloatWrapper   , floatwrapper   , TBAG_API)
TBAG_LUA_USERDATA_PROTO(DoubleWrapper  , doublewrapper  , TBAG_API)

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAEXTENSION_HPP__

