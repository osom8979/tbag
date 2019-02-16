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
#include <libtbag/Type.hpp>

#include <cmath>
#include <cstdlib>
#include <cstdint>

#include <algorithm>
#include <sstream>
#include <string>
#include <numeric>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

template <typename T> struct BasePoint;
template <typename T> struct BaseSize;

/**
 * 2-dimensions Point template class.
 *
 * @author zer0
 * @date   2015-06-12
 * @date   2015-08-22 (Move the world library)
 * @date   2015-10-18 (Change to the trivial type)
 * @date   2017-11-02 (Change to the class type)
 */
template <typename BaseType>
struct BasePoint
{
    using Type = BaseType;

    Type x, y;

    BasePoint() : x(), y()
    { /* EMPTY. */ }

    BasePoint(Type const & x_, Type const & y_) : x(x_), y(y_)
    { /* EMPTY. */ }

    template <typename T>
    BasePoint(BasePoint<T> const & obj) : x(obj.x), y(obj.y)
    { /* EMPTY. */ }

    ~BasePoint()
    { /* EMPTY. */ }

    template <typename T>
    BasePoint & operator =(BasePoint<T> const & obj)
    {
        if (this != &obj) {
            x = obj.x;
            y = obj.y;
        }
        return *this;
    }

    void swap(BasePoint & obj)
    {
        if (this != &obj) {
            std::swap(x, obj.x);
            std::swap(y, obj.y);
        }
    }

    friend void swap(BasePoint & lh, BasePoint & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BasePoint<T>() const
    {
        return BasePoint<T>(static_cast<T>(x), static_cast<T>(y));
    }

    /** The Point are equal? */
    bool operator ==(BasePoint const & obj) const
    {
        return x == obj.x && y == obj.y;
    }

    bool operator !=(BasePoint const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BasePoint & operator +=(T const & v)
    {
        x += v;
        y += v;
        return *this;
    }

    template <typename T>
    BasePoint & operator -=(T const & v)
    {
        x -= v;
        y -= v;
        return *this;
    }

    template <typename T>
    BasePoint & operator *=(T const & v)
    {
        x *= v;
        y *= v;
        return *this;
    }

    template <typename T>
    BasePoint & operator /=(T const & v)
    {
        x /= v;
        y /= v;
        return *this;
    }

    template <typename T>
    BasePoint & operator +=(BasePoint<T> const & obj)
    {
        x += obj.x;
        y += obj.y;
        return *this;
    }

    template <typename T>
    BasePoint & operator -=(BasePoint<T> const & obj)
    {
        x -= obj.x;
        y -= obj.y;
        return *this;
    }

    template <typename T>
    BasePoint & operator +=(BaseSize<T> const & obj)
    {
        x += obj.width;
        y += obj.height;
        return *this;
    }

    template <typename T>
    BasePoint & operator -=(BaseSize<T> const & obj)
    {
        x -= obj.width;
        y -= obj.height;
        return *this;
    }

    /** L2 norm */
    double norm() const
    {
        return std::sqrt(((double)x * x) + ((double)y * y));
    }

    /** dot product. */
    template <typename T>
    Type dot(BasePoint<T> const & p) const
    {
        return (x * p.x) + (y * p.y);
    }

    /** dot product computed in double-precision arithmetics. */
    template <typename T>
    double ddot(BasePoint<T> const & p) const
    {
        return ((double)x * (double)p.x) + ((double)y * (double)p.y);
    }

    /** cross-product. */
    template <typename T>
    double cross(BasePoint<T> const & p) const
    {
        return ((double)x * (double)p.y) - ((double)y * (double)p.x);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << x << DEFAULT_DELIMITER << y;
        return ss.str();
    }

    BaseSize<Type> toSize() const
    {
        return BaseSize<Type>(x, y);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x, y};
    }
};

template <typename T>
BasePoint<T> operator +(BasePoint<T> const & p, T const & v)
{
    BasePoint<T> result = p;
    result += v;
    return result;
}

template <typename T>
BasePoint<T> operator -(BasePoint<T> const & p, T const & v)
{
    BasePoint<T> result = p;
    result -= v;
    return result;
}

template <typename T>
BasePoint<T> operator *(BasePoint<T> const & p, T const & v)
{
    BasePoint<T> result = p;
    result *= v;
    return result;
}

template <typename T>
BasePoint<T> operator /(BasePoint<T> const & p, T const & v)
{
    BasePoint<T> result = p;
    result /= v;
    return result;
}

template <typename T>
BasePoint<T> operator +(BasePoint<T> const & p1, BasePoint<T> const & p2)
{
    BasePoint<T> result = p1;
    result += p2;
    return result;
}

template <typename T>
BasePoint<T> operator -(BasePoint<T> const & p1, BasePoint<T> const & p2)
{
    BasePoint<T> result = p1;
    result -= p2;
    return result;
}

template <typename T>
BasePoint<T> operator +(BasePoint<T> const & p, BaseSize<T> const & s)
{
    BasePoint<T> result = p;
    result += s;
    return result;
}

template <typename T>
BasePoint<T> operator -(BasePoint<T> const & p, BaseSize<T> const & s)
{
    BasePoint<T> result = p;
    result -= s;
    return result;
}

/**
 * 2-dimensions Size template class.
 *
 * @author zer0
 * @date   2015-10-21
 * @date   2017-11-02 (Change to the class type)
 */
template <typename BaseType>
struct BaseSize
{
    using Type = BaseType;

