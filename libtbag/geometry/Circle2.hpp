/**
 * @file   Circle2.hpp
 * @brief  Circle2 class prototype.
 * @author zer0
 * @date   2020-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_CIRCLE2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_CIRCLE2_HPP__

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
 * 2-dimensions Circle template class.
 *
 * @author zer0
 * @date   2020-01-19
 */
template <typename BaseType>
struct BaseCircle2
{
    using Type = BaseType;

    Type cx, cy;
    Type r;

    BaseCircle2() : cx(), cy(), r()
    { /* EMPTY. */ }

    BaseCircle2(Type const & cx_, Type const & cy_, Type const & r_)
            : cx(cx_), cy(cy_), r(r_)
    { /* EMPTY. */ }

    BaseCircle2(BasePoint2<Type> const & c, Type const & r_)
            : cx(c.x), cy(c.y), r(r_)
    { /* EMPTY. */ }

    template <typename T>
    BaseCircle2(BaseCircle2<T> const & obj) : cx(obj.cx), cy(obj.cy), r(obj.r)
    { /* EMPTY. */ }

    ~BaseCircle2()
    { /* EMPTY. */ }

    template <typename T>
    BaseCircle2 & operator =(BaseCircle2<T> const & obj)
    {
        if (this != &obj) {
            cx = obj.cx;
            cy = obj.cy;
            r = obj.r;
        }
        return *this;
    }

    void swap(BaseCircle2 & obj)
    {
        if (this != &obj) {
            std::swap(cx, obj.cx);
            std::swap(cy, obj.cy);
            std::swap(r, obj.r);
        }
    }

    friend void swap(BaseCircle2 & lh, BaseCircle2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseCircle2<T>() const
    {
        return BaseCircle2<T>(static_cast<T>(cx), static_cast<T>(cy), static_cast<T>(r));
    }

    bool operator ==(BaseCircle2 const & obj) const
    {
        return cx == obj.cx && cy == obj.cy && r == obj.r;
    }

    bool operator !=(BaseCircle2 const & obj) const
    {
        return !((*this) == obj);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << cx << DEFAULT_DELIMITER << cy << DEFAULT_DELIMITER << r;
        return ss.str();
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{cx, cy, r};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseCircle2 const & p)
    {
        os << p.toString();
        return os;
    }
};

// ------------
// Pre-defined.
// ------------

using Circle2b  = BaseCircle2<bool>;
using Circle2c  = BaseCircle2<char>;
using Circle2s  = BaseCircle2<short>;
using Circle2i  = BaseCircle2<int>;
using Circle2u  = BaseCircle2<unsigned>;
using Circle2l  = BaseCircle2<long>;
using Circle2ll = BaseCircle2<int64_t>;
using Circle2f  = BaseCircle2<float>;
using Circle2d  = BaseCircle2<double>;
using Circle2   = Circle2i;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_CIRCLE2_HPP__

