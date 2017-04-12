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

TimePoint::TimePoint() : _system_tp(Microsec(0)), _local_diff(0)
{
    // EMPTY.
}

TimePoint::TimePoint(now_time const & UNUSED_PARAM(flag))
{
    using namespace std::chrono;
    _system_tp  = SystemClock::now();
    _local_diff = duration_cast<Microsec>(getCurrentLocalDuration());
}

TimePoint::TimePoint(SystemTp const & time_point) : _system_tp(time_point)
{
    using namespace std::chrono;
    _local_diff = duration_cast<Microsec>(getCurrentLocalDuration());
}

TimePoint::TimePoint(SystemTp const & time_point, Microsec const & local_diff)
        : _system_tp(time_point), _local_diff(local_diff)
{
    // EMPTY.
}

TimePoint::TimePoint(Rep microseconds) : _system_tp(Microsec(microseconds))
{
    using namespace std::chrono;
    _local_diff = duration_cast<Microsec>(getCurrentLocalDuration());
}

TimePoint::TimePoint(Rep microseconds, Rep local_diff)
        : _system_tp(Microsec(microseconds)), _local_diff(local_diff)
{
    // EMPTY.
}

TimePoint::TimePoint(int y, int m, int d, int hour, int min, int sec, int milli, int micro)
{
    setAll(y, m, d, hour, min, sec, milli, micro);
    using namespace std::chrono;
    _local_diff = duration_cast<Microsec>(getCurrentLocalDuration());
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

TimePoint & TimePoint::operator =(Microsec const & microseconds)
{
    _system_tp = SystemTp(microseconds);
    return *this;
}

TimePoint & TimePoint::operator =(Rep microseconds)
{
    _system_tp = SystemTp(Microsec(microseconds));
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
        _system_tp  = obj._system_tp;
        _local_diff = obj._local_diff;
    }
    return *this;
}

void TimePoint::setTimePoint(SystemTp const & time_point)
{
    _system_tp = time_point;
}

void TimePoint::setTimePoint(Microsec const & microseconds)
{
    _system_tp = SystemTp(microseconds);
}

void TimePoint::setTimePoint(Rep microseconds)
{
    _system_tp = SystemTp(Microsec(microseconds));
}

void TimePoint::setLocalDiff(Microsec const & microseconds)
{
    _local_diff = microseconds;
}

void TimePoint::setLocalDiff(Rep microseconds)
{
    _local_diff = Microsec(microseconds);
}

void TimePoint::setNow()
{
    _system_tp = SystemClock::now();
}

void TimePoint::setLocalDiff()
{
    using namespace std::chrono;
    _local_diff = duration_cast<Microsec>(getCurrentLocalDuration());
}

void TimePoint::setAll(int y, int m, int d, int hour, int min, int sec, int milli, int micro)
{
    if (fromString(TIMESTAMP_LONG_FORMAT, getLongTimeString(y, m, d, hour, min, sec, milli, micro)) == false) {
        _system_tp = date::sys_days(date::year(y) / m / d)
                     + std::chrono::hours(hour) + std::chrono::minutes(min) + std::chrono::seconds(sec)
                     + std::chrono::milliseconds(milli) + std::chrono::microseconds(micro);
    }
}

TimePoint::Microsec TimePoint::getTimeSinceEpoch() const
{
    using namespace std::chrono;
    return duration_cast<Microsec>(getTimePoint().time_since_epoch());
}

TimePoint::Microsec TimePoint::getLocalTimeSinceEpoch() const
{
    using namespace std::chrono;
    return duration_cast<Microsec>(getLocalTimePoint().time_since_epoch());
}

TimePoint::Rep TimePoint::getRepTimeSinceEpoch() const
{
    return getTimeSinceEpoch().count();
}

TimePoint::Rep TimePoint::getLocalRepTimeSinceEpoch() const
{
    return getLocalTimeSinceEpoch().count();
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

int TimePoint::lyear    () const { return libtbag::time::getYear    (getLocalTimePoint()); }
int TimePoint::lmonth   () const { return libtbag::time::getMonth   (getLocalTimePoint()); }
int TimePoint::lday     () const { return libtbag::time::getDay     (getLocalTimePoint()); }
int TimePoint::lhours   () const { return libtbag::time::getHours   (getLocalTimePoint()); }
int TimePoint::lminutes () const { return libtbag::time::getMinutes (getLocalTimePoint()); }
int TimePoint::lseconds () const { return libtbag::time::getSeconds (getLocalTimePoint()); }
int TimePoint::lmillisec() const { return libtbag::time::getMillisec(getLocalTimePoint()); }
int TimePoint::lmicrosec() const { return libtbag::time::getMicrosec(getLocalTimePoint()); }
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

bool TimePoint::fromString(std::string const & format, std::string const & time_string)
{
    std::istringstream in(time_string);
    std::chrono::system_clock::time_point tp;
    date::from_stream(in, format.c_str(), tp);

    if (in.bad() || in.eof()) {
        return false;
    }

    _system_tp = tp;
    return true;
}

std::string TimePoint::getLongTimeString(int y, int m, int d, int hour, int min, int sec, int milli, int micro)
{
    std::array<char, 32> buffer;
    double sec_all = (double)sec + ((double)milli / 1000.0) + ((double)micro / (1000.0 * 1000.0));
    int const WRITE_SIZE = sprintf(buffer.data(), "%04d-%02d-%02dT%02d:%02d:%.6f", y, m, d, hour, min, sec_all);
    return std::string(buffer.begin(), buffer.begin() + WRITE_SIZE);
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

