/**
 * @file   TbuiLog.hpp
 * @brief  TbuiLog class prototype.
 * @author zer0
 * @date   2018-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBUI_LOG_TBUILOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBUI_LOG_TBUILOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/log/Log.hpp>
#include <libtbui/export.h>

namespace libtbui {
namespace log     {

TBAG_CONSTEXPR char const * const DEFAULT_LOGGER_NAME = "tu"; // 'T'bag 'U'ser-Interface library.

using Logger   = libtbag::log::Logger;
using Severity = libtbag::log::Severity;

TBAG_CONSTEXPR bool const TBUI_LOG_MUTEX_FLAG = false;
TBAG_CONSTEXPR bool const TBUI_LOG_FLUSH_FLAG = false;

// @formatter:off
Severity const       OFF_SEVERITY = ::libtbag::log::OFF_SEVERITY;
Severity const EMERGENCY_SEVERITY = ::libtbag::log::EMERGENCY_SEVERITY;
Severity const     ALERT_SEVERITY = ::libtbag::log::ALERT_SEVERITY;
Severity const  CRITICAL_SEVERITY = ::libtbag::log::CRITICAL_SEVERITY;
Severity const     ERROR_SEVERITY = ::libtbag::log::ERROR_SEVERITY;
Severity const   WARNING_SEVERITY = ::libtbag::log::WARNING_SEVERITY;
Severity const    NOTICE_SEVERITY = ::libtbag::log::NOTICE_SEVERITY;
Severity const      INFO_SEVERITY = ::libtbag::log::INFO_SEVERITY;
Severity const     DEBUG_SEVERITY = ::libtbag::log::DEBUG_SEVERITY;
// @formatter:on

TBUI_API Logger * createColorStdoutLogger();
TBUI_API Logger * createRawStdoutLogger();
TBUI_API Logger * createStdoutLogger();

TBUI_API Logger * getLogger();
TBUI_API bool removeLogger();

TBUI_API void setLevel(int level);
TBUI_API void setSeverity(Severity level);

} // namespace log
} // namespace libtbui

#define tuLogM(...)  tLogM(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogA(...)  tLogA(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogC(...)  tLogC(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogE(...)  tLogE(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogW(...)  tLogW(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogN(...)  tLogN(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogI(...)  tLogI(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogD(...)  tLogD(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)

#define tuLogIfM(c, ...)  tLogIfM(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogIfA(c, ...)  tLogIfA(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogIfC(c, ...)  tLogIfC(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogIfE(c, ...)  tLogIfE(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogIfW(c, ...)  tLogIfW(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogIfN(c, ...)  tLogIfN(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogIfI(c, ...)  tLogIfI(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogIfD(c, ...)  tLogIfD(c, ::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)

#define tuLogSM(...)  tLogSM(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogSA(...)  tLogSA(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogSC(...)  tLogSC(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogSE(...)  tLogSE(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogSW(...)  tLogSW(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogSN(...)  tLogSN(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogSI(...)  tLogSI(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)
#define tuLogSD(...)  tLogSD(::libtbui::log::DEFAULT_LOGGER_NAME, __VA_ARGS__)

#endif // __INCLUDE_LIBTBAG__LIBTBUI_LOG_TBUILOG_HPP__

