/**
 * @file   LuaMachine.hpp
 * @brief  LuaMachine class prototype.
 * @author zer0
 * @date   2019-03-10
 * @date   2019-04-27 (Rename: SolState -> LuaMachine)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/LuaBypass.hpp>
#include <libtbag/util/Version.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

/**
 * LuaMachine class prototype.
 *
 * @author zer0
 * @date   2019-03-10
 * @date   2019-04-27 (Rename: SolState -> LuaMachine)
 */
class TBAG_API LuaMachine
{
public:
    using State = lua_State;
    using SharedState = std::shared_ptr<State>;
    using Version = libtbag::util::Version;

public:
    TBAG_CONSTEXPR static char const * const lua_tbag_name() TBAG_NOEXCEPT
    { return "tbag"; }

private:
    SharedState _state;

public:
    LuaMachine();
    LuaMachine(std::nullptr_t) TBAG_NOEXCEPT;
    LuaMachine(LuaMachine const & obj) TBAG_NOEXCEPT;
    LuaMachine(LuaMachine && obj) TBAG_NOEXCEPT;
    ~LuaMachine();

public:
    LuaMachine & operator =(LuaMachine const & obj) TBAG_NOEXCEPT;
    LuaMachine & operator =(LuaMachine && obj) TBAG_NOEXCEPT;

public:
    void copy(LuaMachine const & obj) TBAG_NOEXCEPT;
    void swap(LuaMachine & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(LuaMachine & lh, LuaMachine & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_state); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline lua_State * state() TBAG_NOEXCEPT
    { return _state.get(); }
    inline lua_State const * state() const TBAG_NOEXCEPT
    { return _state.get(); }

public:
    void reset();

public:
    std::string getLuaPath() const;
    std::vector<std::string> getLuaPaths() const;
    bool appendLuaPath(std::string const & path);

    std::string getLuaCPath() const;
    std::vector<std::string> getLuaCPaths() const;
    bool appendLuaCPath(std::string const & path);

public:
    bool runScriptFile(std::string const & path);
    bool runScript(std::string const & code);

public:
    std::string findScriptPath(std::string const & filename, bool include_working = true) const;

public:
    Version getLuaVersion() const;
    Version getLuaJITVersion() const;
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_LUAMACHINE_HPP__

