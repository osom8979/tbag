/**
 * @file   Number.hpp
 * @brief  Number class prototype.
 * @author zer0
 * @date   2018-01-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_NUMBER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_NUMBER_HPP__

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

namespace math {

/**
 * Make sure it is a multiple of 2.
 *
 * @remarks:
 *  Example) @n
 *   on successful case: @n
 *   <pre>
 *     1000 (8)
 *   & 0111 (8-1)
 *   ------
 *     0000 => is success.
 *   </pre>
 *   @n
 *   on failure case: @n
 *   <pre>
 *     1001 (9)
 *   & 1000 (9-1)
 *   ------
 *     1000 => is failure.
 *   </pre>
 */
template <typename T>
inline bool isMultiplesOfTwo(T value) TBAG_NOEXCEPT
{
    static_assert(std::is_integral<T>::value, "T is only allowed for integers.");
    return !(value & (value - 1));
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_NUMBER_HPP__

