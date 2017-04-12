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
class TBAG_API TimePoint
{
public:
    using SystemClock = std::chrono::system_clock;
    using SystemTp    = typename SystemClock::time_point;

    using Microsec = std::chrono::microseconds;
    using Rep      = typename Microsec::rep;

public:
    struct now_time { /* EMPTY. */ };

private:
    SystemTp _system_tp;
    Microsec _local_diff;

public:
    TimePoint();
    explicit TimePoint(now_time const & UNUSED_PARAM(flag));
    TimePoint(SystemTp const & time_point);
    TimePoint(SystemTp const & time_point, Microsec const & local_diff);
    TimePoint(Rep microseconds);
    TimePoint(Rep microseconds, Rep local_diff);
    TimePoint(int y, int m, int d, int hour, int min, int sec, int milli = 0, int micro = 0);
    TimePoint(TimePoint const & obj);
    TimePoint(TimePoint && obj);
    ~TimePoint();

public:
    TimePoint & operator =(SystemTp const & time_point);
    TimePoint & operator =(Microsec const & microseconds);
    TimePoint & operator =(Rep microseconds);
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
    // @formatter:on

    inline TimePoint & operator +=(Microsec const & microsec)
    {
        _system_tp += microsec;
        return *this;
    }

    inline TimePoint & operator -=(Microsec const & microsec)
    {
        _system_tp -= microsec;
        return *this;
    }

    inline TimePoint & operator +=(Rep microsec)
    {
        _system_tp += Microsec(microsec);
        return *this;
    }

    inline TimePoint & operator -=(Rep microsec)
    {
        _system_tp -= Microsec(microsec);
        return *this;
    }

    inline friend TimePoint operator +(TimePoint const & lh, Microsec const & microsec)
    {
        TimePoint tp = lh;
        tp += microsec;
        return tp;
    }

    inline friend TimePoint operator -(TimePoint const & lh, Microsec const & microsec)
    {
        TimePoint tp = lh;
        tp -= microsec;
        return tp;
    }

    inline friend TimePoint operator +(TimePoint const & lh, Rep microsec)
    {
        TimePoint tp = lh;
        tp += microsec;
        return tp;
    }

    inline friend TimePoint operator -(TimePoint const & lh, Rep microsec)
    {
        TimePoint tp = lh;
        tp -= microsec;
        return tp;
    }

    inline friend void swap(TimePoint & lh, TimePoint & rh)
    {
        std::swap(lh._system_tp , rh._system_tp );
        std::swap(lh._local_diff, rh._local_diff);
    }

public:
    void setTimePoint(SystemTp const & time_point);
    void setTimePoint(Microsec const & microseconds);
    void setTimePoint(Rep microseconds);

    void setLocalDiff(Microsec const & microseconds);
    void setLocalDiff(Rep microseconds);

public:
    void setNow();
    void setLocalDiff();

public:
    void setAll(int y, int m, int d, int hour = 0, int min = 0, int sec = 0, int milli = 0, int micro = 0);

public:
    // @formatter:off
    inline SystemTp getTimePoint() const
    { return _system_tp; }
    inline SystemTp getLocalTimePoint() const
    { return _system_tp + _local_diff; }
    inline Microsec getLocalDiff() const
    { return _local_diff; }
    // @formatter:on

    /**
     * The amount of time between this time_point and the clock's epoch.
     *
     * @remarks
     *  Elapsed time since UTC (1970/01/01T00:00:00).
     */
    Microsec getTimeSinceEpoch() const;
    Microsec getLocalTimeSinceEpoch() const;

    Rep getRepTimeSinceEpoch() const;
    Rep getLocalRepTimeSinceEpoch() const;

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

    int lyear    () const;
    int lmonth   () const;
    int lday     () const;
    int lhours   () const;
    int lminutes () const;
    int lseconds () const;
    int lmillisec() const;
    int lmicrosec() const;
    // @formatter:on

public:
    int getDays() const;
    int getLocalDays() const;

public:
    std::string toString(std::string const & format) const;
    std::string toLongString() const;
    std::string toShortString() const;

    std::string toLocalString(std::string const & format) const;
    std::string toLocalLongString() const;
    std::string toLocalShortString() const;

public:
    inline static TimePoint now()
    {
        return TimePoint(now_time());
    }
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_TIMEPOINT_HPP__

