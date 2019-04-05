/**
 * @file   DurationString.hpp
 * @brief  DurationString class prototype.
 * @author zer0
 * @date   2016-09-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRING_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRING_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

TBAG_CONSTEXPR char const * const    __TBAG_NANO_DURATION_STRING__ =  "nano";
TBAG_CONSTEXPR char const * const   __TBAG_MICRO_DURATION_STRING__ = "micro";
TBAG_CONSTEXPR char const * const   __TBAG_MILLI_DURATION_STRING__ = "milli";
TBAG_CONSTEXPR char const * const     __TBAG_SEC_DURATION_STRING__ =   "sec";
TBAG_CONSTEXPR char const * const     __TBAG_MIN_DURATION_STRING__ =   "min";
TBAG_CONSTEXPR char const * const    __TBAG_HOUR_DURATION_STRING__ =  "hour";
TBAG_CONSTEXPR char const * const __TBAG_DEFAULT_DURATION_STRING__ =   "dur";

template <typename Duration>
inline TBAG_CONSTEXPR char const * getDurationString() TBAG_NOEXCEPT
{
    return __TBAG_DEFAULT_DURATION_STRING__;
}

// clang-format off
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::nanoseconds> () TBAG_NOEXCEPT { return __TBAG_NANO_DURATION_STRING__; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::microseconds>() TBAG_NOEXCEPT { return __TBAG_MICRO_DURATION_STRING__; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::milliseconds>() TBAG_NOEXCEPT { return __TBAG_MILLI_DURATION_STRING__; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::seconds>     () TBAG_NOEXCEPT { return __TBAG_SEC_DURATION_STRING__; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::minutes>     () TBAG_NOEXCEPT { return __TBAG_MIN_DURATION_STRING__; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::hours>       () TBAG_NOEXCEPT { return __TBAG_HOUR_DURATION_STRING__; }
// clang-format on

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRING_HPP__

