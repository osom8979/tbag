/**
 * @file   SingletonUtils.hpp
 * @brief  SingletonUtils class prototype.
 * @author zer0
 * @date   2016-11-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UTIL_SINGLETONUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UTIL_SINGLETONUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

TBAG_API void  createSingletonObjects();
TBAG_API void releaseSingletonObjects();

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UTIL_SINGLETONUTILS_HPP__

