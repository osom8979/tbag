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

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

TBAG_API int luaL_unsupport(lua_State * L);

TBAG_API void luadebug_printstack(lua_State * L);
TBAG_API void luaL_register_metatable(lua_State * L, char const * name, luaL_Reg const * l);

TBAG_API std::vector<lua_Integer> luaL_checkinteger_array(lua_State * L, int arg_num);
TBAG_API std::vector<lua_Number>  luaL_checknumber_array(lua_State * L, int arg_num);
TBAG_API std::vector<std::string> luaL_checkstring_array(lua_State * L, int arg_num);

#ifndef TBAG_LUA_USERDATA_REG
#define TBAG_LUA_USERDATA_REG(type, name, upper, lower, more_regs)              \
    TBAG_CONSTEXPR static char const * const METATABLE_##upper = #name;         \
    static type * _luaL_push##lower(lua_State * L, type const * src = nullptr)  \
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
    static type * _luaL_check##lower(lua_State * L, int num_arg)                \
    {                                                                           \
        auto * result = (type*)luaL_checkudata(L, num_arg, METATABLE_##upper);  \
        if (result == nullptr) {                                                \
            luaL_typerror(L, num_arg, METATABLE_##upper);                       \
            return nullptr;                                                     \
        }                                                                       \
        return result;                                                          \
    }                                                                           \
    static int _##name(lua_State * L)                                           \
    {                                                                           \
        _luaL_push##lower(L);                                                   \
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
    static luaL_Reg const __lua_lay_##lower[] = {                               \
            { "__gc", _##name##_gc },                                           \
            { "__tostring", _##name##_tostring },                               \
            more_regs                                                           \
            { nullptr, nullptr }                                                \
    }; /* -- END -- */
#endif

#ifndef TBAG_LUA_USERDATA_DEFAULT_REG
#define TBAG_LUA_USERDATA_DEFAULT_REG(type, upper, lower) \
    TBAG_LUA_USERDATA_REG(type, type, upper, lower,)
#endif

template <typename T>
struct PodWrapper
{
    T value;
};

using BooleanWrapper = PodWrapper<bool>;
using IntegerWrapper = PodWrapper<int>;
using FloatWrapper   = PodWrapper<float>;
using DoubleWrapper  = PodWrapper<double>;

TBAG_API BooleanWrapper * luaL_pushbooleanwrapper(lua_State * L, BooleanWrapper const * src = nullptr);
TBAG_API BooleanWrapper * luaL_checkbooleanwrapper(lua_State * L, int num_arg);

TBAG_API IntegerWrapper * luaL_pushintegerwrapper(lua_State * L, IntegerWrapper const * src = nullptr);
TBAG_API IntegerWrapper * luaL_checkintegerwrapper(lua_State * L, int num_arg);

TBAG_API FloatWrapper * luaL_pushfloatwrapper(lua_State * L, FloatWrapper const * src = nullptr);
TBAG_API FloatWrapper * luaL_checkfloatwrapper(lua_State * L, int num_arg);

TBAG_API DoubleWrapper * luaL_pushdoublewrapper(lua_State * L, DoubleWrapper const * src = nullptr);
TBAG_API DoubleWrapper * luaL_checkdoublewrapper(lua_State * L, int num_arg);

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAEXTENSION_HPP__

