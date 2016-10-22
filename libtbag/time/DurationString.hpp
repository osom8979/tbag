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

template <typename Duration> struct DurationString;

#ifndef TBAG_TIME_DURATIONSTRING_IMPL
# if defined(TBAG_HAS_CONSTEXPR)
#  define TBAG_TIME_DURATIONSTRING_IMPL(duration, name) \
    template <> struct DurationString<duration>         \
    { static TBAG_CONSTEXPR char const * const value = name; };
# else
#  define TBAG_TIME_DURATIONSTRING_IMPL(duration, name) \
    template <> struct DurationString<duration>         \
    { static char const * const value; };               \
    const char * const DurationString<duration>::value = name;
# endif
#endif // TBAG_TIME_DURATIONSTRING_IMPL

TBAG_CONSTEXPR char const * const    __TBAG_NANO_DURATION_STRING__ =  "nano";
TBAG_CONSTEXPR char const * const   __TBAG_MICRO_DURATION_STRING__ = "micro";
TBAG_CONSTEXPR char const * const   __TBAG_MILLI_DURATION_STRING__ = "milli";
TBAG_CONSTEXPR char const * const     __TBAG_SEC_DURATION_STRING__ =   "sec";
TBAG_CONSTEXPR char const * const     __TBAG_MIN_DURATION_STRING__ =   "min";
TBAG_CONSTEXPR char const * const    __TBAG_HOUR_DURATION_STRING__ =  "hour";
TBAG_CONSTEXPR char const * const __TBAG_DEFAULT_DURATION_STRING__ =   "dur";

TBAG_TIME_DURATIONSTRING_IMPL(std::chrono::nanoseconds,   __TBAG_NANO_DURATION_STRING__);
TBAG_TIME_DURATIONSTRING_IMPL(std::chrono::microseconds, __TBAG_MICRO_DURATION_STRING__);
TBAG_TIME_DURATIONSTRING_IMPL(std::chrono::milliseconds, __TBAG_MILLI_DURATION_STRING__);
TBAG_TIME_DURATIONSTRING_IMPL(std::chrono::seconds,        __TBAG_SEC_DURATION_STRING__);
TBAG_TIME_DURATIONSTRING_IMPL(std::chrono::minutes,        __TBAG_MIN_DURATION_STRING__);
TBAG_TIME_DURATIONSTRING_IMPL(std::chrono::hours,         __TBAG_HOUR_DURATION_STRING__);

#if defined(TBAG_HAS_CONSTEXPR)
template <typename Duration> struct DurationString
{ static TBAG_CONSTEXPR char const * const value = __TBAG_DEFAULT_DURATION_STRING__; };
#else
template <typename Duration> struct DurationString
{ static char const * const value; };
template <typename Duration>
const char * const DurationString<Duration>::value = __TBAG_DEFAULT_DURATION_STRING__;
#endif

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRING_HPP__

