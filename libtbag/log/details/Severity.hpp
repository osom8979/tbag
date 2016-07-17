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
#include <libtbag/config/macro.hpp>

#include <string>
#include <numeric>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

using SeverityFlagType = uint32_t;

static_assert(std::is_unsigned<SeverityFlagType>::value, "SeverityFlagType must be a unsigned");

/**
 * List of severity.
 *
 * @author zer0
 * @date   2016-07-09
 */
struct DefaultSeverityProperty
{
    char const * const text;
    int level;
    SeverityFlagType flag;
};

constexpr DefaultSeverityProperty const DEFAULT_SEVERITY [] = {
        { "EMERGENCY",      0, 0b00000001 }, // System is unusable.
        { "ALERT",          1, 0b00000011 }, // Action must be taken immediately.
        { "CRITICAL",       2, 0b00000111 }, // Critical conditions.
        { "ERROR",          3, 0b00001111 }, // Error conditions.
        { "WARNING",        4, 0b00011111 }, // Warning conditions.
        { "NOTICE",         5, 0b00111111 }, // Normal but significant condition.
        { "INFORMATIONAL",  6, 0b01111111 }, // Informational messages.
        { "DEBUG",          7, 0b11111111 }, // Debug-level messages.
        { "OFF",            8, 0b00000000 }, // Hide all messages.
};

int const LOG_SEVERITY_COUNT = sizeof(DEFAULT_SEVERITY) / sizeof(DEFAULT_SEVERITY[0]);
char const * const UNKNOWN_LOG_SEVERITY_STRING = "UNKNOWN";

constexpr int const LOG_SEVERITY_EMERGENCY     = DEFAULT_SEVERITY[0].level;
constexpr int const LOG_SEVERITY_ALERT         = DEFAULT_SEVERITY[1].level;
constexpr int const LOG_SEVERITY_CRITICAL      = DEFAULT_SEVERITY[2].level;
constexpr int const LOG_SEVERITY_ERROR         = DEFAULT_SEVERITY[3].level;
constexpr int const LOG_SEVERITY_WARNING       = DEFAULT_SEVERITY[4].level;
constexpr int const LOG_SEVERITY_NOTICE        = DEFAULT_SEVERITY[5].level;
constexpr int const LOG_SEVERITY_INFORMATIONAL = DEFAULT_SEVERITY[6].level;
constexpr int const LOG_SEVERITY_DEBUG         = DEFAULT_SEVERITY[7].level;
constexpr int const LOG_SEVERITY_OFF           = DEFAULT_SEVERITY[8].level;

inline const char * const getLogString(int level) noexcept
{
    if (0 <= COMPARE_AND(level) < LOG_SEVERITY_COUNT) {
        return DEFAULT_SEVERITY[level].text;
    }
    return UNKNOWN_LOG_SEVERITY_STRING;
}

/**
 * Severity class prototype.
 *
 * @author zer0
 * @date   2016-07-09
 */
class Severity
{
public:
    using String = std::basic_string<char>;
    using Flag   = SeverityFlagType;

private:
    String _text;
    Flag   _flag;

public:
    Severity() noexcept;
    Severity(String const & text, Flag flag) noexcept;
    Severity(DefaultSeverityProperty const & property) noexcept;
    Severity(int level) noexcept;

    Severity(Severity const & obj) noexcept;
    Severity(Severity && obj) noexcept;
    ~Severity() noexcept;

public:
    Severity & operator =(Severity const & obj) noexcept;
    Severity & operator =(Severity && obj) noexcept;

public:
    operator String() const noexcept;

public:
    Severity & copy(Severity const & obj) noexcept;
    void swap(Severity & obj) noexcept;

public:
    Severity & operator |=(Severity const & obj) noexcept;
    Severity & operator ^=(Severity const & obj) noexcept;
    Severity & operator &=(Severity const & obj) noexcept;

public:
    bool operator ==(Severity const & obj) const noexcept;
    bool operator !=(Severity const & obj) const noexcept;

public:
    void setText(String const & text) noexcept;
    String getText() const noexcept;

public:
    void setFlag(Flag flag) noexcept;
    Flag getFlag() const noexcept;
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_SEVERITY_HPP__

