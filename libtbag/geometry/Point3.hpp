/**
 * @file   Point3.hpp
 * @brief  Point3 class prototype.
 * @author zer0
 * @date   2019-02-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT3_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT3_HPP__

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

template <typename T> struct BasePoint3;
template <typename T> struct BaseSize3;

/**
 * 3-dimensions Point template class.
 *
 * @author zer0
 * @date   2019-02-20
 */
template <typename BaseType>
struct BasePoint3
{
    using Type = BaseType;

    Type x, y, z;

    BasePoint3() : x(), y(), z()
    { /* EMPTY. */ }

    BasePoint3(Type const & x_, Type const & y_, Type const & z_) : x(x_), y(y_), z(z_)
    { /* EMPTY. */ }

    template <typename T>
    BasePoint3(BasePoint3<T> const & obj) : x(obj.x), y(obj.y), z(obj.z)
    { /* EMPTY. */ }

    ~BasePoint3()
    { /* EMPTY. */ }

    template <typename T>
    BasePoint3 & operator =(BasePoint3<T> const & obj)
    {
        if (this != &obj) {
            x = obj.x;
            y = obj.y;
            z = obj.z;
        }
        return *this;
    }

    void swap(BasePoint3 & obj)
    {
        if (this != &obj) {
            std::swap(x, obj.x);
            std::swap(y, obj.y);
            std::swap(z, obj.z);
        }
    }

    friend void swap(BasePoint3 & lh, BasePoint3 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BasePoint3<T>() const
    {
        return BasePoint3<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(z));
    }

    /** The Point are equal? */
    bool operator ==(BasePoint3 const & obj) const
    {
        return x == obj.x && y == obj.y && z == obj.z;
    }

    bool operator !=(BasePoint3 const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BasePoint3 & operator +=(T const & v)
    {
        x += v;
        y += v;
        z += v;
        return *this;
    }

    template <typename T>
    BasePoint3 & operator -=(T const & v)
    {
        x -= v;
        y -= v;
        z -= v;
        return *this;
    }

    template <typename T>
    BasePoint3 & operator *=(T const & v)
    {
        x *= v;
        y *= v;
        z *= v;
        return *this;
    }

    template <typename T>
    BasePoint3 & operator /=(T const & v)
    {
        x /= v;
        y /= v;
        z /= v;
        return *this;
    }

    template <typename T>
    BasePoint3 & operator +=(BasePoint3<T> const & obj)
    {
        x += obj.x;
        y += obj.y;
        z += obj.z;
        return *this;
    }

    template <typename T>
    BasePoint3 & operator -=(BasePoint3<T> const & obj)
    {
        x -= obj.x;
        y -= obj.y;
        z -= obj.z;
        return *this;
    }

    template <typename T>
    BasePoint3 & operator +=(BaseSize3<T> const & obj)
    {
        x += obj.width;
        y += obj.height;
        z += obj.depth;
        return *this;
    }

    template <typename T>
    BasePoint3 & operator -=(BaseSize3<T> const & obj)
    {
        x -= obj.width;
        y -= obj.height;
        z -= obj.depth;
        return *this;
    }

    /** L2 norm */
    double norm() const
    {
        return std::sqrt(((double)x * x) + ((double)y * y) + ((double)z * z));
    }

    /** dot product. */
    template <typename T>
    Type dot(BasePoint3<T> const & p) const
    {
        return (x * p.x) + (y * p.y) + (z * p.z);
    }

    /** dot product computed in double-precision arithmetics. */
    template <typename T>
    double ddot(BasePoint3<T> const & p) const
    {
        return ((double)x * (double)p.x) + ((double)y * (double)p.y) + ((double)z * (double)p.z);
    }

    /** cross-product. */
    BasePoint3 cross(BasePoint3 const & p) const
    {
        return BaseSize3<Type>((y * p.z) - (z * p.y),
                               (z * p.x) - (x * p.z),
                               (x * p.y) - (y * p.x));
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << x << DEFAULT_DELIMITER << y << DEFAULT_DELIMITER << z;
        return ss.str();
    }

    BaseSize3<Type> toSize() const
    {
        return BaseSize3<Type>(x, y, z);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x, y, z};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BasePoint3 const & p)
    {
        os << p.toString();
        return os;
    }
};

