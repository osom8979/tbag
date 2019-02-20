/**
 * @file   BoundingBox.hpp
 * @brief  BoundingBox utility prototype.
 * @author zer0
 * @date   2017-09-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_BOUNDINGBOX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_BOUNDINGBOX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point2.hpp>
#include <libtbag/geometry/Rect2.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

template <typename T>
BaseRect2<typename remove_cr<T>::type>
getBoundingBox(BasePoint2<T> const * points, std::size_t size) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T>::type __point_tparam;
    if (points == nullptr || size <= 0) {
        return BaseRect2<__point_tparam>(0, 0, 0, 0);
    }

    BasePoint2<__point_tparam> lt = *points;
    BasePoint2<__point_tparam> rd = *points;

    for (std::size_t i = 1; i < size; ++i) {
        // @formatter:off
        if ((points + i)->x < lt.x) { lt.x = (points + i)->x; }
        if ((points + i)->y < lt.y) { lt.y = (points + i)->y; }
        if ((points + i)->x > rd.x) { rd.x = (points + i)->x; }
        if ((points + i)->y > rd.y) { rd.y = (points + i)->y; }
        // @formatter:on
    }

    return BaseRect2<__point_tparam>(lt, rd);
}

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_BOUNDINGBOX_HPP__

