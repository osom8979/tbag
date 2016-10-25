/**
 * @file   msvc.hpp
 * @brief  msvc compiler config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_MSVC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_MSVC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_MSC_VER)
#define __COMP_MSVC__
#endif

//  1.0            _MSC_VER ==  800
//  3.0            _MSC_VER ==  900
//  4.0            _MSC_VER == 1000
//  4.2            _MSC_VER == 1020
//  5.0            _MSC_VER == 1100
//  6.0            _MSC_VER == 1200
//  6.0 SP6        _MSC_VER == 1200 && _MSC_FULL_VER == 12008804
//  7.0            _MSC_VER == 1300 && _MSC_FULL_VER == 13009466
//  7.1 (2003)     _MSC_VER == 1310 && _MSC_FULL_VER == 13103077
//  8.0 (2005)     _MSC_VER == 1400 && _MSC_FULL_VER == 140050727
//  9.0 (2008)     _MSC_VER == 1500 && _MSC_FULL_VER == 150021022
//  9.0 SP1        _MSC_VER == 1500 && _MSC_FULL_VER == 150030729
// 10.0 (2010)     _MSC_VER == 1600 && _MSC_FULL_VER == 160030319
// 10.0 (2010) SP1 _MSC_VER == 1600 && _MSC_FULL_VER == 160040219
// 11.0 (2012)     _MSC_VER == 1700 && _MSC_FULL_VER == 170050727
// 12.0 (2013)     _MSC_VER == 1800 && _MSC_FULL_VER == 180021005
// 14.0 (2015)     _MSC_VER == 1900 && _MSC_FULL_VER == ?????????

#if defined(_MSC_VER)
# define __COMP_MSVC_VERSION__ _MSC_VER
#else
# define __COMP_MSVC_VERSION__ 0
#endif

// =============
// FEATURE OPEN.
#if defined(__COMP_MSVC__)
// C++11 features supported by VC++ 12 (aka 2013).
# if (_MSC_FULL_VER >= 180020827)
#  define TBAG_HAS_DEFAULTED_FUNCTIONS
#  define TBAG_HAS_DELETED_FUNCTIONS
#  define TBAG_HAS_EXPLICIT_CONVERSION_OPERATORS
#  define TBAG_HAS_FUNCTION_TEMPLATE_DEFAULT_ARGS
#  define TBAG_HAS_RAW_LITERALS
#  define TBAG_HAS_TEMPLATE_ALIASES
#  define TBAG_HAS_TRAILING_RESULT_TYPES
#  define TBAG_HAS_VARIADIC_TEMPLATES
#  define TBAG_HAS_UNIFIED_INITIALIZATION_SYNTAX
#  define TBAG_HAS_DECLTYPE_N3276
# endif

// VC++ 12 <= version < VC++ 14
// move constructors and move assignment operators is not supported.
// see: https://msdn.microsoft.com/en-US/library/hh567368.aspx
# if (_MSC_FULL_VER >= 180020827) && (_MSC_FULL_VER < 190023026)
#  define TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION
# endif

// C++11 features supported by VC++ 14 (aka 2015)
# if (_MSC_FULL_VER >= 190023026)
#  define TBAG_HAS_NOEXCEPT
#  define TBAG_HAS_REF_QUALIFIERS
#  define TBAG_HAS_USER_DEFINED_LITERALS
#  define TBAG_HAS_ALIGNAS
#  define TBAG_HAS_INLINE_NAMESPACES
#  define TBAG_HAS_CHAR16_T
#  define TBAG_HAS_CHAR32_T
#  define TBAG_HAS_UNICODE_LITERALS
#  define TBAG_HAS_DECLTYPE_AUTO
#  define TBAG_HAS_INITIALIZED_LAMBDA_CAPTURES
#  define TBAG_HAS_RETURN_TYPE_DEDUCTION
#  define TBAG_HAS_BINARY_LITERALS
#  define TBAG_HAS_GENERIC_LAMBDAS
#  define TBAG_HAS_DIGIT_SEPARATORS
# endif

# if (__COMP_MSVC_VERSION__ >= 1900)
#  define TBAG_HAS_CONSTEXPR // Unknown _MSC_FULL_VER
# endif
#endif // defined(__COMP_MSVC__)
// FEATURE CLOSE.
// ==============

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_MSVC_HPP__

