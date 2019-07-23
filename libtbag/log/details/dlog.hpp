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
# include <libtbag/string/fmt/format.h>
# include <libtbag/tty/Tces.hpp>
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 5
#  define __tbag_profile(...) ::libtbag::log::details::dlog(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_WHITE "P/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_profile(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 4
#  define __tbag_debug(...) ::libtbag::log::details::dlog(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_BLUE "D/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_debug(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 3
#  define __tbag_info(...) ::libtbag::log::details::dlog(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_GREEN "I/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_info(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 2
#  define __tbag_warning(...) ::libtbag::log::details::dlog(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_YELLOW "W/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_warning(...)
# endif
# if ENABLE_TBAG_LIBRARY_DEBUGGING_LOG >= 1
#  define __tbag_error(...) ::libtbag::log::details::dlog(::fmt::format(TBAG_OS_TTY_DISPLAY_ATTRIBUTE_FG_RED "E/" DEBUG_STAMP TBAG_OS_TTY_DISPLAY_ATTRIBUTE_RESET " " __VA_ARGS__).c_str())
# else
#  define __tbag_error(...)
# endif
#else
# define __tbag_profile(...)
# define __tbag_debug(...)
# define __tbag_info(...)
# define __tbag_warning(...)
# define __tbag_error(...)
#endif

#define _dp(...) __tbag_profile(__VA_ARGS__)
#define _dd(...) __tbag_debug(__VA_ARGS__)
#define _di(...) __tbag_info(__VA_ARGS__)
#define _dw(...) __tbag_warning(__VA_ARGS__)
#define _de(...) __tbag_error(__VA_ARGS__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_DLOG_HPP__

