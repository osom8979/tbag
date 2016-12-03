/**
 * @file   cygwin.hpp
 * @brief  cygwin platform config.
 * @author zer0
 * @date   2016-09-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_CYGWIN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_CYGWIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__CYGWIN__)
#define __PLATFORM_CYGWIN__
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_PLATFORM_CYGWIN_HPP__

