/**
 * @file   Arc2.hpp
 * @brief  Arc2 class prototype.
 * @author zer0
 * @date   2020-01-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ARC2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ARC2_HPP__

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
 * 2-dimensions Arc template class.
 *
 * @author zer0
 * @date   2020-01-19
 */
template <typename BaseType>
struct BaseArc2
{
    using Type = BaseType;

    Type cx, cy;
    Type rx, ry;
    Type start;
    Type sweep;

    BaseArc2() : cx(), cy(), rx(), ry(), start(), sweep()
    { /* EMPTY. */ }

    BaseArc2(Type const & cx_, Type const & cy_,
             Type const & rx_, Type const & ry_,
             Type const & start_, Type const & sweep_)
            : cx(cx_), cy(cy_), rx(rx_), ry(ry_), start(start_), sweep(sweep_)
    { /* EMPTY. */ }

    BaseArc2(BasePoint2<Type> const & c,
             BasePoint2<Type> const & r,
             Type const & start_, Type const & sweep_)
            : cx(c.x), cy(c.y), rx(r.x), ry(r.y), start(start_), sweep(sweep_)
    { /* EMPTY. */ }

    template <typename T>
    BaseArc2(BaseArc2<T> const & obj)
            : cx(obj.cx), cy(obj.cy),
              rx(obj.rx), ry(obj.ry),
              start(obj.start), sweep(obj.sweep)
    { /* EMPTY. */ }

    ~BaseArc2()
    { /* EMPTY. */ }

    template <typename T>
    BaseArc2 & operator =(BaseArc2<T> const & obj)
    {
        if (this != &obj) {
            cx = obj.cx;
            cy = obj.cy;
            rx = obj.rx;
            ry = obj.ry;
            start = obj.start;
            sweep = obj.sweep;
        }
        return *this;
    }

    void swap(BaseArc2 & obj)
    {
        if (this != &obj) {
            std::swap(cx, obj.cx);
            std::swap(cy, obj.cy);
            std::swap(rx, obj.rx);
            std::swap(ry, obj.ry);
            std::swap(start, obj.start);
            std::swap(sweep, obj.sweep);
        }
    }

    friend void swap(BaseArc2 & lh, BaseArc2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseArc2<T>() const
    {
        return BaseArc2<T>(static_cast<T>(cx), static_cast<T>(cy),
                           static_cast<T>(rx), static_cast<T>(ry),
                           static_cast<T>(start), static_cast<T>(sweep));
    }

    bool operator ==(BaseArc2 const & obj) const
    {
        return cx = obj.cx && cy = obj.cy &&
               rx = obj.rx && ry = obj.ry &&
               start = obj.start && sweep = obj.sweep;
    }

    bool operator !=(BaseArc2 const & obj) const
    {
        return !((*this) == obj);
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER1 = 'x';
    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER2 = '/';

    std::string toString() const
    {
        std::stringstream ss;
        ss << cx << DEFAULT_DELIMITER1 << cy << DEFAULT_DELIMITER2
           << rx << DEFAULT_DELIMITER1 << ry << DEFAULT_DELIMITER2
           << start << DEFAULT_DELIMITER1 << sweep;
        return ss.str();
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{cx, cy, rx, ry, start, sweep};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseArc2 const & p)
    {
        os << p.toString();
        return os;
    }
};

// ------------
// Pre-defined.
// ------------

using Arc2b  = BaseArc2<bool>;
using Arc2c  = BaseArc2<char>;
using Arc2s  = BaseArc2<short>;
using Arc2i  = BaseArc2<int>;
using Arc2u  = BaseArc2<unsigned>;
using Arc2l  = BaseArc2<long>;
using Arc2ll = BaseArc2<int64_t>;
using Arc2f  = BaseArc2<float>;
using Arc2d  = BaseArc2<double>;
using Arc2   = Arc2i;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_ARC2_HPP__

