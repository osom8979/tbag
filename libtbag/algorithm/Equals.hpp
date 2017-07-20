/**
 * @file   Equals.hpp
 * @brief  Equals class prototype.
 * @author zer0
 * @date   2017-07-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_EQUALS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_EQUALS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdlib>
#include <cmath>

#include <type_traits>

#ifndef TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON
#define TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON  0.001
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace algorithm {

/**
 * Check the equals for the floating point types.
 */
template <typename T>
bool isFloatingPointEquals(T a, T b, T epsilon = TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON)
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");
    return (std::fabs(a - b) < epsilon);
}

template <typename T, typename E, bool __is_floating_point__> struct __EqualsBackend;

template <typename T, typename E>
struct __EqualsBackend<T, E, true> : public std::true_type
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");

    inline static bool __equals(T lh, T rh, E epsilon) TBAG_NOEXCEPT
    { return isFloatingPointEquals(lh, rh, epsilon); }
};

template <typename T, typename E>
struct __EqualsBackend<T, E, false> : public std::false_type
{
    static_assert(std::is_integral<T>::value, "Only integer types are supported.");

    inline static bool __equals(T lh, T rh, E UNUSED_PARAM(epsilon)) TBAG_NOEXCEPT
    { return lh == rh; }
};

/**
 * Equals class prototype.
 *
 * @author zer0
 * @date   2017-07-20
 */
template <typename T, typename E>
struct Equals : protected __EqualsBackend<T, E, std::is_floating_point<T>::value>
{
    using Parent = __EqualsBackend<T, E, std::is_floating_point<T>::value>;
    static_assert(std::is_pod<T>::value, "Only POD types are supported.");

    inline static bool equals(T lh, T rh, E epsilon = static_cast<E>(TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON)) TBAG_NOEXCEPT
    { return Parent::__equals(lh, rh, epsilon); }
};

template <typename T, typename E>
bool equals(T a, T b, E epsilon)
{
    return Equals<T, E>::equals(a, b, epsilon);
}

template <typename T>
bool equals(T a, T b)
{
    return Equals<T, T>::equals(a, b);
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_EQUALS_HPP__

