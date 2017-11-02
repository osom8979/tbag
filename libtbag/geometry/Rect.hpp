/**
 * @file   Rect.hpp
 * @brief  Rect class prototype.
 * @author zer0
 * @date   2016-04-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point.hpp>
#include <libtbag/Type.hpp>

#include <cmath>
#include <cstdlib>

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
 * @date   2016-08-24 (Remove BasePoint & BaseSize)
 * @date   2017-11-02 (Change to the class type)
 */
template <typename BaseType>
struct BaseRect
{
    using Type = BaseType;

    Type x, y;
    Type width, height;

    BaseRect() : x(), y(), width(), height()
    { /* EMPTY. */ }

    BaseRect(std::initializer_list<Type> list) : x(), y(), width(), height()
    {
        auto itr = list.begin();
        if (itr == list.end()) {
            return;
        }
        x = *itr;
        if ((++itr) == list.end()) {
            return;
        }
        y = *itr;
        if ((++itr) == list.end()) {
            return;
        }
        width = *itr;
        if ((++itr) == list.end()) {
            return;
        }
        height = *itr;
    }

    template <typename T>
    BaseRect(T const & x_, T const & y_, T const & w_, T const & h_) : x(x_), y(y_), width(w_), height(h_)
    { /* EMPTY. */ }

    template <typename T>
    BaseRect(BasePoint<T> const & p, BaseSize<T> const & s) : x(p.x), y(p.y), width(s.width), height(s.height)
    { /* EMPTY. */ }

    template <typename T>
    BaseRect(BasePoint<T> const & lt, BasePoint<T> const & rb) : x(lt.x), y(lt.y), width(rb.x - lt.x), height(rb.y - lt.y)
    { /* EMPTY. */ }

    template <typename T>
    BaseRect(BaseRect<T> const & obj) : x(obj.x), y(obj.y), width(obj.width), height(obj.height)
    { /* EMPTY. */ }

    ~BaseRect()
    { /* EMPTY. */ }

    template <typename T>
    BaseRect & operator =(BaseRect<T> const & obj)
    {
        if (this != &obj) {
            x = obj.x;
            y = obj.y;
            width  = obj.width;
            height = obj.height;
        }
        return *this;
    }

    inline BasePoint<Type> getPoint() const { return BasePoint<Type>(x, y); }
    inline BaseSize<Type> getSize() const { return BaseSize<Type>(width, height); }
    inline Type getArea() const { return width * height; }

    /** The Rect are equal? */
    template <typename T>
    bool operator ==(BaseRect<T> const & obj) const
    {
        return x == obj.x && y == obj.y && width == obj.width && height == obj.height;
    }

    template <typename T>
    bool operator !=(BaseRect<T> const & obj) const
    {
        return !((*this) == obj);
    }

    template <typename T>
    BaseRect & operator +=(BasePoint<T> const & p)
    {
        x += p.x;
        y += p.y;
        return *this;
    }

    template <typename T>
    BaseRect & operator -=(BasePoint<T> const & p)
    {
        x -= p.x;
        y -= p.y;
        return *this;
    }

    template <typename T>
    BaseRect & operator +=(BaseSize<T> const & s)
    {
        width  += s.width;
        height += s.height;
        return *this;
    }

    template <typename T>
    BaseRect & operator -=(BaseSize<T> const & s)
    {
        width  -= s.width;
        height -= s.height;
        return *this;
    }

    template <typename T>
    friend BaseRect<T> operator +(BaseRect<T> const & r, BasePoint<T> const & p)
    {
        BaseRect<T> result = r;
        return result += p;
    }

    template <typename T>
    friend BaseRect<T> operator -(BaseRect<T> const & r, BasePoint<T> const & p)
    {
        BaseRect<T> result = r;
        return result -= p;
    }

    template <typename T>
    friend BaseRect<T> operator +(BaseRect<T> const & r, BaseSize<T> const & s)
    {
        BaseRect<T> result = r;
        return result += s;
    }

    template <typename T>
    friend BaseRect<T> operator -(BaseRect<T> const & r, BaseSize<T> const & s)
    {
        BaseRect<T> result = r;
        return result -= s;
    }

