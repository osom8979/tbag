/**
 * @file   Rect2.hpp
 * @brief  Rect2 class prototype.
 * @author zer0
 * @date   2016-04-20
 * @date   2019-02-20 (Rename: Rect -> Rect2)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT2_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT2_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point2.hpp>
#include <libtbag/Type.hpp>

#include <cmath>
#include <cstdlib>
#include <cstdint>

#include <ostream>
#include <algorithm>
#include <sstream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace geometry {

/**
 * 2-dimensions Rectangle template class.
 *
 * @author zer0
 * @date   2015-06-12
 * @date   2015-08-22 (Move the world library)
 * @date   2015-10-18 (Change to the trivial type)
 * @date   2015-10-21 (Bug fix: assign atomic type)
 * @date   2016-08-24 (Remove BasePoint2 & BaseSize2)
 * @date   2017-11-02 (Change to the class type)
 * @date   2019-02-20 (Rename: BaseRect -> BaseRect2)
 */
template <typename BaseType>
struct BaseRect2
{
    using Type = BaseType;

    Type x, y;
    Type width, height;

    BaseRect2() : x(), y(), width(), height()
    { /* EMPTY. */ }

    BaseRect2(Type const & x_, Type const & y_, Type const & w_, Type const & h_) : x(x_), y(y_), width(w_), height(h_)
    { /* EMPTY. */ }

    template <typename T>
    BaseRect2(BasePoint2<T> const & p, BaseSize2<T> const & s) : x(p.x), y(p.y), width(s.width), height(s.height)
    { /* EMPTY. */ }

    template <typename T>
    BaseRect2(BasePoint2<T> const & lt, BasePoint2<T> const & rb) : x(lt.x), y(lt.y), width(rb.x - lt.x), height(rb.y - lt.y)
    { /* EMPTY. */ }

    template <typename T>
    BaseRect2(BaseRect2<T> const & obj) : x(obj.x), y(obj.y), width(obj.width), height(obj.height)
    { /* EMPTY. */ }

    ~BaseRect2()
    { /* EMPTY. */ }

    inline BasePoint2<Type> point () const { return BasePoint2<Type>(x, y); }
    inline BasePoint2<Type> point1() const { return point(); }
    inline BasePoint2<Type> point2() const { return BasePoint2<Type>(x + width, y + height); }
    inline BasePoint2<Type> center() const { return BasePoint2<Type>(x + (width * 0.5), y + (height * 0.5)); }

    inline BaseSize2<Type> size() const { return BaseSize2<Type>(width, height); }
    inline BaseSize2<Type> half() const { return BaseSize2<Type>(width * 0.5, height  * 0.5); }

    inline Type  area() const { return width * height; }
    inline bool empty() const { return width <= 0 || height <= 0; }

    template <typename T>
    BaseRect2 & operator =(BaseRect2<T> const & obj)
    {
        if (this != &obj) {
            x = obj.x;
            y = obj.y;
            width  = obj.width;
            height = obj.height;
        }
        return *this;
    }

    void swap(BaseRect2 & obj)
    {
        if (this != &obj) {
            std::swap(x, obj.x);
            std::swap(y, obj.y);
            std::swap(width, obj.width);
            std::swap(height, obj.height);
        }
    }

    friend void swap(BaseRect2 & lh, BaseRect2 & rh)
    {
        lh.swap(rh);
    }

    template <typename T>
    operator BaseRect2<T>() const
    {
        return BaseRect2<T>(static_cast<T>(x), static_cast<T>(y), static_cast<T>(width), static_cast<T>(height));
    }

    /** The Rect are equal? */
    template <typename T>
    bool operator ==(BaseRect2<T> const & obj) const
    {
        return x == obj.x && y == obj.y && width == obj.width && height == obj.height;
    }

