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
#include <libtbag/algorithm/Equals.hpp>
#include <libtbag/geometry/Point2.hpp>
#include <libtbag/math/equation/LinearEquation.hpp>

#include <cassert>
#include <algorithm>
#include <limits>

//#define _DEBUGGING_IS_CROSS

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
template <typename T, typename PointType = libtbag::geometry::BasePoint2<T> >
inline LinearEquation<T>
getLinearEquationWithTwoPoint(PointType const & p1, PointType const & p2)
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
    return algorithm::equals<T>(e1.a, e2.a);
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
    return algorithm::equals<T>(e1.a * e2.b, static_cast<T>(-1));
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
template <typename T, typename PointType = libtbag::geometry::BasePoint2<T> >
inline PointType getIntersectionWithTwoLinearEquation(LinearEquation<T> const & e1, LinearEquation<T> const & e2)
{
    assert(e1.a != e2.a);
    PointType result;
    result.x = (e2.b - e1.b) / (e1.a - e2.a);
    result.y = e1.a * result.x + e1.b;
    return result;
}

/**
 * Check if it is included in the range.
 */
template <typename T>
inline bool isAlmostContains(T const & v1, T const & v2, T const & check) TBAG_NOEXCEPT
{
    T const MIN = std::min(v1, v2);
    T const MAX = std::max(v1, v2);
    using namespace algorithm;
    return lessOrEquals(MIN, check) && greaterOrEquals(MAX, check);
}

/**
 * Check if it is included in the range.
 */
template <typename T, typename PointType = libtbag::geometry::BasePoint2<T> >
inline bool isAlmostContains2d(PointType const & p1, PointType const & p2, PointType const & check) TBAG_NOEXCEPT
{
    return isAlmostContains(p1.x, p2.x, check.x) && isAlmostContains(p1.y, p2.y, check.y);
}

/**
 *
 * @remarks
 *  Equations of the form <code>x = a</code> are impossible. @n
 *  These equations are calculated separately.
 */
template <typename T, typename PointType = libtbag::geometry::BasePoint2<T> >
bool isCross(PointType const & p11, PointType const & p12, PointType const & p21, PointType const & p22, PointType & cross)
{
#if defined(_DEBUGGING_IS_CROSS)
    std::cout << p11.x << 'x' << p11.y << ','
              << p12.x << 'x' << p12.y << ','
              << p21.x << 'x' << p21.y << ','
              << p22.x << 'x' << p22.y << std::endl;
#endif

    equation::LinearEquation<T, PointType> e1;
    equation::LinearEquation<T, PointType> e2;

    if (e1.determinedByPoints(p11, p12) == false || e2.determinedByPoints(p21, p22) == false) {
        return false;
    }
    if (e1.intersection(e2, cross) == false) {
        return false;
    }

    bool const IS_CONTAINS = isAlmostContains2d<T, PointType>(p11, p12, cross) && isAlmostContains2d<T, PointType>(p21, p22, cross);
    if (IS_CONTAINS) {
#if defined(_DEBUGGING_IS_CROSS)
        std::cout << "IsCross" << std::endl;
#endif
        return true;
    }
    return false;
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_HPP__

