/**
 * @file   Point2.hpp
 * @brief  Point2 class prototype.
 * @author zer0
 * @date   2016-04-20
 * @date   2019-02-20 (Rename: Point -> Point2)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT2_HPP__

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

#include <ostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <numeric>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

template <typename T> struct BasePoint2;
template <typename T> struct BaseSize2;

/**
 * 2-dimensions Point template class.
 *
 * @author zer0
 * @date   2015-06-12
 * @date   2015-08-22 (Move the world library)
 * @date   2015-10-18 (Change to the trivial type)
 * @date   2017-11-02 (Change to the class type)
 * @date   2019-02-20 (Rename: BasePoint -> BasePoint2)
 */
template <typename BaseType>
struct BasePoint2
{
    using Type = BaseType;

    Type x, y;

    BasePoint2() : x(), y()
    { /* EMPTY. */ }

    BasePoint2(Type const & x_, Type const & y_) : x(x_), y(y_)
    { /* EMPTY. */ }

    template <typename T>
    BasePoint2(BasePoint2<T> const & obj) : x(obj.x), y(obj.y)
    { /* EMPTY. */ }

    ~BasePoint2()
    { /* EMPTY. */ }

    template <typename T>
    BasePoint2 & operator =(BasePoint2<T> const & obj)
    {
        if (this != &obj) {
            x = obj.x;
            y = obj.y;
        }
        return *this;
    }

    void swap(BasePoint2 & obj)
    {
        if (this != &obj) {
            std::swap(x, obj.x);
            std::swap(y, obj.y);
        }
    }

    friend void swap(BasePoint2 & lh, BasePoint2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BasePoint2<T>() const
    {
        return BasePoint2<T>(static_cast<T>(x), static_cast<T>(y));
    }

    /** The Point are equal? */
    bool operator ==(BasePoint2 const & obj) const
    {
        return x == obj.x && y == obj.y;
    }

    bool operator !=(BasePoint2 const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BasePoint2 & operator +=(T const & v)
    {
        x += v;
        y += v;
        return *this;
    }

    template <typename T>
    BasePoint2 & operator -=(T const & v)
    {
        x -= v;
        y -= v;
        return *this;
    }

    template <typename T>
    BasePoint2 & operator *=(T const & v)
    {
        x *= v;
        y *= v;
        return *this;
    }

    template <typename T>
    BasePoint2 & operator /=(T const & v)
    {
        x /= v;
        y /= v;
        return *this;
    }

    template <typename T>
    BasePoint2 & operator +=(BasePoint2<T> const & obj)
    {
        x += obj.x;
        y += obj.y;
        return *this;
    }

    template <typename T>
    BasePoint2 & operator -=(BasePoint2<T> const & obj)
    {
        x -= obj.x;
        y -= obj.y;
        return *this;
    }

    template <typename T>
    BasePoint2 & operator +=(BaseSize2<T> const & obj)
    {
        x += obj.width;
        y += obj.height;
        return *this;
    }

    template <typename T>
    BasePoint2 & operator -=(BaseSize2<T> const & obj)
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
    Type dot(BasePoint2<T> const & p) const
    {
        return (x * p.x) + (y * p.y);
    }

    /** dot product computed in double-precision arithmetics. */
    template <typename T>
    double ddot(BasePoint2<T> const & p) const
    {
        return ((double)x * (double)p.x) + ((double)y * (double)p.y);
    }

    /** cross-product. */
    template <typename T>
    double cross(BasePoint2<T> const & p) const
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

    BaseSize2<Type> toSize() const
    {
        return BaseSize2<Type>(x, y);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x, y};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BasePoint2 const & p)
    {
        os << p.toString();
        return os;
    }
};

template <typename T>
BasePoint2<T> operator +(BasePoint2<T> const & p, T const & v)
{
    BasePoint2<T> result = p;
    result += v;
    return result;
}

template <typename T>
BasePoint2<T> operator -(BasePoint2<T> const & p, T const & v)
{
    BasePoint2<T> result = p;
    result -= v;
    return result;
}

template <typename T>
BasePoint2<T> operator *(BasePoint2<T> const & p, T const & v)
{
    BasePoint2<T> result = p;
    result *= v;
    return result;
}

template <typename T>
BasePoint2<T> operator /(BasePoint2<T> const & p, T const & v)
{
    BasePoint2<T> result = p;
    result /= v;
    return result;
}

template <typename T>
BasePoint2<T> operator +(BasePoint2<T> const & p1, BasePoint2<T> const & p2)
{
    BasePoint2<T> result = p1;
    result += p2;
    return result;
}

template <typename T>
BasePoint2<T> operator -(BasePoint2<T> const & p1, BasePoint2<T> const & p2)
{
    BasePoint2<T> result = p1;
    result -= p2;
    return result;
}

template <typename T>
BasePoint2<T> operator +(BasePoint2<T> const & p, BaseSize2<T> const & s)
{
    BasePoint2<T> result = p;
    result += s;
    return result;
}

template <typename T>
BasePoint2<T> operator -(BasePoint2<T> const & p, BaseSize2<T> const & s)
{
    BasePoint2<T> result = p;
    result -= s;
    return result;
}

/**
 * 2-dimensions Size template class.
 *
 * @author zer0
 * @date   2015-10-21
 * @date   2017-11-02 (Change to the class type)
 * @date   2019-02-20 (Rename: BaseSize -> BaseSize2)
 */
template <typename BaseType>
struct BaseSize2
{
    using Type = BaseType;

    Type width, height;

    BaseSize2() : width(), height()
    { /* EMPTY. */ }

