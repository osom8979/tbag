/**
 * @file   TimePoint.cpp
 * @brief  TimePoint class implementation.
 * @author zer0
 * @date   2017-04-12
 */

#include <libtbag/time/TimePoint.hpp>
#include <libtbag/time/Time.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/3rd/date/date.h>

#include <ctime>

#include <array>
#include <sstream>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

TimePoint::TimePoint() : _system_tp(SystemDuration(0)), _local_diff(0)
{
    // EMPTY.
}

TimePoint::TimePoint(now_time const & UNUSED_PARAM(flag))
{
    setNow();
    setLocalDiff();
}

TimePoint::TimePoint(SystemTp const & time_point) : _system_tp(time_point)
{
    setLocalDiff();
}

TimePoint::TimePoint(SystemTp const & time_point, Duration const & local_diff)
        : _system_tp(time_point), _local_diff(local_diff)
{
    // EMPTY.
}

TimePoint::TimePoint(Rep rep) : _system_tp(std::chrono::duration_cast<SystemDuration>(Duration(rep)))
{
    setLocalDiff();
}

TimePoint::TimePoint(Rep rep, Rep local_diff)
        : _system_tp(std::chrono::duration_cast<SystemDuration>(Duration(rep))), _local_diff(local_diff)
{
    // EMPTY.
}

TimePoint::TimePoint(int y, int m, int d, int hour, int min, int sec, int milli, int micro, int nano)
{
    setAll(y, m, d, hour, min, sec, milli, micro, nano);
    setLocalDiff();
}

TimePoint::TimePoint(TimePoint const & obj)
{
    (*this) = obj;
}

TimePoint::TimePoint(TimePoint && obj)
{
    (*this) = std::move(obj);
}

TimePoint::~TimePoint()
{
    // EMPTY.
}

TimePoint & TimePoint::operator =(SystemTp const & time_point)
{
    _system_tp = time_point;
    return *this;
}

TimePoint & TimePoint::operator =(Duration const & dur)
{
    using namespace std::chrono;
    _system_tp = SystemTp(duration_cast<SystemDuration>(dur));
    return *this;
}

TimePoint & TimePoint::operator =(Rep rep)
{
    using namespace std::chrono;
    _system_tp = SystemTp(duration_cast<SystemDuration>(Duration(rep)));
    return *this;
}

TimePoint & TimePoint::operator =(TimePoint const & obj)
{
    if (this != &obj) {
        _system_tp  = obj._system_tp;
        _local_diff = obj._local_diff;
    }
    return *this;
}

TimePoint & TimePoint::operator =(TimePoint && obj)
{
    if (this != &obj) {
        _system_tp  = std::move(obj._system_tp);
        _local_diff = std::move(obj._local_diff);
    }
    return *this;
}

void TimePoint::setTimePoint(SystemTp const & time_point)
{
    _system_tp = time_point;
}

void TimePoint::setTimePoint(Duration const & dur)
{
    using namespace std::chrono;
    _system_tp = SystemTp(duration_cast<SystemDuration>(dur));
}

void TimePoint::setTimePoint(Rep rep)
{
    using namespace std::chrono;
    _system_tp = SystemTp(duration_cast<SystemDuration>(Duration(rep)));
}

void TimePoint::setLocalDiff(Duration const & dur)
{
    _local_diff = dur;
}

void TimePoint::setLocalDiff(Rep rep)
{
    _local_diff = Duration(rep);
}

void TimePoint::setNow()
{
    using namespace std::chrono;
    _system_tp = SystemTp(duration_cast<SystemDuration>(SystemClock::now().time_since_epoch()));
}

void TimePoint::setLocalDiff()
{
    using namespace std::chrono;
    _local_diff = duration_cast<Duration>(getCurrentLocalDuration());
}

void TimePoint::setAll(int y, int m, int d, int hour, int min, int sec, int milli, int micro, int nano)
{
    assert(y >= 1970);
    assert(1 <= COMPARE_AND(m) <= 12);
    assert(1 <= COMPARE_AND(d) <= 31);

    assert(0 <= COMPARE_AND(hour) < 24);
    assert(0 <= COMPARE_AND(min ) < 60);
    assert(0 <= COMPARE_AND(sec ) < 60);

    assert(0 <= COMPARE_AND(milli) < 1000);
    assert(0 <= COMPARE_AND(micro) < 1000);
    assert(0 <= COMPARE_AND(nano ) < 1000);

    using namespace date;
    using namespace std::chrono;

    auto sys_date = date::sys_days(date::year(y) / m / d);
    auto sys_time = std::chrono::hours(hour) + std::chrono::minutes(min) + std::chrono::seconds(sec);
    auto subsec   = duration_cast<SystemDuration>(milliseconds(milli) + microseconds(micro) + nanoseconds(nano));
    _system_tp = sys_date + sys_time + subsec;
}

