/**
 * @file   constant.hpp
 * @brief  Mathematical constant.
 * @author zer0
 * @date   2016-04-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_CONSTANT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_CONSTANT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>

/**
 * @defgroup __DOXYGEN_GROUP__MATHEMATICAL_CONSTANT__ Mathematical constant
 * @remarks
 *  Even though these might be more useful as long doubles, POSIX requires
 *  that they be double-precision literals.
 * @brief Mathematical constant.
 * @{
 */

#define MATH_CONSTANT_E         2.71828182845904523536028747135266250   ///< e
#define MATH_CONSTANT_LOG2E     1.44269504088896340735992468100189214   ///< log2(e)
#define MATH_CONSTANT_LOG10E    0.434294481903251827651128918916605082  ///< log10(e)
#define MATH_CONSTANT_LN2       0.693147180559945309417232121458176568  ///< loge(2)
#define MATH_CONSTANT_LN10      2.30258509299404568401799145468436421   ///< loge(10)
#define MATH_CONSTANT_PI        3.14159265358979323846264338327950288   ///< pi
#define MATH_CONSTANT_PI_2      1.57079632679489661923132169163975144   ///< pi/2
#define MATH_CONSTANT_PI_4      0.785398163397448309615660845819875721  ///< pi/4
#define MATH_CONSTANT_1_PI      0.318309886183790671537767526745028724  ///< 1/pi
#define MATH_CONSTANT_2_PI      0.636619772367581343075535053490057448  ///< 2/pi
#define MATH_CONSTANT_2_SQRTPI  1.12837916709551257389615890312154517   ///< 2/sqrt(pi)
#define MATH_CONSTANT_SQRT2     1.41421356237309504880168872420969808   ///< sqrt(2)
#define MATH_CONSTANT_SQRT1_2   0.707106781186547524400844362104849039  ///< 1/sqrt(2)

/**
 * @}
 */

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

// EMPTY.

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_CONSTANT_HPP__

