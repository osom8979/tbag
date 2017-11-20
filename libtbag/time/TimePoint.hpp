/**
 * @file   TimePoint.hpp
 * @brief  TimePoint class prototype.
 * @author zer0
 * @date   2017-04-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIMEPOINT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIMEPOINT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/string/Format.hpp>

#include <string>
#include <chrono>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/**
 * TimePoint class prototype.
 *
 * @author zer0
 * @date   2017-04-05
 * @date   2017-04-12 (Rename: DateTime -> TimePoint)
 *
 * @remarks
 *  Unix time (also known as POSIX time or epoch time) is a system for describing instants in time, @n
 *  defined as the number of seconds that have elapsed since 00:00:00 Coordinated Universal Time (UTC), @n
 *  Thursday, 1 January 1970 (1970/01/01T00:00:00).
 *
 * @warning
 *  Not counting leap seconds.
 *
 * @see <https://en.wikipedia.org/wiki/Unix_time>
 * @see <https://en.wikipedia.org/wiki/Leap_second>
 */
class TBAG_API TimePoint : public string::Format
{
public:
    using SystemClock    = std::chrono::system_clock;
    using SystemTp       = typename SystemClock::time_point;
    using SystemDuration = typename SystemClock::duration;

    using Duration = std::chrono::nanoseconds;
    using Rep      = typename Duration::rep;

    using Microsec = std::chrono::microseconds;
    using MicroRep = typename Microsec::rep;

public:
    struct now_time { /* EMPTY. */ };

private:
    SystemTp   _system_tp;
    Duration   _local_diff;

public:
    TimePoint();
    explicit TimePoint(now_time const & UNUSED_PARAM(flag));
    TimePoint(SystemTp const & time_point);
    TimePoint(SystemTp const & time_point, Duration const & local_diff);
    TimePoint(Rep rep);
    TimePoint(Rep rep, Rep local_diff);
    TimePoint(int y, int m, int d, int hour, int min, int sec, int milli = 0, int micro = 0, int nano = 0);
    TimePoint(TimePoint const & obj);
    TimePoint(TimePoint && obj);
    ~TimePoint();

public:
    TimePoint & operator =(SystemTp const & time_point);
    TimePoint & operator =(Duration const & dur);
    TimePoint & operator =(Rep rep);
    TimePoint & operator =(TimePoint const & obj);
    TimePoint & operator =(TimePoint && obj);

public:
    // @formatter:off
    inline friend bool operator ==(TimePoint const & lh, TimePoint const & rh) TBAG_NOEXCEPT
    { return lh._system_tp == rh._system_tp; }
    inline friend bool operator !=(TimePoint const & lh, TimePoint const & rh) TBAG_NOEXCEPT
    { return lh._system_tp != rh._system_tp; }

    inline friend bool operator <(TimePoint const & lh, TimePoint const & rh) TBAG_NOEXCEPT
    { return lh._system_tp < rh._system_tp; }
    inline friend bool operator >(TimePoint const & lh, TimePoint const & rh) TBAG_NOEXCEPT
    { return lh._system_tp > rh._system_tp; }

    inline friend bool operator <=(TimePoint const & lh, TimePoint const & rh) TBAG_NOEXCEPT
    { return lh._system_tp <= rh._system_tp; }
    inline friend bool operator >=(TimePoint const & lh, TimePoint const & rh) TBAG_NOEXCEPT
    { return lh._system_tp >= rh._system_tp; }

    inline TimePoint & operator +=(Duration const & dur)
    { _system_tp += std::chrono::duration_cast<SystemDuration>(dur); return *this; }
    inline TimePoint & operator -=(Duration const & dur)
    { _system_tp -= std::chrono::duration_cast<SystemDuration>(dur); return *this; }
    inline TimePoint & operator +=(Rep rep)
    { _system_tp += std::chrono::duration_cast<SystemDuration>(Duration(rep)); return *this; }
    inline TimePoint & operator -=(Rep rep)
    { _system_tp -= std::chrono::duration_cast<SystemDuration>(Duration(rep)); return *this; }

    inline friend TimePoint operator +(TimePoint const & lh, Duration const & dur)
    { TimePoint tp = lh; tp += dur; return tp; }
    inline friend TimePoint operator -(TimePoint const & lh, Duration const & dur)
    { TimePoint tp = lh; tp -= dur; return tp; }
    inline friend TimePoint operator +(TimePoint const & lh, Rep rep)
    { TimePoint tp = lh; tp += rep; return tp; }
    inline friend TimePoint operator -(TimePoint const & lh, Rep rep)
    { TimePoint tp = lh; tp -= rep; return tp; }
    // @formatter:on

public:
    inline void swap(TimePoint & obj)
    {
        if (this != &obj) {
            string::Format::swap(obj);
            std::swap(_system_tp, obj._system_tp);
            std::swap(_local_diff, obj._local_diff);
        }
    }

