/**
 * @file   Clamp.hpp
 * @brief  Clamp class prototype.
 * @author zer0
 * @date   2017-07-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_CLAMP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_CLAMP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace algorithm {

template <typename T>
inline T clamp(T value, T min, T max) TBAG_NOEXCEPT
{
    assert(max >= min);
    if (value > max) {
        value = max;
    } else if (value < min) {
        value = min;
    }
    return value;
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_CLAMP_HPP__

