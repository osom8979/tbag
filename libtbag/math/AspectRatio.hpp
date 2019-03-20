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

#include <cassert>
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
    assert(GCD != 0);
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
    assert(static_cast<ScaleType>(src.width) != 0);
    assert(static_cast<ScaleType>(src.height) != 0);

    auto const SCALE_X = scale_up.width / static_cast<ScaleType>(src.width);
    auto const SCALE_Y = scale_up.height / static_cast<ScaleType>(src.height);

    if (SCALE_X < SCALE_Y) {
        return libtbag::geometry::BaseSize2<T>(src.width * SCALE_X, src.height * SCALE_X);
    } else {
        return libtbag::geometry::BaseSize2<T>(src.width * SCALE_Y, src.height * SCALE_Y);
    }
}

template <typename T>
libtbag::geometry::BaseSize2<T> getSizeOfAspectRatio(T input_width, T input_height, T resize_width)
{
    assert(input_width != 0);
    return libtbag::geometry::BaseSize2<T>(resize_width, (input_height * resize_width) / input_width);
}

template <typename T>
libtbag::geometry::BaseSize2<T> getSizeOfAspectRatio(T input_width, T input_height, T resize_width, T minimum_height)
{
    auto result = getSizeOfAspectRatio<T>(input_width, input_height, resize_width);
    if (minimum_height > 0 && result.height >= minimum_height) {
        assert(input_height != 0);
        result.width = ((input_width * minimum_height) / input_height);
        result.height = minimum_height;
    }
    return result;
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_ASPECTRATIO_HPP__

