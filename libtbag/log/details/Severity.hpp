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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

const struct {
    char const * text;
    int level;
} SEVERITY_TABLE [] = {
        { "EMERGENCY",      0 }, // System is unusable.
        { "ALERT",          1 }, // Action must be taken immediately.
        { "CRITICAL",       2 }, // Critical conditions.
        { "ERROR",          3 }, // Error conditions.
        { "WARNING",        4 }, // Warning conditions.
        { "NOTICE",         5 }, // Normal but significant condition.
        { "INFORMATIONAL",  6 }, // Informational messages.
        { "DEBUG",          7 }, // Debug-level messages.
        { "OFF",            8 }, // Hide all messages.
};

int const LOG_SEVERITY_COUNT = sizeof(SEVERITY_TABLE) / sizeof(SEVERITY_TABLE[0]);
char const * const UNKNOWN_LOG_SEVERITY_STRING = "UNKNOWN";

int const LOG_SEVERITY_EMERGENCY     = SEVERITY_TABLE[0].level;
int const LOG_SEVERITY_ALERT         = SEVERITY_TABLE[1].level;
int const LOG_SEVERITY_CRITICAL      = SEVERITY_TABLE[2].level;
int const LOG_SEVERITY_ERROR         = SEVERITY_TABLE[3].level;
int const LOG_SEVERITY_WARNING       = SEVERITY_TABLE[4].level;
int const LOG_SEVERITY_NOTICE        = SEVERITY_TABLE[5].level;
int const LOG_SEVERITY_INFORMATIONAL = SEVERITY_TABLE[6].level;
int const LOG_SEVERITY_DEBUG         = SEVERITY_TABLE[7].level;
int const LOG_SEVERITY_OFF           = SEVERITY_TABLE[8].level;


inline const char * const getLogString(int level) noexcept
{
    if (0 <= COMPARE_AND(level) < LOG_SEVERITY_COUNT) {
        return SEVERITY_TABLE[level].text;
    }
    return UNKNOWN_LOG_SEVERITY_STRING;
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_SEVERITY_HPP__

