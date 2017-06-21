/**
 * @file   Rect.hpp
 * @brief  Rect class prototype.
 * @author zer0
 * @date   2016-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point.hpp>
#include <libtbag/Type.hpp>

#include <cmath>
#include <cstdlib>

#include <algorithm>
#include <sstream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

/**
 * 2-dimensions Rectangle template class.
 *
 * @author zer0
 * @date   2015-06-12
 * @date   2015-08-22 (Move the world library)
 * @date   2015-10-18 (Change to the trivial type)
 * @date   2015-10-21 (Bug fix: assign atomic type)
 * @date   2016-08-24 (Remove BasePoint & BaseSize)
 */
template <typename T>
struct BaseRect
{
    T x, y;
    T width, height;
};

// ------------
// Pre-defined.
// ------------

using Rect = BaseRect<int>;

Rect const EMPTY_RECT = {0, 0, 0, 0};

static_assert(std::is_trivial<Rect>::value, "Rect is not trivial type.");

// ----------------------
// Operation overloading.
// ----------------------

/** The Rect are equal? */
template <typename T>
inline bool
operator ==(BaseRect<T> const & r1, BaseRect<T> const & r2) TBAG_NOEXCEPT
{
    return r1.x == r2.x && r1.y == r2.y && r1.width == r2.width && r1.height == r2.height;
}

template <typename T>
inline bool
operator != (BaseRect<T> const & r1, BaseRect<T> const & r2) TBAG_NOEXCEPT
{
    return !(r1 == r2);
}

template <typename T>
inline BaseRect<T> &
operator +=(BaseRect<T> & r, BasePoint<T> const & p) TBAG_NOEXCEPT
{
    r.x += p.x;
    r.y += p.y;
    return r;
}

template <typename T>
inline BaseRect<T> &
operator -=(BaseRect<T> & r, BasePoint<T> const & p) TBAG_NOEXCEPT
{
    r.x -= p.x;
    r.y -= p.y;
    return r;
}

template <typename T>
inline BaseRect<T> &
operator +=(BaseRect<T> & r, BaseSize<T> const & s) TBAG_NOEXCEPT
{
    r.width  += s.width;
    r.height += s.height;
    return r;
}

template <typename T>
inline BaseRect<T> &
operator -=(BaseRect<T> & r, BaseSize<T> const & s) TBAG_NOEXCEPT
{
    r.width  -= s.width;
    r.height -= s.height;
    return r;
}

template <typename T>
inline BaseRect<T> &
operator &=(BaseRect<T> & r1, BaseRect<T> const & r2) TBAG_NOEXCEPT
{
    T x1 = std::max(r1.x, r2.x);
    T y1 = std::max(r1.y, r2.y);
    r1.width  = std::min(r1.x + r1.width , r2.x + r2.width ) - x1;
    r1.height = std::min(r1.y + r1.height, r2.y + r2.height) - y1;
    r1.x = x1;
    r1.y = y1;
    return r1;
}

template <typename T>
inline BaseRect<T> &
operator |=(BaseRect<T> & r1, BaseRect<T> const & r2) TBAG_NOEXCEPT
{
    T x1 = std::min(r1.x, r2.x);
    T y1 = std::min(r1.y, r2.y);
    r1.width  = std::max(r1.x + r1.width , r2.x + r2.width ) - x1;
    r1.height = std::max(r1.y + r1.height, r2.y + r2.height) - y1;
    r1.x = x1;
    r1.y = y1;
    return r1;
}

template <typename T>
inline BaseRect<T>
operator +(BaseRect<T> const & r, BasePoint<T> const & p) TBAG_NOEXCEPT
{
    return BaseRect<T>{r.x + p.x, r.y + p.y, r.width, r.height};
}

template <typename T>
inline BaseRect<T>
operator -(BaseRect<T> const & r, BasePoint<T> const & p) TBAG_NOEXCEPT
{
    return BaseRect<T>{r.x - p.x, r.y - p.y, r.width, r.height};
}

template <typename T>
inline BaseRect<T>
operator +(BaseRect<T> const & r, BaseSize<T> const & s) TBAG_NOEXCEPT
{
    return BaseRect<T>{r.x, r.y, r.width + s.width, r.height + s.height};
}

template <typename T>
inline BaseRect<T>
operator -(BaseRect<T> const & r, BaseSize<T> const & s) TBAG_NOEXCEPT
{
    return BaseRect<T>{r.x, r.y, r.width - s.width, r.height - s.height};
}

template <typename T>
inline BaseRect<T>
operator &(BaseRect<T> const & r1, BaseRect<T> const & r2) TBAG_NOEXCEPT
{
    BaseRect<T> result = r1;
    return result &= r2;
}

template <typename T>
inline BaseRect<T>
operator |(BaseRect<T> const & r1, BaseRect<T> const & r2) TBAG_NOEXCEPT
{
    BaseRect<T> result = r1;
    return result |= r2;
}

// --------
// Utility.
// --------

