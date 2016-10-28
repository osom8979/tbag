/**
 * @file   Trigonometry.hpp
 * @brief  Trigonometry functions.
 * @author zer0
 * @date   2016-04-21
 *
 * @remarks
 *  @code
 *                   C
 *                   *
 *                  /|
 *  h: hypotenuse  / |
 *                /  | a: opposite
 *               /   |
 *            A *----* B: Right angle (90-degree)
 *          b: adjacent
 *  @endcode
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_TRIGONOMETRY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_TRIGONOMETRY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

/**
 * Return the sine A.
 *
 * @warning
 *  Didn't use the DivideByZeroException.
 */
template <typename T>
inline T sinA(T a, T h)
{
    return a / h;
}

/**
 * Return the cosine A.
 *
 * @warning
 *  Didn't use the DivideByZeroException.
 */
template <typename T>
inline T cosA(T b, T h)
{
    return b / h;
}

/**
 * Return the tangent A.
 *
 * @warning
 *  Didn't use the DivideByZeroException.
 */
template <typename T>
inline T tanA(T a, T b)
{
    return a / b;
}

/**
 * Return the cosecant A (Inverses sine A).
 *
 * @warning
 *  Didn't use the DivideByZeroException.
 */
template <typename T>
inline T cscA(T a, T h)
{
    return h / a;
}

/**
 * Return the secant A (Inverses cosine A).
 *
 * @warning
 *  Didn't use the DivideByZeroException.
 */
template <typename T>
inline T secA(T b, T h)
{
    return h / b;
}

/**
 * Return the cotangent A (Inverses tangent A).
 *
 * @warning
 *  Didn't use the DivideByZeroException.
 */
template <typename T>
inline T cotA(T a, T b)
{
    return b / a;
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_TRIGONOMETRY_HPP__

