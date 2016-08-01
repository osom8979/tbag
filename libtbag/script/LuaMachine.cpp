/**
 * @file   LuaMachine.cpp
 * @brief  LuaMachine class implementation.
 * @author zer0
 * @date   2016-07-06
 */

#include <libtbag/script/LuaMachine.hpp>

#include <cassert>
#include <string>
#include <algorithm>

#include <lua.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

template <typename T>
inline lua_State * lua_state_cast(T * object)
{
    return static_cast<lua_State*>(object);
}

LuaMachine::LuaMachine() : _machine(nullptr)
{
    // EMPTY.
}

LuaMachine::~LuaMachine()
{
    this->release();
}

bool LuaMachine::initialize()
{
    _machine = static_cast<FakeLuaState*>(luaL_newstate());
    if (_machine != nullptr) {
        luaL_openlibs(lua_state_cast(_machine));
    }

    return this->isReady();
}

void LuaMachine::release()
{
    if (this->isReady()) {
        lua_close(lua_state_cast(_machine));
        _machine = nullptr;
    }
}

bool LuaMachine::load(std::string const & path)
{
    if (this->isReady() == false) {
        return false;
    }

    if (luaL_loadfile(lua_state_cast(_machine), path.c_str()) != 0) {
        // maybe LUA_ERRFILE
        return false;
    }

    lua_pcall(lua_state_cast(_machine), 0, 0, 0);
    return true;
}

int LuaMachine::getStackSize() const
{
    return lua_gettop(lua_state_cast(_machine));
}

void LuaMachine::pop(int size)
{
    lua_pop(lua_state_cast(_machine), size);
}

void LuaMachine::push()
{
    lua_pushnil(lua_state_cast(_machine));
}

void LuaMachine::push(bool value)
{
    lua_pushboolean(lua_state_cast(_machine), value);
}

void LuaMachine::push(int value)
{
    lua_pushinteger(lua_state_cast(_machine), value);
}

void LuaMachine::push(double value)
{
    lua_pushnumber(lua_state_cast(_machine), value);
}

void LuaMachine::push(std::string const & value)
{
    lua_pushstring(lua_state_cast(_machine), value.c_str());
}

std::string LuaMachine::getLuaString(std::string const & name)
{
    lua_pushstring(lua_state_cast(_machine), name.c_str());
    lua_rawget(lua_state_cast(_machine), LUA_REGISTRYINDEX);

    if (lua_isstring(lua_state_cast(_machine), -1)) {
        assert(false);
        return std::string();
    }

    std::string result = lua_tostring(lua_state_cast(_machine), -1);
    lua_pop(lua_state_cast(_machine), 1);
    return result;
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

