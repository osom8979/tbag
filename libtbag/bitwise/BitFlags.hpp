/**
 * @file   BitFlags.hpp
 * @brief  BitFlags class prototype.
 * @author zer0
 * @date   2017-04-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITFLAGS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITFLAGS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#ifndef TBAG_BIT_FLAG_CHECK
#define TBAG_BIT_FLAG_CHECK(original, flag) ((bool)((original) & (flag)))
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

template <typename T>
inline T setFlag(T original, T flag, bool enable = true) TBAG_NOEXCEPT
{
    if (enable) {
        original |= flag;
    } else {
        original &= (~flag);
    }
    return original;
}

template <typename T>
inline bool checkFlag(T original, T flag) TBAG_NOEXCEPT
{
    return (original & flag) == flag;
}

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITFLAGS_HPP__