    BaseSize2(Type const & w, Type const & h) : width(w), height(h)
    { /* EMPTY. */ }

    template <typename T>
    BaseSize2(BaseSize2<T> const & obj) : width(obj.width), height(obj.height)
    { /* EMPTY. */ }

    ~BaseSize2()
    { /* EMPTY. */ }

    inline Type       area() const { return width * height; }
    inline bool      empty() const { return width <= 0 || height <= 0; }
    inline BaseSize2  half() const { return BaseSize2(width / 2, height / 2); }

    template <typename T>
    BaseSize2 & operator =(BaseSize2<T> const & obj)
    {
        if (this != &obj) {
            width  = obj.width;
            height = obj.height;
        }
        return *this;
    }

    void swap(BaseSize2 & obj)
    {
        if (this != &obj) {
            std::swap( width, obj.width);
            std::swap(height, obj.height);
        }
    }

    friend void swap(BaseSize2 & lh, BaseSize2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseSize2<T>() const
    {
        return BaseSize2<T>(static_cast<T>(width), static_cast<T>(height));
    }

    /** The Size are equal? */
    bool operator ==(BaseSize2 const & obj) const
    {
        return width == obj.width && height == obj.height;
    }

    bool operator !=(BaseSize2 const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BaseSize2 & operator +=(T const & v)
    {
        width  += v;
        height += v;
        return *this;
    }

    template <typename T>
    BaseSize2 & operator -=(T const & v)
    {
        width  -= v;
        height -= v;
        return *this;
    }

    template <typename T>
    BaseSize2 & operator *=(T const & v)
    {
        width  *= v;
        height *= v;
        return *this;
    }

    template <typename T>
    BaseSize2 & operator /=(T const & v)
    {
        width  /= v;
        height /= v;
        return *this;
    }

    template <typename T>
    BaseSize2 & operator +=(BaseSize2<T> const & obj)
    {
        width  += obj.width;
        height += obj.height;
        return *this;
    }

    template <typename T>
    BaseSize2 & operator -=(BaseSize2<T> const & obj)
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
    BaseSize2 & operator +=(BasePoint2<T> const & obj)
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
    BaseSize2 & operator -=(BasePoint2<T> const & obj)
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

    BasePoint2<Type> toPoint() const
    {
        return BasePoint2<Type>(width, height);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{width, height};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseSize2 const & s)
    {
        os << s.toString();
        return os;
    }
};

template <typename T>
BaseSize2<T> operator +(BaseSize2<T> const & s, T const & v)
{
    BaseSize2<T> result = s;
    result += v;
    return result;
}

template <typename T>
BaseSize2<T> operator -(BaseSize2<T> const & s, T const & v)
{
    BaseSize2<T> result = s;
    result -= v;
    return result;
}

template <typename T>
BaseSize2<T> operator *(BaseSize2<T> const & s, T const & v)
{
    BaseSize2<T> result = s;
    result *= v;
    return result;
}

template <typename T>
BaseSize2<T> operator /(BaseSize2<T> const & s, T const & v)
{
    BaseSize2<T> result = s;
    result /= v;
    return result;
}

template <typename T>
BaseSize2<T> operator +(BaseSize2<T> const & s1, BaseSize2<T> const & s2)
{
    BaseSize2<T> result = s1;
    result += s2;
    return result;
}

template <typename T>
BaseSize2<T> operator -(BaseSize2<T> const & s1, BaseSize2<T> const & s2)
{
    BaseSize2<T> result = s1;
    result -= s2;
    return result;
}

template <typename T>
BaseSize2<T> operator +(BaseSize2<T> const & s, BasePoint2<T> const & p)
{
    BaseSize2<T> result = s;
    result += p;
    return result;
}

template <typename T>
BaseSize2<T> operator -(BaseSize2<T> const & s, BasePoint2<T> const & p)
{
    BaseSize2<T> result = s;
    result -= p;
    return result;
}

// ------------
// Pre-defined.
// ------------

using Point2b  = BasePoint2<bool>;
using Point2c  = BasePoint2<char>;
using Point2s  = BasePoint2<short>;
using Point2i  = BasePoint2<int>;
using Point2u  = BasePoint2<unsigned>;
using Point2l  = BasePoint2<long>;
using Point2ll = BasePoint2<int64_t>;
using Point2f  = BasePoint2<float>;
using Point2d  = BasePoint2<double>;
using Point2   = Point2i;

using Size2b  = BaseSize2<bool>;
using Size2c  = BaseSize2<char>;
using Size2s  = BaseSize2<short>;
using Size2i  = BaseSize2<int>;
using Size2u  = BaseSize2<unsigned>;
using Size2l  = BaseSize2<long>;
using Size2ll = BaseSize2<int64_t>;
using Size2f  = BaseSize2<float>;
using Size2d  = BaseSize2<double>;
using Size2   = Size2i;

using Pointb  = Point2b;
using Pointc  = Point2c;
using Points  = Point2s;
using Pointi  = Point2i;
using Pointu  = Point2u;
using Pointl  = Point2l;
using Pointll = Point2ll;
using Pointf  = Point2f;
using Pointd  = Point2d;
using Point   = Point2;

using Sizeb  = Size2b;
using Sizec  = Size2c;
using Sizes  = Size2s;
using Sizei  = Size2i;
using Sizeu  = Size2u;
using Sizel  = Size2l;
using Sizell = Size2ll;
using Sizef  = Size2f;
using Sized  = Size2d;
using Size   = Size2;

Point const EMPTY_POINT(0, 0);
Size  const EMPTY_SIZE (0, 0);

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT2_HPP__

