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

#define TBAG_NANO_DURATION_STRING   "ns"
#define TBAG_MICRO_DURATION_STRING  "us"
#define TBAG_MILLI_DURATION_STRING  "ms"
#define TBAG_SEC_DURATION_STRING    "s"
#define TBAG_MIN_DURATION_STRING    "min"
#define TBAG_HOUR_DURATION_STRING   "h"
#define TBAG_DAY_DURATION_STRING    "d"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

template <typename Duration>
inline TBAG_CONSTEXPR char const * getDurationString() TBAG_NOEXCEPT
{
    return "";
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
TBAG_API std::size_t toDays        (std::string const & str);
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

