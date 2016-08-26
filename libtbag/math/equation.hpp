/**
 * @file   equation.hpp
 * @brief  equation class prototype.
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
#include <cassert>

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
 *  - a: 기울기 (Gradient).
 *  - b: Y절편 (Y-intercept).
 */
template <typename T>
struct LinearEquation
{
    T a; ///< Gradient.
    T b; ///< Y-intercept.
};

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

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_EQUATION_HPP__

