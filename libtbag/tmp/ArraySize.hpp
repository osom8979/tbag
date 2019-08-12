/**
 * @file   ArraySize.hpp
 * @brief  ArraySize class prototype.
 * @author zer0
 * @date   2019-08-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TMP_ARRAYSIZE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TMP_ARRAYSIZE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstddef>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tmp {

/**
 * The arraysize(arr) macro returns the # of elements in an array arr.
 * The expression is a compile-time constant, and therefore can be used in defining new arrays, for example.
 * If you use arraysize on a pointer by mistake, you will get a compile-time error.
 *
 * This template function declaration is used in defining arraysize.
 * Note that the function doesn't need an implementation, as we only use its type.
 *
 * @see webrtc/src/rtc_base/arraysize.h
 */
template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

} // namespace tmp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#ifndef TBAG_ARRAY_SIZE
#define TBAG_ARRAY_SIZE(array) (sizeof(libtbag::tmp::ArraySizeHelper(array)))
#endif

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TMP_ARRAYSIZE_HPP__

