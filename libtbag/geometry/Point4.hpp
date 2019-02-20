/**
 * @file   Point4.hpp
 * @brief  Point4 class prototype.
 * @author zer0
 * @date   2019-02-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT4_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT4_HPP__

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

template <typename T> struct BasePoint4;
template <typename T> struct BaseSize4;

/**
 * 4-dimensions Point template class.
 *
 * @author zer0
 * @date   2019-02-20
 */
template <typename BaseType>
struct BasePoint4
{
    using Type = BaseType;

    Type x, y, z, w;

    BasePoint4() : x(), y(), z(), w()
    { /* EMPTY. */ }

    BasePoint4(Type const & x_, Type const & y_, Type const & z_ ,Type const & w_) : x(x_), y(y_), z(z_), w(w_)
    { /* EMPTY. */ }

    template <typename T>
    BasePoint4(BasePoint4<T> const & obj) : x(obj.x), y(obj.y), z(obj.z), w(obj.w)
    { /* EMPTY. */ }

    ~BasePoint4()
    { /* EMPTY. */ }

    template <typename T>
    BasePoint4 & operator =(BasePoint4<T> const & obj)
    {
        if (this != &obj) {
            x = obj.x;
            y = obj.y;
            z = obj.z;
            w = obj.w;
        }
        return *this;
    }

    void swap(BasePoint4 & obj)
    {
        if (this != &obj) {
            std::swap(x, obj.x);
            std::swap(y, obj.y);
            std::swap(z, obj.z);
            std::swap(w, obj.w);
        }
    }

    friend void swap(BasePoint4 & lh, BasePoint4 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BasePoint4<T>() const
    {
        return BasePoint4<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z), static_cast<T>(w));
    }

    /** The Point are equal? */
    bool operator ==(BasePoint4 const & obj) const
    {
        return x == obj.x && y == obj.y && z == obj.z && w == obj.w;
    }

    bool operator !=(BasePoint4 const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BasePoint4 & operator +=(T const & v)
    {
        x += v;
        y += v;
        z += v;
        w += v;
        return *this;
    }

    template <typename T>
    BasePoint4 & operator -=(T const & v)
    {
        x -= v;
        y -= v;
        z -= v;
        w -= v;
        return *this;
    }

    template <typename T>
    BasePoint4 & operator *=(T const & v)
    {
        x *= v;
        y *= v;
        z *= v;
        w *= v;
        return *this;
    }

    template <typename T>
    BasePoint4 & operator /=(T const & v)
    {
        x /= v;
        y /= v;
        z /= v;
        w /= v;
        return *this;
    }

    template <typename T>
    BasePoint4 & operator +=(BasePoint4<T> const & obj)
    {
        x += obj.x;
        y += obj.y;
        z += obj.z;
        w += obj.w;
        return *this;
    }

    template <typename T>
    BasePoint4 & operator -=(BasePoint4<T> const & obj)
    {
        x -= obj.x;
        y -= obj.y;
        z -= obj.z;
        w -= obj.w;
        return *this;
    }

    template <typename T>
    BasePoint4 & operator +=(BaseSize4<T> const & obj)
    {
        x += obj.width;
        y += obj.height;
        z += obj.depth;
        w += obj.length;
        return *this;
    }

    template <typename T>
    BasePoint4 & operator -=(BaseSize4<T> const & obj)
    {
        x -= obj.width;
        y -= obj.height;
        z -= obj.depth;
        w -= obj.length;
        return *this;
    }

    /** L2 norm */
    double norm() const
    {
        return std::sqrt(((double)x * x) + ((double)y * y) + ((double)z * z) + ((double)w * w));
    }

    /** dot product. */
    template <typename T>
    Type dot(BasePoint4<T> const & p) const
    {
        return (x * p.x) + (y * p.y) + (z * p.z) + (w * p.w);
    }

    /** dot product computed in double-precision arithmetics. */
    template <typename T>
    double ddot(BasePoint4<T> const & p) const
    {
        return ((double)x * (double)p.x) + ((double)y * (double)p.y) +
               ((double)z * (double)p.z) + ((double)w * (double)p.w);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << x << DEFAULT_DELIMITER << y << DEFAULT_DELIMITER << z << DEFAULT_DELIMITER << w;
        return ss.str();
    }

    BaseSize4<Type> toSize() const
    {
        return BaseSize4<Type>(x, y, z, w);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x, y, z, w};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BasePoint4 const & p)
    {
        os << p.toString();
        return os;
    }
};