TimePoint::Duration TimePoint::getTimeSinceEpoch() const
{
    using namespace std::chrono;
    return duration_cast<Duration>(getTimePoint().time_since_epoch());
}

TimePoint::Duration TimePoint::getLocalTimeSinceEpoch() const
{
    using namespace std::chrono;
    return duration_cast<Duration>(getLocalTimePoint().time_since_epoch());
}

TimePoint::Rep TimePoint::getRepTimeSinceEpoch() const
{
    return getTimeSinceEpoch().count();
}

TimePoint::Rep TimePoint::getLocalRepTimeSinceEpoch() const
{
    return getLocalTimeSinceEpoch().count();
}

TimePoint::MicroRep TimePoint::getMicrosecTimeSinceEpoch() const
{
    using namespace std::chrono;
    return duration_cast<Microsec>(getTimePoint().time_since_epoch()).count();
}

TimePoint::MicroRep TimePoint::getLocalMicrosecTimeSinceEpoch() const
{
    using namespace std::chrono;
    return duration_cast<Microsec>(getLocalTimePoint().time_since_epoch()).count();
}

// @formatter:off
int TimePoint::year    () const { return libtbag::time::getYear    (getTimePoint()); }
int TimePoint::month   () const { return libtbag::time::getMonth   (getTimePoint()); }
int TimePoint::day     () const { return libtbag::time::getDay     (getTimePoint()); }
int TimePoint::hours   () const { return libtbag::time::getHours   (getTimePoint()); }
int TimePoint::minutes () const { return libtbag::time::getMinutes (getTimePoint()); }
int TimePoint::seconds () const { return libtbag::time::getSeconds (getTimePoint()); }
int TimePoint::millisec() const { return libtbag::time::getMillisec(getTimePoint()); }
int TimePoint::microsec() const { return libtbag::time::getMicrosec(getTimePoint()); }
int TimePoint::nanosec () const { return libtbag::time::getNanosec (getTimePoint()); }

int TimePoint::lyear    () const { return libtbag::time::getYear    (getLocalTimePoint()); }
int TimePoint::lmonth   () const { return libtbag::time::getMonth   (getLocalTimePoint()); }
int TimePoint::lday     () const { return libtbag::time::getDay     (getLocalTimePoint()); }
int TimePoint::lhours   () const { return libtbag::time::getHours   (getLocalTimePoint()); }
int TimePoint::lminutes () const { return libtbag::time::getMinutes (getLocalTimePoint()); }
int TimePoint::lseconds () const { return libtbag::time::getSeconds (getLocalTimePoint()); }
int TimePoint::lmillisec() const { return libtbag::time::getMillisec(getLocalTimePoint()); }
int TimePoint::lmicrosec() const { return libtbag::time::getMicrosec(getLocalTimePoint()); }
int TimePoint::lnanosec () const { return libtbag::time::getNanosec (getLocalTimePoint()); }
// @formatter:on

int TimePoint::getDays() const
{
    return libtbag::time::getDays(getTimePoint());
}

int TimePoint::getLocalDays() const
{
    return libtbag::time::getDays(getLocalTimePoint());
}

std::string TimePoint::toString(std::string const & format) const
{
    std::stringstream ss;
    date::to_stream(ss, format.c_str(), getTimePoint());
    return ss.str();
}

std::string TimePoint::toLongString() const
{
    return toString(TIMESTAMP_LONG_FORMAT);
}

std::string TimePoint::toShortString() const
{
    return toString(TIMESTAMP_SHORT_FORMAT);
}

std::string TimePoint::toLocalString(std::string const & format) const
{
    std::stringstream ss;
    date::to_stream(ss, format.c_str(), getLocalTimePoint());
    return ss.str();
}

std::string TimePoint::toLocalLongString() const
{
    return toLocalString(TIMESTAMP_LONG_FORMAT);
}

std::string TimePoint::toLocalShortString() const
{
    return toLocalString(TIMESTAMP_SHORT_FORMAT);
}

std::string TimePoint::getLongTimeString(int y, int m, int d, int hour, int min, int sec, int milli, int micro, int nano)
{
    std::array<char, 32> buffer;
    double sec_all = (double)sec
                     + ((double)milli / (1000.0))
                     + ((double)micro / (1000.0 * 1000.0))
                     + ((double)nano  / (1000.0 * 1000.0 * 1000.0));
    int const WRITE_SIZE = sprintf(buffer.data(), "%04d-%02d-%02dT%02d:%02d:%f", y, m, d, hour, min, sec_all);
    return std::string(buffer.begin(), buffer.begin() + WRITE_SIZE);
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

