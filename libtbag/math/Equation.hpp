/**
 * @file   Equation.hpp
 * @brief  Equation.
 * @author zer0
 * @date   2016-08-26
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point.hpp>

#include <cassert>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

/**
 * Linear Equation.
 *
 * @translate{ko, 일차 방정식.}
 *
 * @author zer0
 * @date   2016-08-26
 *
 * @remarks
 *  \f[
 *   y = ax + b
 *  \f]
 *  - a: Gradient (기울기).
 *  - b: Y-intercept (Y절편).
 *
 * @warning
 *  Impossible equation: \f$ x = b \f$
 */
template <typename T>
struct LinearEquation
{
    T a; ///< Gradient.
    T b; ///< Y-intercept.
};

template <typename T>
inline T getY(LinearEquation<T> e, T x)
{
    return e.a * x + e.b;
}

/**
 * Quadratic Equation.
 *
 * @translate{ko, 이차 방정식.}
 *
 * @author zer0
 * @date   2016-08-26
 *
 * @remarks
 *  \f[
 *   y = ax^{2} + bx + c
 *  \f]
 */
template <typename T>
struct QuadraticEquation
{
    T a, b, c;
};

/**
 * Obtain the linear-equation.
 *
 * @param x1 [in] Point 1, x.
 * @param y1 [in] Point 1, y.
 * @param x2 [in] Point 2, x.
 * @param y2 [in] Point 2, y.
 *
 * @remarks
 *  \f[
 *   \begin{align}
 *   y &= ax + b \\
 *     &= (\frac{y_2 - y_1}{x_2 - x_1}) (x - x_1) + y_1 \\
 *     &= (\frac{y_2 - y_1}{x_2 - x_1})x - (\frac{y_2 - y_1}{x_2 - x_1})x_1 + y_1 \\
 *   \end{align}
 *   \\
 *   RESULT: a = \frac{y_2 - y_1}{x_2 - x_1}, b = -ax_1 + y_1, (x_1 \neq x_2)
 *  \f]
 */
template <typename T>
inline LinearEquation<T>
getLinearEquationWithTwoPoint(T x1, T y1, T x2, T y2)
{
    assert(x1 != x2);
    LinearEquation<T> result;
    result.a = (y2 - y1) / (x2 - x1);
    result.b = result.a * (-1) * x1 + y1;
    return result;
}

/**
 * Obtain the linear-equation.
 *
 * @translate{ko, 두 점을 사용하여 일차 방정식을 구한다.}
 *
 * @param p1 [in] Point 1.
 * @param p2 [in] Point 2.
 */
template <typename T, typename Point = geometry::BasePoint<T> >
inline LinearEquation<T>
getLinearEquationWithTwoPoint(Point const & p1, Point const & p2)
{
    return getLinearEquationWithTwoPoint(p1.x, p1.y, p2.x, p2.y);
}

/**
 * Check the two equations are parallel.
 *
 * @translate{ko, 두 직선이 평행한지 확인한다.}
 */
template <typename T>
inline bool
isParallelWithTwoLinearEquation(LinearEquation<T> const & e1, LinearEquation<T> const & e2)
{
    return e1.a == e2.a;
}

/**
 * Check the two equations are perpendicular.
 *
 * @translate{ko, 두 직선이 수직한지 확인한다.}
 */
template <typename T>
inline bool
isPerpendicularWithTwoLinearEquation(LinearEquation<T> const & e1, LinearEquation<T> const & e2)
{
    return e1.a * e2.b == -1;
}

/**
 * Calculate the intersection of two equations(lines).
 *
 * @translate{ko, 두 직선의 교차점을 구한다.}
 *
 * @remarks
 *  Two linear equation:
 *  \f[
 *   y = a_1x + b_1 \\
 *   y = a_2x + b_2 \\
 *  \f]
 *  Explanation:
 *  \f[
 *   \begin{align}
 *     a_1x + b_1   &= a_2x + b_2 \\
 *     a_1x - a_2x  &= b_2 - b_1 \\
 *    (a_1  - a_2)x &= b_2 - b_1 \\
 *                x &= \frac{b_2 - b_1}{a_1 - a_2} \\
 *   \end{align}
 *   \\
 *   RESULT: x = \frac{b_2 - b_1}{a_1 - a_2}, y = a_1x + b_1, (a_1 \neq a_2)
 *  \f]
 */
template <typename T, typename Point = geometry::BasePoint<T> >
inline Point getIntersectionWithTwoLinearEquation(LinearEquation<T> const & e1, LinearEquation<T> const & e2)
{
    assert(e1.a != e2.a);
    Point result;
    result.x = (e2.b - e1.b) / (e1.a - e2.a);
    result.y = e1.a * result.x + e1.b;
    return result;
}

template <typename T, typename Point = geometry::BasePoint<T> >
bool isContains(Point const & p1, Point const & p2, Point const & check)
{
    T const X_MIN = std::min(p1.x, p2.x);
    T const X_MAX = std::max(p1.x, p2.x);
    T const Y_MIN = std::min(p1.y, p2.y);
    T const Y_MAX = std::max(p1.y, p2.y);
    return (X_MIN <= COMPARE_AND(check.x) <= X_MAX &&
            Y_MIN <= COMPARE_AND(check.y) <= Y_MAX);
}

template <typename T, typename Point = geometry::BasePoint<T> >
bool isCross(Point const & p11, Point const & p12, Point const & p21, Point const & p22, Point & cross)
{
    if (p11.x != p12.x && p21.x != p22.x) {
        LinearEquation<T> e1 = getLinearEquationWithTwoPoint<T, Point>(p11, p12);
        LinearEquation<T> e2 = getLinearEquationWithTwoPoint<T, Point>(p21, p22);

        if (isParallelWithTwoLinearEquation<T>(e1, e2) == false) {
            cross = getIntersectionWithTwoLinearEquation<T, Point>(e1, e2);
            return isContains<T, Point>(p11, p12, cross) && isContains<T, Point>(p21, p22, cross);
        } else {
            return false;
        }

    } else if (p11.x != p12.x && p21.x == p22.x) {
        LinearEquation<T> e1 = getLinearEquationWithTwoPoint<T, Point>(p11, p12);
        Point cross = Point(p21.x, getY<T>(e1, p21.x));
        return isContains<T, Point>(p11, p12, cross);

    } else if (p11.x == p12.x && p21.x != p22.x) {
        LinearEquation<T> e2 = getLinearEquationWithTwoPoint<T, Point>(p21, p22);
        Point cross = Point(p11.x, getY<T>(e2, p11.x));
        return isContains<T, Point>(p21, p22, cross);
    }

    return false;
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_HPP__