template <typename T>
BasePoint4<T> operator +(BasePoint4<T> const & p, T const & v)
{
    BasePoint4<T> result = p;
    result += v;
    return result;
}

template <typename T>
BasePoint4<T> operator -(BasePoint4<T> const & p, T const & v)
{
    BasePoint4<T> result = p;
    result -= v;
    return result;
}

template <typename T>
BasePoint4<T> operator *(BasePoint4<T> const & p, T const & v)
{
    BasePoint4<T> result = p;
    result *= v;
    return result;
}

template <typename T>
BasePoint4<T> operator /(BasePoint4<T> const & p, T const & v)
{
    BasePoint4<T> result = p;
    result /= v;
    return result;
}

template <typename T>
BasePoint4<T> operator +(BasePoint4<T> const & p1, BasePoint4<T> const & p2)
{
    BasePoint4<T> result = p1;
    result += p2;
    return result;
}

template <typename T>
BasePoint4<T> operator -(BasePoint4<T> const & p1, BasePoint4<T> const & p2)
{
    BasePoint4<T> result = p1;
    result -= p2;
    return result;
}

template <typename T>
BasePoint4<T> operator +(BasePoint4<T> const & p, BaseSize4<T> const & s)
{
    BasePoint4<T> result = p;
    result += s;
    return result;
}

template <typename T>
BasePoint4<T> operator -(BasePoint4<T> const & p, BaseSize4<T> const & s)
{
    BasePoint4<T> result = p;
    result -= s;
    return result;
}

/**
 * 3-dimensions Size template class.
 *
 * @author zer0
 * @date   2019-02-20
 */
template <typename BaseType>
struct BaseSize4
{
    using Type = BaseType;

    Type width, height, depth, length;

    BaseSize4() : width(), height(), depth(), length()
    { /* EMPTY. */ }

    BaseSize4(Type const & w, Type const & h, Type const & d, Type const & l) : width(w), height(h), depth(d), length(l)
    { /* EMPTY. */ }

    template <typename T>
    BaseSize4(BaseSize4<T> const & obj) : width(obj.width), height(obj.height), depth(obj.depth), length(obj.length)
    { /* EMPTY. */ }

    ~BaseSize4()
    { /* EMPTY. */ }

    inline Type      area() const { return width * height * depth * length; }
    inline bool     empty() const { return width <= 0 || height <= 0 || depth <= 0 || length <= 0; }
    inline BaseSize4  half() const { return BaseSize4(width / 2, height / 2, depth / 2, length / 2); }

    template <typename T>
    BaseSize4 & operator =(BaseSize4<T> const & obj)
    {
        if (this != &obj) {
            width  = obj.width;
            height = obj.height;
            depth  = obj.depth;
            length = obj.length;
        }
        return *this;
    }

    void swap(BaseSize4 & obj)
    {
        if (this != &obj) {
            std::swap( width, obj.width);
            std::swap(height, obj.height);
            std::swap( depth, obj.depth);
            std::swap(length, obj.length);
        }
    }

