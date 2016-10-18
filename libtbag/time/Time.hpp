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
#include <libtbag/macro/attributes.hpp>

#include <ctime>
#include <string>
#include <chrono>

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
TBAG_EXPORTS int getMillisec(std::chrono::system_clock::time_point const & time);

TBAG_EXPORTS std::string  getMillisecMbs(std::chrono::system_clock::time_point const & time);
TBAG_EXPORTS std::wstring getMillisecWcs(std::chrono::system_clock::time_point const & time);

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

TBAG_EXPORTS std::chrono::system_clock::time_point getNowSystemClock() noexcept;
TBAG_EXPORTS time_t getTime(std::chrono::system_clock::time_point const & time_point) noexcept;

/** Obtain current time. */
TBAG_EXPORTS time_t getCurrentTime() noexcept;
TBAG_EXPORTS bool getGmtTime(time_t const & t, tm * output);
TBAG_EXPORTS bool getLocalTime(time_t const & t, tm * output);

TBAG_EXPORTS std::string  getFormatString(std::string  const & format, tm const * t, std::size_t allocate_size = 128);
TBAG_EXPORTS std::wstring getFormatString(std::wstring const & format, tm const * t, std::size_t allocate_size = 128);

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIME_HPP__