template <typename T>
BasePoint3<T> operator +(BasePoint3<T> const & p, T const & v)
{
    BasePoint3<T> result = p;
    result += v;
    return result;
}

template <typename T>
BasePoint3<T> operator -(BasePoint3<T> const & p, T const & v)
{
    BasePoint3<T> result = p;
    result -= v;
    return result;
}

template <typename T>
BasePoint3<T> operator *(BasePoint3<T> const & p, T const & v)
{
    BasePoint3<T> result = p;
    result *= v;
    return result;
}

template <typename T>
BasePoint3<T> operator /(BasePoint3<T> const & p, T const & v)
{
    BasePoint3<T> result = p;
    result /= v;
    return result;
}

template <typename T>
BasePoint3<T> operator +(BasePoint3<T> const & p1, BasePoint3<T> const & p2)
{
    BasePoint3<T> result = p1;
    result += p2;
    return result;
}

template <typename T>
BasePoint3<T> operator -(BasePoint3<T> const & p1, BasePoint3<T> const & p2)
{
    BasePoint3<T> result = p1;
    result -= p2;
    return result;
}

template <typename T>
BasePoint3<T> operator +(BasePoint3<T> const & p, BaseSize3<T> const & s)
{
    BasePoint3<T> result = p;
    result += s;
    return result;
}