    inline friend void swap(TimePoint & lh, TimePoint & rh) { lh.swap(rh); }

public:
    void setTimePoint(SystemTp const & time_point);
    void setTimePoint(Duration const & dur);
    void setTimePoint(Rep rep);

    void setLocalDiff(Duration const & dur);
    void setLocalDiff(Rep rep);

public:
    void setNow();
    void setLocalDiff();

public:
    bool set(int y = 1970, int m = 1, int d = 1,
             int hour = 0, int min = 0, int sec = 0,
             int milli = 0, int micro = 0, int nano = 0);

public:
    // @formatter:off
    inline SystemTp getTimePoint() const { return _system_tp; }
    inline SystemTp getLocalTimePoint() const
    { return _system_tp + std::chrono::duration_cast<SystemDuration>(_local_diff); }
    inline Duration getLocalDiff() const { return _local_diff; }
    // @formatter:on

    /**
     * The amount of time between this time_point and the clock's epoch.
     *
     * @remarks
     *  Elapsed time since UTC (1970/01/01T00:00:00).
     */
    Duration getTimeSinceEpoch() const;
    Duration getLocalTimeSinceEpoch() const;

    Rep getRepTimeSinceEpoch() const;
    Rep getLocalRepTimeSinceEpoch() const;

    MicroRep getMicrosecTimeSinceEpoch() const;
    MicroRep getLocalMicrosecTimeSinceEpoch() const;

public:
    // @formatter:off
    int year    () const;
    int month   () const;
    int day     () const;
    int hours   () const;
    int minutes () const;
    int seconds () const;
    int millisec() const;
    int microsec() const;
    int nanosec () const;
    int week    () const;

    int lyear    () const;
    int lmonth   () const;
    int lday     () const;
    int lhours   () const;
    int lminutes () const;
    int lseconds () const;
    int lmillisec() const;
    int lmicrosec() const;
    int lnanosec () const;
    int lweek    () const;
    // @formatter:on

    int ldhours() const;
    int ldminutes() const;

public:
    int getDays() const;
    int getLocalDays() const;

private:
    static std::string paddingString(int width, int value);

public:
    // @formatter:off
    std::string toYearString     (bool padding = false) const;
    std::string toShortYearString(bool padding = false) const;
    std::string toMonthString    (bool padding = false) const;
    std::string toDayString      (bool padding = false) const;
    std::string toHoursString    (bool padding = false) const;
    std::string toMinutesString  (bool padding = false) const;
    std::string toSecondsString  (bool padding = false) const;
    std::string toMillisecString (bool padding = false) const;
    std::string toMicrosecString (bool padding = false) const;
    std::string toNanosecString  (bool padding = false) const;
    std::string toWeekIndexString() const;
    std::string toWeekString     () const;

    std::string toLocalYearString     (bool padding = false) const;
    std::string toLocalShortYearString(bool padding = false) const;
    std::string toLocalMonthString    (bool padding = false) const;
    std::string toLocalDayString      (bool padding = false) const;
    std::string toLocalHoursString    (bool padding = false) const;
    std::string toLocalMinutesString  (bool padding = false) const;
    std::string toLocalSecondsString  (bool padding = false) const;
    std::string toLocalMillisecString (bool padding = false) const;
    std::string toLocalMicrosecString (bool padding = false) const;
    std::string toLocalNanosecString  (bool padding = false) const;
    std::string toLocalWeekIndexString() const;
    std::string toLocalWeekString     () const;
    // @formatter:on

    std::string toLocalDiffString() const;

public:
    std::string toString(std::string const & format) const;
    std::string toLongString() const;
    std::string toShortString() const;

    std::string toLocalString(std::string const & format) const;
    std::string toLocalLongString() const;
    std::string toLocalShortString() const;

public:
    /**
     * @warning
     *  Not equals strftime() format. @n
     *
     * @remarks
     *  Format list:
     *   - [p][Yy]: Year
     *   - [p][Ee]: Year divided by 100 and truncated to integer
     *   - [p][Mm]: Month
     *   - [p][Dd]: Day
     *   - [p][Hh]: Hours
     *   - [p][Ii]: Minutes
     *   - [p][Ss]: Seconds
     *   - [p][Ll]: Millisec
     *   - [p][Cc]: Microsec
     *   - [p][Nn]: Nanosec
     *   - [f]: Local diff
     *  If you use 'p', the 'padding' option applies. @n
     *  If you use lowercase letters, use local time.
     */
    virtual int onEscape(std::string const & source, std::size_t index, std::string & output) const override;

public:
    inline static TimePoint now() { return TimePoint(now_time()); }
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIMEPOINT_HPP__

