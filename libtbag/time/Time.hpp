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
#include <libtbag/Err.hpp>

#include <ctime>
#include <string>
#include <chrono>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

// ==================
namespace __impl {
/** @warning Don't use this method from user level developers. */
TBAG_API void createInstance();
TBAG_API void releaseInstance();
} // namespace __impl
// ==================

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

using SystemTimePoint = std::chrono::system_clock::time_point;

TBAG_API int getYear(SystemTimePoint const & time);
TBAG_API int getMonth(SystemTimePoint const & time);
TBAG_API int getDay(SystemTimePoint const & time);
TBAG_API int getHours(SystemTimePoint const & time);
TBAG_API int getMinutes(SystemTimePoint const & time);
TBAG_API int getSeconds(SystemTimePoint const & time);
TBAG_API int getSubSeconds(SystemTimePoint const & time);
TBAG_API int getWeek(SystemTimePoint const & time);

TBAG_API std::string getWeekString(SystemTimePoint const & time);

/** milliseconds part of the seconds 0-999. */
TBAG_API int getMillisec(SystemTimePoint const & time);
TBAG_API int getMicrosec(SystemTimePoint const & time);
TBAG_API int getNanosec(SystemTimePoint const & time);

TBAG_API int getDays(SystemTimePoint const & time);

TBAG_API std::string getMillisecMbs(SystemTimePoint const & time);
TBAG_API void getMillisecString(SystemTimePoint const & time, std::string & result);

TBAG_API SystemTimePoint getNowSystemClock() TBAG_NOEXCEPT;
TBAG_API time_t getTime(SystemTimePoint const & time_point) TBAG_NOEXCEPT;

/** Obtain current time. */
TBAG_API time_t getCurrentTime() TBAG_NOEXCEPT;
TBAG_API bool getGmtTime(time_t const & t, tm * output);
TBAG_API bool getLocalTime(time_t const & t, tm * output);

TBAG_API std::chrono::system_clock::duration getCurrentLocalDuration();

TBAG_API std::string getFormatString(std::string  const & format, tm const * t, std::size_t allocate_size = 128);

/** Timezone abbreviation. */
TBAG_API std::string getLocalTimeZoneAbbreviation();

/** get the date and time. */
TBAG_API Err getTimeOfDay(long * sec, long * micro = nullptr);

template <typename ReturnType, typename StartTimePoint, typename TimeoutDuration, typename TickDuration, typename Predicated>
ReturnType syncedWait(ReturnType success_code, StartTimePoint begin, TimeoutDuration timeout, TickDuration tick, Predicated predicated)
{
    while (true) {
        std::this_thread::sleep_for(tick);
        auto const last = predicated();
        if (last == success_code) {
            return success_code;
        }
        if (std::chrono::system_clock::now() - begin >= timeout) {
            return last;
        }
    }
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIME_HPP__

