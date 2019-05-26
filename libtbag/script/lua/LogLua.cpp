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
    char const * name = luaL_checkstring(L, 1);
    auto const severity = libtbag::log::level::getSeverityWithLevelStep(luaL_checkinteger(L, 2));
    auto const text = libtbag::script::lua::luaE_checkfformat(L, 3);
    tLog(name, severity, text);
    return 0;
}

//Logger * createRawStdoutLogger(std::string const & name, bool mutex = true, bool auto_flush = false);
//Logger * createColorStdoutLogger(std::string const & name, bool mutex = true, bool auto_flush = false);
//Logger * createStdoutLogger(std::string const & name, MakeType type = MakeType::DEFAULT,
//                            bool mutex = true, bool auto_flush = false);
//Logger * createFileLogger(std::string const & name, std::string const & path,
//                          MakeType type = MakeType::DEFAULT, bool mutex = true, bool auto_flush = false);
//Logger * createRotateFileLogger(std::string const & name, std::string const & path,
//                                std::size_t max_size = MEGA_BYTE_TO_BYTE,
//                                std::size_t max_file_count = DEFAULT_LOG_FILE_COUNT,
//                                MakeType type = MakeType::DEFAULT, bool mutex = true, bool auto_flush = false);

//Logger * createDefaultRawStdoutLogger(bool mutex = true, bool auto_flush = false);
//Logger * createDefaultColorStdoutLogger(bool mutex = true, bool auto_flush = false);
//Logger * createDefaultStdoutLogger(bool mutex = true, bool auto_flush = false);
//Logger * createDefaultFileLogger(std::string const & path, bool mutex = true, bool auto_flush = false);
//Logger * createDefaultRotateFileLogger(std::string const & path,
//                                       std::size_t max_size = MEGA_BYTE_TO_BYTE,
//                                       std::size_t max_file_count = DEFAULT_LOG_FILE_COUNT,
//                                       bool mutex = true, bool auto_flush = false);

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

//Logger * getLogger(std::string const & name);
//Logger * getDefaultLogger();

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
        LOG_REGISTER(existsLogger),
        LOG_REGISTER(setSeverity),
        LOG_REGISTER(setDefaultSeverity),
        LOG_REGISTER(setLevel),
        LOG_REGISTER(setDefaultLevel),
        LOG_REGISTER(getSeverity),
        LOG_REGISTER(getDefaultSeverity),
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

