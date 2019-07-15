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
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

TBAG_CONSTEXPR char const * const    TBAG_NANO_DURATION_STRING =  "nano";
TBAG_CONSTEXPR char const * const   TBAG_MICRO_DURATION_STRING = "micro";
TBAG_CONSTEXPR char const * const   TBAG_MILLI_DURATION_STRING = "milli";
TBAG_CONSTEXPR char const * const     TBAG_SEC_DURATION_STRING =   "sec";
TBAG_CONSTEXPR char const * const     TBAG_MIN_DURATION_STRING =   "min";
TBAG_CONSTEXPR char const * const    TBAG_HOUR_DURATION_STRING =  "hour";
TBAG_CONSTEXPR char const * const TBAG_DEFAULT_DURATION_STRING =   "dur";

template <typename Duration>
inline TBAG_CONSTEXPR char const * getDurationString() TBAG_NOEXCEPT
{
    return TBAG_DEFAULT_DURATION_STRING;
}

// clang-format off
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::nanoseconds> () TBAG_NOEXCEPT { return TBAG_NANO_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::microseconds>() TBAG_NOEXCEPT { return TBAG_MICRO_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::milliseconds>() TBAG_NOEXCEPT { return TBAG_MILLI_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::seconds>     () TBAG_NOEXCEPT { return TBAG_SEC_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::minutes>     () TBAG_NOEXCEPT { return TBAG_MIN_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::hours>       () TBAG_NOEXCEPT { return TBAG_HOUR_DURATION_STRING; }
// clang-format on

// clang-format off
TBAG_API std::size_t toNanoseconds (std::string const & str);
TBAG_API std::size_t toMicroseconds(std::string const & str);
TBAG_API std::size_t toMilliseconds(std::string const & str);
TBAG_API std::size_t toSeconds     (std::string const & str);
TBAG_API std::size_t toMinutes     (std::string const & str);
TBAG_API std::size_t toHours       (std::string const & str);
// clang-format on

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRING_HPP__

