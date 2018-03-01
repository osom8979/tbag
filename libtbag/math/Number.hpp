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
#include <cmath>

#ifndef TBAG_INT_HALF
#define TBAG_INT_HALF(x)   ((x)>>1)
#endif

#ifndef TBAG_INT_DOUBLE
#define TBAG_INT_DOUBLE(x) ((x)<<1)
#endif

// Integer division and modulo operations are particularly costly
// and should be avoided or replaced with bitwise operations whenever possible:
// If n is a power of 2, ( i / n ) is equivalent to ( i >> log2(n) )
// and ( i % n ) is equivalent to ( i & n - 1 ).

#ifndef TBAG_INT_DIVISION
#define TBAG_INT_DIVISION(x, n/*Power of 2*/) ((x)>>log2(n))
#endif

/**
 * @def TBAG_INT_MODULO
 * @see <https://en.wikipedia.org/wiki/Remainder>
 */
#ifndef TBAG_INT_MODULO
#define TBAG_INT_MODULO(x, n/*Power of 2*/) (((x)&(n))-1)
#endif

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

