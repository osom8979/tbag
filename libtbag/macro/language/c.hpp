/**
 * @file   c.hpp
 * @brief  c language config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LANGUAGE_C_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LANGUAGE_C_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__STDC__)
# define TBAG_LANG_STDC
# if (__STDC_VERSION__ > 0)
#  define TBAG_LANG_STDC_VERSION  __STDC_VERSION__
# else
#  define TBAG_LANG_STDC_VERSION  0
# endif
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MACRO_LANGUAGE_C_HPP__