    template <typename T>
    bool operator !=(BaseRect2<T> const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BaseRect2 & operator +=(BasePoint2<T> const & p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    template <typename T>
    BaseRect2 & operator -=(BasePoint2<T> const & p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    template <typename T>
    BaseRect2 & operator +=(BaseSize2<T> const & s)
    {
        width  += s.width;
        height += s.height;
        return *this;
    }

    template <typename T>
    BaseRect2 & operator -=(BaseSize2<T> const & s)
    {
        width  -= s.width;
        height -= s.height;
        return *this;
    }

    /** obtain the intersection rect. */
    template <typename T>
    BaseRect2 & operator &=(BaseRect2<T> const & obj)
    {
        Type x1 = std::max<Type>(x, obj.x);
        Type y1 = std::max<Type>(y, obj.y);
        width   = std::min<Type>(x + width , obj.x + obj.width ) - x1;
        height  = std::min<Type>(y + height, obj.y + obj.height) - y1;
        x = x1;
        y = y1;
        return *this;
    }

    /** obtain the union rect. */
    template <typename T>
    BaseRect2 & operator |=(BaseRect2<T> const & obj)
    {
        Type x1 = std::min<Type>(x, obj.x);
        Type y1 = std::min<Type>(y, obj.y);
        width   = std::max<Type>(x + width , obj.x + obj.width ) - x1;
        height  = std::max<Type>(y + height, obj.y + obj.height) - y1;
        x = x1;
        y = y1;
        return *this;
    }

    // ------------------
    // Absolute location.
    // ------------------

    inline Type     getLeftTopX() const { return std::min(x, x + width ); }
    inline Type     getLeftTopY() const { return std::min(y, y + height); }
    inline Type    getRightTopX() const { return std::max(x, x + width ); }
    inline Type    getRightTopY() const { return std::min(y, y + height); }
    inline Type  getLeftBottomX() const { return std::min(x, x + width ); }
    inline Type  getLeftBottomY() const { return std::max(y, y + height); }
    inline Type getRightBottomX() const { return std::max(x, x + width ); }
    inline Type getRightBottomY() const { return std::max(y, y + height); }

    inline Type ltx() const { return     getLeftTopX(); }
    inline Type lty() const { return     getLeftTopY(); }
    inline Type rtx() const { return    getRightTopX(); }
    inline Type rty() const { return    getRightTopY(); }
    inline Type lbx() const { return  getLeftBottomX(); }
    inline Type lby() const { return  getLeftBottomY(); }
    inline Type rbx() const { return getRightBottomX(); }
    inline Type rby() const { return getRightBottomY(); }

    inline BasePoint2<Type>     getLeftTop() const { return BasePoint2<Type>(    getLeftTopX(),     getLeftTopY()); }
    inline BasePoint2<Type>    getRightTop() const { return BasePoint2<Type>(   getRightTopX(),    getRightTopY()); }
    inline BasePoint2<Type>  getLeftBottom() const { return BasePoint2<Type>( getLeftBottomX(),  getLeftBottomY()); }
    inline BasePoint2<Type> getRightBottom() const { return BasePoint2<Type>(getRightBottomX(), getRightBottomY()); }

    inline BasePoint2<Type> lt() const {     getLeftTop(); }
    inline BasePoint2<Type> rt() const {    getRightTop(); }
    inline BasePoint2<Type> lb() const {  getLeftBottom(); }
    inline BasePoint2<Type> rb() const { getRightBottom(); }

    // ----------------
    // Offset validate.
    // ----------------

    template <typename T>
    inline bool contains(T const & comp_x, T const & comp_y) const
    {
        return x <= COMPARE_AND(comp_x) <= (x +  width) &&
               y <= COMPARE_AND(comp_y) <= (y + height);
    }

    template <typename T>
    inline bool contains(BasePoint2<T> const & p) const
    {
        return contains(p.x, p.y);
    }

    template <typename T>
    inline bool contains(BaseRect2<T> const & r) const
    {
        return contains(r.point1()) && contains(r.point2());
    }

    // ------------------------
    // Miscellaneous utilities.
    // ------------------------

    BaseRect2<Type> absolute() const
    {
        BaseRect2<Type> result;
        result.x = getLeftTopX();
        result.y = getLeftTopY();
        result.width  = getRightBottomX() - result.x;
        result.height = getRightBottomY() - result.y;
        return result;
    }

    template <typename T>
    bool clip(BaseRect2<T> const & obj, BaseRect2<T> * clip = nullptr) const
    {
        Type lt_x = std::max(    getLeftTopX(), obj.getLeftTopX());
        Type lt_y = std::max(    getLeftTopY(), obj.getLeftTopY());
        Type rb_x = std::min(getRightBottomX(), obj.getRightBottomX());
        Type rb_y = std::min(getRightBottomY(), obj.getRightBottomY());

        if (lt_x < rb_x && lt_y < rb_y) {
            if (clip != nullptr) {
                clip->x = lt_x;
                clip->y = lt_y;
                clip->width  = rb_x - lt_x;
                clip->height = rb_y - lt_y;
            }
            return true;
        }
        return false;
    }

    BaseRect2<Type> stretchByCenter(Type const & half_size)
    {
        return BaseRect2<Type>(x - half_size, y - half_size, width + (half_size * 2), height + (half_size * 2));
    }

    TBAG_CONSTEXPR static char const DEFAULT_DELIMITER = 'x';

    std::string toString() const
    {
        std::stringstream ss;
        ss << x << DEFAULT_DELIMITER << y << DEFAULT_DELIMITER << width << DEFAULT_DELIMITER << height;
        return ss.str();
    }

    template <typename OtherType>
    OtherType toOther() const
    {
        return OtherType{x, y, width, height};
    }

    template <class CharT, class TraitsT>
    friend std::basic_ostream<CharT, TraitsT> & operator<<(std::basic_ostream<CharT, TraitsT> & os, BaseRect2 const & r)
    {
        os << r.toString();
        return os;
    }
};

template <typename T>
BaseRect2<T> operator +(BaseRect2<T> const & r, BasePoint2<T> const & p)
{
    BaseRect2<T> result = r;
    result += p;
    return result;
}

template <typename T>
BaseRect2<T> operator -(BaseRect2<T> const & r, BasePoint2<T> const & p)
{
    BaseRect2<T> result = r;
    result -= p;
    return result;
}

template <typename T>
BaseRect2<T> operator +(BaseRect2<T> const & r, BaseSize2<T> const & s)
{
    BaseRect2<T> result = r;
    result += s;
    return result;
}

template <typename T>
BaseRect2<T> operator -(BaseRect2<T> const & r, BaseSize2<T> const & s)
{
    BaseRect2<T> result = r;
    result -= s;
    return result;
}

template <typename T>
BaseRect2<T> operator &(BaseRect2<T> const & r1, BaseRect2<T> const & r2)
{
    BaseRect2<T> result = r1;
    result &= r2;
    return result;
}

template <typename T>
BaseRect2<T> operator |(BaseRect2<T> const & r1, BaseRect2<T> const & r2)
{
    BaseRect2<T> result = r1;
    result |= r2;
    return result;
}

// ------------
// Pre-defined.
// ------------

using Rect2b  = BaseRect2<bool>;
using Rect2c  = BaseRect2<char>;
using Rect2s  = BaseRect2<short>;
using Rect2i  = BaseRect2<int>;
using Rect2u  = BaseRect2<unsigned>;
using Rect2l  = BaseRect2<long>;
using Rect2ll = BaseRect2<int64_t>;
using Rect2f  = BaseRect2<float>;
using Rect2d  = BaseRect2<double>;
using Rect2   = Rect2i;

using Rectb  = Rect2b;
using Rectc  = Rect2c;
using Rects  = Rect2s;
using Recti  = Rect2i;
using Rectu  = Rect2u;
using Rectl  = Rect2l;
using Rectll = Rect2ll;
using Rectf  = Rect2f;
using Rectd  = Rect2d;
using Rect   = Rect2;

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT2_HPP__

