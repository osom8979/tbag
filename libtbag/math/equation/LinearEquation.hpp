/**
 * @file   LinearEquation.hpp
 * @brief  LinearEquation class prototype.
 * @author zer0
 * @date   2017-12-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_LINEAREQUATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_LINEAREQUATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/algorithm/Equals.hpp>
#include <libtbag/geometry/Point2.hpp>

#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math     {
namespace equation {

/**
 * General form of linear equation.
 *
 * @translate{ko, 일반형 일차 방정식.}
 *
 * @author zer0
 * @date   2017-12-15
 *
 * @remarks
 *  \f[
 *   ax + by + c = 0
 *  \f]
 */
template <typename ValueType, typename PointType = libtbag::geometry::BasePoint2<ValueType> >
class LinearEquation
{
public:
    using Value = ValueType;
    using Point = PointType;

public:
    static_assert(std::is_pod<Value>::value, "Value must be a POD type.");

public:
    Value a, b, c;

public:
    LinearEquation() TBAG_NOEXCEPT : a(), b(), c()
    { /* EMPTY. */ }
    LinearEquation(Value a_, Value b_, Value c_) TBAG_NOEXCEPT : a(a_), b(b_), c(c_)
    { /* EMPTY. */ }

    LinearEquation(Point const & p1, Point const & p2) TBAG_NOEXCEPT : LinearEquation()
    {
        determinedByPoints(p1, p2);
    }

    LinearEquation(LinearEquation const & obj) TBAG_NOEXCEPT : LinearEquation()
    { *this = obj; }
    LinearEquation(LinearEquation && obj) TBAG_NOEXCEPT : LinearEquation()
    { *this = std::move(obj); }

    ~LinearEquation() TBAG_NOEXCEPT
    { /* EMPTY. */ }

public:
    LinearEquation & operator =(LinearEquation const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            a = obj.a;
            b = obj.b;
            c = obj.c;
        }
        return *this;
    }

    LinearEquation & operator =(LinearEquation && obj) TBAG_NOEXCEPT
    {
        swap(obj);
        return *this;
    }

