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
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

template <typename Duration> struct DurationString;

template <> struct DurationString<std::chrono::nanoseconds>
{ static constexpr char const * const value = "nano"; };
template <> struct DurationString<std::chrono::microseconds>
{ static constexpr char const * const value = "micro"; };
template <> struct DurationString<std::chrono::milliseconds>
{ static constexpr char const * const value = "milli"; };
template <> struct DurationString<std::chrono::seconds>
{ static constexpr char const * const value = "sec"; };
template <> struct DurationString<std::chrono::minutes>
{ static constexpr char const * const value = "min"; };
template <> struct DurationString<std::chrono::hours>
{ static constexpr char const * const value = "hour"; };

// TODO: CHECK THIS CLASS!!
template <typename Duration> struct DurationString
{
	static constexpr char const * const value = "duration";
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DURATIONSTRING_HPP__

