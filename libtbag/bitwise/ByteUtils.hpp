/**
 * @file   ByteUtils.hpp
 * @brief  ByteUtils class prototype.
 * @author zer0
 * @date   2017-04-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BYTEUTILS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BYTEUTILS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cassert>
#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

template <typename T, typename OriginalType = char>
inline void assign(OriginalType * destination, T data) TBAG_NOEXCEPT
{
    assert(destination != nullptr);
    *((T*)(destination)) = data;
}

template <typename T, typename OriginalType = char>
inline void obtain(OriginalType const * source, T * destination) TBAG_NOEXCEPT
{
    assert(source != nullptr);
    if (destination != nullptr) {
        *destination = *((T*)(source));
    }
}

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BYTEUTILS_HPP__

