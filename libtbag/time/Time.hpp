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
#include <libtbag/predef.hpp>

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
TBAG_CONSTEXPR char const * const TIMESTAMP_LONG_FORMAT = "%Y-%m-%dT%H:%M:%S";

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
TBAG_CONSTEXPR char const * const TIMESTAMP_SHORT_FORMAT = "%Y%m%dT%H%M%S";

/** Years since 1900 */
TBAG_CONSTEXPR int const YEARS_SINCE = 1900;

/** Months since january: 0-11 */
TBAG_CONSTEXPR int const MONTHS_SINCE = 1;

/** millisecond part of the second 0-999. */
TBAG_EXPORTS int getMillisec(std::chrono::system_clock::time_point const & time);

TBAG_EXPORTS std::string getMillisecMbs(std::chrono::system_clock::time_point const & time);
TBAG_EXPORTS void getMillisecString(std::chrono::system_clock::time_point const & time, std::string & result);

TBAG_EXPORTS std::chrono::system_clock::time_point getNowSystemClock() TBAG_NOEXCEPT;
TBAG_EXPORTS time_t getTime(std::chrono::system_clock::time_point const & time_point) TBAG_NOEXCEPT;

/** Obtain current time. */
TBAG_EXPORTS time_t getCurrentTime() TBAG_NOEXCEPT;
TBAG_EXPORTS bool getGmtTime(time_t const & t, tm * output);
TBAG_EXPORTS bool getLocalTime(time_t const & t, tm * output);

TBAG_EXPORTS std::string getFormatString(std::string  const & format, tm const * t, std::size_t allocate_size = 128);

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIME_HPP__

