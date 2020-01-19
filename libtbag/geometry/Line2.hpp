/**
 * @file   Line2.hpp
 * @brief  Line2 class prototype.
 * @author zer0
 * @date   2020-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_LINE2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_LINE2_HPP__

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
 * 2-dimensions Line template class.
 *
 * @author zer0
 * @date   2020-01-19
 */
template <typename BaseType>
struct BaseLine2
{
    using Type = BaseType;

    Type x0, y0;
    Type x1, y1;

    BaseLine2() : x0(), y0(), x1(), y1()
    { /* EMPTY. */ }

    BaseLine2(Type const & x0_, Type const & y0_, Type const & x1_, Type const & y1_)
            : x0(x0_), y0(y0_), x1(x1_), y1(y1_)
    { /* EMPTY. */ }

    BaseLine2(BasePoint2<Type> const & p0, BasePoint2<Type> const & p1)
            : x0(p0.x), y0(p0.y), x1(p1.x), y1(p1.y)
    { /* EMPTY. */ }

    template <typename T>
    BaseLine2(BaseLine2<T> const & obj) : x0(obj.x0), y0(obj.y0), x1(obj.x1), y1(obj.y1)
    { /* EMPTY. */ }

    ~BaseLine2()
    { /* EMPTY. */ }

    template <typename T>
    BaseLine2 & operator =(BaseLine2<T> const & obj)
    {
        if (this != &obj) {
            x0 = obj.x0;
            y0 = obj.y0;
            x1 = obj.x1;
            y1 = obj.y1;
        }
        return *this;
    }

    void swap(BaseLine2 & obj)
    {
        if (this != &obj) {
            std::swap(x0, obj.x0);
            std::swap(y0, obj.y0);
            std::swap(x1, obj.x1);
            std::swap(y1, obj.y1);
        }
    }

    friend void swap(BaseLine2 & lh, BaseLine2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseLine2<T>() const
    {
        return BaseLine2<T>(static_cast<T>(x0), static_cast<T>(y0),
                            static_cast<T>(x1), static_cast<T>(y1));
    }

    bool operator ==(BaseLine2 const & obj) const
    {
        return x0 == obj.x0 && y0 == obj.y0 && x1 == obj.x1 && y1 == obj.y1;
    }

    bool operator !=(BaseLine2 const & obj) const
    {
        return !((*this) == obj);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER1 = 'x';
    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER2 = '/';

    std::string toString() const
    {
        std::stringstream ss;
        ss << x0 << DEFAULT_DELIMITER1 << y0 << DEFAULT_DELIMITER2
           << x1 << DEFAULT_DELIMITER1 << y1;
        return ss.str();
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x0, y0, x1, y1};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseLine2 const & p)
    {
        os << p.toString();
        return os;
    }
};

// ------------
// Pre-defined.
// ------------

using Line2b  = BaseLine2<bool>;
using Line2c  = BaseLine2<char>;
using Line2s  = BaseLine2<short>;
using Line2i  = BaseLine2<int>;
using Line2u  = BaseLine2<unsigned>;
using Line2l  = BaseLine2<long>;
using Line2ll = BaseLine2<int64_t>;
using Line2f  = BaseLine2<float>;
using Line2d  = BaseLine2<double>;
using Line2   = Line2i;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_LINE2_HPP__

