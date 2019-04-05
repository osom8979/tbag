/**
 * @file   Rounding.hpp
 * @brief  Rounding class prototype.
 * @author zer0
 * @date   2018-07-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MATH_ROUNDING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MATH_ROUNDING_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cmath>
#include <cfenv>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace math {

// Access to Floating-point environment
// http://www.cplusplus.com/reference/cfenv/FENV_ACCESS/
#if defined(TBAG_ENABLE_STDC_FENV_ACCESS)
// If set to on, the program informs the compiler that it might access the
// floating-point environment to test its status flags (exceptions) or run
// under control modes other than the one by default.
//#pragma STDC FENV_ACCESS ON
#endif

#if defined(TBAG_DISABLE_STDC_FENV_ACCESS)
// If set to off, the compiler may perform certain optimizations that can
// subvert these tests and mode changes, and thus accessing the
// floating-point environment in the cases described above, causes undefined
// behavior.
//#pragma STDC FENV_ACCESS OFF
#endif

struct RoundingDirectionMode
{
    /**
     * std::rint differs from std::round in that halfway cases are rounded to even
     * rather than away from zero.
     */
    inline void nearest() { std::fesetround(FE_TOWARDZERO); }

    /** std::rint is equivalent to std::floor */
    inline void down() { std::fesetround(FE_DOWNWARD); }

    /** std::rint is equivalent to std::ceil */
    inline void up() { std::fesetround(FE_UPWARD); }

    /** std::rint is equivalent to std::trunc */
    inline void zero() { std::fesetround(FE_TOWARDZERO); }

    /** Get rounding direction mode. */
    inline int get() { return std::fegetround(); }
};

template <typename T>
struct RoundingCastCommon : public RoundingDirectionMode
{
    using ResultType = T;

    static_assert(!std::is_unsigned<ResultType>::value, "ResultType must not be a unsigned type.");

    // clang-format off
    static ResultType   up(      float v) { return static_cast<ResultType>(ceilf (v)); }
    static ResultType   up(     double v) { return static_cast<ResultType>(ceil  (v)); }
    static ResultType   up(long double v) { return static_cast<ResultType>(ceill (v)); }
    static ResultType down(      float v) { return static_cast<ResultType>(floorf(v)); }
    static ResultType down(     double v) { return static_cast<ResultType>(floor (v)); }
    static ResultType down(long double v) { return static_cast<ResultType>(floorl(v)); }
    static ResultType zero(      float v) { return static_cast<ResultType>(truncf(v)); }
    static ResultType zero(     double v) { return static_cast<ResultType>(trunc (v)); }
    static ResultType zero(long double v) { return static_cast<ResultType>(truncl(v)); }
    // clang-format on
};

/**
 * Rounding helper.
 *
 * @author zer0
 * @date   2018-07-11
 */
template <typename T>
struct RoundingCast : public RoundingCastCommon<T>
{
    using ResultType = typename RoundingCastCommon<T>::ResultType;

    // clang-format off
    static ResultType  rint(      float v) { return static_cast<ResultType>(std::rintf (v)); }
    static ResultType  rint(     double v) { return static_cast<ResultType>(std::rint  (v)); }
    static ResultType  rint(long double v) { return static_cast<ResultType>(std::rintl (v)); }
    static ResultType round(      float v) { return static_cast<ResultType>(std::roundf(v)); }
    static ResultType round(     double v) { return static_cast<ResultType>(std::round (v)); }
    static ResultType round(long double v) { return static_cast<ResultType>(std::roundl(v)); }
    // clang-format on
};

template <>
struct RoundingCast<long int> : public RoundingCastCommon<long int>
{
    using ResultType = typename RoundingCastCommon<long int>::ResultType;

    // clang-format off
    static ResultType  rint(      float v) { return static_cast<ResultType>(std::lrintf (v)); }
    static ResultType  rint(     double v) { return static_cast<ResultType>(std::lrint  (v)); }
    static ResultType  rint(long double v) { return static_cast<ResultType>(std::lrintl (v)); }
    static ResultType round(      float v) { return static_cast<ResultType>(std::lroundf(v)); }
    static ResultType round(     double v) { return static_cast<ResultType>(std::lround (v)); }
    static ResultType round(long double v) { return static_cast<ResultType>(std::lroundl(v)); }
    // clang-format on
};

template <>
struct RoundingCast<long long int> : public RoundingCastCommon<long long int>
{
    using ResultType = typename RoundingCastCommon<long long int>::ResultType;

    // clang-format off
    static ResultType  rint(      float v) { return static_cast<ResultType>(std::llrintf (v)); }
    static ResultType  rint(     double v) { return static_cast<ResultType>(std::llrint  (v)); }
    static ResultType  rint(long double v) { return static_cast<ResultType>(std::llrintl (v)); }
    static ResultType round(      float v) { return static_cast<ResultType>(std::llroundf(v)); }
    static ResultType round(     double v) { return static_cast<ResultType>(std::llround (v)); }
    static ResultType round(long double v) { return static_cast<ResultType>(std::llroundl(v)); }
    // clang-format on
};

template <typename ResultType, typename T>
inline ResultType round(T v)
{
    static_assert(std::is_floating_point<T>::value, "T must be floating point.");
    return RoundingCast<ResultType>::round(v);
}

template <typename ResultType, typename T>
inline ResultType rint(T v)
{
    static_assert(std::is_floating_point<T>::value, "T must be floating point.");
    return RoundingCast<ResultType>::rint(v);
}

template <typename ResultType, typename T>
inline ResultType ceil(T v)
{
    static_assert(std::is_floating_point<T>::value, "T must be floating point.");
    return RoundingCast<ResultType>::up(v);
}

template <typename ResultType, typename T>
inline ResultType floor(T v)
{
    static_assert(std::is_floating_point<T>::value, "T must be floating point.");
    return RoundingCast<ResultType>::down(v);
}

template <typename ResultType, typename T>
inline ResultType trunc(T v)
{
    static_assert(std::is_floating_point<T>::value, "T must be floating point.");
    return RoundingCast<ResultType>::zero(v);
}

} // namespace math

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MATH_ROUNDING_HPP__

