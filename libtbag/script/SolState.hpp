/**
 * @file   SolState.hpp
 * @brief  SolState class prototype.
 * @author zer0
 * @date   2019-03-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_SOLSTATE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_SOLSTATE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/script/Sol2Bypass.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

/**
 * SolState class prototype.
 *
 * @author zer0
 * @date   2019-03-10
 */
class TBAG_API SolState
{
public:
    using State = sol::state;
    using SharedState = std::shared_ptr<State>;

private:
    SharedState _state;

public:
    SolState();
    SolState(std::nullptr_t) TBAG_NOEXCEPT;
    SolState(SolState const & obj) TBAG_NOEXCEPT;
    SolState(SolState && obj) TBAG_NOEXCEPT;
    ~SolState();

public:
    SolState & operator =(SolState const & obj) TBAG_NOEXCEPT;
    SolState & operator =(SolState && obj) TBAG_NOEXCEPT;

public:
    void copy(SolState const & obj) TBAG_NOEXCEPT;
    void swap(SolState & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(SolState & lh, SolState & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_state); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline State       * get()       TBAG_NOEXCEPT { return _state.get(); }
    inline State const * get() const TBAG_NOEXCEPT { return _state.get(); }

public:
    inline State       & operator *()       TBAG_NOEXCEPT { return *_state; }
    inline State const & operator *() const TBAG_NOEXCEPT { return *_state; }

public:
    friend inline bool operator <(SolState const & x, SolState const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(SolState const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(SolState const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();

public:
    bool loadDynAsm();
    bool loadLibraries();

public:
    std::string getLuaPath() const;
    std::vector<std::string> getLuaPaths() const;
    bool appendLuaPath(std::string const & path);

public:
    bool runScriptFile(std::string const & path);
    bool runScript(std::string const & code);

public:
    std::string findScriptPath(std::string const & filename, bool include_working = true) const;
};

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SCRIPT_SOLSTATE_HPP__

