/**
 * @file   Severity.hpp
 * @brief  Severity class prototype.
 * @author zer0
 * @date   2016-07-09
 * @date   2017-04-13 (Move namespace: log/details -> log/level)
 * @date   2019-07-01 (Move namespace: log/level -> log)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SEVERITY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SEVERITY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <algorithm>
#include <utility>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log {

/**
 * Severity class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
class Severity
{
public:
    struct constexpr_init_t { /* EMPTY. */ };

public:
    TBAG_CONSTEXPR static constexpr_init_t const constexpr_init = {};

private:
    char const * _text;
    int _level;

public:
    Severity() TBAG_NOEXCEPT : _text(), _level()
    { /* EMPTY */ }
    Severity(char const * t, int l) TBAG_NOEXCEPT : _text(t), _level(l)
    { /* EMPTY */ }

    TBAG_CONSTEXPR Severity(constexpr_init_t, char const * t, int l) TBAG_NOEXCEPT : _text(t), _level(l)
    { /* EMPTY */ }

    Severity(Severity const & obj) TBAG_NOEXCEPT : _text(obj._text), _level(obj._level)
    { /* EMPTY. */ }

public:
    inline Severity & operator =(Severity const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _level = obj._level;
            _text  = obj._text;
        }
        return *this;
    }

public:
    inline void swap(Severity & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(_text, obj._text);
            std::swap(_level, obj._level);
        }
    }

    inline friend void swap(Severity & lh, Severity & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
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

public:
    static inline bool isContain(int threshold_level, int comparison_target_level) TBAG_NOEXCEPT
    { return comparison_target_level <= threshold_level; }

public:
    inline bool isContain(int level) const TBAG_NOEXCEPT
    { return isContain(_level, level); }
    inline bool isContain(Severity const & obj) const TBAG_NOEXCEPT
    { return isContain(obj._level); }

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

TBAG_CONSTEXPR char const * const       OFF_TEXT = "OFF";
TBAG_CONSTEXPR char const * const EMERGENCY_TEXT = "EMERGENCY";
TBAG_CONSTEXPR char const * const     ALERT_TEXT = "ALERT";
TBAG_CONSTEXPR char const * const  CRITICAL_TEXT = "CRITICAL";
TBAG_CONSTEXPR char const * const     ERROR_TEXT = "ERROR";
TBAG_CONSTEXPR char const * const   WARNING_TEXT = "WARNING";
TBAG_CONSTEXPR char const * const    NOTICE_TEXT = "NOTICE";
TBAG_CONSTEXPR char const * const      INFO_TEXT = "INFO";
TBAG_CONSTEXPR char const * const     DEBUG_TEXT = "DEBUG";
TBAG_CONSTEXPR char const * const   UNKNOWN_TEXT = "UNKNOWN";

TBAG_CONSTEXPR char const * const       OFF_DETAILS = "Hide all messages.";
TBAG_CONSTEXPR char const * const EMERGENCY_DETAILS = "System is unusable.";
TBAG_CONSTEXPR char const * const     ALERT_DETAILS = "Action must be taken immediately.";
TBAG_CONSTEXPR char const * const  CRITICAL_DETAILS = "Critical conditions.";
TBAG_CONSTEXPR char const * const     ERROR_DETAILS = "Error conditions.";
TBAG_CONSTEXPR char const * const   WARNING_DETAILS = "Warning conditions.";
TBAG_CONSTEXPR char const * const    NOTICE_DETAILS = "Normal but significant condition.";
TBAG_CONSTEXPR char const * const      INFO_DETAILS = "Informational messages.";
TBAG_CONSTEXPR char const * const     DEBUG_DETAILS = "Debug-level messages.";
TBAG_CONSTEXPR char const * const   UNKNOWN_DETAILS = "Unknown level.";

TBAG_CONSTEXPR Severity const       OFF_SEVERITY(Severity::constexpr_init,       OFF_TEXT,       OFF_LEVEL);
TBAG_CONSTEXPR Severity const EMERGENCY_SEVERITY(Severity::constexpr_init, EMERGENCY_TEXT, EMERGENCY_LEVEL);
TBAG_CONSTEXPR Severity const     ALERT_SEVERITY(Severity::constexpr_init,     ALERT_TEXT,     ALERT_LEVEL);
TBAG_CONSTEXPR Severity const  CRITICAL_SEVERITY(Severity::constexpr_init,  CRITICAL_TEXT,  CRITICAL_LEVEL);
TBAG_CONSTEXPR Severity const     ERROR_SEVERITY(Severity::constexpr_init,     ERROR_TEXT,     ERROR_LEVEL);
TBAG_CONSTEXPR Severity const   WARNING_SEVERITY(Severity::constexpr_init,   WARNING_TEXT,   WARNING_LEVEL);
TBAG_CONSTEXPR Severity const    NOTICE_SEVERITY(Severity::constexpr_init,    NOTICE_TEXT,    NOTICE_LEVEL);
TBAG_CONSTEXPR Severity const      INFO_SEVERITY(Severity::constexpr_init,      INFO_TEXT,      INFO_LEVEL);
TBAG_CONSTEXPR Severity const     DEBUG_SEVERITY(Severity::constexpr_init,     DEBUG_TEXT,     DEBUG_LEVEL);
TBAG_CONSTEXPR Severity const   UNKNOWN_SEVERITY(Severity::constexpr_init,   UNKNOWN_TEXT,   UNKNOWN_LEVEL);

TBAG_API char const * const getTextWithLevelStep(int level) TBAG_NOEXCEPT;
TBAG_API char const * const getDetailsWithLevelStep(int level) TBAG_NOEXCEPT;
TBAG_API Severity getSeverityWithLevelStep(int level) TBAG_NOEXCEPT;
TBAG_API Severity findSeverity(std::string const & name);

} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SEVERITY_HPP__

