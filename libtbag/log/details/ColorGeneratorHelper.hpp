/**
 * @file   ColorGeneratorHelper.hpp
 * @brief  ColorGeneratorHelper class prototype.
 * @author zer0
 * @date   2020-05-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_COLORGENERATORHELPER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_COLORGENERATORHELPER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tty/Tces.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Severity.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

#ifndef _EMERGENCY_ATTRIBUTE_BG
#define _EMERGENCY_ATTRIBUTE_BG \
    TBAG_TTY_DISPLAY_ATTRIBUTE_BG_RED \
    TBAG_TTY_DISPLAY_ATTRIBUTE_FG_CYAN
#endif

inline char const * getColorPrefix(int level) TBAG_NOEXCEPT
{
    using namespace libtbag::tty;
    // clang-format off
    switch (level) {
    case       OFF_LEVEL: return TBAG_EMPTY_STRING;
    case EMERGENCY_LEVEL: return _EMERGENCY_ATTRIBUTE_BG;
    case     ALERT_LEVEL: return DISPLAY_ATTRIBUTE_FG_MAGENTA;
    case  CRITICAL_LEVEL: return DISPLAY_ATTRIBUTE_FG_RED;
    case     ERROR_LEVEL: return DISPLAY_ATTRIBUTE_FG_RED;
    case   WARNING_LEVEL: return DISPLAY_ATTRIBUTE_FG_YELLOW;
    case    NOTICE_LEVEL: return DISPLAY_ATTRIBUTE_FG_GREEN;
    case      INFO_LEVEL: return DISPLAY_ATTRIBUTE_FG_GREEN;
    case     DEBUG_LEVEL: return DISPLAY_ATTRIBUTE_FG_BLUE;
    case   UNKNOWN_LEVEL: return TBAG_EMPTY_STRING;
    default: /*--------*/ return TBAG_EMPTY_STRING;
    }
    // clang-format on
}

inline char const * getColorSuffix(int level) TBAG_NOEXCEPT
{
    if (EMERGENCY_LEVEL <= COMPARE_AND(level) <= DEBUG_LEVEL) {
        return libtbag::tty::DISPLAY_ATTRIBUTE_RESET;
    } else {
        return TBAG_EMPTY_STRING;
    }
}

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_COLORGENERATORHELPER_HPP__

