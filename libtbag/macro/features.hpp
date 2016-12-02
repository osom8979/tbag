/**
 * @file   features.hpp
 * @brief  cxx features config.
 * @author zer0
 * @date   2016-11-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_FEATURES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_FEATURES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

// Operating System.
#include <libtbag/macro/os/android.hpp>
#include <libtbag/macro/os/apple.hpp>
#include <libtbag/macro/os/cygwin.hpp>
#include <libtbag/macro/os/linux.hpp>
#include <libtbag/macro/os/unix.hpp>
#include <libtbag/macro/os/windows.hpp>

// Compiler.
#include <libtbag/macro/compiler/gcc.hpp>
#include <libtbag/macro/compiler/clang.hpp>
#include <libtbag/macro/compiler/llvm.hpp>
#include <libtbag/macro/compiler/msvc.hpp>

#ifndef TBAG_CONSTEXPR
# if defined(TBAG_HAS_CONSTEXPR)
#  define TBAG_CONSTEXPR constexpr
# else
#  define TBAG_CONSTEXPR
# endif
#endif

#ifndef TBAG_NOEXCEPT
# if defined(TBAG_HAS_NOEXCEPT)
#  define TBAG_NOEXCEPT noexcept
#  define TBAG_NOEXCEPT_EXPR(e) noexcept(e)
# else
#  define TBAG_NOEXCEPT
#  define TBAG_NOEXCEPT_EXPR(e)
# endif
#endif

/**
 * @def TBAG_DEFAULTED_FUNCTIONS
 *
 * @warning
 * move constructors and move assignment operators example:
 * @code{.cpp}
 * #if defind(TBAG_HAS_DEFAULTED_FUNCTIONS) && !defined(TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION)
 *  class(class && v) TBAG_DEFAULTED_FUNCTIONS;
 *  class & operator =(class && v) TBAG_DEFAULTED_FUNCTIONS;
 * #endif
 * @endcode
 */
#ifndef TBAG_DEFAULTED_FUNCTIONS
# if defined(TBAG_HAS_DEFAULTED_FUNCTIONS)
#  define TBAG_DEFAULTED_FUNCTIONS =default
# else
#  define TBAG_DEFAULTED_FUNCTIONS
# endif
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_FEATURES_HPP__

