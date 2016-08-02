/**
 * @file   gcc.hpp
 * @brief  gcc compiler config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_GCC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_GCC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__GNUC__)
#define __COMP_GNUC__
# if defined(__cplusplus)
#  define __COMP_GNUC_CXX__
# endif
#endif

#if defined(__COMP_GNUC__)
#define __COMP_GNUC_VERSION__           \
    ( (__GNUC__            * 10000 )    \
    + (__GNUC_MINOR__      * 100   )    \
    + (__GNUC_PATCHLEVEL__ * 1     ) )
#else
#define __COMP_GNUC_VERSION__ 0
#endif

/**
 * @def CHECK_GNUC_CXX_REGEX
 *
 * Test regex: GCC, C++, GCC < 4.9
 *
 * @remarks
 *  Usage:
 *  @code{.cpp}
 *   #define CHECK_GNUC_CXX_REGEX
 *   #include <libtbag/predef.hpp>
 *  @endcode
 */
#if defined(CHECK_GNUC_CXX_REGEX)
# if defined(__COMP_GNUC_CXX__) && !defined(__clang__)
#  if (__COMP_GNUC_VERSION__ < 40900)
#   error "Not supported regex."
#  endif
# endif
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_GCC_HPP__

