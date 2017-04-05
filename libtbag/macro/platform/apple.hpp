/**
 * @file   apple.hpp
 * @brief  apple platform config.
 * @author zer0
 * @date   2016-11-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_APPLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_APPLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if (defined(__APPLE__) && defined(__MACH__))
# if defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#  define TBAG_PLATFORM_IOS
# else
#  define TBAG_PLATFORM_MACOS
# endif
#elif defined(macintosh) || defined(Macintosh)
# define TBAG_PLATFORM_MACOS
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_APPLE_HPP__

