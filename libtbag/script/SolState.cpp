/**
 * @file   SolState.cpp
 * @brief  SolState class implementation.
 * @author zer0
 * @date   2019-03-10
 */

#include <libtbag/script/SolState.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/script/lua/luasfml.hpp>

#include <cassert>
#include <sstream>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

static int __sol_state_exception_cb(lua_State * L,
                                    sol::optional<std::exception const &> exception,
                                    sol::string_view description)
{
    if (exception) {
        if (exception->what() != nullptr) {
            tDLogE("__sol_state_exception_cb() Catch exception: {}", exception->what());
        } else {
            tDLogE("__sol_state_exception_cb() Catch unknown exception.");
        }
        tDLogD("__sol_state_exception_cb() Details information: {}", std::string(description.data(), description.size()));
    }
    return sol::stack::push(L, description);
}

SolState::SolState() : _state(std::make_shared<State>())
{
    assert(static_cast<bool>(_state));
    _state->set_exception_handler(&__sol_state_exception_cb);

    lua_gc(_state->lua_state(), LUA_GCSTOP, 0);
    COMMENT("Load all libraries.") {
        luaL_openlibs(_state->lua_state());
        libtbag::script::lua::luaopen_sfml(_state->lua_state());
    }
    lua_gc(_state->lua_state(), LUA_GCRESTART, -1);

    std::stringstream ss_path;
    ss_path << "." LUA_DIRSEP LUA_PATH_MARK ".lua" << LUA_PATHSEP
            << "." LUA_DIRSEP LUA_PATH_MARK LUA_DIRSEP "init.lua";
    (*_state)["package"]["path"] = ss_path.str();

    std::stringstream ss_cpath;
    ss_cpath << "." LUA_DIRSEP LUA_PATH_MARK LUA_MODULE_SUFFIX;
    (*_state)["package"]["cpath"] = ss_cpath.str();
}

SolState::SolState(std::nullptr_t) TBAG_NOEXCEPT : _state(nullptr)
{
    // EMPTY.
}

SolState::SolState(SolState const & obj) TBAG_NOEXCEPT : SolState(nullptr)
{
    (*this) = obj;
}

SolState::SolState(SolState && obj) TBAG_NOEXCEPT : SolState(nullptr)
{
    (*this) = std::move(obj);
}

SolState::~SolState()
{
    // EMPTY.
}

SolState & SolState::operator =(SolState const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

SolState & SolState::operator =(SolState && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SolState::copy(SolState const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _state = obj._state;
    }
}

void SolState::swap(SolState & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _state.swap(obj._state);
    }
}

void SolState::reset()
{
    _state.reset();
}

std::string SolState::getLuaPath() const
{
    try {
        return (*_state)["package"]["path"];
    } catch (...) {
        return std::string();
    }
}

std::vector<std::string> SolState::getLuaPaths() const
{
    return libtbag::string::splitTokens(getLuaPath(), LUA_PATHSEP);
}

bool SolState::appendLuaPath(std::string const & path)
{
    std::string lua_path = getLuaPath();
    if (!lua_path.empty()) {
        lua_path += LUA_PATHSEP;
    }
    lua_path += path;

    try {
        (*_state)["package"]["path"] = lua_path;
        return true;
    } catch (...) {
        return false;
    }
}

std::string SolState::getLuaCPath() const
{
    try {
        return (*_state)["package"]["cpath"];
    } catch (...) {
        return std::string();
    }
}

std::vector<std::string> SolState::getLuaCPaths() const
{
    return libtbag::string::splitTokens(getLuaCPath(), LUA_PATHSEP);
}

bool SolState::appendLuaCPath(std::string const & path)
{
    std::string lua_cpath = getLuaCPath();
    if (!lua_cpath.empty()) {
        lua_cpath += LUA_PATHSEP;
    }
    lua_cpath += path;

    try {
        (*_state)["package"]["cpath"] = lua_cpath;
        return true;
    } catch (...) {
        return false;
    }
}

bool SolState::runScriptFile(std::string const & path)
{
    if (!libtbag::filesystem::Path(path).isRegularFile()) {
        return false;
    }

    try {
        _state->script_file(path);
        return true;
    } catch (...) {
        return false;
    }
}

bool SolState::runScript(std::string const & code)
{
    if (code.empty()) {
        return false;
    }

    try {
        _state->script(code);
        return true;
    } catch (...) {
        return false;
    }
}

std::string SolState::findScriptPath(std::string const & filename, bool include_working) const
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

SolState::Version SolState::getLuaVersion() const
{
    return Version((LUA_VERSION_NUM/100), (LUA_VERSION_NUM%100));
}

SolState::Version SolState::getLuaJITVersion() const
{
    return Version(LUAJIT_VERSION_NUM, 10000, 100, 1);
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

