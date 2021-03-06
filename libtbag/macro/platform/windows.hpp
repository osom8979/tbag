/**
 * @file   windows.hpp
 * @brief  windows platform config.
 * @author zer0
 * @date   2016-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_WINDOWS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_WINDOWS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(WIN32) || defined(_WIN32) || \
    defined(WIN64) || defined(_WIN64) || \
    defined(WINCE) || defined(_WINCE) || \
    defined(__TOS_WIN__) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || \
    defined(__WINDOWS__)
#define TBAG_PLATFORM_WINDOWS
#endif

// -------------------------
// Windows platform version.
// -------------------------

#if defined(TBAG_PLATFORM_WINDOWS)
# ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN
# endif

# ifndef _WIN32_WINNT
# define _WIN32_WINNT 0x0600
# endif

# ifndef _WIN32_WINDOWS
# define _WIN32_WINDOWS 0x0410
# endif

// Minimum platform version.
# ifndef WINVER
# define WINVER _WIN32_WINNT
# endif

// Minimum internet explorer version.
# ifndef _WIN32_IE
# define _WIN32_IE 0x0700
# endif
#endif // defined(TBAG_PLATFORM_WINDOWS)

// -----------------
// Windows platform.
// -----------------

#if defined(__MINGW32__) || defined(__MINGW64__)
#define TBAG_PLATFORM_MINGW
#endif

#if defined(__PLATFORM_WINDOWS) && \
    (!defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP))
#define TBAG_PLATFORM_WINDOWS_DESKTOP
#endif

#if defined(__PLATFORM_WINDOWS) && \
    defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_APP)
#define TBAG_PLATFORM_WINDOWS_STORE
#endif

#if defined(__PLATFORM_WINDOWS) && \
    defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP)
#define TBAG_PLATFORM_WINDOWS_PHONE
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_WINDOWS_HPP__

