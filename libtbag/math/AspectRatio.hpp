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
#include <libtbag/geometry/Size.hpp>
#include <libtbag/math/Euclidean.hpp>

#include <utility>

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

template <typename T>
geometry::BaseSize<T> scaleUpAspectRatio(geometry::BaseSize<T> const & src,
                                         geometry::BaseSize<T> const & scale_up)
{
    auto const SCALE_X = (double)scale_up.width  / (double)src.width;
    auto const SCALE_Y = (double)scale_up.height / (double)src.height;
    if (SCALE_X < SCALE_Y) {
        return {src.width * SCALE_X, src.height * SCALE_X};
    } else {
        return {src.width * SCALE_Y, src.height * SCALE_Y};
    }
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_ASPECTRATIO_HPP__

