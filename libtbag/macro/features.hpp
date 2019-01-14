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

#include <libtbag/macro/platform.hpp>
#include <libtbag/macro/compiler.hpp>

#ifndef TBAG_FINAL
# if defined(TBAG_HAS_FINAL)
#  define TBAG_FINAL final
# else
#  define TBAG_FINAL
# endif
#endif

#ifndef TBAG_CONSTEXPR
# if defined(TBAG_HAS_CONSTEXPR)
#  define TBAG_CONSTEXPR constexpr
# else
#  define TBAG_CONSTEXPR
# endif
#endif

/**
 * @def TBAG_NOEXCEPT_OPERATOR
 *
 * noexcept operator (since C++11) @n
 * @n
 * The noexcept operator performs a compile-time check that returns true if an expression is declared to not throw any exceptions.
 * It can be used within a function template's noexcept specifier to declare that the function will throw exceptions for some types but not others.
 *
 * @remarks
 *  Bracket ~ <code>()</code> ~ was used as a trick to distinguish OPERATOR and Specifier.
 *
 * @see <http://en.cppreference.com/w/cpp/language/noexcept>
 */
#ifndef TBAG_NOEXCEPT
# if defined(TBAG_HAS_NOEXCEPT)
#  define TBAG_NOEXCEPT_OPERATOR(e)  (noexcept(e))
#  define TBAG_NOEXCEPT_SPECIFIER(e) noexcept(e)
#  define TBAG_NOEXCEPT_SP_OP(e)     TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(e))
#  define TBAG_NOEXCEPT              TBAG_NOEXCEPT_SPECIFIER(true)
# else
#  define TBAG_NOEXCEPT_OPERATOR(e)
#  define TBAG_NOEXCEPT_SPECIFIER(e)
#  define TBAG_NOEXCEPT_SP_OP(e)
#  define TBAG_NOEXCEPT
# endif
#
# /* TBAG_NOEXCEPT_ONLY_DEBUG & TBAG_NOEXCEPT_ONLY_RELEASE */
#
# if defined(NDEBUG)
#  define TBAG_NOEXCEPT_ONLY_DEBUG
#  define TBAG_NOEXCEPT_ONLY_RELEASE TBAG_NOEXCEPT
# else
#  define TBAG_NOEXCEPT_ONLY_DEBUG TBAG_NOEXCEPT
#  define TBAG_NOEXCEPT_ONLY_RELEASE
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

