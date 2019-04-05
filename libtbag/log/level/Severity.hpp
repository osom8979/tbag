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

TBAG_CONSTEXPR int LEVEL_STEP = 100;

Severity const           OFF_SEVERITY("OFF"      , 0 * LEVEL_STEP); // Hide all messages.
Severity const     EMERGENCY_SEVERITY("EMERGENCY", 1 * LEVEL_STEP); // System is unusable.
Severity const         ALERT_SEVERITY("ALERT"    , 2 * LEVEL_STEP); // Action must be taken immediately.
Severity const      CRITICAL_SEVERITY("CRITICAL" , 3 * LEVEL_STEP); // Critical conditions.
Severity const         ERROR_SEVERITY("ERROR"    , 4 * LEVEL_STEP); // Error conditions.
Severity const       WARNING_SEVERITY("WARNING"  , 5 * LEVEL_STEP); // Warning conditions.
Severity const        NOTICE_SEVERITY("NOTICE"   , 6 * LEVEL_STEP); // Normal but significant condition.
Severity const INFORMATIONAL_SEVERITY("INFO"     , 7 * LEVEL_STEP); // Informational messages.
Severity const         DEBUG_SEVERITY("DEBUG"    , 8 * LEVEL_STEP); // Debug-level messages.
Severity const       UNKNOWN_SEVERITY("UNKNOWN"  , 9 * LEVEL_STEP); // Unknown level.

inline Severity getSeverityWithLevelStep(int level) TBAG_NOEXCEPT
{
    // clang-format off
    switch (level) {
    case 0:  return           OFF_SEVERITY;
    case 1:  return     EMERGENCY_SEVERITY;
    case 2:  return         ALERT_SEVERITY;
    case 3:  return      CRITICAL_SEVERITY;
    case 4:  return         ERROR_SEVERITY;
    case 5:  return       WARNING_SEVERITY;
    case 6:  return        NOTICE_SEVERITY;
    case 7:  return INFORMATIONAL_SEVERITY;
    case 8:  return         DEBUG_SEVERITY;
    default: return       UNKNOWN_SEVERITY;
    }
    // clang-format on
}

} // namespace level
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_LEVEL_SEVERITY_HPP__

