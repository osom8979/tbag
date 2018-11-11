/**
 * @file   BitHacks.hpp
 * @brief  BitHacks class prototype.
 * @author zer0
 * @date   2018-11-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITHACKS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITHACKS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

template <typename T>
inline bool isPowerOf2(T const & value) TBAG_NOEXCEPT
{
    static_assert(std::is_integral<T>::value, "Only integer type is supported.");
    return ((value & (value - 1)) == 0 && value/*(Ignore 0 number)*/);
}

/**
 * The most significant bit (MSB, also called the high-order bit)
 * is the bit position in a binary number having the greatest value.
 */
template <typename T>
inline int findMostSignificantBit(T value) TBAG_NOEXCEPT
{
    static_assert(std::is_integral<T>::value, "Only integer type is supported.");
    int pos = 0;
    while (value) {
        value >>= 1;
        ++pos;
    }
    return pos;
}

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_BITHACKS_HPP__

