/**
 * @file   Time.hpp
 * @brief  Time class prototype.
 * @author zer0
 * @date   2016-04-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIME_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/string/Strings.hpp>

#include <ctime>

#include <string>
#include <vector>
#include <chrono>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/**
 * Long date/time format.
 *
 * @warning
 *  Not ISO/IEC 8601:2004
 *
 * @remarks
 *  @code
 *   YYYY-MM-DDThh:mm:ss
 *  @endcode
 */
constexpr char    const * const TIMESTAMP_LONG_FORMAT      =  "%Y-%m-%dT%H:%M:%S";
constexpr wchar_t const * const WIDE_TIMESTAMP_LONG_FORMAT = L"%Y-%m-%dT%H:%M:%S";

template <typename CharType = char>
constexpr CharType const * const getDefaultTimestampLongFormat() noexcept;

template <>
constexpr char const * const getDefaultTimestampLongFormat<char>() noexcept
{
    return TIMESTAMP_LONG_FORMAT;
}

template <>
constexpr wchar_t const * const getDefaultTimestampLongFormat<wchar_t>() noexcept
{
    return WIDE_TIMESTAMP_LONG_FORMAT;
}

/**
 * Short date/time format.
 *
 * @warning
 *  Not ISO/IEC 8601:2004
 *
 * @remarks
 *  @code
 *   YYYYMMDDThhmmss
 *  @endcode
 */
constexpr char    const * const TIMESTAMP_SHORT_FORMAT      =  "%Y%m%dT%H%M%S";
constexpr wchar_t const * const WIDE_TIMESTAMP_SHORT_FORMAT = L"%Y%m%dT%H%M%S";

template <typename CharType>
constexpr CharType const * const getDefaultTimestampShortFormat() noexcept;

template <>
constexpr char const * const getDefaultTimestampShortFormat<char>() noexcept
{
    return TIMESTAMP_SHORT_FORMAT;
}

template <>
constexpr wchar_t const * const getDefaultTimestampShortFormat<wchar_t>() noexcept
{
    return WIDE_TIMESTAMP_SHORT_FORMAT;
}

/** Years since 1900 */
constexpr int const YEARS_SINCE = 1900;

/** Months since january: 0-11 */
constexpr int const MONTHS_SINCE = 1;

/** millisecond part of the second 0-999. */
int getMillisec(std::chrono::system_clock::time_point const & time);

std::string  getMillisecMbs(std::chrono::system_clock::time_point const & time);
std::wstring getMillisecWcs(std::chrono::system_clock::time_point const & time);

template <typename CharType>
void getMillisecString(std::chrono::system_clock::time_point const & time
                     , std::basic_string<CharType> & result);

template <>
inline void getMillisecString<char>(std::chrono::system_clock::time_point const & time
                                  , std::basic_string<char> & result)
{
    result = getMillisecMbs(time);
}

template <>
inline void getMillisecString<wchar_t>(std::chrono::system_clock::time_point const & time
                                     , std::basic_string<wchar_t> & result)
{
    result = getMillisecWcs(time);
}

std::chrono::system_clock::time_point getNowSystemClock() noexcept;
time_t getTime(std::chrono::system_clock::time_point const & time_point) noexcept;

/** Obtain current time. */
time_t getCurrentTime() noexcept;
tm * getGmtTime(time_t const & t) noexcept;
tm * getLocalTime(time_t const & t) noexcept;

std::string  getFormatString(std::string  const & format, tm const * t, std::size_t allocate_size = 128);
std::wstring getFormatString(std::wstring const & format, tm const * t, std::size_t allocate_size = 128);

// ----------
// Time unit.
// ----------

template <typename Duration> struct TimeUnit;

template <> struct TimeUnit<std::chrono::nanoseconds>
{ static constexpr char const * const value = "nano"; };
template <> struct TimeUnit<std::chrono::microseconds>
{ static constexpr char const * const value = "micro"; };
template <> struct TimeUnit<std::chrono::milliseconds>
{ static constexpr char const * const value = "milli"; };
template <> struct TimeUnit<std::chrono::seconds>
{ static constexpr char const * const value = "sec"; };
template <> struct TimeUnit<std::chrono::minutes>
{ static constexpr char const * const value = "min"; };
template <> struct TimeUnit<std::chrono::hours>
{ static constexpr char const * const value = "hour"; };

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIME_HPP__