public:
    void swap(LinearEquation & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(a, obj.a);
            std::swap(b, obj.b);
            std::swap(c, obj.c);
        }
    }

    inline friend void swap(LinearEquation & lh, LinearEquation & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline void set(Value a_, Value b_, Value c_) TBAG_NOEXCEPT
    {
        a = a_;
        b = b_;
        c = c_;
    }

public:
    /**
     * @remarks
     *  \f[
     *   (x_{2} - x_{1})(y - y_{1})=(y_{2} - y_{1})(x - x_{1}) \\
     *   x_{2}y - x_{2}y_{1} - x_{1}y + x_{1}y_{1} = y_{2}x - y_{2}x_{1} - y_{1}x + y_{1}x_{1} \\
     *   (x_{2} - x_{1})y - x_{2}y_{1} + x_{1}y_{1} = (y_{2} - y_{1})x - y_{2}x_{1} + y_{1}x_{1} \\
     *   (x_{2} - x_{1})y - (y_{2} - y_{1})x - x_{2}y_{1} + x_{1}y_{1} + y_{2}x_{1} - y_{1}x_{1} = 0 \\
     *   (y_{1} - y_{2})x + (x_{2} - x_{1})y + y_{2}x_{1} - x_{2}y_{1} = 0 \\
     *   \\
     *   RESULT: \\
     *   a = y_{1} - y_{2} \\
     *   b = x_{2} - x_{1} \\
     *   c = y_{2}x_{1} - x_{2}y_{1}
     *  \f]
     *
     * @see <https://en.wikipedia.org/wiki/Linear_equation#Two-point_form>
     */
    inline bool determinedByPoints(Value x1, Value y1, Value x2, Value y2) TBAG_NOEXCEPT
    {
        if (x1 == x2 && y1 == y2) {
            return false;
        }
        a = y1 - y2;
        b = x2 - x1;
        c = (y2 * x1) - (x2 * y1);
        return true;
    }

    inline bool determinedByPoints(Point const & p1, Point const & p2) TBAG_NOEXCEPT
    {
        return determinedByPoints(p1.x, p1.y, p2.x, p2.y);
    }

public:
    /**
     * Obtain the x.
     *
     * @remarks
     *  \f[
     *   ax + by + c = 0 \\
     *   ax = -(by + c) \\
     *    x = -\frac{by + c}{a} \\
     *  \f]
     */
    inline Value getX(Value y)
    {
        assert(a != 0);
        return -(b * y + c) / a;
    }

    /**
     * Obtain the y.
     *
     * @remarks
     *  \f[
     *   ax + by + c = 0 \\
     *   by = -(ax + c) \\
     *    y = -\frac{ax + c}{b} \\
     *  \f]
     */
    inline Value getY(Value x)
    {
        assert(b != 0);
        return -(a * x + c) / b;
    }

public:
    /**
     * Obtain the gradient of x.
     *
     * @remarks
     *  \f[
     *   ax + by + c = 0 \\
     *   ax = -(by + c) \\
     *    x = -\frac{by + c}{a} \\
     *    x = -\frac{b}{a}y - \frac{c}{a} \\
     *   \\
     *   RESULT: \\
     *   m = -\frac{b}{a}
     *  \f]
     */
    inline Value getXGradient()
    {
        assert(a != 0);
        return -(b / a);
    }

    /**
     * Obtain the gradient of x.
     *
     * @remarks
     *  \f[
     *   ax + by + c = 0 \\
     *   by = -(ax + c) \\
     *    y = -\frac{ax + c}{b} \\
     *    y = -\frac{a}{b}x - \frac{c}{b} \\
     *   \\
     *   RESULT: \\
     *   m = -\frac{a}{b}
     *  \f]
     */
    inline Value getYGradient()
    {
        assert(b != 0);
        return -(a / b);
    }

    inline Value getGradient()
    {
        return getYGradient();
    }

public:
    /**
     * Obtain the x-intercept. (y is 0)
     *
     * @remarks
     *  \f[
     *   ax + c = 0 \\
     *   x = -\frac{c}{a} \\
     *  \f]
     */
    inline Value getXIntercept()
    {
        assert(a != 0);
        return -(c / a);
    }

    /**
     * Obtain the y-intercept. (x is 0)
     *
     * @remarks
     *  \f[
     *   by + c = 0 \\
     *   y = -\frac{c}{b} \\
     *  \f]
     */
    inline Value getYIntercept()
    {
        assert(b != 0);
        return -(c / b);
    }

public:
    /**
     * Line–line intersection
     *
     * @remarks
     *  아래와 같이 \f$ c_{N-1} \f$ 를 구할 수 있다. @n
     *  \f[
     *   E1: y = -\frac{a_{1}}{b_{1}}x - \frac{c_{1}}{b_{1}} \\
     *   E2: y = -\frac{a_{2}}{b_{2}}x - \frac{c_{2}}{b_{2}}
     *  \f]
     *  기울기가 동일할 경우 두 직선은 평행이다. @n
     *  \f[
     *   -\frac{a_{1}}{b_{1}} = -\frac{a_{2}}{b_{2}} \\
     *   \frac{a_{1}}{b_{1}} = \frac{a_{2}}{b_{2}} \\
     *   a_{1}b_{2} = a_{2}b_{1} \\
     *   a_{1}b_{2} - a_{2}b_{1} = 0
     *  \f]
     *  E1과 E2를 사용하여 값을 획득할 수 있다. @n
     *  \f[
     *   -\frac{a_{1}}{b_{1}}x - \frac{c_{1}}{b_{1}} = -\frac{a_{2}}{b_{2}}x - \frac{c_{2}}{b_{2}} \\
     *   \frac{a_{1}}{b_{1}}x - \frac{a_{2}}{b_{2}}x = \frac{c_{2}}{b_{2}} - \frac{c_{1}}{b_{1}} \\
     *   a_{1}b_{2}x - a_{2}b_{1}x = c_{2}b_{1} - c_{1}b_{2} \\
     *   (a_{1}b_{2} - a_{2}b_{1})x = c_{2}b_{1} - c_{1}b_{2} \\
     *   x = \frac{c_{2}b_{1} - c_{1}b_{2}}{a_{1}b_{2} - a_{2}b_{1}}
     *  \f]
     *  동일한 방법으로 y값을 구할 수 있다. @n
     *  \f[
     *   E1': x = -\frac{b_{1}}{a_{1}}y - \frac{c_{1}}{a_{1}} \\
     *   E2': x = -\frac{b_{2}}{a_{2}}y - \frac{c_{2}}{a_{2}} \\
     *   ...
     *   y = \frac{c_{1}a_{2} - c_{2}a_{1}}{a_{1}b_{2} - a_{2}b_{1}}
     *  \f]
     */
    inline bool intersection(LinearEquation const & e, Point & result) TBAG_NOEXCEPT
    {
        Value const DETERMINE = (a * e.b) - (e.a * b);
        if (algorithm::equals<Value>(DETERMINE, static_cast<Value>(0))) {
            return false; // Lines are parallel.
        }
        result.x = ((e.c * b) - (c * e.b)) / DETERMINE;
        result.y = ((c * e.a) - (e.c * a)) / DETERMINE;
        return true;
    }
};

} // namespace equation
} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_LINEAREQUATION_HPP__

