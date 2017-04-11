/**
 * @file   DateTime.hpp
 * @brief  DateTime class prototype.
 * @author zer0
 * @date   2017-04-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_DATETIME_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_DATETIME_HPP__

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
 * DateTime class prototype.
 *
 * @author zer0
 * @date   2017-04-05
 *
 * @remarks
 *  Elapsed time since UTC(1970/01/01T00:00:00).
 */
class TBAG_API DateTime
{
public:
    using Clock     = std::chrono::system_clock;
    using TimePoint = Clock::time_point;
    using Microsec  = std::chrono::microseconds;
    using Rep       = Microsec::rep;
    using Period    = Microsec::period;

    static_assert(std::is_same<Rep, long long>::value
            , "Rep must be the same type as long long");

private:
    Rep _microseconds;
    Rep _local_diff;

public:
    DateTime();
    DateTime(Rep microseconds, Rep local_diff = 0L);
    DateTime(int y, int m, int d, int hour = 0, int min = 0, int sec = 0, int milli = 0, int micro = 0);
    DateTime(DateTime const & datetime);
    DateTime(DateTime && datetime);
    ~DateTime();

public:
    DateTime & operator =(DateTime const & datetime);
    DateTime & operator =(DateTime && datetime);

public:
    // @formatter:off
    inline friend bool operator ==(DateTime const & lh, DateTime const & rh) TBAG_NOEXCEPT
    { return lh._microseconds == rh._microseconds; }
    inline friend bool operator !=(DateTime const & lh, DateTime const & rh) TBAG_NOEXCEPT
    { return lh._microseconds != rh._microseconds; }

    inline friend bool operator <(DateTime const & lh, DateTime const & rh) TBAG_NOEXCEPT
    { return lh._microseconds < rh._microseconds; }
    inline friend bool operator >(DateTime const & lh, DateTime const & rh) TBAG_NOEXCEPT
    { return lh._microseconds > rh._microseconds; }

    inline friend bool operator <=(DateTime const & lh, DateTime const & rh) TBAG_NOEXCEPT
    { return lh._microseconds <= rh._microseconds; }
    inline friend bool operator >=(DateTime const & lh, DateTime const & rh) TBAG_NOEXCEPT
    { return lh._microseconds >= rh._microseconds; }
    // @formatter:on

    inline friend void swap(DateTime & lh, DateTime & rh)
    {
        std::swap(lh._microseconds, rh._microseconds);
        std::swap(lh._local_diff  , rh._local_diff  );
    }

public:
    inline void set(Rep microseconds) TBAG_NOEXCEPT
    {
        _microseconds = microseconds;
    }

    inline void setLocalDiff(Rep microseconds) TBAG_NOEXCEPT
    {
        _local_diff = microseconds;
    }

    template <typename FromDuration>
    inline void setDuration(FromDuration && duration)
    {
        using namespace std::chrono;
        _microseconds = duration_cast<Microsec>(std::forward<FromDuration>(duration)).count();
    }

    void setNow();
    void setLocalDiff();

public:
    void setAll(int y, int m, int d, int hour = 0, int min = 0, int sec = 0, int milli = 0, int micro = 0);

public:
    inline Rep get() const TBAG_NOEXCEPT
    {
        return _microseconds;
    }

    inline Rep getLocalDiff() const TBAG_NOEXCEPT
    {
        return _local_diff;
    }

    inline Rep getLocal() const TBAG_NOEXCEPT
    {
        return _microseconds + _local_diff;
    }

public:
    TimePoint getTimePoint() const;
    TimePoint getLocalTimePoint() const;
    Microsec getDuration() const;
    Microsec getLocalDuration() const;

public:
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;
    int getMillisec() const;
    int getMicrosec() const;

public:
    int getLocalYear() const;
    int getLocalMonth() const;
    int getLocalDay() const;
    int getLocalHours() const;
    int getLocalMinutes() const;
    int getLocalSeconds() const;
    int getLocalMillisec() const;
    int getLocalMicrosec() const;

public:
    int getDays() const;
    int getLocalDays() const;

public:
    std::string toString(std::string const & format);
    std::string toLongString();
    std::string toShortString();

    std::string toLocalString(std::string const & format);
    std::string toLocalLongString();
    std::string toLocalShortString();
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_DATETIME_HPP__

