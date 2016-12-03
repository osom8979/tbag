/**
 * @file   platform.hpp
 * @brief  platform class prototype.
 * @author zer0
 * @date   2016-12-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/macro/platform/android.hpp>
#include <libtbag/macro/platform/apple.hpp>
#include <libtbag/macro/platform/cygwin.hpp>
#include <libtbag/macro/platform/linux.hpp>
#include <libtbag/macro/platform/unix.hpp>
#include <libtbag/macro/platform/windows.hpp>

#ifndef __PLATFORM_UNIX_LIKE__
# if !defined(__PLATFORM_WINDOWS__)
#  define __PLATFORM_UNIX_LIKE__
# endif
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_HPP__

