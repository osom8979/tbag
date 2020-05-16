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
#include <libtbag/ErrPair.hpp>

#include <chrono>
#include <string>

#define TBAG_NANO_DURATION_STRING  "ns"
#define TBAG_MICRO_DURATION_STRING "us"
#define TBAG_MILLI_DURATION_STRING "ms"
#define TBAG_SEC_DURATION_STRING   "s"
#define TBAG_MIN_DURATION_STRING   "min"
#define TBAG_HOUR_DURATION_STRING  "h"
#define TBAG_DAY_DURATION_STRING   "d"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

template <typename Duration>
inline TBAG_CONSTEXPR char const * getDurationString() TBAG_NOEXCEPT
{
    return "";
}

TBAG_CONSTEXPR char const * const NANO_DURATION_STRING  = TBAG_NANO_DURATION_STRING ;
TBAG_CONSTEXPR char const * const MICRO_DURATION_STRING = TBAG_MICRO_DURATION_STRING;
TBAG_CONSTEXPR char const * const MILLI_DURATION_STRING = TBAG_MILLI_DURATION_STRING;
TBAG_CONSTEXPR char const * const SEC_DURATION_STRING   = TBAG_SEC_DURATION_STRING  ;
TBAG_CONSTEXPR char const * const MIN_DURATION_STRING   = TBAG_MIN_DURATION_STRING  ;
TBAG_CONSTEXPR char const * const HOUR_DURATION_STRING  = TBAG_HOUR_DURATION_STRING ;
TBAG_CONSTEXPR char const * const DAY_DURATION_STRING   = TBAG_DAY_DURATION_STRING  ;

// clang-format off
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::nanoseconds> () TBAG_NOEXCEPT { return NANO_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::microseconds>() TBAG_NOEXCEPT { return MICRO_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::milliseconds>() TBAG_NOEXCEPT { return MILLI_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::seconds>     () TBAG_NOEXCEPT { return SEC_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::minutes>     () TBAG_NOEXCEPT { return MIN_DURATION_STRING; }
template <> inline TBAG_CONSTEXPR char const * getDurationString<std::chrono::hours>       () TBAG_NOEXCEPT { return HOUR_DURATION_STRING; }
// clang-format on

using ErrSize = libtbag::ErrPair<std::size_t>;

// clang-format off
TBAG_API ErrSize parseNanoseconds (std::string const & str);
TBAG_API ErrSize parseMicroseconds(std::string const & str);
TBAG_API ErrSize parseMilliseconds(std::string const & str);
TBAG_API ErrSize parseSeconds     (std::string const & str);
TBAG_API ErrSize parseMinutes     (std::string const & str);
TBAG_API ErrSize parseHours       (std::string const & str);
// clang-format on

template <typename Duration>
std::string getTimeText(Duration const & duration)
{
    return std::to_string(duration.count()) + getDurationString<Duration>();
}

TBAG_API std::string getDayText(std::size_t day);

// clang-format off
TBAG_API std::string getUpperTimeText(std::chrono::nanoseconds  const & duration);
TBAG_API std::string getUpperTimeText(std::chrono::microseconds const & duration);
TBAG_API std::string getUpperTimeText(std::chrono::milliseconds const & duration);
TBAG_API std::string getUpperTimeText(std::chrono::seconds      const & duration);
TBAG_API std::string getUpperTimeText(std::chrono::minutes      const & duration);
TBAG_API std::string getUpperTimeText(std::chrono::hours        const & duration);
TBAG_API std::string getUpperTimeTextByNanoseconds (std::size_t count);
TBAG_API std::string getUpperTimeTextByMicroseconds(std::size_t count);
TBAG_API std::string getUpperTimeTextByMilliseconds(std::size_t count);
TBAG_API std::string getUpperTimeTextBySeconds     (std::size_t count);
TBAG_API std::string getUpperTimeTextByMinutes     (std::size_t count);
TBAG_API std::string getUpperTimeTextByHours       (std::size_t count);
TBAG_API std::string getUpperTimeTextByDays        (std::size_t count);
// clang-format on

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRING_HPP__

