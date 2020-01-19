/**
 * @file   RoundRect2.hpp
 * @brief  RoundRect2 class prototype.
 * @author zer0
 * @date   2020-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ROUNDRECT2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ROUNDRECT2_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point2.hpp>

#include <cstdint>
#include <algorithm>
#include <sstream>
#include <ostream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

/**
 * 2-dimensions RoundRect template class.
 *
 * @author zer0
 * @date   2020-01-19
 */
template <typename BaseType>
struct BaseRoundRect2
{
    using Type = BaseType;

    Type x, y;
    Type width, height;
    Type rx, ry;

    BaseRoundRect2() : x(), y(), width(), height(), rx(), ry()
    { /* EMPTY. */ }

    BaseRoundRect2(Type const & x_, Type const & y_,
                   Type const & w_, Type const & h_,
                   Type const & rx_, Type const & ry_)
            : x(x_), y(y_), width(w_), height(h_), rx(rx_), ry(ry_)
    { /* EMPTY. */ }

    template <typename T>
    BaseRoundRect2(BasePoint2<T> const & p, BaseSize2<T> const & s, BasePoint2<T> const & r)
            : x(p.x), y(p.y), width(s.width), height(s.height), rx(r.x), ry(r.y)
    { /* EMPTY. */ }

    template <typename T>
    BaseRoundRect2(BasePoint2<T> const & lt, BasePoint2<T> const & rb, BasePoint2<T> const & r)
            : x(lt.x), y(lt.y),
              width(rb.x - lt.x), height(rb.y - lt.y),
              rx(r.x), ry(r.y)
    { /* EMPTY. */ }

    template <typename T>
    BaseRoundRect2(BaseRoundRect2<T> const & obj)
            : x(obj.x), y(obj.y),
              width(obj.width), height(obj.height),
              rx(obj.rx), ry(obj.ry)
    { /* EMPTY. */ }

    ~BaseRoundRect2()
    { /* EMPTY. */ }

    inline BasePoint2<Type> point () const { return BasePoint2<Type>(x, y); }
    inline BasePoint2<Type> point1() const { return point(); }
    inline BasePoint2<Type> point2() const { return BasePoint2<Type>(x + width, y + height); }
    inline BasePoint2<Type> center() const { return BasePoint2<Type>(x + (width * 0.5), y + (height * 0.5)); }
    inline BasePoint2<Type> round () const { return BasePoint2<Type>(rx, ry); }

    inline BaseSize2<Type> size() const { return BaseSize2<Type>(width, height); }
    inline BaseSize2<Type> half() const { return BaseSize2<Type>(width * 0.5, height  * 0.5); }

    inline Type  area() const { return width * height; }
    inline bool empty() const { return width <= 0 || height <= 0; }

    template <typename T>
    BaseRoundRect2 & operator =(BaseRoundRect2<T> const & obj)
    {
        if (this != &obj) {
            x = obj.x;
            y = obj.y;
            width  = obj.width;
            height = obj.height;
            rx = obj.rx;
            ry = obj.ry;
        }
        return *this;
    }

    void swap(BaseRoundRect2 & obj)
    {
        if (this != &obj) {
            std::swap(x, obj.x);
            std::swap(y, obj.y);
            std::swap(width, obj.width);
            std::swap(height, obj.height);
            std::swap(rx, obj.rx);
            std::swap(ry, obj.ry);
        }
    }

    friend void swap(BaseRoundRect2 & lh, BaseRoundRect2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseRoundRect2<T>() const
    {
        return BaseRoundRect2<T>(static_cast<T>(x), static_cast<T>(y),
                                 static_cast<T>(width), static_cast<T>(height),
                                 static_cast<T>(rx), static_cast<T>(ry));
    }

    template <typename T>
    bool operator ==(BaseRoundRect2<T> const & obj) const
    {
        return x == obj.x && y == obj.y &&
               width == obj.width && height == obj.height &&
               rx == obj.rx && ry == obj.ry;
    }

    template <typename T>
    bool operator !=(BaseRoundRect2<T> const & obj) const
    {
        return !((*this) == obj);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER1 = 'x';
    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER2 = '/';

    std::string toString() const
    {
        std::stringstream ss;
        ss << x << DEFAULT_DELIMITER1 << y << DEFAULT_DELIMITER1
           << width << DEFAULT_DELIMITER1 << height << DEFAULT_DELIMITER2
           << rx << DEFAULT_DELIMITER1 << ry;
        return ss.str();
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x, y, width, height, rx, ry};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseRoundRect2 const & p)
    {
        os << p.toString();
        return os;
    }
};

// ------------
// Pre-defined.
// ------------

using RoundRect2b  = BaseRoundRect2<bool>;
using RoundRect2c  = BaseRoundRect2<char>;
using RoundRect2s  = BaseRoundRect2<short>;
using RoundRect2i  = BaseRoundRect2<int>;
using RoundRect2u  = BaseRoundRect2<unsigned>;
using RoundRect2l  = BaseRoundRect2<long>;
using RoundRect2ll = BaseRoundRect2<int64_t>;
using RoundRect2f  = BaseRoundRect2<float>;
using RoundRect2d  = BaseRoundRect2<double>;
using RoundRect2   = RoundRect2i;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ROUNDRECT2_HPP__

