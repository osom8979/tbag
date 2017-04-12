/**
 * @file   Severity.hpp
 * @brief  Severity class prototype.
 * @author zer0
 * @date   2016-07-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_SEVERITY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_SEVERITY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

/**
 * Severity class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
class Severity
{
public:
    char const * text;
    int level;

public:
    // @formatter:off
    Severity() : text(), level()
    { /* EMPTY */ }
    Severity(char const * t, int l) : text(t), level(l)
    { /* EMPTY */ }
    Severity(Severity const & obj) : text(obj.text), level(obj.level)
    { /* EMPTY */ }
    ~Severity()
    { /* EMPTY */ }
    // @formatter:on

public:
    Severity & operator =(Severity const & obj)
    {
        if (this != &obj) {
            level = obj.level;
            text  = obj.text;
        }
        return *this;
    }

public:
    // @formatter:off
    inline operator int() const TBAG_NOEXCEPT
    { return level; }
    inline operator char const *() const TBAG_NOEXCEPT
    { return text; }

    inline friend bool operator ==(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh.level == rh.level; }
    inline friend bool operator !=(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh.level != rh.level; }

    inline friend bool operator <(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh.level < rh.level; }
    inline friend bool operator >(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh.level > rh.level; }

    inline friend bool operator <=(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh.level <= rh.level; }
    inline friend bool operator >=(Severity const & lh, Severity const & rh) TBAG_NOEXCEPT
    { return lh.level >= rh.level; }

    inline bool isContain(Severity const & obj) const TBAG_NOEXCEPT
    { return level >= obj.level; }
    // @formatter:on
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

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_SEVERITY_HPP__

