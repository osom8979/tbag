/**
 * @file   clang.hpp
 * @brief  clang compiler config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_CLANG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_CLANG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__clang__)
#define __COMP_CLANG__
#endif

#if defined(__COMP_CLANG__)
#define __COMP_CLANG_VERSION__          \
    ( (__clang_major__      * 10000 )   \
    + (__clang_minor__      * 100   )   \
    + (__clang_patchlevel__ * 1     ) )
#else
#define __COMP_CLANG_VERSION__ 0
#endif

// =============
// FEATURE OPEN.
#if defined(__COMP_CLANG__)
# if __has_feature(cxx_constexpr)
# define TBAG_HAS_CONSTEXPR
# endif

# if __has_feature(cxx_noexcept)
# define TBAG_HAS_NOEXCEPT
# endif

# if __has_feature(cxx_defaulted_functions)
# define TBAG_HAS_DEFAULTED_FUNCTIONS
# endif

# if __has_feature(cxx_deleted_functions)
# define TBAG_HAS_DELETED_FUNCTIONS
# endif
#endif // defined(__COMP_CLANG__)
// FEATURE CLOSE.
// ==============

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_CLANG_HPP__

