/**
 * @file   TimePoint.cpp
 * @brief  TimePoint class implementation.
 * @author zer0
 * @date   2017-04-12
 */

#include <libtbag/time/TimePoint.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/time/Time.hpp>

#include <date/date.h>

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
    set(y, m, d, hour, min, sec, milli, micro, nano);
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
        setEscape(obj.getEscape());
        _system_tp  = obj._system_tp;
        _local_diff = obj._local_diff;
    }
    return *this;
}

TimePoint & TimePoint::operator =(TimePoint && obj)
{
    swap(obj);
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

bool TimePoint::set(int y, int m, int d, int hour, int min, int sec, int milli, int micro, int nano)
{
    // clang-format off
    if (y < RANGE_OR(1970, 9999) < y) { return false; }
    if (m < RANGE_OR(   1,   12) < m) { return false; }
    if (d < RANGE_OR(   1,   31) < d) { return false; }

    if (hour < RANGE_OR(0, 24) <= hour) { return false; }
    if ( min < RANGE_OR(0, 60) <=  min) { return false; }
    if ( sec < RANGE_OR(0, 60) <=  sec) { return false; }

    if (milli < RANGE_OR(0, 1000) <= milli) { return false; }
    if (micro < RANGE_OR(0, 1000) <= micro) { return false; }
    if ( nano < RANGE_OR(0, 1000) <=  nano) { return false; }
    // clang-format on

    using namespace date;
    using namespace std::chrono;

    auto sys_date = date::sys_days(date::year(y) / m / d);
    auto sys_time = std::chrono::hours(hour) + std::chrono::minutes(min) + std::chrono::seconds(sec);
    auto subsec   = duration_cast<SystemDuration>(milliseconds(milli) + microseconds(micro) + nanoseconds(nano));
    _system_tp = sys_date + sys_time + subsec;

    return true;
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

// clang-format off
int TimePoint::year    () const { return libtbag::time::getYear    (getTimePoint()); }
int TimePoint::month   () const { return libtbag::time::getMonth   (getTimePoint()); }
int TimePoint::day     () const { return libtbag::time::getDay     (getTimePoint()); }
int TimePoint::hours   () const { return libtbag::time::getHours   (getTimePoint()); }
int TimePoint::minutes () const { return libtbag::time::getMinutes (getTimePoint()); }
int TimePoint::seconds () const { return libtbag::time::getSeconds (getTimePoint()); }
int TimePoint::millisec() const { return libtbag::time::getMillisec(getTimePoint()); }
int TimePoint::microsec() const { return libtbag::time::getMicrosec(getTimePoint()); }
int TimePoint::nanosec () const { return libtbag::time::getNanosec (getTimePoint()); }
int TimePoint::week    () const { return libtbag::time::getWeek    (getTimePoint()); }

int TimePoint::lyear    () const { return libtbag::time::getYear    (getLocalTimePoint()); }
int TimePoint::lmonth   () const { return libtbag::time::getMonth   (getLocalTimePoint()); }
int TimePoint::lday     () const { return libtbag::time::getDay     (getLocalTimePoint()); }
int TimePoint::lhours   () const { return libtbag::time::getHours   (getLocalTimePoint()); }
int TimePoint::lminutes () const { return libtbag::time::getMinutes (getLocalTimePoint()); }
int TimePoint::lseconds () const { return libtbag::time::getSeconds (getLocalTimePoint()); }
int TimePoint::lmillisec() const { return libtbag::time::getMillisec(getLocalTimePoint()); }
int TimePoint::lmicrosec() const { return libtbag::time::getMicrosec(getLocalTimePoint()); }
int TimePoint::lnanosec () const { return libtbag::time::getNanosec (getLocalTimePoint()); }
int TimePoint::lweek    () const { return libtbag::time::getWeek    (getLocalTimePoint()); }
// clang-format on

int TimePoint::ldhours() const
{
    return static_cast<int>(date::make_time(getLocalDiff()).hours().count());
}

int TimePoint::ldminutes() const
{
    return static_cast<int>(date::make_time(getLocalDiff()).minutes().count());
}

int TimePoint::getDays() const
{
    return libtbag::time::getDays(getTimePoint());
}

int TimePoint::getLocalDays() const
{
    return libtbag::time::getDays(getLocalTimePoint());
}

std::string TimePoint::paddingString(int width, int value)
{
    if (width == 4) {
        return string::fformat("{:0>4}", value);
    } else if (width == 3) {
        return string::fformat("{:0>3}", value);
    } else if (width == 2) {
        return string::fformat("{:0>2}", value);
    }
    return string::fformat("{}", value);
}

// clang-format off
std::string TimePoint::toYearString     (bool p) const { return paddingString(p ? 4 : 0,     year()); }
std::string TimePoint::toShortYearString(bool p) const { return paddingString(p ? 2 : 0,     year() % 100); }
std::string TimePoint::toMonthString    (bool p) const { return paddingString(p ? 2 : 0,    month()); }
std::string TimePoint::toDayString      (bool p) const { return paddingString(p ? 2 : 0,      day()); }
std::string TimePoint::toHoursString    (bool p) const { return paddingString(p ? 2 : 0,    hours()); }
std::string TimePoint::toMinutesString  (bool p) const { return paddingString(p ? 2 : 0,  minutes()); }
std::string TimePoint::toSecondsString  (bool p) const { return paddingString(p ? 2 : 0,  seconds()); }
std::string TimePoint::toMillisecString (bool p) const { return paddingString(p ? 3 : 0, millisec()); }
std::string TimePoint::toMicrosecString (bool p) const { return paddingString(p ? 3 : 0, microsec()); }
std::string TimePoint::toNanosecString  (bool p) const { return paddingString(p ? 3 : 0,  nanosec()); }
std::string TimePoint::toWeekIndexString() const { return std::to_string(week()); }
std::string TimePoint::toWeekString     () const { return getWeekString(getTimePoint()); }

std::string TimePoint::toLocalYearString     (bool p) const { return paddingString(p ? 4 : 0,     lyear()); }
std::string TimePoint::toLocalShortYearString(bool p) const { return paddingString(p ? 2 : 0,     lyear() % 100); }
std::string TimePoint::toLocalMonthString    (bool p) const { return paddingString(p ? 2 : 0,    lmonth()); }
std::string TimePoint::toLocalDayString      (bool p) const { return paddingString(p ? 2 : 0,      lday()); }
std::string TimePoint::toLocalHoursString    (bool p) const { return paddingString(p ? 2 : 0,    lhours()); }
std::string TimePoint::toLocalMinutesString  (bool p) const { return paddingString(p ? 2 : 0,  lminutes()); }
std::string TimePoint::toLocalSecondsString  (bool p) const { return paddingString(p ? 2 : 0,  lseconds()); }
std::string TimePoint::toLocalMillisecString (bool p) const { return paddingString(p ? 3 : 0, lmillisec()); }
std::string TimePoint::toLocalMicrosecString (bool p) const { return paddingString(p ? 3 : 0, lmicrosec()); }
std::string TimePoint::toLocalNanosecString  (bool p) const { return paddingString(p ? 3 : 0,  lnanosec()); }
std::string TimePoint::toLocalWeekIndexString() const { return std::to_string(lweek()); }
std::string TimePoint::toLocalWeekString     () const { return getWeekString(getLocalTimePoint()); }
// clang-format on

std::string TimePoint::toLocalDiffString() const
{
    return string::fformat("{}{:0>2}{:0>2}", (_local_diff < _local_diff.zero() ? '-' : '+'), ldhours(), ldminutes());
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

int TimePoint::onEscape(std::string const & source, std::size_t index, std::string & output) const
{
    bool padding = false;
    int  consume = 1;
    char cursor  = source[index];

    if (cursor == 'f') {
        output = toLocalDiffString();
        return 1;
    }

    if (cursor == 'P' || cursor == 'p') {
        if (index + 1 >= source.size()) {
            output.assign(1, source[index]);
            return 1;
        }
        padding = true;
        consume = 2;
        cursor  = source[index + 1];
    }

    // clang-format off
    switch (cursor) {
    // Global Time:
    case 'Y': output = toYearString          (padding); break;
    case 'E': output = toShortYearString     (padding); break;
    case 'M': output = toMonthString         (padding); break;
    case 'D': output = toDayString           (padding); break;
    case 'H': output = toHoursString         (padding); break;
    case 'I': output = toMinutesString       (padding); break;
    case 'S': output = toSecondsString       (padding); break;
    case 'L': output = toMillisecString      (padding); break;
    case 'C': output = toMicrosecString      (padding); break;
    case 'N': output = toNanosecString       (padding); break;
    // Local Time:
    case 'y': output = toLocalYearString     (padding); break;
    case 'e': output = toLocalShortYearString(padding); break;
    case 'm': output = toLocalMonthString    (padding); break;
    case 'd': output = toLocalDayString      (padding); break;
    case 'h': output = toLocalHoursString    (padding); break;
    case 'i': output = toLocalMinutesString  (padding); break;
    case 's': output = toLocalSecondsString  (padding); break;
    case 'l': output = toLocalMillisecString (padding); break;
    case 'c': output = toLocalMicrosecString (padding); break;
    case 'n': output = toLocalNanosecString  (padding); break;
    default:  output.assign(1, source[index]);          break;
    }
    // clang-format on

    return consume;
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

