/**
 * @file   Average.hpp
 * @brief  Average class prototype.
 * @author zer0
 * @date   2017-04-08
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_AVERAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_AVERAGE_HPP__

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
 * Step by step average.
 *
 * @translate{ko, 점진적으로 평균을 구할 수 있다.}
 *
 * @remarks
 *  \f[
 *   \begin{align}
 *   c_N &= \frac{1}{N}(x_1 + x_2 + x_3 + ... + x_N) \\
 *   &= \frac{1}{N} \sum_{i=1}^{N}x_i \\
 *   &= \frac{1}{N} \left( \sum_{i=1}^{N-1}x_i + x_N \right) \\
 *   &= \frac{N-1}{N} \frac{1}{N-1} \sum_{i=1}^{N-1}x_i + \frac{1}{N}x_N
 *   \end{align}
 *  \f]
 *  아래와 같이 \f$ c_{N-1} \f$ 를 구할 수 있다. @n
 *  \f[
 *   c_{N-1} = \frac{1}{N-1} \sum_{i=1}^{N-1}x_i
 *  \f]
 *  아래와 같이 정의할 경우, @n
 *  \f[
 *   \alpha = \frac{N-1}{N}
 *  \f]
 *  아래와 같이 유도할 수 있다. @n
 *  \f[
 *   1 - \alpha = \frac{1}{N}
 *  \f]
 *  최종적으로 아래의 식이 도출된다. @n
 *  \f[
 *   \begin{align}
 *   c_N &= \frac{N-1}{N} \frac{1}{N-1} \sum_{i=1}^{N-1}x_i + \frac{1}{N}x_N \\
 *   &= \frac{N-1}{N} c_{N-1} + \frac{1}{N}x_N \\
 *   &= \alpha c_{N-1} + (1 - \alpha)x_N
 *   \end{align}
 *  \f]
 */
template <typename Base, typename Result = Base, typename Integer = int>
Result averageStep(Base prev_average, Base current, Integer n)
{
    Result alpha = (static_cast<Result>(n) - static_cast<Result>(1)) / static_cast<Result>(n);
    return (alpha * static_cast<Result>(prev_average)) +
            ((static_cast<Result>(1) - alpha) * static_cast<Result>(current));
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_AVERAGE_HPP__

