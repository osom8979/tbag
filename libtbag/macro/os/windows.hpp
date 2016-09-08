/**
 * @file   windows.hpp
 * @brief  windows os config.
 * @author zer0
 * @date   2016-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_WINDOWS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_WINDOWS_HPP__

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
#define __OS_WINDOWS__
#endif

// -------------------------
// Windows platform version.
// -------------------------

#if defined(__OS_WINDOWS__)
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
#endif // defined(__OS_WINDOWS__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_WINDOWS_HPP__

