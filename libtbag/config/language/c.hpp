/**
 * @file   c.hpp
 * @brief  c language config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_C_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_C_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__STDC__)
# define __LANG_STDC__
# if (__STDC_VERSION__ > 0)
#  define __LANG_STDC_VERSION__  __STDC_VERSION__
# else
#  define __LANG_STDC_VERSION__  0
# endif
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_C_HPP__

