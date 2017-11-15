/**
 * @file   Jaccard.hpp
 * @brief  Jaccard class prototype.
 * @author zer0
 * @date   2017-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_JACCARD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_JACCARD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Rect.hpp>

#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

/**
 * measure similarity between two sample sets.
 *
 * @see <https://en.wikipedia.org/wiki/Jaccard_index>
 */
template <typename T>
inline double jaccardIndex(geometry::BaseRect<T> const & a, geometry::BaseRect<T> const & b)
{
    double const area_a = a.area();
    double const area_b = b.area();

    if ((area_a + area_b) <= std::numeric_limits<T>::epsilon()) {
        return 1.0;
    }

    double const area_ab = (a & b).area();
    return area_ab / (area_a + area_b - area_ab);
}

/**
 * measure dissimilarity between two sample sets.
 *
 * computes the complement of the Jaccard Index as described in <https://en.wikipedia.org/wiki/Jaccard_index>.
 * For rectangles this reduces to computing the intersection over the union.
 */
template <typename T>
inline double jaccardDistance(geometry::BaseRect<T> const & a, geometry::BaseRect<T> const & b)
{
    return 1.0 - jaccardIndex(a, b); // distance = 1 - jaccard_index
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_JACCARD_HPP__

