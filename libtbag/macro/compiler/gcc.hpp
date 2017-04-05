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
#define TBAG_COMP_GNUC
# if defined(__cplusplus)
#  define TBAG_COMP_GNUC_CXX
# endif
#endif

#if defined(TBAG_COMP_GNUC)
#define TBAG_COMP_GNUC_VERSION          \
    ( (__GNUC__            * 10000 )    \
    + (__GNUC_MINOR__      * 100   )    \
    + (__GNUC_PATCHLEVEL__ * 1     ) )
#else
#define TBAG_COMP_GNUC_VERSION 0
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
# if defined(TBAG_COMP_GNUC_CXX) && !defined(__clang__)
#  if (TBAG_COMP_GNUC_VERSION < 40900)
#   error "Not supported regex."
#  endif
# endif
#endif

// =============
// FEATURE OPEN.
#if defined(TBAG_COMP_GNUC_CXX)
// C++0x features in 4.4.n and later
# if (TBAG_COMP_GNUC_VERSION >= 40400)
#  define TBAG_HAS_AUTO_DECLARATIONS
#  define TBAG_HAS_AUTO_MULTIDECLARATIONS
#  define TBAG_HAS_CHAR16_T
#  define TBAG_HAS_CHAR32_T
#  define TBAG_HAS_HDR_INITIALIZER_LIST
#  define TBAG_HAS_DEFAULTED_FUNCTIONS
# endif

// C++0x features in 4.6.n and later
# if (TBAG_COMP_GNUC_VERSION >= 40600)
#  define TBAG_HAS_CONSTEXPR
#  define TBAG_HAS_NOEXCEPT
#  define TBAG_HAS_NULLPTR
#  define TBAG_HAS_RANGE_BASED_FOR
#  define TBAG_HAS_UNIFIED_INITIALIZATION_SYNTAX
# endif

// C++0x features in 4.7.n and later
#if (TBAG_COMP_GNUC_VERSION >= 40700)
#  define TBAG_HAS_FINAL
#  define TBAG_HAS_TEMPLATE_ALIASES
#  define TBAG_HAS_USER_DEFINED_LITERALS
#  define TBAG_HAS_FIXED_LENGTH_VARIADIC_TEMPLATE_EXPANSION_PACKS
#endif
#endif // defined(TBAG_COMP_GNUC_CXX)
// FEATURE CLOSE.
// ==============

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_GCC_HPP__

