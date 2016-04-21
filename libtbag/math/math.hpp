/**
 * @file   math.hpp
 * @brief  Representative math header.
 * @author zer0
 * @date   2016-04-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_MATH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_MATH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

#include <cstdlib>
#include <cmath>

#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

constexpr float const DEFAULT_FLOATING_POINT_EQUALS_EPSILON = 0.001;

/**
 * Check the equals for the floating point types.
 */
template <typename T>
bool isFloatingPointEquals(T a, T b, T epsilon = DEFAULT_FLOATING_POINT_EQUALS_EPSILON)
{
    static_assert(std::is_floating_point<T>::value, "Not floating point type.");
    return (std::fabs(a - b) < epsilon);
}

/**
 * Adjust of value range.
 *
 * @warning
 *  No check of DivideByZeroException.
 */
template <typename From, typename To = From, typename Point = float>
To scale(From from_min, From from_max, To to_min, To to_max, From from_value)
{
    // // Don't use this code.
    // if ((from_max - from_min) == 0) {
    //     return 0;
    // }

    Point point = static_cast<Point>(from_value - from_min)
                / static_cast<Point>(from_max   - from_min);

    To result = static_cast<To>(point
                              * static_cast<Point>(to_max - to_min)
                              + static_cast<Point>(to_min));

    if (result < to_min) {
        return to_min;
    } else if (result > to_max) {
        return to_max;
    }
    return result;
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_MATH_HPP__

