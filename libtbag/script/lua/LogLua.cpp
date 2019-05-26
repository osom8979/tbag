/**
 * @file   LogLua.cpp
 * @brief  LogLua class implementation.
 * @author zer0
 * @date   2019-05-24
 */

#include <libtbag/script/lua/LogLua.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {
namespace lua    {

using namespace libtbag::log;

TBAG_CONSTEXPR static int const MAX_PACKED_ARGS_SIZE = fmt::ArgList::MAX_PACKED_ARGS-1;
TBAG_CONSTEXPR static int const MIN_LOG_ARGS_SIZE = 3;

static int _Log(lua_State * L)
{
    using BasicFormatter = fmt::BasicFormatter<char>;
    using ArgList = fmt::ArgList;
    using ArgArray = fmt::internal::ArgArray<MAX_PACKED_ARGS_SIZE>;
    using ArgArrayType = typename ArgArray::Type;

    int const top = lua_gettop(L);
    if (top < MIN_LOG_ARGS_SIZE) {
        return luaL_error(L, "More than 3 arguments are required.");
    }
    if ((top-MIN_LOG_ARGS_SIZE) > MAX_PACKED_ARGS_SIZE) {
        return luaL_error(L, "The number of arguments is too large.");
    }

    char const * NAME = luaL_checkstring(L, 1);
    auto const SEVERITY = libtbag::log::level::getSeverityWithLevelStep(luaL_checkinteger(L, 2));
    char const * FORMAT = luaL_checkstring(L, 3);
    if (top == MIN_LOG_ARGS_SIZE) {
        tLog(NAME, SEVERITY, FORMAT);
        return 0;
    }

    uint64_t types = 0;
    ArgArrayType array = {0,};
    int next_lua_arg_index = MIN_LOG_ARGS_SIZE + 1;
    int insert_index = 0;

    for (; next_lua_arg_index <= top; ++next_lua_arg_index) {
        auto const current_type = lua_type(L, next_lua_arg_index);
        switch (current_type) {
        case LUA_TNONE:
            break;

        case LUA_TNIL:
            break;

        case LUA_TBOOLEAN:
            COMMENT("Lua boolean type") {
                bool value = luaL_checkboolean(L, next_lua_arg_index);
                types |= (fmt::internal::make_type(value) << (4*insert_index));
                array[insert_index] = ArgArray::make<BasicFormatter>(value);
            }
            ++insert_index;
            break;

        case LUA_TLIGHTUSERDATA:
            break;

        case LUA_TNUMBER:
            if (lua_isinteger(L, next_lua_arg_index)) {
                int value = luaL_checkinteger(L, next_lua_arg_index);
                types |= (fmt::internal::make_type(value) << (4*insert_index));
                array[insert_index] = ArgArray::make<BasicFormatter>(value);
            } else {
                double value = luaL_checknumber(L, next_lua_arg_index);
                types |= (fmt::internal::make_type(value) << (4*insert_index));
                array[insert_index] = ArgArray::make<BasicFormatter>(value);
            }
            ++insert_index;
            break;

        case LUA_TSTRING:
            COMMENT("") {
                char const * value = luaL_checkstring(L, next_lua_arg_index);
                types |= (fmt::internal::make_type(value) << (4*insert_index));
                array[insert_index] = ArgArray::make<BasicFormatter>(value);
                ++insert_index;
            }
            break;

        case LUA_TTABLE:
            break;

        case LUA_TFUNCTION:
            break;

        case LUA_TUSERDATA:
            break;

        case LUA_TTHREAD:
            break;

        default:
            return luaL_error(L, "Unknown lua type.");
        }
    }

    tLog(NAME, SEVERITY, fmt::format(FORMAT, ArgList(types, array)));
    return 0;
}

#ifndef LOG_REGISTER
#define LOG_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_log[] = {
        LOG_REGISTER(Log),
        { nullptr, nullptr }
};

#ifndef LOG_INT_SYMBOL
#define LOG_INT_SYMBOL(L, s)                                            \
    do {                                                                \
        lua_pushinteger(L, (int(s)/libtbag::log::level::LEVEL_STEP));   \
        lua_setfield(L, -2, #s);                                        \
    } while (false) /* -- END -- */
#endif

static void luaL_push_log_symbols(lua_State * L)
{
    LOG_INT_SYMBOL(L, OFF_SEVERITY);
    LOG_INT_SYMBOL(L, EMERGENCY_SEVERITY);
    LOG_INT_SYMBOL(L, ALERT_SEVERITY);
    LOG_INT_SYMBOL(L, CRITICAL_SEVERITY);
    LOG_INT_SYMBOL(L, ERROR_SEVERITY);
    LOG_INT_SYMBOL(L, WARNING_SEVERITY);
    LOG_INT_SYMBOL(L, NOTICE_SEVERITY);
    LOG_INT_SYMBOL(L, INFO_SEVERITY);
    LOG_INT_SYMBOL(L, DEBUG_SEVERITY);
}

bool luaopen_log(lua_State * L)
{
    luaL_register(L, lua_tbag_name(), __lua_log);
    {
        luaL_push_log_symbols(L);
    }
    lua_pop(L, 1);
    return true;
}

} // namespace lua
} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

