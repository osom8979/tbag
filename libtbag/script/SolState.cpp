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
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

static char const * const __get_lug_pathsep()
{
#if defined(LUA_PATHSEP)
    return LUA_PATHSEP;
#else
    return ";";
#endif
}

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

bool SolState::loadDynAsm()
{
    return false;
}

bool SolState::loadLibraries()
{
    luaL_openlibs(_state->lua_state());
    Err code = libtbag::script::lua::luaopen_sfml(_state->lua_state());
    if (isFailure(code)) {
        if (code != Err::E_ENOSYS) {
            tDLogW("SolState::loadLibraries() Failed to register lua-sfml library: {}", code);
        }
    }
    return true;
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
    return libtbag::string::splitTokens(getLuaPath(), __get_lug_pathsep());
}

bool SolState::appendLuaPath(std::string const & path)
{
    std::string lua_path = getLuaPath();
    if (!lua_path.empty()) {
        lua_path += __get_lug_pathsep();
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
    return libtbag::string::splitTokens(getLuaCPath(), __get_lug_pathsep());
}

bool SolState::appendLuaCPath(std::string const & path)
{
    std::string lua_cpath = getLuaCPath();
    if (!lua_cpath.empty()) {
        lua_cpath += __get_lug_pathsep();
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

