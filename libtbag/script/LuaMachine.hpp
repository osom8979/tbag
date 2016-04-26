/**
 * @file   LuaMachine.hpp
 * @brief  LuaMachine class prototype.
 * @author zer0
 * @date   2016-04-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <cassert>

#include <string>
#include <algorithm>

#include <lua.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

/**
 * LuaMachine class prototype.
 *
 * @author zer0
 * @date   2016-04-26
 */
class LuaMachine : public Noncopyable
{
public:
    enum class LuaType : int
    {
        NIL = 0,
        BOOLEAN,
        FUNCTION,
        NUMBER,
        STRING,
        TABLE,
        THREAD,
        USERDATA,
    };

private:
    lua_State * _machine;

public:
    LuaMachine() : _machine(nullptr) {
        __EMPTY_BLOCK__
    }

    ~LuaMachine() {
        this->release();
    }

public:
    inline bool isReady() const {
        return (this->_machine != nullptr ? true : false);
    }

public:
    bool initialize() {
        this->_machine = luaL_newstate();
        if (this->_machine != nullptr) {
            luaL_openlibs(this->_machine);
        }
        return isReady();
    }

    void release() {
        if (isReady()) {
            lua_close(this->_machine);
            this->_machine = nullptr;
        }
    }

// File operators.
public:
    bool load(std::string const & path) {
        if (!isReady()) {
            return false;
        }

        if (luaL_loadfile(this->_machine, path.c_str()) != 0) {
            // maybe LUA_ERRFILE
            return false;
        }
        lua_pcall(this->_machine, 0, 0, 0);
        return true;
    }

// Stack operators.
public:
    int getStackSize() const {
        return lua_gettop(this->_machine);
    }

    void pop(int size) {
        lua_pop(this->_machine, size);
    }

    void push() {
        lua_pushnil(this->_machine);
    }

    void push(bool value) {
        lua_pushboolean(this->_machine, value);
    }

    void push(int value) {
        lua_pushinteger(this->_machine, value);
    }

    void push(double value) {
        lua_pushnumber(this->_machine, value);
    }

    void push(std::string const & value) {
        lua_pushstring(this->_machine, value.c_str());
    }

// Variables.
public:
    std::string getLuaString(std::string const & name) {
        lua_pushstring(this->_machine, name.c_str());
        lua_rawget(this->_machine, LUA_REGISTRYINDEX);

        if (lua_isstring(this->_machine, -1)) {
            assert(false);
        }

        std::string result = lua_tostring(this->_machine, -1);
        lua_pop(this->_machine, 1);
        return result;
    }
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

