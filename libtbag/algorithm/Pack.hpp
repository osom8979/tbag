/**
 * @file   Pack.hpp
 * @brief  Pack class prototype.
 * @author zer0
 * @date   2018-01-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_PACK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_PACK_HPP__

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

namespace algorithm {

/**
 * If packed, check the number of packs.
 *
 * @remarks
 *  Example) @n
 *   - 0 total_elem, 4 pack_elem -> 0
 *   - 1 total_elem, 4 pack_elem -> 1
 *   - 2 total_elem, 4 pack_elem -> 1
 *   - 3 total_elem, 4 pack_elem -> 1
 *   - 4 total_elem, 4 pack_elem -> 1
 *   - 5 total_elem, 4 pack_elem -> 2
 *   - 6 total_elem, 4 pack_elem -> 2
 *   - ...
 */
template <typename T>
inline T getPackedSize(T total_element_size, T packed_element_size)
{
    static_assert(std::is_integral<T>::value, "T is only allowed for integers.");
    return (total_element_size + packed_element_size - 1) / packed_element_size;
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_PACK_HPP__

