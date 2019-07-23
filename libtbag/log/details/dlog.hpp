/**
 * @file   dlog.hpp
 * @brief  dlog class prototype.
 * @author zer0
 * @date   2019-07-23
 *
 * @warning
 *  Don't use this macros from user level developers.
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_DLOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_DLOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/fmt/format.h>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

TBAG_CONSTEXPR char const * const DEBUG_LOG_LEVEL_ENV_NAME = "TBAG_LIBRARY_DEBUG_LOG_LEVEL";

TBAG_API int getDebugLogLevel() TBAG_NOEXCEPT;

TBAG_API void setDebugLogLevel(int level) TBAG_NOEXCEPT;
TBAG_API void setDebugLogLevelFromEnvs();

TBAG_API void dlog(int level, char const * file, int line, std::string const & text);

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#define _tp_level 5
#define _td_level 4
#define _ti_level 3
#define _tw_level 2
#define _te_level 1

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG) && (ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= _tp_level)
#define _tp(...) ::libtbag::log::details::dlog(_tp_level, FILE_STRING, LINE_NUMBER, ::fmt::format(__VA_ARGS__))
#else
#define _tp(...)
#endif

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG) && (ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= _td_level)
#define _td(...) ::libtbag::log::details::dlog(_td_level, FILE_STRING, LINE_NUMBER, ::fmt::format(__VA_ARGS__))
#else
#define _td(...)
#endif

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG) && (ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= _ti_level)
#define _ti(...) ::libtbag::log::details::dlog(_ti_level, FILE_STRING, LINE_NUMBER, ::fmt::format(__VA_ARGS__))
#else
#define _ti(...)
#endif

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG) && (ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= _tw_level)
#define _tw(...) ::libtbag::log::details::dlog(_tw_level, FILE_STRING, LINE_NUMBER, ::fmt::format(__VA_ARGS__))
#else
#define _tw(...)
#endif

#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG) && (ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= _te_level)
#define _te(...) ::libtbag::log::details::dlog(_te_level, FILE_STRING, LINE_NUMBER, ::fmt::format(__VA_ARGS__))
#else
#define _te(...)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_DLOG_HPP__

