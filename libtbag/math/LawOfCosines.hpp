/**
 * @file   LawOfCosines.hpp
 * @brief  LawOfCosines class prototype.
 * @author zer0
 * @date   2019-05-02
 *
 * @see <https://en.wikipedia.org/wiki/Law_of_cosines>
 * @see <https://ko.wikipedia.org/wiki/%EC%BD%94%EC%82%AC%EC%9D%B8_%EB%B2%95%EC%B9%99>
 *
 * @translate{ko, 코사인 법칙은 삼각형의 두 변과 그 사잇각을 알 때 남은 한 변을 구하거나, 세 변을 알 때 세 각을 구하는 데 사용될 수 있다.}
 *
 * @remarks
 *  @code
 *          C
 *          *
 *         /|
 *     b  / |
 *       /  | a
 *      /   |
 *   A *----* B
 *       c
 *  @endcode
 *  - <code>A</code> is internal angle of between <code>b</code> and <code>c</code>.
 *  - <code>B</code> is internal angle of between <code>a</code> and <code>c</code>.
 *  - <code>C</code> is internal angle of between <code>a</code> and <code>b</code>.
 *  - <code>a</code> is the distance between <code>B</code> and <code>C</code>.
 *  - <code>b</code> is the distance between <code>A</code> and <code>C</code>.
 *  - <code>c</code> is the distance between <code>A</code> and <code>B</code>.
 *  The law of cosines states:
 *  \f[
 *   c^{2} = a^{2} + b^{2} - 2ab\cos{C}
 *  \f]
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_LAWOFCOSINES_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_LAWOFCOSINES_HPP__

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

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_LAWOFCOSINES_HPP__

