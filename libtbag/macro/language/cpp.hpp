/**
 * @file   cpp.hpp
 * @brief  c++ language config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LANGUAGE_CPP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LANGUAGE_CPP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__cplusplus)
# define TBAG_LANG_STDCPP
# if (__cplusplus > 0)
#  define TBAG_LANG_STDCPP_VERSION  __cplusplus
# else
#  define TBAG_LANG_STDCPP_VERSION  0
# endif
#endif

#if defined(TBAG_LANG_STDCPP_VERSION)
# if   (TBAG_LANG_STDCPP_VERSION == 201402L)
#  define __LANG_STDCPP_14__
# elif (TBAG_LANG_STDCPP_VERSION == 201103L)
#  define __LANG_STDCPP_11__
# endif
#endif // defined(TBAG_LANG_STDCPP_VERSION)

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LANGUAGE_CPP_HPP__

