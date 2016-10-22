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
#include <numeric>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

TBAG_CONSTEXPR char const * const DEFAULT_SEVERITY [] = {
        "EMERGENCY"     , // System is unusable.
        "ALERT"         , // Action must be taken immediately.
        "CRITICAL"      , // Critical conditions.
        "ERROR"         , // Error conditions.
        "WARNING"       , // Warning conditions.
        "NOTICE"        , // Normal but significant condition.
        "INFORMATIONAL" , // Informational messages.
        "DEBUG"         , // Debug-level messages.
        "OFF"           , // Hide all messages.
};

int const LOG_SEVERITY_COUNT = sizeof(DEFAULT_SEVERITY) / sizeof(DEFAULT_SEVERITY[0]);
char const * const UNKNOWN_LOG_SEVERITY_STRING = "UNKNOWN";

/**
 * Logging level.
 */
enum class LogLevel : int
{
    LEVEL_EMERGENCY = 0,
    LEVEL_ALERT        ,
    LEVEL_CRITICAL     ,
    LEVEL_ERROR        ,
    LEVEL_WARNING      ,
    LEVEL_NOTICE       ,
    LEVEL_INFO         ,
    LEVEL_DEBUG        ,
    LEVEL_OFF          ,
};

inline const char * const getLogString(int level) TBAG_NOEXCEPT
{
    if (0 <= COMPARE_AND(level) < LOG_SEVERITY_COUNT) {
        return DEFAULT_SEVERITY[level];
    }
    return UNKNOWN_LOG_SEVERITY_STRING;
}

inline const char * const getLogString(LogLevel level) TBAG_NOEXCEPT
{
    return getLogString(static_cast<int>(level));
}

/**
 * Severity class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
struct Severity
{
public:
    using String = std::basic_string<char>;

public:
    int    level;
    String text;

public:
    Severity() : level(), text()
    { /* EMPTY */ }
    Severity(LogLevel log_level) : level(static_cast<int>(log_level)), text(getLogString(static_cast<int>(log_level)))
    { /* EMPTY */ }

    Severity(Severity const & obj) : level(obj.level), text(obj.text)
    { /* EMPTY */ }
    Severity(Severity && obj) : level(obj.level), text(std::move(obj.text))
    { /* EMPTY */ }

public:
    Severity & operator =(LogLevel log_level)
    {
        level = static_cast<int>(log_level);
        text  = getLogString(static_cast<int>(log_level));
        return *this;
    }

    Severity & operator =(Severity const & obj)
    {
        if (this != &obj) {
            level = obj.level;
            text  = obj.text;
        }
        return *this;
    }

    Severity & operator =(Severity && obj)
    {
        if (this != &obj) {
            std::swap(level, obj.level);
            std::swap(text , obj.text );
        }
        return *this;
    }

public:
    inline operator int() const TBAG_NOEXCEPT
    { return level; }
    inline operator String() const TBAG_NOEXCEPT
    { return text; }

public:
    inline bool operator ==(Severity const & obj) const TBAG_NOEXCEPT
    { return level == obj.level; }
    inline bool operator !=(Severity const & obj) const TBAG_NOEXCEPT
    { return level != obj.level; }

    inline bool operator <(Severity const & obj) const TBAG_NOEXCEPT
    { return level < obj.level; }
    inline bool operator >(Severity const & obj) const TBAG_NOEXCEPT
    { return level > obj.level; }

    inline bool operator <=(Severity const & obj) const TBAG_NOEXCEPT
    { return level <= obj.level; }
    inline bool operator >=(Severity const & obj) const TBAG_NOEXCEPT
    { return level >= obj.level; }

    inline bool isContain(Severity const & obj) const TBAG_NOEXCEPT
    { return level >= obj.level; }
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_SEVERITY_HPP__

