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
#include <libtbag/geometry/Point.hpp>
#include <libtbag/type.hpp>

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
 */
template <typename T>
struct BaseRect
{
    BasePoint<T> point;
    BaseSize<T>  size;
};

// ------------
// Pre-defined.
// ------------

using Rect = BaseRect<int>;

Rect const EMPTY_RECT = { {0, 0}, {0, 0} };

static_assert(std::is_trivial<Rect>::value, "Rect is not trivial type.");

// -----------------------------
// Equals Operation overloading.
// -----------------------------

/** The Rect are equal? */
template <typename T>
inline bool
operator == (BaseRect<T> const & r1, BaseRect<T> const & r2) noexcept
{
    if (/**/r1.point.x != r2.point.x
         || r1.point.y != r2.point.y
         || r1.size.w  != r2.size.w
         || r1.size.h  != r2.size.h) {
        return false;
    }
    return true;
}

template <typename T>
inline bool
operator != (BaseRect<T> const & r1, BaseRect<T> const & r2) noexcept
{
    return !(r1 == r2);
}

// --------
// Utility.
// --------

template <typename T1, typename T2 = T1, typename T3 = T1, typename T4 = T1>
constexpr BaseRect<typename remove_cr<T1>::type>
makeRect(T1 && x, T2 && y, T3 && w, T4 && h) noexcept
{
    typedef typename remove_cr<T1>::type __remove_cr;
    typedef BaseRect<__remove_cr> __rect_type;
    return (__rect_type{ {std::forward<T1>(x), std::forward<T2>(y)}
                       , {std::forward<T3>(w), std::forward<T4>(h)} });
}

template <typename T>
constexpr BaseRect<typename remove_cr<T>::type>
makeRect(BasePoint<T> const & p, BaseSize<T> const & s) noexcept
{
    typedef typename remove_cr<T>::type __remove_cr;
    typedef BaseRect<__remove_cr> __rect_type;
    return (__rect_type{ {p.x, p.y}, {s.w, s.h} });
}

// ------------------
// Absolute location.
// ------------------

template <typename T>
inline typename remove_cr<T>::type
getLeftTopX(BaseRect<T> const & r) noexcept
{
    return std::min(r.point.x, r.point.x + r.size.w);
}

template <typename T>
inline typename remove_cr<T>::type
getLeftTopY(BaseRect<T> const & r) noexcept
{
    return std::min(r.point.y, r.point.y + r.size.h);
}

template <typename T>
inline typename remove_cr<T>::type
getRightTopX(BaseRect<T> const & r) noexcept
{
    return std::max(r.point.x, r.point.x + r.size.w);
}

template <typename T>
inline typename remove_cr<T>::type
getRightTopY(BaseRect<T> const & r) noexcept
{
    return std::min(r.point.y, r.point.y + r.size.h);
}

template <typename T>
inline typename remove_cr<T>::type
getLeftBottomX(BaseRect<T> const & r) noexcept
{
    return std::min(r.point.x, r.point.x + r.size.w);
}

template <typename T>
inline typename remove_cr<T>::type
getLeftBottomY(BaseRect<T> const & r) noexcept
{
    return std::max(r.point.y, r.point.y + r.size.h);
}

template <typename T>
inline typename remove_cr<T>::type
getRightBottomX(BaseRect<T> const & r) noexcept
{
    return std::max(r.point.x, r.point.x + r.size.w);
}

template <typename T>
inline typename remove_cr<T>::type
getRightBottomY(BaseRect<T> const & r) noexcept
{
    return std::max(r.point.y, r.point.y + r.size.h);
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getLeftTop(BaseRect<T> const & r) noexcept
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getLeftTopX(r), getLeftTopY(r)};
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getRightTop(BaseRect<T> const & r) noexcept
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getRightTopX(r), getRightTopY(r)};
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getLeftBottom(BaseRect<T> const & r) noexcept
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getLeftBottomX(r), getLeftBottomY(r)};
}

template <typename T>
inline BasePoint<typename remove_cr<T>::type>
getRightBottom(BaseRect<T> const & r) noexcept
{
    typedef typename remove_cr<T>::type __point_tparam;
    return BasePoint<__point_tparam>{getRightBottomX(r), getRightBottomY(r)};
}

// ----------------
// Offset validate.
// ----------------

template <typename T>
inline bool
checkInside(BaseRect<T> const & base, T const & x, T const & y) noexcept
{
    if (/**/base.point.x <= COMPARE_AND(x) <= (base.point.x + base.size.w)
         && base.point.y <= COMPARE_AND(y) <= (base.point.y + base.size.h)) {
        return true;
    }
    return false;
}

template <typename T>
inline bool
checkInside(BaseRect<T> const & base, BasePoint<T> const & p) noexcept
{
    return checkInside(base, p.x, p.y);
}

// ---------
// Clipping.
// ---------

template <typename T>
inline BaseRect<typename remove_cr<T>::type>
clipRect(BaseRect<T> const & r1, BaseRect<T> const & r2) noexcept
{
    typedef typename remove_cr<T>::type __remove_cr;
    typedef BaseRect<__remove_cr> __rect_type;
    __rect_type result;
    result.point.x = std::max(getLeftTopX(r1), getLeftTopX(r2));
    result.point.y = std::max(getLeftTopY(r1), getLeftTopY(r2));
    result.size.w = std::min(getRightBottomX(r1), getRightBottomX(r2)) - result.point.x;
    result.size.h = std::min(getRightBottomY(r1), getRightBottomY(r2)) - result.point.y;
    return result;
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
       << rect.point.x << __RECT_STREAM_VALUE_SEPARATOR
       << rect.point.y << __RECT_STREAM_VALUE_SEPARATOR
       << rect.size.w  << __RECT_STREAM_VALUE_SEPARATOR
       << rect.size.h  << __RECT_STREAM_BRACE_CLOSE;
    return ss.str();
}

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT_HPP__

