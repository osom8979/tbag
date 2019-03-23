/**
 * @file   GeometryTypes.hpp
 * @brief  GeometryTypes class prototype.
 * @author zer0
 * @date   2019-03-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_GEOMETRYTYPES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_GEOMETRYTYPES_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point2.hpp>
#include <libtbag/geometry/Point3.hpp>
#include <libtbag/geometry/Point4.hpp>
#include <libtbag/geometry/Rect2.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

/**
 * GeometryTypes class prototype.
 *
 * @author zer0
 * @date   2019-03-23
 */
struct GeometryTypes
{
    using Point2b  = libtbag::geometry::Point2b;
    using Point2c  = libtbag::geometry::Point2c;
    using Point2s  = libtbag::geometry::Point2s;
    using Point2i  = libtbag::geometry::Point2i;
    using Point2u  = libtbag::geometry::Point2u;
    using Point2l  = libtbag::geometry::Point2l;
    using Point2ll = libtbag::geometry::Point2ll;
    using Point2f  = libtbag::geometry::Point2f;
    using Point2d  = libtbag::geometry::Point2d;
    using Point2   = libtbag::geometry::Point2;

    using Size2b   = libtbag::geometry::Size2b;
    using Size2c   = libtbag::geometry::Size2c;
    using Size2s   = libtbag::geometry::Size2s;
    using Size2i   = libtbag::geometry::Size2i;
    using Size2u   = libtbag::geometry::Size2u;
    using Size2l   = libtbag::geometry::Size2l;
    using Size2ll  = libtbag::geometry::Size2ll;
    using Size2f   = libtbag::geometry::Size2f;
    using Size2d   = libtbag::geometry::Size2d;
    using Size2    = libtbag::geometry::Size2;

    using Pointb   = libtbag::geometry::Pointb;
    using Pointc   = libtbag::geometry::Pointc;
    using Points   = libtbag::geometry::Points;
    using Pointi   = libtbag::geometry::Pointi;
    using Pointu   = libtbag::geometry::Pointu;
    using Pointl   = libtbag::geometry::Pointl;
    using Pointll  = libtbag::geometry::Pointll;
    using Pointf   = libtbag::geometry::Pointf;
    using Pointd   = libtbag::geometry::Pointd;
    using Point    = libtbag::geometry::Point;

    using Sizeb    = libtbag::geometry::Sizeb;
    using Sizec    = libtbag::geometry::Sizec;
    using Sizes    = libtbag::geometry::Sizes;
    using Sizei    = libtbag::geometry::Sizei;
    using Sizeu    = libtbag::geometry::Sizeu;
    using Sizel    = libtbag::geometry::Sizel;
    using Sizell   = libtbag::geometry::Sizell;
    using Sizef    = libtbag::geometry::Sizef;
    using Sized    = libtbag::geometry::Sized;
    using Size     = libtbag::geometry::Size;

    using Point3b  = libtbag::geometry::Point3b;
    using Point3c  = libtbag::geometry::Point3c;
    using Point3s  = libtbag::geometry::Point3s;
    using Point3i  = libtbag::geometry::Point3i;
    using Point3u  = libtbag::geometry::Point3u;
    using Point3l  = libtbag::geometry::Point3l;
    using Point3ll = libtbag::geometry::Point3ll;
    using Point3f  = libtbag::geometry::Point3f;
    using Point3d  = libtbag::geometry::Point3d;
    using Point3   = libtbag::geometry::Point3;

    using Size3b   = libtbag::geometry::Size3b;
    using Size3c   = libtbag::geometry::Size3c;
    using Size3s   = libtbag::geometry::Size3s;
    using Size3i   = libtbag::geometry::Size3i;
    using Size3u   = libtbag::geometry::Size3u;
    using Size3l   = libtbag::geometry::Size3l;
    using Size3ll  = libtbag::geometry::Size3ll;
    using Size3f   = libtbag::geometry::Size3f;
    using Size3d   = libtbag::geometry::Size3d;
    using Size3    = libtbag::geometry::Size3;

    using Point4b  = libtbag::geometry::Point4b;
    using Point4c  = libtbag::geometry::Point4c;
    using Point4s  = libtbag::geometry::Point4s;
    using Point4i  = libtbag::geometry::Point4i;
    using Point4u  = libtbag::geometry::Point4u;
    using Point4l  = libtbag::geometry::Point4l;
    using Point4ll = libtbag::geometry::Point4ll;
    using Point4f  = libtbag::geometry::Point4f;
    using Point4d  = libtbag::geometry::Point4d;
    using Point4   = libtbag::geometry::Point4;

    using Size4b   = libtbag::geometry::Size4b;
    using Size4c   = libtbag::geometry::Size4c;
    using Size4s   = libtbag::geometry::Size4s;
    using Size4i   = libtbag::geometry::Size4i;
    using Size4u   = libtbag::geometry::Size4u;
    using Size4l   = libtbag::geometry::Size4l;
    using Size4ll  = libtbag::geometry::Size4ll;
    using Size4f   = libtbag::geometry::Size4f;
    using Size4d   = libtbag::geometry::Size4d;
    using Size4    = libtbag::geometry::Size4;

    using Rect2b   = libtbag::geometry::Rect2b;
    using Rect2c   = libtbag::geometry::Rect2c;
    using Rect2s   = libtbag::geometry::Rect2s;
    using Rect2i   = libtbag::geometry::Rect2i;
    using Rect2u   = libtbag::geometry::Rect2u;
    using Rect2l   = libtbag::geometry::Rect2l;
    using Rect2ll  = libtbag::geometry::Rect2ll;
    using Rect2f   = libtbag::geometry::Rect2f;
    using Rect2d   = libtbag::geometry::Rect2d;
    using Rect2    = libtbag::geometry::Rect2;

    using Rectb    = libtbag::geometry::Rectb;
    using Rectc    = libtbag::geometry::Rectc;
    using Rects    = libtbag::geometry::Rects;
    using Recti    = libtbag::geometry::Recti;
    using Rectu    = libtbag::geometry::Rectu;
    using Rectl    = libtbag::geometry::Rectl;
    using Rectll   = libtbag::geometry::Rectll;
    using Rectf    = libtbag::geometry::Rectf;
    using Rectd    = libtbag::geometry::Rectd;
    using Rect     = libtbag::geometry::Rect;
};

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_GEOMETRYTYPES_HPP__

