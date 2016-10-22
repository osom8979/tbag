/**
 * @file   vector.hpp
 * @brief  vector class prototype.
 * @author zer0
 * @date   2016-08-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_VECTOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_VECTOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point.hpp>
#include <cmath>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

template <typename T>
struct Vector2
{
    T x, y;
};

template <typename T>
struct Vector3
{
    T x, y, z;
};

template <typename T, std::size_t N>
struct VectorN
{
    T v[N];
};

/**
 * @remarks
 *  \f[
 *   | \vec{v} | = \sqrt{x^2 \times y^2 \times z^2}
 *  \f]
 */
template <typename T>
inline T getScalar(Vector3<T> const & v)
{
    return std::sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

/**
 * @remarks
 *  \f[
 *   | \vec{v} | = \sqrt{x^2 \times y^2}
 *  \f]
 */
template <typename T>
inline T getScalar(Vector2<T> const & v)
{
    return std::sqrt((v.x * v.x) + (v.y * v.y));
}

/**
 * Obtain the dot-product.
 *
 * @translate{ko, 내적(스칼라곱)을 획득한다.}
 *
 * @remarks
 *  \f[
 *   \vec{v_1} \cdot \vec{v_2} = (x_1 \times x_2) + (y_1 \times y_2) + (z_1 \times z_2)
 *  \f]
 */
template <typename T>
inline T getDotProduct(Vector3<T> const & v1, Vector3<T> const & v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
}

/**
 * Obtain the dot-product.
 *
 * @translate{ko, 내적(스칼라곱)을 획득한다.}
 *
 * @remarks
 *  \f[
 *   \vec{v_1} \cdot \vec{v_2} = (x_1 \times x_2) + (y_1 \times y_2)
 *  \f]
 */
template <typename T>
inline T getDotProduct(Vector2<T> const & v1, Vector2<T> const & v2)
{
    return (v1.x * v2.x) + (v1.y * v2.y);
}


template <typename T1, typename T2>
inline T1 getDotProductWithCosineResult(T1 s1, T1 s2, T2 cosine_result)
{
    return s1 * s2 * cosine_result;
}

/**
 * Obtain the dot-product.
 *
 * @translate{ko, 내적(스칼라곱)을 획득한다.}
 *
 * @param s1    [in] Vector 01 scalar.
 * @param s2    [in] Vector 02 scalar.
 * @param theta [in] Cosine parameter.
 *
 * @remarks
 *  \f[
 *   \vec{v_1} \cdot \vec{v_2} = |\vec{v_1}| \cdot |\vec{v_2}| \cdot cos(\theta)
 *  \f]
 */
template <typename T>
inline T getDotProduct(T s1, T s2, double theta)
{
    return getDotProductWithCosineResult(s1, s2, std::cos(theta));
}

/**
 * Obtain the cross-product.
 *
 * @translate{ko, 외적(벡터곱)을 획득한다.}
 *
 * @remarks
 *  두 벡터에 모두 수직인 벡터를 구한다.
 *  \f[
 *   \vec{v_1} \times \vec{v_2} = [(y_1 z_2 - z_1 y_2), (z_1 x_2 - x_1 z_2), (x_1 y_2 - y_1 x_2)]
 *  \f]
 *
 * @warning
 *  행렬을 획득하게 되는 외적(Outer-product)과는 다르다.
 *
 * @see <https://en.wikipedia.org/wiki/Cross_product>
 */
template <typename T>
inline Vector3<T>
getCrossProduct(Vector3<T> const & v1, Vector3<T> const & v2)
{
    return Vector3<T>{(v1.y * v2.z) - (v1.z * v2.y)
                    , (v1.z * v2.x) - (v1.x * v2.z)
                    , (v1.x * v2.y) - (v1.y * v2.x)};
}

/**
 * Obtain the relative position of point to line.
 *
 * @translate{ko, 라인을 기준으로 점의 상대적 위치를 확인한다.}
 *
 * @param p0    [in] Point 0 of line.
 * @param p1    [in] Point 1 of line.
 * @param check [in] Check point.
 *
 * @return If a positive number, the left side of the vector. @n
 *         if a negative number, the right side of the vector. @n
 *         if 0 is on the line.
 *
 * @remarks
 *  Example:
 *  @code
 *    |
 *    |         * p2
 *    |
 *    |  p0 *------>* p1
 *    |
 *   -+------------------
 *    |
 *  @endcode
 *  \f[
 *   \vec{v_1} = p1 - p0 = [p1_x - p0_x, p1_y - p0_y, 0]
 *   \vec{v_2} = p2 - p0 = [p2_x - p0_x, p2_y - p0_y, 0]
 *   \vec{v_1} \times \vec{v_2} = [0, 0, ((p1_x - p0_x) * (p2_y - p0_y) - (p1_y - p0_y) * (p2_x - p0_x))]
 *  \f]
 *  외적(Cross-Product)의 오늘손 법칙을 사용하여, Vector(p1 - p0)과 Vector(p2 - p0)의 위치에 따라 음수 또는 양수로 변하는 성질을 이용한다.
 */
template <typename T>
inline T
getRelativePositionOfPointToLine(geometry::BasePoint<T> const & p0
                               , geometry::BasePoint<T> const & p1
                               , geometry::BasePoint<T> const & check)
{
    return ((p1.x - p0.x) * (check.y - p0.y)) - ((p1.y - p0.y) * (check.x - p0.x));
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_VECTOR_HPP__

