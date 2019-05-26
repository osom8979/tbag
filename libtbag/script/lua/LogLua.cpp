/**
 * @file   LogLua.cpp
 * @brief  LogLua class implementation.
 * @author zer0
 * @date   2019-05-24
 */

#include <libtbag/script/lua/LogLua.hpp>
#include <libtbag/script/lua/StringLua.hpp>
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
    char const * name = luaL_checkstring(L, 1);
    auto const severity = libtbag::log::level::getSeverityWithLevelStep(luaL_checkinteger(L, 2));
    auto const text = libtbag::script::lua::luaE_checkfformat(L, 3);
    tLog(name, severity, text);
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
#define LOG_INT_SYMBOL(L, s)        \
    do {                            \
        lua_pushinteger(L, int(s)); \
        lua_setfield(L, -2, #s);    \
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

bool luaE_open_log(lua_State * L)
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

