/**
 * @file   Euclidean.hpp
 * @brief  Euclidean class prototype.
 * @author zer0
 * @date   2018-07-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_EUCLIDEAN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_EUCLIDEAN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cassert>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

/**
 * Greatest common divisor.
 *
 * @remarks
 *  <code>gcd(a,b) = gcd(a,r)</code>
 *
 * @see <https://en.wikipedia.org/wiki/Euclidean_algorithm>
 * @see <https://en.wikipedia.org/wiki/Greatest_common_divisor>
 *
 * @translate{ko, 최대공약수를 계산한다.}
 */
template <typename T>
T gcd(T a, T b)
{
    static_assert(std::is_integral<T>::value, "T must be an integer.");

    if (a == b) {
        return a;
    } else if (a < b) {
        return gcd(b, a);
    }

    assert(a > b);

    int r;
    while (b) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/**
 * Least common multiple.
 *
 * @remarks
 *  <code>lcm(a,b)gcd(a,b) = ab</code>
 *
 * @see <https://en.wikipedia.org/wiki/Least_common_multiple>
 *
 * @translate{ko, 최소공배수를 계산한다.}
 */
template <typename T>
T lcm(T a, T b)
{
    static_assert(std::is_integral<T>::value, "T must be an integer.");
    return a * b / gcd(a, b);
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_EUCLIDEAN_HPP__

