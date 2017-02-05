/**
 * @file   Bitwise.hpp
 * @brief  Bitwise class prototype.
 * @author zer0
 * @date   2017-02-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TMP_BITWISE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TMP_BITWISE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tmp {

template <typename T>
inline T setFlags(T original, T flag, bool enable = true) TBAG_NOEXCEPT
{
    if (enable) {
        return original | flag;
    } else {
        return original | (~flag);
    }
}

template <typename T>
inline bool checkFlags(T original, T check_flag) TBAG_NOEXCEPT
{
    return (original & check_flag) == check_flag;
}

} // namespace tmp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TMP_BITWISE_HPP__

