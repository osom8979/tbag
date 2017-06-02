/**
 * @file   Severity.hpp
 * @brief  Severity class prototype.
 * @author zer0
 * @date   2016-07-09
 * @date   2017-04-13 (Move namespace: log/details -> log/level)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_LEVEL_SEVERITY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_LEVEL_SEVERITY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log   {
namespace level {

/**
 * Severity class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
class Severity
{
private:
    char const * _text;
    int _level;

public:
    // @formatter:off
    Severity() TBAG_NOEXCEPT : _text(), _level()
    { /* EMPTY */ }
    Severity(char const * t, int l) TBAG_NOEXCEPT : _text(t), _level(l)
    { /* EMPTY */ }
    Severity(Severity const & obj) TBAG_NOEXCEPT : _text(obj._text), _level(obj._level)
    { /* EMPTY */ }
    ~Severity() TBAG_NOEXCEPT
    { /* EMPTY */ }
    // @formatter:on

public:
    Severity & operator =(Severity const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _level = obj._level;
            _text  = obj._text;
        }
        return *this;
    }

public:
    // @formatter:off
    inline operator int() const TBAG_NOEXCEPT
    { return _level; }
    inline operator char const *() const TBAG_NOEXCEPT
    { return _text; }

    inline friend bool operator ==(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh._level == rh._level; }
    inline friend bool operator !=(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh._level != rh._level; }

    inline friend bool operator <(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh._level < rh._level; }
    inline friend bool operator >(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh._level > rh._level; }

    inline friend bool operator <=(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh._level <= rh._level; }
    inline friend bool operator >=(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh._level >= rh._level; }

    inline bool isContain(Severity const & obj) const TBAG_NOEXCEPT
    { return _level >= obj._level; }
    // @formatter:on

    inline friend void swap(Severity & lh, Severity & rh) TBAG_NOEXCEPT
    {
        if (&lh != &rh) {
            std::swap(lh._text, rh._text);
            std::swap(lh._level, rh._level);
        }
    }

public:
    inline char const * getText() const TBAG_NOEXCEPT { return _text; }
    inline int getLevel() const TBAG_NOEXCEPT { return _level; }
};

Severity const           OFF_SEVERITY("OFF"      ,   0); // Hide all messages.
Severity const     EMERGENCY_SEVERITY("EMERGENCY", 100); // System is unusable.
Severity const         ALERT_SEVERITY("ALERT"    , 200); // Action must be taken immediately.
Severity const      CRITICAL_SEVERITY("CRITICAL" , 300); // Critical conditions.
Severity const         ERROR_SEVERITY("ERROR"    , 400); // Error conditions.
Severity const       WARNING_SEVERITY("WARNING"  , 500); // Warning conditions.
Severity const        NOTICE_SEVERITY("NOTICE"   , 600); // Normal but significant condition.
Severity const INFORMATIONAL_SEVERITY("INFO"     , 700); // Informational messages.
Severity const         DEBUG_SEVERITY("DEBUG"    , 800); // Debug-level messages.

} // namespace level
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LEVEL_SEVERITY_HPP__

