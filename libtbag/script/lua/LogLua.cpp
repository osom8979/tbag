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

static int _log(lua_State * L)
{
    tLog(luaL_checkstring(L, 1),
         libtbag::log::level::getSeverityWithLevelStep(luaL_checkinteger(L, 2)),
         luaE_checkfformat(L, 3));
    return 0;
}

static int _logm(lua_State * L)
{
    tLogM(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _loga(lua_State * L)
{
    tLogA(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _logc(lua_State * L)
{
    tLogC(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _loge(lua_State * L)
{
    tLogE(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _logw(lua_State * L)
{
    tLogW(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _logn(lua_State * L)
{
    tLogN(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _logi(lua_State * L)
{
    tLogI(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _logd(lua_State * L)
{
    tLogD(luaL_checkstring(L, 1), luaE_checkfformat(L, 2));
    return 0;
}

static int _createRawStdoutLogger(lua_State * L)
{
    lua_pushboolean(L, createRawStdoutLogger(luaL_checkstring(L, 1),
                                             luaL_optboolean(L, 2, true),
                                             luaL_optboolean(L, 3, false)) != nullptr);
    return 1;
}

static int _createColorStdoutLogger(lua_State * L)
{
    lua_pushboolean(L, createColorStdoutLogger(luaL_checkstring(L, 1),
                                               luaL_optboolean(L, 2, true),
                                               luaL_optboolean(L, 3, false)) != nullptr);
    return 1;
}

TBAG_CONSTEXPR static int DEFAULT_MAKE_TYPE = static_cast<int>(MakeType::DEFAULT);

static int _createStdoutLogger(lua_State * L)
{
    lua_pushboolean(L, createStdoutLogger(luaL_checkstring(L, 1),
                                          static_cast<MakeType>(luaL_optinteger(L, 2, DEFAULT_MAKE_TYPE)),
                                          luaL_optboolean(L, 3, true),
                                          luaL_optboolean(L, 4, false)) != nullptr);
    return 1;
}

static int _createFileLogger(lua_State * L)
{
    lua_pushboolean(L, createFileLogger(luaL_checkstring(L, 1),
                                        luaL_checkstring(L, 2),
                                        static_cast<MakeType>(luaL_optinteger(L, 3, DEFAULT_MAKE_TYPE)),
                                        luaL_optboolean(L, 4, true),
                                        luaL_optboolean(L, 5, false)) != nullptr);
    return 1;
}

static int _createRotateFileLogger(lua_State * L)
{
    lua_pushboolean(L, createRotateFileLogger(luaL_checkstring(L, 1),
                                              luaL_checkstring(L, 2),
                                              luaL_optinteger(L, 3, MEGA_BYTE_TO_BYTE),
                                              luaL_optinteger(L, 4, DEFAULT_LOG_FILE_COUNT),
                                              static_cast<MakeType>(luaL_optinteger(L, 5, DEFAULT_MAKE_TYPE)),
                                              luaL_optboolean(L, 6, true),
                                              luaL_optboolean(L, 7, false)) != nullptr);
    return 1;
}

static int _removeLogger(lua_State * L)
{
    lua_pushboolean(L, removeLogger(luaL_checkstring(L, 1)));
    return 1;
}

static int _removeDefaultLogger(lua_State * L)
{
    lua_pushboolean(L, removeDefaultLogger());
    return 1;
}

static int _existsLogger(lua_State * L)
{
    lua_pushboolean(L, existsLogger(luaL_checkstring(L, 1)));
    return 1;
}

static int _setSeverity(lua_State * L)
{
    setSeverity(luaL_checkstring(L, 1),
                libtbag::log::level::getSeverityWithLevelStep(luaL_checkinteger(L, 2)));
    return 0;
}

static int _setDefaultSeverity(lua_State * L)
{
    setDefaultSeverity(libtbag::log::level::getSeverityWithLevelStep(luaL_checkinteger(L, 1)));
    return 0;
}

static int _setLevel(lua_State * L)
{
    setLevel(luaL_checkstring(L, 1), luaL_checkinteger(L, 2));
    return 0;
}

static int _setDefaultLevel(lua_State * L)
{
    setDefaultLevel(luaL_checkinteger(L, 1));
    return 0;
}

static int _getSeverity(lua_State * L)
{
    lua_pushinteger(L, getSeverity(luaL_checkstring(L, 1)).getLevel());
    return 1;
}

static int _getDefaultSeverity(lua_State * L)
{
    lua_pushinteger(L, getDefaultSeverity().getLevel());
    return 1;
}

#ifndef LOG_REGISTER
#define LOG_REGISTER(name) { #name, _##name }
#endif

static luaL_Reg const __lua_log[] = {
        LOG_REGISTER(log),
        LOG_REGISTER(logm),
        LOG_REGISTER(loga),
        LOG_REGISTER(logc),
        LOG_REGISTER(loge),
        LOG_REGISTER(logw),
        LOG_REGISTER(logn),
        LOG_REGISTER(logi),
        LOG_REGISTER(logd),
        LOG_REGISTER(createRawStdoutLogger),
        LOG_REGISTER(createColorStdoutLogger),
        LOG_REGISTER(createStdoutLogger),
        LOG_REGISTER(createFileLogger),
        LOG_REGISTER(createRotateFileLogger),
        LOG_REGISTER(existsLogger),
        LOG_REGISTER(setSeverity),
        LOG_REGISTER(setDefaultSeverity),
        LOG_REGISTER(setLevel),
        LOG_REGISTER(setDefaultLevel),
        LOG_REGISTER(getSeverity),
        LOG_REGISTER(getDefaultSeverity),
        { nullptr, nullptr }
};

#ifndef LOG_STRING_SYMBOL
#define LOG_STRING_SYMBOL(L, s, v)  \
    do {                            \
        lua_pushstring(L, v);       \
        lua_setfield(L, -2, #s);    \
    } while (false) /* -- END -- */
#endif

#ifndef LOG_INT_SYMBOL
#define LOG_INT_SYMBOL(L, s, v)     \
    do {                            \
        lua_pushinteger(L, int(v)); \
        lua_setfield(L, -2, #s);    \
    } while (false) /* -- END -- */
#endif

#ifndef LOG_INT_SYMBOL2
#define LOG_INT_SYMBOL2(L, s) LOG_INT_SYMBOL(L, s, int(s))
#endif

static void luaL_push_log_symbols(lua_State * L)
{
    LOG_STRING_SYMBOL(L, DEFAULT_LOGGER_NAME, TBAG_DEFAULT_LOGGER_NAME);

    LOG_INT_SYMBOL(L, MAKETYPE_DEFAULT      , MakeType::DEFAULT);
    LOG_INT_SYMBOL(L, MAKETYPE_DEFAULT_COLOR, MakeType::DEFAULT_COLOR);
    LOG_INT_SYMBOL(L, MAKETYPE_RAW          , MakeType::RAW);

    LOG_INT_SYMBOL2(L, OFF_SEVERITY);
    LOG_INT_SYMBOL2(L, EMERGENCY_SEVERITY);
    LOG_INT_SYMBOL2(L, ALERT_SEVERITY);
    LOG_INT_SYMBOL2(L, CRITICAL_SEVERITY);
    LOG_INT_SYMBOL2(L, ERROR_SEVERITY);
    LOG_INT_SYMBOL2(L, WARNING_SEVERITY);
    LOG_INT_SYMBOL2(L, NOTICE_SEVERITY);
    LOG_INT_SYMBOL2(L, INFO_SEVERITY);
    LOG_INT_SYMBOL2(L, DEBUG_SEVERITY);
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