template <typename T1, typename T2 = T1, typename T3 = T1, typename T4 = T1>
TBAG_CONSTEXPR BaseRect<typename remove_cr<T1>::type>
makeRect(T1 && x, T2 && y, T3 && width, T4 && height) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T1>::type __remove_cr;
    typedef BaseRect<__remove_cr> __rect_type;
    return __rect_type{std::forward<T1>(x), std::forward<T2>(y), std::forward<T3>(width), std::forward<T4>(height)};
}

template <typename T>
TBAG_CONSTEXPR BaseRect<typename remove_cr<T>::type>
makeRect(BasePoint<T> const & p, BaseSize<T> const & s) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T>::type __remove_cr;
    typedef BaseRect<__remove_cr> __rect_type;
    return __rect_type{p.x, p.y, s.width, s.height};
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getPoint(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return BasePoint<typename remove_cr<T>::type>{r.x, r.y};
}

template <typename T>
inline BaseSize<typename remove_cr<T>::type>
getSize(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return BaseSize<typename remove_cr<T>::type>{r.width, r.height};
}

template <typename T>
inline typename remove_cr<T>::type
getArea(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return r.width * r.height;
}

// ------------------
// Absolute location.
// ------------------

template <typename T>
inline typename remove_cr<T>::type
getLeftTopX(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::min(r.x, r.x + r.width);
}

template <typename T>
inline typename remove_cr<T>::type
getLeftTopY(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::min(r.y, r.y + r.height);
}

template <typename T>
inline typename remove_cr<T>::type
getRightTopX(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::max(r.x, r.x + r.width);
}

template <typename T>
inline typename remove_cr<T>::type
getRightTopY(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::min(r.y, r.y + r.height);
}

template <typename T>
inline typename remove_cr<T>::type
getLeftBottomX(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::min(r.x, r.x + r.width);
}

template <typename T>
inline typename remove_cr<T>::type
getLeftBottomY(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::max(r.y, r.y + r.height);
}

template <typename T>
inline typename remove_cr<T>::type
getRightBottomX(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::max(r.x, r.x + r.width);
}

template <typename T>
inline typename remove_cr<T>::type
getRightBottomY(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    return std::max(r.y, r.y + r.height);
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getLeftTop(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getLeftTopX(r), getLeftTopY(r)};
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getRightTop(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getRightTopX(r), getRightTopY(r)};
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getLeftBottom(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getLeftBottomX(r), getLeftBottomY(r)};
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getRightBottom(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getRightBottomX(r), getRightBottomY(r)};
}

// ----------------
// Offset validate.
// ----------------

template <typename T>
inline bool
checkInside(BaseRect<T> const & base, T const & x, T const & y) TBAG_NOEXCEPT
{
    if (/* * */base.x <= COMPARE_AND(x) <= (base.x + base.width)
            && base.y <= COMPARE_AND(y) <= (base.y + base.height)) {
        return true;
    }
    return false;
}

template <typename T>
inline bool
checkInside(BaseRect<T> const & base, BasePoint<T> const & p) TBAG_NOEXCEPT
{
    return checkInside(base, p.x, p.y);
}

// ------------------------
// Miscellaneous utilities.
// ------------------------

template <typename T>
inline BaseRect<typename remove_cr<T>::type>
getAbsoluteRect(BaseRect<T> const & r) TBAG_NOEXCEPT
{
    typedef typename remove_cr<T>::type __point_tparam;
    __point_tparam x = getLeftTopX(r);
    __point_tparam y = getLeftTopY(r);
    __point_tparam w = getRightBottomX(r) - x;
    __point_tparam h = getRightBottomY(r) - y;
    return makeRect(x, y, w, h);
}

template <typename T>
inline bool
clipRect(BaseRect<T> const & r1, BaseRect<T> const & r2, BaseRect<T> * clip = nullptr) TBAG_NOEXCEPT
{
    T lt_x = std::max(    getLeftTopX(r1),     getLeftTopX(r2));
    T lt_y = std::max(    getLeftTopY(r1),     getLeftTopY(r2));
    T rb_x = std::min(getRightBottomX(r1), getRightBottomX(r2));
    T rb_y = std::min(getRightBottomY(r1), getRightBottomY(r2));

    if (lt_x < rb_x && lt_y < rb_y) {
        if (clip != nullptr) {
            clip->x = lt_x;
            clip->y = lt_y;
            clip->width  = rb_x - lt_x;
            clip->height = rb_y - lt_y;
        }
        return true;
    }
    return false;
}

// -------------------
// String overloading.
// -------------------

char const __RECT_PREFIX_CHAR             = 'R';
char const __RECT_STREAM_BRACE_OPEN       = '{';
char const __RECT_STREAM_VALUE_SEPARATOR  = ',';
char const __RECT_STREAM_BRACE_CLOSE      = '}';

template <typename T>
std::string toString(BaseRect<T> const & rect)
{
    std::stringstream ss;
    ss << __RECT_PREFIX_CHAR << __RECT_STREAM_BRACE_OPEN
       << rect.x << __RECT_STREAM_VALUE_SEPARATOR
       << rect.y << __RECT_STREAM_VALUE_SEPARATOR
       << rect.width  << __RECT_STREAM_VALUE_SEPARATOR
       << rect.height << __RECT_STREAM_BRACE_CLOSE;
    return ss.str();
}

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT_HPP__

