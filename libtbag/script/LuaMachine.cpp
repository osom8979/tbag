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

    _state.reset(lua, [](lua_State * l){
        lua_close(l);
    });
    assert(static_cast<bool>(_state));

    initDefaultOpenLibraries();
    initDefaultLuaPath();
    initDefaultLuaCPath();
    initDefaultTbagTable();
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

std::string LuaMachine::getDefaultLuaPath()
{
    std::stringstream path;
    path << "." LUA_DIRSEP LUA_PATH_MARK ".lua" << LUA_PATHSEP
         << "." LUA_DIRSEP LUA_PATH_MARK LUA_DIRSEP "init.lua";
    return path.str();
}

std::string LuaMachine::getDefaultLuaCPath()
{
    std::stringstream cpath;
    cpath << "." LUA_DIRSEP LUA_PATH_MARK LUA_MODULE_SUFFIX;
    return cpath.str();
}

void LuaMachine::initDefaultOpenLibraries()
{
    auto * lua = _state.get();
    assert(lua != nullptr);

    lua_gc(lua, LUA_GCSTOP, 0);
    luaL_openlibs(lua);
    lua_gc(lua, LUA_GCRESTART, -1);
}

void LuaMachine::initDefaultLuaPath()
{
    setLuaPath(getDefaultLuaPath());
}

void LuaMachine::initDefaultLuaCPath()
{
    setLuaCPath(getDefaultLuaCPath());
}

void LuaMachine::initDefaultTbagTable()
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_newtable(lua);
    lua_pushstring(lua, libtbag::util::getTbagVersion().toString().c_str());
    lua_setfield(lua, -2, "version");
    lua_setglobal(lua, lua_tbag_name());
}

void LuaMachine::reset()
{
    _state.reset();
}

std::string LuaMachine::getLuaPath() const
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_getfield(lua, -1, "path");
    std::string const PACKAGE_PATH = lua_tostring(lua, -1);
    lua_pop(lua, 2);
    return PACKAGE_PATH;
}

std::vector<std::string> LuaMachine::getLuaPaths() const
{
    return libtbag::string::splitTokens(getLuaPath(), LUA_PATHSEP);
}

void LuaMachine::setLuaPath(std::string const & path)
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_pushstring(lua, path.c_str());
    lua_setfield(lua, -2, "path");
    lua_pop(lua, 1);
}

bool LuaMachine::appendLuaPath(std::string const & path)
{
    auto const PATH = getLuaPath();
    if (PATH.empty()) {
        setLuaPath(path);
    } else {
        setLuaPath(PATH + LUA_PATHSEP + path);
    }
    return true;
}

std::string LuaMachine::getLuaCPath() const
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_getfield(lua, -1, "cpath");
    std::string const PACKAGE_PATH = lua_tostring(lua, -1);
    lua_pop(lua, 2);
    return PACKAGE_PATH;
}

std::vector<std::string> LuaMachine::getLuaCPaths() const
{
    return libtbag::string::splitTokens(getLuaCPath(), LUA_PATHSEP);
}

void LuaMachine::setLuaCPath(std::string const & path)
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, "package");
    lua_pushstring(lua, path.c_str());
    lua_setfield(lua, -2, "cpath");
    lua_pop(lua, 1);
}

bool LuaMachine::appendLuaCPath(std::string const & path)
{
    auto const PATH = getLuaCPath();
    if (PATH.empty()) {
        setLuaCPath(path);
    } else {
        setLuaCPath(PATH + LUA_PATHSEP + path);
    }
    return true;
}

std::string LuaMachine::getTbagVersion() const
{
    auto * lua = _state.get();
    assert(lua != nullptr);
    lua_getglobal(lua, lua_tbag_name());
    lua_getfield(lua, -1, "version");
    std::string const VERSION_PATH = lua_tostring(lua, -1);
    lua_pop(lua, 2);
    return VERSION_PATH;
}

bool LuaMachine::runScriptFile(std::string const & path)
{
    using namespace libtbag::filesystem;
    if (!Path(path).isRegularFile()) {
        return false;
    }
    auto * lua = _state.get();
    assert(lua != nullptr);
    return luaL_dofile(lua, path.c_str()) == 0;
}

bool LuaMachine::runScript(std::string const & code)
{
    if (code.empty()) {
        return false;
    }
    auto * lua = _state.get();
    assert(lua != nullptr);
    return luaL_dostring(lua, code.c_str()) == 0;
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

int LuaMachine::getTop() const
{
    return lua_gettop(_state.get());
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

