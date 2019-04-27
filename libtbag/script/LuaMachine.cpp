/**
 * @file   LuaMachine.cpp
 * @brief  LuaMachine class implementation.
 * @author zer0
 * @date   2019-03-10
 * @date   2019-04-27 (Rename: SolState -> LuaMachine)
 */

#include <libtbag/script/LuaMachine.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <sstream>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

LuaMachine::LuaMachine() : _state(nullptr)
{
    auto * lua = luaL_newstate();
    assert(lua != nullptr);
    lua_gc(lua, LUA_GCSTOP, 0);
    luaL_openlibs(lua);
    lua_gc(lua, LUA_GCRESTART, -1);

    _state.reset(lua, [](lua_State * l){
        lua_close(l);
    });
    assert(static_cast<bool>(_state));

//    std::stringstream ss_path;
//    ss_path << "." LUA_DIRSEP LUA_PATH_MARK ".lua" << LUA_PATHSEP
//            << "." LUA_DIRSEP LUA_PATH_MARK LUA_DIRSEP "init.lua";
//    (*_state)["package"]["path"] = ss_path.str();
//
//    std::stringstream ss_cpath;
//    ss_cpath << "." LUA_DIRSEP LUA_PATH_MARK LUA_MODULE_SUFFIX;
//    (*_state)["package"]["cpath"] = ss_cpath.str();
//
//    (*_state)[lua_tbag_name()] = (*_state).create_table();
}

LuaMachine::LuaMachine(std::nullptr_t) TBAG_NOEXCEPT : _state(nullptr)
{
    // EMPTY.
}

LuaMachine::LuaMachine(LuaMachine const & obj) TBAG_NOEXCEPT : LuaMachine(nullptr)
{
    (*this) = obj;
}

LuaMachine::LuaMachine(LuaMachine && obj) TBAG_NOEXCEPT : LuaMachine(nullptr)
{
    (*this) = std::move(obj);
}

LuaMachine::~LuaMachine()
{
    // EMPTY.
}

LuaMachine & LuaMachine::operator =(LuaMachine const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

LuaMachine & LuaMachine::operator =(LuaMachine && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void LuaMachine::copy(LuaMachine const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _state = obj._state;
    }
}

void LuaMachine::swap(LuaMachine & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _state.swap(obj._state);
    }
}

void LuaMachine::reset()
{
    _state.reset();
}

std::string LuaMachine::getLuaPath() const
{
//    try {
//        return (*_state)["package"]["path"];
//    } catch (...) {
//        return std::string();
//    }
    return "";
}

std::vector<std::string> LuaMachine::getLuaPaths() const
{
    return libtbag::string::splitTokens(getLuaPath(), LUA_PATHSEP);
}

bool LuaMachine::appendLuaPath(std::string const & path)
{
//    std::string lua_path = getLuaPath();
//    if (!lua_path.empty()) {
//        lua_path += LUA_PATHSEP;
//    }
//    lua_path += path;
//
//    try {
//        (*_state)["package"]["path"] = lua_path;
//        return true;
//    } catch (...) {
//        return false;
//    }
    return false;
}

std::string LuaMachine::getLuaCPath() const
{
//    try {
//        return (*_state)["package"]["cpath"];
//    } catch (...) {
//        return std::string();
//    }
    return "";
}

std::vector<std::string> LuaMachine::getLuaCPaths() const
{
    return libtbag::string::splitTokens(getLuaCPath(), LUA_PATHSEP);
}

bool LuaMachine::appendLuaCPath(std::string const & path)
{
//    std::string lua_cpath = getLuaCPath();
//    if (!lua_cpath.empty()) {
//        lua_cpath += LUA_PATHSEP;
//    }
//    lua_cpath += path;
//
//    try {
//        (*_state)["package"]["cpath"] = lua_cpath;
//        return true;
//    } catch (...) {
//        return false;
//    }
    return false;
}

bool LuaMachine::runScriptFile(std::string const & path)
{
//    if (!libtbag::filesystem::Path(path).isRegularFile()) {
//        return false;
//    }
//
//    try {
//        _state->script_file(path);
//        return true;
//    } catch (...) {
//        return false;
//    }
    return false;
}

bool LuaMachine::runScript(std::string const & code)
{
//    if (code.empty()) {
//        return false;
//    }
//
//    try {
//        _state->script(code);
//        return true;
//    } catch (...) {
//        return false;
//    }
    return false;
}

std::string LuaMachine::findScriptPath(std::string const & filename, bool include_working) const
{
    for (auto & path : getLuaPaths()) {
        using namespace libtbag::filesystem;
        auto const FILE_PATH = Path(path) / filename;
        if (FILE_PATH.isRegularFile()) {
            return FILE_PATH;
        }
    }

    if (include_working) {
        auto const FILE_PATH = libtbag::filesystem::Path::getWorkDir() / filename;
        if (FILE_PATH.isRegularFile()) {
            return FILE_PATH;
        }
    }

    return std::string();
}

LuaMachine::Version LuaMachine::getLuaVersion() const
{
    return Version((LUA_VERSION_NUM/100), (LUA_VERSION_NUM%100));
}

LuaMachine::Version LuaMachine::getLuaJITVersion() const
{
    return Version(LUAJIT_VERSION_NUM, 10000, 100, 1);
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

