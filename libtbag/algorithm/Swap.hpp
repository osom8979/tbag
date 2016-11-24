/**
 * @file   Swap.hpp
 * @brief  Swap algorithm prototype.
 * @author zer0
 * @date   2016-04-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_SWAP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_SWAP_HPP__

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
 * swap POD type.
 *
 * @translate{ko, @c std::swap 을 사용하는 것이 최적화에 유리하다.}
 *
 * @warning
 *  @c std::swap is more advantageous for optimization.
 */
template <typename T>
inline void swapWithPod(T & a, T & b) TBAG_NOEXCEPT
{
    static_assert(std::is_pod<T>::value, "Not POD type.");
    // @formatter:off
    a ^= b; b ^= a; a ^= b;
    // @formatter:on
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_SWAP_HPP__