template <typename T>
BasePoint3<T> operator -(BasePoint3<T> const & p, BaseSize3<T> const & s)
{
    BasePoint3<T> result = p;
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
struct BaseSize3
{
    using Type = BaseType;

    Type width, height, depth;

    BaseSize3() : width(), height(), depth()
    { /* EMPTY. */ }

    BaseSize3(Type const & w, Type const & h, Type const & d) : width(w), height(h), depth(d)
    { /* EMPTY. */ }

    template <typename T>
    BaseSize3(BaseSize3<T> const & obj) : width(obj.width), height(obj.height), depth(obj.depth)
    { /* EMPTY. */ }

    ~BaseSize3()
    { /* EMPTY. */ }

    inline Type      area() const { return width * height * depth; }
    inline bool     empty() const { return width <= 0 || height <= 0 || depth <= 0; }
    inline BaseSize3  half() const { return BaseSize3(width / 2, height / 2, depth / 2); }

    template <typename T>
    BaseSize3 & operator =(BaseSize3<T> const & obj)
    {
        if (this != &obj) {
            width  = obj.width;
            height = obj.height;
            depth  = obj.depth;
        }
        return *this;
    }

    void swap(BaseSize3 & obj)
    {
        if (this != &obj) {
            std::swap( width, obj.width);
            std::swap(height, obj.height);
            std::swap( depth, obj.depth);
        }
    }

    friend void swap(BaseSize3 & lh, BaseSize3 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseSize3<T>() const
    {
        return BaseSize3<T>(static_cast<T>(width), static_cast<T>(height), static_cast<T>(depth));
    }

    /** The Size are equal? */
    bool operator ==(BaseSize3 const & obj) const
    {
        return width == obj.width && height == obj.height && depth == obj.depth;
    }

    bool operator !=(BaseSize3 const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BaseSize3 & operator +=(T const & v)
    {
        width  += v;
        height += v;
        depth  += v;
        return *this;
    }

    template <typename T>
    BaseSize3 & operator -=(T const & v)
    {
        width  -= v;
        height -= v;
        depth  -= v;
        return *this;
    }

    template <typename T>
    BaseSize3 & operator *=(T const & v)
    {
        width  *= v;
        height *= v;
        depth  *= v;
        return *this;
    }

    template <typename T>
    BaseSize3 & operator /=(T const & v)
    {
        width  /= v;
        height /= v;
        depth  /= v;
        return *this;
    }

    template <typename T>
    BaseSize3 & operator +=(BaseSize3<T> const & obj)
    {
        width  += obj.width;
        height += obj.height;
        depth  += obj.depth;
        return *this;
    }

    template <typename T>
    BaseSize3 & operator -=(BaseSize3<T> const & obj)
    {
        width  -= obj.width;
        height -= obj.height;
        depth  -= obj.depth;
        return *this;
    }

    /**
     * @remarks
     *  You can not add a position at size. @n
     *  A scalar does not have a direction, but vector has a direction. @n
     *  However, this operator is computable.
     */
    template <typename T>
    BaseSize3 & operator +=(BasePoint3<T> const & obj)
    {
        width  += obj.x;
        height += obj.y;
        depth  += obj.z;
        return *this;
    }

    /**
     * @remarks
     *  You can not add a position at size. @n
     *  A scalar does not have a direction, but vector has a direction. @n
     *  However, this operator is computable.
     */
    template <typename T>
    BaseSize3 & operator -=(BasePoint3<T> const & obj)
    {
        width  -= obj.x;
        height -= obj.y;
        depth  -= obj.z;
        return *this;
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << width << DEFAULT_DELIMITER << height << DEFAULT_DELIMITER << depth;
        return ss.str();
    }

    BasePoint3<Type> toPoint() const
    {
        return BasePoint3<Type>(width, height, depth);
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{width, height, depth};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseSize3 const & s)
    {
        os << s.toString();
        return os;
    }
};

template <typename T>
BaseSize3<T> operator +(BaseSize3<T> const & s, T const & v)
{
    BaseSize3<T> result = s;
    result += v;
    return result;
}

template <typename T>
BaseSize3<T> operator -(BaseSize3<T> const & s, T const & v)
{
    BaseSize3<T> result = s;
    result -= v;
    return result;
}

template <typename T>
BaseSize3<T> operator *(BaseSize3<T> const & s, T const & v)
{
    BaseSize3<T> result = s;
    result *= v;
    return result;
}

template <typename T>
BaseSize3<T> operator /(BaseSize3<T> const & s, T const & v)
{
    BaseSize3<T> result = s;
    result /= v;
    return result;
}

template <typename T>
BaseSize3<T> operator +(BaseSize3<T> const & s1, BaseSize3<T> const & s2)
{
    BaseSize3<T> result = s1;
    result += s2;
    return result;
}

template <typename T>
BaseSize3<T> operator -(BaseSize3<T> const & s1, BaseSize3<T> const & s2)
{
    BaseSize3<T> result = s1;
    result -= s2;
    return result;
}

template <typename T>
BaseSize3<T> operator +(BaseSize3<T> const & s, BasePoint3<T> const & p)
{
    BaseSize3<T> result = s;
    result += p;
    return result;
}

template <typename T>
BaseSize3<T> operator -(BaseSize3<T> const & s, BasePoint3<T> const & p)
{
    BaseSize3<T> result = s;
    result -= p;
    return result;
}

// ------------
// Pre-defined.
// ------------

using Point3b  = BasePoint3<bool>;
using Point3c  = BasePoint3<char>;
using Point3s  = BasePoint3<short>;
using Point3i  = BasePoint3<int>;
using Point3u  = BasePoint3<unsigned>;
using Point3l  = BasePoint3<long>;
using Point3ll = BasePoint3<int64_t>;
using Point3f  = BasePoint3<float>;
using Point3d  = BasePoint3<double>;
using Point3   = Point3i;

using Size3b  = BaseSize3<bool>;
using Size3c  = BaseSize3<char>;
using Size3s  = BaseSize3<short>;
using Size3i  = BaseSize3<int>;
using Size3u  = BaseSize3<unsigned>;
using Size3l  = BaseSize3<long>;
using Size3ll = BaseSize3<int64_t>;
using Size3f  = BaseSize3<float>;
using Size3d  = BaseSize3<double>;
using Size3   = Size3i;

Point3 const EMPTY_POINT3(0, 0, 0);
Size3  const EMPTY_SIZE3 (0, 0, 0);

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_POINT3_HPP__

