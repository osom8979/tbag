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
#define TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON  1e-5
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace algorithm {

/**
 * Check the equals for the floating point types.
 */
template <typename T>
bool isFloatingPointEquals(T a, T b, T epsilon = TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON) TBAG_NOEXCEPT
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");
    return (std::fabs(a - b) <= epsilon);
}

/**
 * Check if the first number is greater than or equal to the second number.
 */
template <typename T>
bool isFloatingPointGreaterOrEqual(T a, T b, T epsilon = TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON) TBAG_NOEXCEPT
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");
    return (a > b) || isFloatingPointEquals(a, b, epsilon);
}

/**
 * Check if the first number is less than or equal to the second number.
 */
template <typename T>
bool isFloatingPointLessOrEqual(T a, T b, T epsilon = TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON) TBAG_NOEXCEPT
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");
    return (a < b) || isFloatingPointEquals(a, b, epsilon);
}

// -------
// Equals.
// -------

template <typename T, typename E, bool __is_floating_point__> struct __EqualsBackend;

template <typename T, typename E>
struct __EqualsBackend<T, E, true> : public std::true_type
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");

    inline static bool test(T lh, T rh, E epsilon) TBAG_NOEXCEPT
    { return isFloatingPointEquals(lh, rh, epsilon); }
};

template <typename T, typename E>
struct __EqualsBackend<T, E, false> : public std::false_type
{
    static_assert(std::is_integral<T>::value, "Only integer types are supported.");

    inline static bool test(T lh, T rh, E UNUSED_PARAM(epsilon)) TBAG_NOEXCEPT
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

    TBAG_CONSTEXPR static bool const value = Parent::value;

    TBAG_CONSTEXPR static bool isFloatingPoint() TBAG_NOEXCEPT { return value; }

    inline static bool test(T lh, T rh, E epsilon = static_cast<E>(TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON)) TBAG_NOEXCEPT
    { return Parent::test(lh, rh, epsilon); }
};

// ----------------
// GreaterOrEquals.
// ----------------

template <typename T, typename E, bool __is_floating_point__> struct __GreaterOrEqualsBackend;

template <typename T, typename E>
struct __GreaterOrEqualsBackend<T, E, true> : public std::true_type
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");

    inline static bool test(T lh, T rh, E epsilon) TBAG_NOEXCEPT
    { return isFloatingPointGreaterOrEqual(lh, rh, epsilon); }
};

template <typename T, typename E>
struct __GreaterOrEqualsBackend<T, E, false> : public std::false_type
{
    static_assert(std::is_integral<T>::value, "Only integer types are supported.");

    inline static bool test(T lh, T rh, E UNUSED_PARAM(epsilon)) TBAG_NOEXCEPT
    { return lh >= rh; }
};

/**
 * GreaterOrEquals class prototype.
 *
 * @author zer0
 * @date   2017-12-14
 */
template <typename T, typename E>
struct GreaterOrEquals : protected __GreaterOrEqualsBackend<T, E, std::is_floating_point<T>::value>
{
    using Parent = __GreaterOrEqualsBackend<T, E, std::is_floating_point<T>::value>;
    static_assert(std::is_pod<T>::value, "Only POD types are supported.");

    TBAG_CONSTEXPR static bool const value = Parent::value;

    TBAG_CONSTEXPR static bool isFloatingPoint() TBAG_NOEXCEPT { return value; }

    inline static bool test(T lh, T rh, E epsilon = static_cast<E>(TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON)) TBAG_NOEXCEPT
    { return Parent::test(lh, rh, epsilon); }
};

// -------------
// LessOrEquals.
// -------------

template <typename T, typename E, bool __is_floating_point__> struct __LessOrEqualsBackend;

template <typename T, typename E>
struct __LessOrEqualsBackend<T, E, true> : public std::true_type
{
    static_assert(std::is_floating_point<T>::value, "Only floating point types are supported.");

    inline static bool test(T lh, T rh, E epsilon) TBAG_NOEXCEPT
    { return isFloatingPointLessOrEqual(lh, rh, epsilon); }
};

template <typename T, typename E>
struct __LessOrEqualsBackend<T, E, false> : public std::false_type
{
    static_assert(std::is_integral<T>::value, "Only integer types are supported.");

    inline static bool test(T lh, T rh, E UNUSED_PARAM(epsilon)) TBAG_NOEXCEPT
    { return lh <= rh; }
};

/**
 * LessOrEquals class prototype.
 *
 * @author zer0
 * @date   2017-12-14
 */
template <typename T, typename E>
struct LessOrEquals : protected __LessOrEqualsBackend<T, E, std::is_floating_point<T>::value>
{
    using Parent = __LessOrEqualsBackend<T, E, std::is_floating_point<T>::value>;
    static_assert(std::is_pod<T>::value, "Only POD types are supported.");

    TBAG_CONSTEXPR static bool const value = Parent::value;

    TBAG_CONSTEXPR static bool isFloatingPoint() TBAG_NOEXCEPT { return value; }

    inline static bool test(T lh, T rh, E epsilon = static_cast<E>(TBAG_DEFAULT_FLOATING_POINT_EQUALS_EPSILON)) TBAG_NOEXCEPT
    { return Parent::test(lh, rh, epsilon); }
};

// ----------
// Front-end.
// ----------

template <typename T, typename E>
inline bool equals(T a, T b, E epsilon) TBAG_NOEXCEPT
{
    return Equals<T, E>::test(a, b, epsilon);
}

template <typename T>
inline bool equals(T a, T b) TBAG_NOEXCEPT
{
    return Equals<T, T>::test(a, b);
}

template <typename T, typename E>
inline bool lessOrEquals(T a, T b, E epsilon) TBAG_NOEXCEPT
{
    return LessOrEquals<T, E>::test(a, b, epsilon);
}

template <typename T>
inline bool lessOrEquals(T a, T b) TBAG_NOEXCEPT
{
    return LessOrEquals<T, T>::test(a, b);
}

template <typename T, typename E>
inline bool greaterOrEquals(T a, T b, E epsilon) TBAG_NOEXCEPT
{
    return GreaterOrEquals<T, E>::test(a, b, epsilon);
}

template <typename T>
inline bool greaterOrEquals(T a, T b) TBAG_NOEXCEPT
{
    return GreaterOrEquals<T, T>::test(a, b);
}

} // namespace algorithm

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ALGORITHM_EQUALS_HPP__

