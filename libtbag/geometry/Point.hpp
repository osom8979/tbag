/**
 * @file   Point.hpp
 * @brief  Point class prototype.
 * @author zer0
 * @date   2016-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/type.hpp>

#include <algorithm>
#include <sstream>
#include <string>
#include <numeric>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

/**
 * 2-dimensions Point template class.
 *
 * @author zer0
 * @date   2015-06-12
 * @date   2015-08-22 (Move the world library)
 * @date   2015-10-18 (Change to the trivial type)
 */
template <typename T>
struct BasePoint
{
    T x, y;
};

/**
 * 2-dimensions Size template class.
 *
 * @author zer0
 * @date   2015-10-21
 */
template <typename T>
struct BaseSize
{
    T w, h;
};

// ------------
// Pre-defined.
// ------------

using Point = BasePoint<int>;
using Size  = BaseSize <int>;

Point const EMPTY_POINT = {0, 0};
Size  const EMPTY_SIZE  = {0, 0};

static_assert(std::is_trivial<Point>::value, "Point is not trivial type.");
static_assert(std::is_trivial< Size>::value,  "Size is not trivial type.");

// ------------------------------------
// Operation overloading for the Point.
// ------------------------------------

template <typename T>
inline BasePoint<T> &
operator +=(BasePoint<T> & p1, BasePoint<T> const & p2) noexcept
{
    p1.x += p2.x;
    p1.y += p2.y;
    return p1;
}

template <typename T>
inline BasePoint<T> &
operator -=(BasePoint<T> & p1, BasePoint<T> const & p2) noexcept
{
    p1.x -= p2.x;
    p1.y -= p2.y;
    return p1;
}

template <typename T>
inline BasePoint<T>
operator +(BasePoint<T> const & p1, BasePoint<T> const & p2) noexcept
{
    return BasePoint<T>{p1.x + p2.x, p1.y + p2.y};
}

template <typename T>
inline BasePoint<T>
operator -(BasePoint<T> const & p1, BasePoint<T> const & p2) noexcept
{
    return BasePoint<T>{p1.x - p2.x, p1.y - p2.y};
}

template <typename T>
inline BasePoint<T> &
operator +=(BasePoint<T> & p, BaseSize<T> const & s) noexcept
{
    p.x += s.w;
    p.y += s.h;
    return p;
}

template <typename T>
inline BasePoint<T> &
operator -=(BasePoint<T> & p, BaseSize<T> const & s) noexcept
{
    p.x -= s.w;
    p.y -= s.h;
    return p;
}

template <typename T>
inline BasePoint<T>
operator +(BasePoint<T> const & p, BaseSize<T> const & s) noexcept
{
    return BasePoint<T>{p.x + s.w, p.y + s.h};
}

template <typename T>
inline BasePoint<T>
operator -(BasePoint<T> const & p, BaseSize<T> const & s) noexcept
{
    return BasePoint<T>{p.x - s.w, p.y - s.h};
}

// -----------------------------------
// Operation overloading for the Size.
// -----------------------------------

template <typename T>
inline BaseSize<T> &
operator +=(BaseSize<T> & s1, BaseSize<T> const & s2) noexcept
{
    s1.w += s2.w;
    s1.h += s2.h;
    return s1;
}

template <typename T>
inline BaseSize<T> &
operator -=(BaseSize<T> & s1, BaseSize<T> const & s2) noexcept
{
    s1.w -= s2.w;
    s1.h -= s2.h;
    return s1;
}

template <typename T>
inline BaseSize<T>
operator +(BaseSize<T> const & s1, BaseSize<T> const & s2) noexcept
{
    return BaseSize<T>{s1.w + s2.w, s1.h + s2.h};
}

template <typename T>
inline BaseSize<T>
operator -(BaseSize<T> const & s1, BaseSize<T> const & s2) noexcept
{
    return BaseSize<T>{s1.w - s2.w, s1.h - s2.h};
}

/**
 * You can not add a position at size.
 *
 * @remarks
 *  A scalar does not have a direction, but vector has a direction.
 */
template <typename T>
inline BaseSize<T>
operator +(BaseSize<T> const & s, BasePoint<T> const & p) noexcept = delete;

/**
 * You can not subtract a position at size.
 *
 * @remarks
 *  A scalar does not have a direction, but vector has a direction.
 */
template <typename T>
inline BaseSize<T>
operator -(BaseSize<T> const & s, BasePoint<T> const & p) noexcept = delete;

// -----------------------------
// Equals Operation overloading.
// -----------------------------

/** The Point are equal? */
template <typename T>
inline bool
operator ==(BasePoint<T> const & p1, BasePoint<T> const & p2) noexcept
{
    if (p1.x != p2.x || p1.y != p2.y) {
        return false;
    }
    return true;
}

template <typename T>
inline bool
operator !=(BasePoint<T> const & p1, BasePoint<T> const & p2) noexcept
{
    return !(p1 == p2);
}

/** The Size are equal? */
template <typename T>
inline bool
operator ==(BaseSize<T> const & s1, BaseSize<T> const & s2) noexcept
{
    if (s1.w != s2.w || s1.h != s2.h) {
        return false;
    }
    return true;
}

template <typename T>
inline bool
operator !=(BaseSize<T> const & s1, BaseSize<T> const & s2) noexcept
{
    return !(s1 == s2);
}

// -------------------
// String overloading.
// -------------------

char const __POINT_PREFIX_CHAR = 'P';
char const  __SIZE_PREFIX_CHAR = 'S';

char const __POINT_STREAM_BRACE_OPEN       = '{';
char const __POINT_STREAM_VALUE_SEPARATOR  = ',';
char const __POINT_STREAM_BRACE_CLOSE      = '}';

template <typename T>
std::string toString(BasePoint<T> const & point)
{
    std::stringstream ss;
    ss << __POINT_PREFIX_CHAR << __POINT_STREAM_BRACE_OPEN
       << point.x << __POINT_STREAM_VALUE_SEPARATOR
       << point.y << __POINT_STREAM_BRACE_CLOSE;
    return ss.str();
}

template <typename T>
std::string toString(BaseSize<T> const & size)
{
    std::stringstream ss;
    ss << __SIZE_PREFIX_CHAR << __POINT_STREAM_BRACE_OPEN
       << size.w << __POINT_STREAM_VALUE_SEPARATOR
       << size.h << __POINT_STREAM_BRACE_CLOSE;
    return ss.str();
}

// -----------------
// Create structure.
// -----------------

template <typename T1, typename T2 = T1>
constexpr BasePoint<typename remove_cr<T1>::type>
makePoint(T1 && x, T2 && y) noexcept
{
    return makeTrivial2<BasePoint, T1, T2>(std::forward<T1>(x), std::forward<T2>(y));
}

template <typename T1, typename T2 = T1>
constexpr BaseSize<typename remove_cr<T1>::type>
makeSize(T1 && x, T2 && y) noexcept
{
    return makeTrivial2<BaseSize, T1, T2>(std::forward<T1>(x), std::forward<T2>(y));
}

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT_HPP__

