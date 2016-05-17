/**
 * @file   cpp.hpp
 * @brief  c++ language config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_CPP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_CPP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__cplusplus)
# define __LANG_STDCPP__
# if (__cplusplus > 0)
#  define __LANG_STDCPP_VERSION__  __cplusplus
# else
#  define __LANG_STDCPP_VERSION__  0
# endif
#endif

#if defined(__LANG_STDCPP_VERSION__)
# if   (__LANG_STDCPP_VERSION__ == 201402L)
#  define __LANG_STDCPP_14__
# elif (__LANG_STDCPP_VERSION__ == 201103L)
#  define __LANG_STDCPP_11__
# endif
#endif // defined(__LANG_STDCPP_VERSION__)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_CPP_HPP__

