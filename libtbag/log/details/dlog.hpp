/**
 * @file   dlog.hpp
 * @brief  dlog class prototype.
 * @author zer0
 * @date   2019-07-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_DLOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_DLOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/preprocessor/control/If.hpp>
#include <libtbag/preprocessor/variadic/VariadicSize.hpp>
#include <libtbag/preprocessor/comparison/Equal.hpp>
#include <libtbag/tty/Tces.hpp>
#include <libtbag/string/fmt/format.h>

#define __TBAG_DLOG_PREFIX_PROFILE    TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_MAGENTA "[P] " DEBUG_STAMP " "
#define __TBAG_DLOG_PREFIX_DEBUG      TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_BLUE    "[D] " DEBUG_STAMP " "
#define __TBAG_DLOG_PREFIX_INFO       TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_GREEN   "[I] " DEBUG_STAMP " "
#define __TBAG_DLOG_PREFIX_WARNING    TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_YELLOW  "[W] " DEBUG_STAMP " "
#define __TBAG_DLOG_PREFIX_ERROR      TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_RED     "[E] " DEBUG_STAMP " "

#if defined(TBAG_PLATFORM_WINDOWS)
#define __TBAG_DLOG_SUFFIX  TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET "\r\n"
#else
#define __TBAG_DLOG_SUFFIX  TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET "\n"
#endif

#define __TBAG_DLOG_FMT_PROFILE1(f)    ::fmt::format(__TBAG_DLOG_PREFIX_PROFILE f __TBAG_DLOG_SUFFIX).c_str()
#define __TBAG_DLOG_FMT_DEBUG1(f)      ::fmt::format(__TBAG_DLOG_PREFIX_DEBUG   f __TBAG_DLOG_SUFFIX).c_str()
#define __TBAG_DLOG_FMT_INFO1(f)       ::fmt::format(__TBAG_DLOG_PREFIX_INFO    f __TBAG_DLOG_SUFFIX).c_str()
#define __TBAG_DLOG_FMT_WARNING1(f)    ::fmt::format(__TBAG_DLOG_PREFIX_WARNING f __TBAG_DLOG_SUFFIX).c_str()
#define __TBAG_DLOG_FMT_ERROR1(f)      ::fmt::format(__TBAG_DLOG_PREFIX_ERROR   f __TBAG_DLOG_SUFFIX).c_str()

#define __TBAG_DLOG_FMT_PROFILE2(f, ...)    ::fmt::format(__TBAG_DLOG_PREFIX_PROFILE f __TBAG_DLOG_SUFFIX, __VA_ARGS__).c_str()
#define __TBAG_DLOG_FMT_DEBUG2(f, ...)      ::fmt::format(__TBAG_DLOG_PREFIX_DEBUG   f __TBAG_DLOG_SUFFIX, __VA_ARGS__).c_str()
#define __TBAG_DLOG_FMT_INFO2(f, ...)       ::fmt::format(__TBAG_DLOG_PREFIX_INFO    f __TBAG_DLOG_SUFFIX, __VA_ARGS__).c_str()
#define __TBAG_DLOG_FMT_WARNING2(f, ...)    ::fmt::format(__TBAG_DLOG_PREFIX_WARNING f __TBAG_DLOG_SUFFIX, __VA_ARGS__).c_str()
#define __TBAG_DLOG_FMT_ERROR2(f, ...)      ::fmt::format(__TBAG_DLOG_PREFIX_ERROR   f __TBAG_DLOG_SUFFIX, __VA_ARGS__).c_str()

#define __TBAG_DLOG_FMT_PROFILE_I(...)    TBAG_PP_IF(TBAG_PP_EQUAL(1, TBAG_PP_VARIADIC_SIZE(__VA_ARGS__)), __TBAG_DLOG_FMT_PROFILE1, __TBAG_DLOG_FMT_PROFILE2)
#define __TBAG_DLOG_FMT_DEBUG_I(...)      TBAG_PP_IF(TBAG_PP_EQUAL(1, TBAG_PP_VARIADIC_SIZE(__VA_ARGS__)), __TBAG_DLOG_FMT_DEBUG1, __TBAG_DLOG_FMT_DEBUG2)
#define __TBAG_DLOG_FMT_INFO_I(...)       TBAG_PP_IF(TBAG_PP_EQUAL(1, TBAG_PP_VARIADIC_SIZE(__VA_ARGS__)), __TBAG_DLOG_FMT_INFO1, __TBAG_DLOG_FMT_INFO2)
#define __TBAG_DLOG_FMT_WARNING_I(...)    TBAG_PP_IF(TBAG_PP_EQUAL(1, TBAG_PP_VARIADIC_SIZE(__VA_ARGS__)), __TBAG_DLOG_FMT_WARNING1, __TBAG_DLOG_FMT_WARNING2)
#define __TBAG_DLOG_FMT_ERROR_I(...)      TBAG_PP_IF(TBAG_PP_EQUAL(1, TBAG_PP_VARIADIC_SIZE(__VA_ARGS__)), __TBAG_DLOG_FMT_ERROR1, __TBAG_DLOG_FMT_ERROR2)

#define __TBAG_DLOG_FMT_PROFILE(...)    __TBAG_DLOG_FMT_PROFILE_I(__VA_ARGS__)(__VA_ARGS__)
#define __TBAG_DLOG_FMT_DEBUG(...)      __TBAG_DLOG_FMT_DEBUG_I(__VA_ARGS__)(__VA_ARGS__)
#define __TBAG_DLOG_FMT_INFO(...)       __TBAG_DLOG_FMT_INFO_I(__VA_ARGS__)(__VA_ARGS__)
#define __TBAG_DLOG_FMT_WARNING(...)    __TBAG_DLOG_FMT_WARNING_I(__VA_ARGS__)(__VA_ARGS__)
#define __TBAG_DLOG_FMT_ERROR(...)      __TBAG_DLOG_FMT_ERROR_I(__VA_ARGS__)(__VA_ARGS__)

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

TBAG_API void dlog(char const * TBAG_RESTRICT text);

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

/**
 * @def ENABLE_TBAG_LIBRARY_DEBUGGING_LOG
 *
 * @warning
 *  Don't use this macros from user level developers.
 */
#if defined(ENABLE_TBAG_LIBRARY_DEBUGGING_LOG)
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 5
#  define _tp(...) ::libtbag::log::details::dlog(__TBAG_DLOG_FMT_PROFILE(__VA_ARGS__))
# else
#  define _tp(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 4
#  define _td(...) ::libtbag::log::details::dlog(__TBAG_DLOG_FMT_DEBUG(__VA_ARGS__))
# else
#  define _td(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 3
#  define _ti(...) ::libtbag::log::details::dlog(__TBAG_DLOG_FMT_INFO(__VA_ARGS__))
# else
#  define _ti(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 2
#  define _tw(...) ::libtbag::log::details::dlog(__TBAG_DLOG_FMT_WARNING(__VA_ARGS__))
# else
#  define _tw(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 1
#  define _te(...) ::libtbag::log::details::dlog(__TBAG_DLOG_FMT_ERROR(__VA_ARGS__))
# else
#  define _te(...)
# endif
#else
# define _tp(...)
# define _td(...)
# define _ti(...)
# define _tw(...)
# define _te(...)
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_DLOG_HPP__

