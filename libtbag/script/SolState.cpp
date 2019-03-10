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

SolState::SolState() : _state(std::make_shared<State>())
{
    assert(static_cast<bool>(_state));
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
    using namespace libtbag::filesystem::details;
    return libtbag::string::splitTokens(getLuaPath(), std::string(1, PATH_SPLITTER));
}

bool SolState::appendLuaPath(std::string const & path)
{
    if (!libtbag::filesystem::Path(path).isDirectory()) {
        return false;
    }

    std::string lua_path = getLuaPath();
    try {
        if (!lua_path.empty()) {
            lua_path += libtbag::filesystem::details::PATH_SPLITTER;
        }
        lua_path += path;
        (*_state)["package"]["path"] = lua_path;
    } catch (...) {
        return false;
    }
    return true;
}

bool SolState::runScriptFile(std::string const & path)
{
    if (!libtbag::filesystem::Path(path).isRegularFile()) {
        return false;
    }
    try {
        _state->script_file(path);
    } catch (...) {
        return false;
    }
    return true;
}

bool SolState::runScript(std::string const & code)
{
    if (code.empty()) {
        return false;
    }
    try {
        _state->script(code);
    } catch (...) {
        return false;
    }
    return true;
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

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

