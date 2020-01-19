/**
 * @file   Ellipse2.hpp
 * @brief  Ellipse2 class prototype.
 * @author zer0
 * @date   2020-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ELLIPSE2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ELLIPSE2_HPP__

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
 * 2-dimensions Ellipse template class.
 *
 * @author zer0
 * @date   2020-01-19
 */
template <typename BaseType>
struct BaseEllipse2
{
    using Type = BaseType;

    Type cx, cy;
    Type rx, ry;

    BaseEllipse2() : cx(), cy(), rx(), ry()
    { /* EMPTY. */ }

    BaseEllipse2(Type const & cx_, Type const & cy_, Type const & rx_, Type const & ry_)
            : cx(cx_), cy(cy_), rx(rx_), ry(ry_)
    { /* EMPTY. */ }

    BaseEllipse2(BasePoint2<Type> const & c, BasePoint2<Type> const & r)
            : cx(c.x), cy(c.y), rx(r.x), ry(r.y)
    { /* EMPTY. */ }

    template <typename T>
    BaseEllipse2(BaseEllipse2<T> const & obj) : cx(obj.cx), cy(obj.cy), rx(obj.rx), ry(obj.ry)
    { /* EMPTY. */ }

    ~BaseEllipse2()
    { /* EMPTY. */ }

    template <typename T>
    BaseEllipse2 & operator =(BaseEllipse2<T> const & obj)
    {
        if (this != &obj) {
            cx = obj.cx;
            cy = obj.cy;
            rx = obj.rx;
            ry = obj.ry;
        }
        return *this;
    }

    void swap(BaseEllipse2 & obj)
    {
        if (this != &obj) {
            std::swap(cx, obj.cx);
            std::swap(cy, obj.cy);
            std::swap(rx, obj.rx);
            std::swap(ry, obj.ry);
        }
    }

    friend void swap(BaseEllipse2 & lh, BaseEllipse2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseEllipse2<T>() const
    {
        return BaseEllipse2<T>(static_cast<T>(cx), static_cast<T>(cy),
                               static_cast<T>(rx), static_cast<T>(ry));
    }

    bool operator ==(BaseEllipse2 const & obj) const
    {
        return cx == obj.cx && cy == obj.cy && rx == obj.rx && ry == obj.ry;
    }

    bool operator !=(BaseEllipse2 const & obj) const
    {
        return !((*this) == obj);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER1 = 'x';
    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER2 = '/';

    std::string toString() const
    {
        std::stringstream ss;
        ss << cx << DEFAULT_DELIMITER1 << cy << DEFAULT_DELIMITER2
           << rx << DEFAULT_DELIMITER1 << ry;
        return ss.str();
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{cx, cy, rx, ry};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseEllipse2 const & p)
    {
        os << p.toString();
        return os;
    }
};

// ------------
// Pre-defined.
// ------------

using Ellipse2b  = BaseEllipse2<bool>;
using Ellipse2c  = BaseEllipse2<char>;
using Ellipse2s  = BaseEllipse2<short>;
using Ellipse2i  = BaseEllipse2<int>;
using Ellipse2u  = BaseEllipse2<unsigned>;
using Ellipse2l  = BaseEllipse2<long>;
using Ellipse2ll = BaseEllipse2<int64_t>;
using Ellipse2f  = BaseEllipse2<float>;
using Ellipse2d  = BaseEllipse2<double>;
using Ellipse2   = Ellipse2i;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ELLIPSE2_HPP__

