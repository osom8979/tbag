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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_COMPILER_MSVC_HPP__

