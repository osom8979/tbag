/**
 * @file   apple.hpp
 * @brief  apple os config.
 * @author zer0
 * @date   2016-11-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_APPLE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_APPLE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_OS_APPLE_HPP__

