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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_MSVC_HPP__

