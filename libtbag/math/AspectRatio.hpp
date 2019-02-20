/**
 * @file   AspectRatio.hpp
 * @brief  AspectRatio class prototype.
 * @author zer0
 * @date   2018-07-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_ASPECTRATIO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_ASPECTRATIO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Size2.hpp>
#include <libtbag/math/Euclidean.hpp>

#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

template <typename T>
void calcAspectRatio(T a, T b, T & a_result, T & b_result)
{
    auto const GCD = gcd(a, b);
    a_result = a / GCD;
    b_result = b / GCD;
}

template <typename T>
std::pair<T, T> calcAspectRatio(T a, T b)
{
    std::pair<T, T> result;
    calcAspectRatio(a, b, result.first, result.second);
    return result;
}

template <typename T, typename ScaleType = typename std::conditional<std::is_floating_point<T>::value, T, double>::type>
libtbag::geometry::BaseSize2<T> scaleUpAspectRatio(libtbag::geometry::BaseSize2<T> const & src,
                                                   libtbag::geometry::BaseSize2<T> const & scale_up)
{
    auto const SCALE_X = scale_up.width / static_cast<ScaleType>(src.width);
    auto const SCALE_Y = scale_up.height / static_cast<ScaleType>(src.height);
    if (SCALE_X < SCALE_Y) {
        return libtbag::geometry::BaseSize2<T>(src.width * SCALE_X, src.height * SCALE_X);
    } else {
        return libtbag::geometry::BaseSize2<T>(src.width * SCALE_Y, src.height * SCALE_Y);
    }
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_ASPECTRATIO_HPP__

