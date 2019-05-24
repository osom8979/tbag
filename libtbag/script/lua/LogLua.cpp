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

static int _Log(lua_State * L)
{
    int const top = lua_gettop(L);
    if (top < 4) {
        return luaL_error(L, "More than 4 arguments are required.");
    }

    using ArgFormatter = fmt::ArgFormatter<char>;
    using BasicFormatter = fmt::BasicFormatter<char, ArgFormatter>;
    using MakeValue = fmt::internal::MakeValue<BasicFormatter>;
    using Value = fmt::internal::Value;
    using ArgList = fmt::ArgList;
    using ArgArray = fmt::internal::ArgArray<ArgList::MAX_PACKED_ARGS>;
    if ((top-3) >= ArgList::MAX_PACKED_ARGS) {
        return luaL_error(L, "(top-2) >= ArgList::MAX_PACKED_ARGS");
    }

    char const * NAME = luaL_checkstring(L, 1);
    auto const SEVERITY = libtbag::log::level::getSeverityWithLevelStep(luaL_checkinteger(L, 2));
    char const * format_text = luaL_checkstring(L, 3);

    Value values[ArgList::MAX_PACKED_ARGS];
    for (int i = 4; i <= top; ++i) {
        if (lua_isinteger(L, i)) {
            values[i] = ArgArray::make<BasicFormatter>(luaL_checkinteger(L, i));
        } else if (lua_isboolean(L, i)) {
            values[i] = ArgArray::make<BasicFormatter>(luaL_checkboolean(L, i));
        } else if (lua_isnumber(L, i)) {
            values[i] = ArgArray::make<BasicFormatter>(luaL_checknumber(L, i));
        } else if (lua_isstring(L, i)) {
            values[i] = ArgArray::make<BasicFormatter>(luaL_checkstring(L, i));
        }
    }

    auto const MESSAGE = ::fmt::format(format_text, ArgList(top-2, values));
    tLog(NAME, SEVERITY, MESSAGE);

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

