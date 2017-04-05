/**
 * @file   unix.hpp
 * @brief  unix platform config.
 * @author zer0
 * @date   2016-11-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_UNIX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_UNIX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(unix) || defined(__unix) || defined(__unix__) || \
    defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)
#define TBAG_PLATFORM_UNIX
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_UNIX_HPP__

