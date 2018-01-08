/**
 * @file   Align.hpp
 * @brief  Align class prototype.
 * @author zer0
 * @date   2018-01-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_ALIGN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_ALIGN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace algorithm {

template <typename T>
inline T getAlignLength(T size, T unit)
{
    return (size + unit - 1) / unit;
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_ALIGN_HPP__