    template <typename T>
    BaseRect & operator &=(BaseRect<T> const & obj)
    {
        Type x1 = std::max<Type>(x, obj.x);
        Type y1 = std::max<Type>(y, obj.y);
        width   = std::min<Type>(x + width , obj.x + obj.width ) - x1;
        height  = std::min<Type>(y + height, obj.y + obj.height) - y1;
        x = x1;
        y = y1;
        return *this;
    }

    template <typename T>
    BaseRect & operator |=(BaseRect<T> const & obj)
    {
        Type x1 = std::min<Type>(x, obj.x);
        Type y1 = std::min<Type>(y, obj.y);
        width   = std::max<Type>(x + width , obj.x + obj.width ) - x1;
        height  = std::max<Type>(y + height, obj.y + obj.height) - y1;
        x = x1;
        y = y1;
        return *this;
    }

    template <typename T>
    friend BaseRect<T> operator &(BaseRect<T> const & r1, BaseRect<T> const & r2)
    {
        BaseRect<T> result = r1;
        return result &= r2;
    }

    template <typename T>
    friend BaseRect<T> operator |(BaseRect<T> const & r1, BaseRect<T> const & r2)
    {
        BaseRect<T> result = r1;
        return result |= r2;
    }

    // ------------------
    // Absolute location.
    // ------------------

    Type     getLeftTopX() const { return std::min(x, x + width ); }
    Type     getLeftTopY() const { return std::min(y, y + height); }
    Type    getRightTopX() const { return std::max(x, x + width ); }
    Type    getRightTopY() const { return std::min(y, y + height); }
    Type  getLeftBottomX() const { return std::min(x, x + width ); }
    Type  getLeftBottomY() const { return std::max(y, y + height); }
    Type getRightBottomX() const { return std::max(x, x + width ); }
    Type getRightBottomY() const { return std::max(y, y + height); }

    BasePoint<Type>     getLeftTop() const { return BasePoint<Type>(    getLeftTopX(),     getLeftTopY()); }
    BasePoint<Type>    getRightTop() const { return BasePoint<Type>(   getRightTopX(),    getRightTopY()); }
    BasePoint<Type>  getLeftBottom() const { return BasePoint<Type>( getLeftBottomX(),  getLeftBottomY()); }
    BasePoint<Type> getRightBottom() const { return BasePoint<Type>(getRightBottomX(), getRightBottomY()); }

    // ----------------
    // Offset validate.
    // ----------------

    template <typename T>
    inline bool checkInside(T const & x_, T const & y_) const
    {
        return x <= COMPARE_AND(x_) <= (x +  width) &&
               y <= COMPARE_AND(y_) <= (y + height);
    }

    template <typename T>
    inline bool checkInside(BasePoint<T> const & p) const
    {
        return checkInside(p.x, p.y);
    }

    template <typename T>
    inline bool checkInside(BaseRect<T> const & r) const
    {
        return checkInside(r.x, r.y) && checkInside(r.x + r.width, r.y + r.height);
    }

    // ------------------------
    // Miscellaneous utilities.
    // ------------------------

    BaseRect<Type> getAbsolute() const
    {
        BaseRect<Type> result;
        result.x = getLeftTopX();
        result.y = getLeftTopY();
        result.width  = getRightBottomX() - result.x;
        result.height = getRightBottomY() - result.y;
        return result;
    }

    template <typename T>
    bool clip(BaseRect<T> const & obj, BaseRect<T> * clip = nullptr) const
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

    std::string toString() const
    {
        std::stringstream ss;
        ss << x << "x" << y << "x" << width << "x" << height;
        return ss.str();
    }
};

// ------------
// Pre-defined.
// ------------

using Rect = BaseRect<int>;

Rect const EMPTY_RECT(0, 0, 0, 0);

} // namespace geometry

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

// --------------
// Output Stream.
// --------------

#include <ostream>

template <typename T>
inline std::ostream & operator << (std::ostream & os, libtbag::geometry::BaseRect<T> const & rect)
{
    os << rect.toString();
    return os;
}

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GEOMETRY_RECT_HPP__

