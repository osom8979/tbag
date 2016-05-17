/**
 * @file   objc.hpp
 * @brief  objective-c language config.
 * @author zer0
 * @date   2016-05-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_OBJC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_OBJC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(__OBJC__)
# define __LANG_OBJC__
# define __LANG_OBJC_VERSION__  0
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONFIG_LANGUAGE_OBJC_HPP__

