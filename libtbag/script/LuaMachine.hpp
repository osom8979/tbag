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

#include <string>

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
    using FakeLuaState = void;

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
    FakeLuaState * _machine;

public:
    LuaMachine();
    ~LuaMachine();

public:
    inline bool isReady() const
    {
        return (_machine != nullptr);
    }

public:
    bool initialize();
    void release();

// File operators.
public:
    bool load(std::string const & path);

// Stack operators.
public:
    int getStackSize() const;

public:
    void pop(int size);

public:
    void push();
    void push(bool value);
    void push(int value);
    void push(double value);
    void push(std::string const & value);

// Variables.
public:
    std::string getLuaString(std::string const & name);
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

