/**
 * @file   predef.hpp
 * @brief  pre-define header.
 * @author zer0
 * @date   2016-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

#ifndef VERSION_NUMBER_ZERO
#define VERSION_NUMBER_ZERO 0
#endif

// ---------
// Language.
// ---------

#if defined(__STDC__)
# define __LANG_STDC__
# if (__STDC_VERSION__ > 0)
#  define __LANG_STDC_VERSION__  __STDC_VERSION__
# else
#  define __LANG_STDC_VERSION__  VERSION_NUMBER_ZERO
# endif
#endif

#if defined(__cplusplus)
# define __LANG_STDCPP__
# if (__cplusplus > 0)
#  define __LANG_STDCPP_VERSION__  __cplusplus
# else
#  define __LANG_STDCPP_VERSION__  VERSION_NUMBER_ZERO
# endif
#endif

#if defined(__OBJC__)
# define __LANG_OBJC__
# define __LANG_OBJC_VERSION__  VERSION_NUMBER_ZERO
#endif

// ---------
// Compiler.
// ---------

#if defined(__clang__)
#define __COMP_CLANG__
#endif

#if defined(__GNUC__)
#define __COMP_GNUC__
#endif

#if defined(__llvm__)
#define __COMP_LLVM__
#endif

#if defined(_MSC_VER)
#define __COMP_MSVC__
#endif

// -----------------
// Operating System.
// -----------------

#if defined(__CYGWIN__)
#define __OS_CYGWIN__
#endif

#if defined(linux) || defined(__linux)
#define __OS_LINUX__
#endif

#if (defined(__APPLE__) && defined(__MACH__))
# if defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#  define __OS_IOS__
# else
#  define __OS_MACOS__
# endif
#elif defined(macintosh) || defined(Macintosh)
# define __OS_MACOS__
#endif

#if defined(unix) || defined(__unix) || \
    defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#define __OS_UNIX__
#endif

#if defined(_WIN32) || defined(_WIN64) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || \
    defined(__WINDOWS__)
#define __OS_WINDOWS__
#endif

// -----------------
// Windows platform.
// -----------------

#if defined(__MINGW32__) || defined(__MINGW64__)
#define __PLAT_MINGW__
#endif

#if defined(__OS_WINDOWS) && \
    (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
#define __PLAT_WINDOWS_DESKTOP__
#endif

#if defined(__OS_WINDOWS) && \
    defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define __PLAT_WINDOWS_STORE__
#endif

#if defined(__OS_WINDOWS) && \
    defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#define __PLAT_WINDOWS_PHONE__
#endif

// ---------------
// Boost settings.
// ---------------

// Predefine collection header: <boost/predef.h>

// Don't using BOOST_ALL_DYN_LINK macro.
//#define BOOST_ALL_DYN_LINK

#if 0
#define BOOST_FILESYSTEM_DYN_LINK
#define BOOST_LOG_DYN_LINK
#endif

#if 0
# error "Don't using boost.test from the main project."
# define BOOST_TEST_DYN_LINK
# define BOOST_TEST_MODULE WorldUnitTester
#endif

#if 0
/**
 * IMPORTANT: In case if message compiler detection fails for some reason,
 * you can explicitly disable support for event log backend by defining the
 * BOOST_LOG_WITHOUT_EVENT_LOG configuration macro when building the library.
 *
 * @see http://www.boost.org/doc/libs/1_57_0_b1/libs/log/doc/html/log/installation.html
 */
# define BOOST_LOG_WITHOUT_EVENT_LOG
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREDEF_HPP__