    friend void swap(BaseSize4 & lh, BaseSize4 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseSize4<T>() const
    {
        return BaseSize4<T>(static_cast<T>(width), static_cast<T>(height), static_cast<T>(depth), static_cast<T>(length));
    }

    /** The Size are equal? */
    bool operator ==(BaseSize4 const & obj) const
    {
        return width == obj.width && height == obj.height && depth == obj.depth && length == obj.length;
    }

    bool operator !=(BaseSize4 const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BaseSize4 & operator +=(T const & v)
    {
        width  += v;
        height += v;
        depth  += v;
        length += v;
        return *this;
    }

    template <typename T>
    BaseSize4 & operator -=(T const & v)
    {
        width  -= v;
        height -= v;
        depth  -= v;
        length -= v;
        return *this;
    }

    template <typename T>
    BaseSize4 & operator *=(T const & v)
    {
        width  *= v;
        height *= v;
        depth  *= v;
        length *= v;
        return *this;
    }

    template <typename T>
    BaseSize4 & operator /=(T const & v)
    {
        width  /= v;
        height /= v;
        depth  /= v;
        length /= v;
        return *this;
    }

    template <typename T>
    BaseSize4 & operator +=(BaseSize4<T> const & obj)
    {
        width  += obj.width;
        height += obj.height;
        depth  += obj.depth;
        length += obj.length;
        return *this;
    }

    template <typename T>
    BaseSize4 & operator -=(BaseSize4<T> const & obj)
    {
        width  -= obj.width;
        height -= obj.height;
        depth  -= obj.depth;
        length -= obj.length;
        return *this;
    }

    /**
     * @remarks
     *  You can not add a position at size. @n
     *  A scalar does not have a direction, but vector has a direction. @n
     *  However, this operator is computable.
     */
    template <typename T>
    BaseSize4 & operator +=(BasePoint4<T> const & obj)
    {
        width  += obj.x;
        height += obj.y;
        depth  += obj.z;
        length += obj.w;
        return *this;
    }

    /**
     * @remarks
     *  You can not add a position at size. @n
     *  A scalar does not have a direction, but vector has a direction. @n
     *  However, this operator is computable.
     */
    template <typename T>
    BaseSize4 & operator -=(BasePoint4<T> const & obj)
    {
        width  -= obj.x;
        height -= obj.y;
        depth  -= obj.z;
        length -= obj.w;
        return *this;
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << width << DEFAULT_DELIMITER << height << DEFAULT_DELIMITER << depth << DEFAULT_DELIMITER << length;
        return ss.str();
    }

    BasePoint4<Type> toPoint() const
    {
        return BasePoint4<Type>(width, height, depth, length);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{width, height, depth, length};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseSize4 const & s)
    {
        os << s.toString();
        return os;
    }
};

template <typename T>
BaseSize4<T> operator +(BaseSize4<T> const & s, T const & v)
{
    BaseSize4<T> result = s;
    result += v;
    return result;
}

template <typename T>
BaseSize4<T> operator -(BaseSize4<T> const & s, T const & v)
{
    BaseSize4<T> result = s;
    result -= v;
    return result;
}

template <typename T>
BaseSize4<T> operator *(BaseSize4<T> const & s, T const & v)
{
    BaseSize4<T> result = s;
    result *= v;
    return result;
}

template <typename T>
BaseSize4<T> operator /(BaseSize4<T> const & s, T const & v)
{
    BaseSize4<T> result = s;
    result /= v;
    return result;
}

template <typename T>
BaseSize4<T> operator +(BaseSize4<T> const & s1, BaseSize4<T> const & s2)
{
    BaseSize4<T> result = s1;
    result += s2;
    return result;
}

template <typename T>
BaseSize4<T> operator -(BaseSize4<T> const & s1, BaseSize4<T> const & s2)
{
    BaseSize4<T> result = s1;
    result -= s2;
    return result;
}

template <typename T>
BaseSize4<T> operator +(BaseSize4<T> const & s, BasePoint4<T> const & p)
{
    BaseSize4<T> result = s;
    result += p;
    return result;
}

template <typename T>
BaseSize4<T> operator -(BaseSize4<T> const & s, BasePoint4<T> const & p)
{
    BaseSize4<T> result = s;
    result -= p;
    return result;
}

// ------------
// Pre-defined.
// ------------

using Point4b  = BasePoint4<bool>;
using Point4c  = BasePoint4<char>;
using Point4s  = BasePoint4<short>;
using Point4i  = BasePoint4<int>;
using Point4u  = BasePoint4<unsigned>;
using Point4l  = BasePoint4<long>;
using Point4ll = BasePoint4<int64_t>;
using Point4f  = BasePoint4<float>;
using Point4d  = BasePoint4<double>;
using Point4   = Point3i;

using Size4b  = BaseSize4<bool>;
using Size4c  = BaseSize4<char>;
using Size4s  = BaseSize4<short>;
using Size4i  = BaseSize4<int>;
using Size4u  = BaseSize4<unsigned>;
using Size4l  = BaseSize4<long>;
using Size4ll = BaseSize4<int64_t>;
using Size4f  = BaseSize4<float>;
using Size4d  = BaseSize4<double>;
using Size4   = Size3i;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT4_HPP__

