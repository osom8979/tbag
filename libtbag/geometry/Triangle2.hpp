/**
 * @file   Triangle2.hpp
 * @brief  Triangle2 class prototype.
 * @author zer0
 * @date   2020-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_TRIANGLE2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_TRIANGLE2_HPP__

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
 * 2-dimensions Triangle template class.
 *
 * @author zer0
 * @date   2020-01-19
 */
template <typename BaseType>
struct BaseTriangle2
{
    using Type = BaseType;

    Type x0, y0;
    Type x1, y1;
    Type x2, y2;

    BaseTriangle2() : x0(), y0(), x1(), y1(), x2(), y2()
    { /* EMPTY. */ }

    BaseTriangle2(Type const & x0_, Type const & y0_,
                  Type const & x1_, Type const & y1_,
                  Type const & x2_, Type const & y2_)
            : x0(x0_), y0(y0_), x1(x1_), y1(y1_), x2(x2_), y2(y2_)
    { /* EMPTY. */ }

    BaseTriangle2(BasePoint2<Type> const & p0,
                  BasePoint2<Type> const & p1,
                  BasePoint2<Type> const & p2)
            : x0(p0.x), y0(p0.y), x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y)
    { /* EMPTY. */ }

    template <typename T>
    BaseTriangle2(BaseTriangle2<T> const & obj)
            : x0(obj.x0), y0(obj.y0),
              x1(obj.x1), y1(obj.y1),
              x2(obj.x2), y2(obj.y2)
    { /* EMPTY. */ }

    ~BaseTriangle2()
    { /* EMPTY. */ }

    template <typename T>
    BaseTriangle2 & operator =(BaseTriangle2<T> const & obj)
    {
        if (this != &obj) {
            x0 = obj.x0;
            y0 = obj.y0;
            x1 = obj.x1;
            y1 = obj.y1;
            x2 = obj.x2;
            y2 = obj.y2;
        }
        return *this;
    }

    void swap(BaseTriangle2 & obj)
    {
        if (this != &obj) {
            std::swap(x0, obj.x0);
            std::swap(y0, obj.y0);
            std::swap(x1, obj.x1);
            std::swap(y1, obj.y1);
            std::swap(x2, obj.x2);
            std::swap(y2, obj.y2);
        }
    }

    friend void swap(BaseTriangle2 & lh, BaseTriangle2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseTriangle2<T>() const
    {
        return BaseTriangle2<T>(static_cast<T>(x0), static_cast<T>(y0),
                                static_cast<T>(x1), static_cast<T>(y1),
                                static_cast<T>(x2), static_cast<T>(y2));
    }

    bool operator ==(BaseTriangle2 const & obj) const
    {
        return x0 == obj.x0 && y0 == obj.y0 &&
               x1 == obj.x1 && y1 == obj.y1 &&
               x2 == obj.x2 && y2 == obj.y2;
    }

    bool operator !=(BaseTriangle2 const & obj) const
    {
        return !((*this) == obj);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER1 = 'x';
    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER2 = '/';

    std::string toString() const
    {
        std::stringstream ss;
        ss << x0 << DEFAULT_DELIMITER1 << y0 << DEFAULT_DELIMITER2
           << x1 << DEFAULT_DELIMITER1 << y1 << DEFAULT_DELIMITER2
           << x2 << DEFAULT_DELIMITER1 << y2;
        return ss.str();
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x0, y0, x1, y1, x2, y2};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseTriangle2 const & p)
    {
        os << p.toString();
        return os;
    }
};

// ------------
// Pre-defined.
// ------------

using Triangle2b  = BaseTriangle2<bool>;
using Triangle2c  = BaseTriangle2<char>;
using Triangle2s  = BaseTriangle2<short>;
using Triangle2i  = BaseTriangle2<int>;
using Triangle2u  = BaseTriangle2<unsigned>;
using Triangle2l  = BaseTriangle2<long>;
using Triangle2ll = BaseTriangle2<int64_t>;
using Triangle2f  = BaseTriangle2<float>;
using Triangle2d  = BaseTriangle2<double>;
using Triangle2   = Triangle2i;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_TRIANGLE2_HPP__

