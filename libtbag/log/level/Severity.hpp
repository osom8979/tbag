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
    // clang-format off
    Severity() TBAG_NOEXCEPT : _text(), _level()
    { /* EMPTY */ }
    Severity(char const * t, int l) TBAG_NOEXCEPT : _text(t), _level(l)
    { /* EMPTY */ }
    Severity(Severity const & obj) TBAG_NOEXCEPT : _text(obj._text), _level(obj._level)
    { /* EMPTY */ }
    ~Severity() TBAG_NOEXCEPT
    { /* EMPTY */ }
    // clang-format on

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
    // clang-format off
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
    // clang-format on

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

TBAG_CONSTEXPR int const       OFF_LEVEL = 0;
TBAG_CONSTEXPR int const EMERGENCY_LEVEL = 1;
TBAG_CONSTEXPR int const     ALERT_LEVEL = 2;
TBAG_CONSTEXPR int const  CRITICAL_LEVEL = 3;
TBAG_CONSTEXPR int const     ERROR_LEVEL = 4;
TBAG_CONSTEXPR int const   WARNING_LEVEL = 5;
TBAG_CONSTEXPR int const    NOTICE_LEVEL = 6;
TBAG_CONSTEXPR int const      INFO_LEVEL = 7;
TBAG_CONSTEXPR int const     DEBUG_LEVEL = 8;
TBAG_CONSTEXPR int const   UNKNOWN_LEVEL = 9;

Severity const       OFF_SEVERITY("OFF"      ,       OFF_LEVEL); // Hide all messages.
Severity const EMERGENCY_SEVERITY("EMERGENCY", EMERGENCY_LEVEL); // System is unusable.
Severity const     ALERT_SEVERITY("ALERT"    ,     ALERT_LEVEL); // Action must be taken immediately.
Severity const  CRITICAL_SEVERITY("CRITICAL" ,  CRITICAL_LEVEL); // Critical conditions.
Severity const     ERROR_SEVERITY("ERROR"    ,     ERROR_LEVEL); // Error conditions.
Severity const   WARNING_SEVERITY("WARNING"  ,   WARNING_LEVEL); // Warning conditions.
Severity const    NOTICE_SEVERITY("NOTICE"   ,    NOTICE_LEVEL); // Normal but significant condition.
Severity const      INFO_SEVERITY("INFO"     ,      INFO_LEVEL); // Informational messages.
Severity const     DEBUG_SEVERITY("DEBUG"    ,     DEBUG_LEVEL); // Debug-level messages.
Severity const   UNKNOWN_SEVERITY("UNKNOWN"  ,   UNKNOWN_LEVEL); // Unknown level.

inline Severity getSeverityWithLevelStep(int level) TBAG_NOEXCEPT
{
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: return       OFF_SEVERITY;
    case EMERGENCY_LEVEL: return EMERGENCY_SEVERITY;
    case     ALERT_LEVEL: return     ALERT_SEVERITY;
    case  CRITICAL_LEVEL: return  CRITICAL_SEVERITY;
    case     ERROR_LEVEL: return     ERROR_SEVERITY;
    case   WARNING_LEVEL: return   WARNING_SEVERITY;
    case    NOTICE_LEVEL: return    NOTICE_SEVERITY;
    case      INFO_LEVEL: return      INFO_SEVERITY;
    case     DEBUG_LEVEL: return     DEBUG_SEVERITY;
    default:              return   UNKNOWN_SEVERITY;
    }
    // clang-format on
}

} // namespace level
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LEVEL_SEVERITY_HPP__

