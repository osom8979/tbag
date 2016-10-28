/**
 * @file   Real.hpp
 * @brief  Real number utilities.
 * @author zer0
 * @date   2016-04-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_REAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_REAL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cstdlib>
#include <cmath>

#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

TBAG_CONSTEXPR float const DEFAULT_FLOATING_POINT_EQUALS_EPSILON = 0.001;

/**
 * Check the equals for the floating point types.
 */
template <typename T>
bool isFloatingPointEquals(T a, T b, T epsilon = DEFAULT_FLOATING_POINT_EQUALS_EPSILON)
{
    static_assert(std::is_floating_point<T>::value, "Not floating point type.");
    return (std::fabs(a - b) < epsilon);
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_REAL_HPP__