    Type width, height;

    BaseSize() : width(), height()
    { /* EMPTY. */ }

    BaseSize(Type const & w, Type const & h) : width(w), height(h)
    { /* EMPTY. */ }

    template <typename T>
    BaseSize(BaseSize<T> const & obj) : width(obj.width), height(obj.height)
    { /* EMPTY. */ }

    ~BaseSize()
    { /* EMPTY. */ }

    inline Type      area() const { return width * height; }
    inline bool     empty() const { return width <= 0 || height <= 0; }
    inline BaseSize  half() const { return BaseSize(width / 2, height / 2); }

    template <typename T>
    BaseSize & operator =(BaseSize<T> const & obj)
    {
        if (this != &obj) {
            width  = obj.width;
            height = obj.height;
        }
        return *this;
    }

    void swap(BaseSize & obj)
    {
        std::swap( width, obj.width);
        std::swap(height, obj.height);
    }

    friend void swap(BaseSize & lh, BaseSize & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseSize<T>() const
    {
        return BaseSize<T>(static_cast<T>(width), static_cast<T>(height));
    }

    /** The Size are equal? */
    bool operator ==(BaseSize const & obj) const
    {
        return width == obj.width && height == obj.height;
    }

    bool operator !=(BaseSize const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BaseSize & operator +=(T const & v)
    {
        width  += v;
        height += v;
        return *this;
    }

    template <typename T>
    BaseSize & operator -=(T const & v)
    {
        width  -= v;
        height -= v;
        return *this;
    }

    template <typename T>
    BaseSize & operator *=(T const & v)
    {
        width  *= v;
        height *= v;
        return *this;
    }

    template <typename T>
    BaseSize & operator /=(T const & v)
    {
        width  /= v;
        height /= v;
        return *this;
    }

    template <typename T>
    BaseSize & operator +=(BaseSize<T> const & obj)
    {
        width  += obj.width;
        height += obj.height;
        return *this;
    }

    template <typename T>
    BaseSize & operator -=(BaseSize<T> const & obj)
    {
        width  -= obj.width;
        height -= obj.height;
        return *this;
    }

    /**
     * @remarks
     *  You can not add a position at size. @n
     *  A scalar does not have a direction, but vector has a direction. @n
     *  However, this operator is computable.
     */
    template <typename T>
    BaseSize & operator +=(BasePoint<T> const & obj)
    {
        width  += obj.x;
        height += obj.y;
        return *this;
    }

    /**
     * @remarks
     *  You can not add a position at size. @n
     *  A scalar does not have a direction, but vector has a direction. @n
     *  However, this operator is computable.
     */
    template <typename T>
    BaseSize & operator -=(BasePoint<T> const & obj)
    {
        width  -= obj.x;
        height -= obj.y;
        return *this;
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << width << DEFAULT_DELIMITER << height;
        return ss.str();
    }

    BasePoint<Type> toPoint() const
    {
        return BasePoint<Type>(width, height);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{width, height};
    }
};

template <typename T>
BaseSize<T> operator +(BaseSize<T> const & s, T const & v)
{
    BaseSize<T> result = s;
    result += v;
    return result;
}

template <typename T>
BaseSize<T> operator -(BaseSize<T> const & s, T const & v)
{
    BaseSize<T> result = s;
    result -= v;
    return result;
}

template <typename T>
BaseSize<T> operator *(BaseSize<T> const & s, T const & v)
{
    BaseSize<T> result = s;
    result *= v;
    return result;
}

template <typename T>
BaseSize<T> operator /(BaseSize<T> const & s, T const & v)
{
    BaseSize<T> result = s;
    result /= v;
    return result;
}

template <typename T>
BaseSize<T> operator +(BaseSize<T> const & s1, BaseSize<T> const & s2)
{
    BaseSize<T> result = s1;
    result += s2;
    return result;
}

template <typename T>
BaseSize<T> operator -(BaseSize<T> const & s1, BaseSize<T> const & s2)
{
    BaseSize<T> result = s1;
    result -= s2;
    return result;
}

template <typename T>
BaseSize<T> operator +(BaseSize<T> const & s, BasePoint<T> const & p)
{
    BaseSize<T> result = s;
    result += p;
    return result;
}

template <typename T>
BaseSize<T> operator -(BaseSize<T> const & s, BasePoint<T> const & p)
{
    BaseSize<T> result = s;
    result -= p;
    return result;
}

// ------------
// Pre-defined.
// ------------

using Pointi = BasePoint<int>;
using Pointu = BasePoint<unsigned>;
using Pointl = BasePoint<int64_t>;
using Pointf = BasePoint<float>;
using Pointd = BasePoint<double>;
using Point  = Pointi;

using Sizei = BaseSize<int>;
using Sizeu = BaseSize<unsigned>;
using Sizel = BaseSize<int64_t>;
using Sizef = BaseSize<float>;
using Sized = BaseSize<double>;
using Size  = Sizei;

Point const EMPTY_POINT(0, 0);
Size  const EMPTY_SIZE (0, 0);

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

// --------------
// Output Stream.
// --------------

#include <ostream>

template <typename T>
inline std::ostream & operator << (std::ostream & os, libtbag::geometry::BasePoint<T> const & point)
{
    os << point.toString();
    return os;
}

template <typename T>
inline std::ostream & operator << (std::ostream & os, libtbag::geometry::BaseSize<T> const & size)
{
    os << size.toString();
    return os;
}

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT_HPP__

