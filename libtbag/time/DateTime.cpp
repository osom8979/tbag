/**
 * @file   DateTime.cpp
 * @brief  DateTime class implementation.
 * @author zer0
 * @date   2017-04-05
 */

#include <libtbag/time/DateTime.hpp>
#include <libtbag/time/Time.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/3rd/date/date.h>

#include <ctime>
#include <sstream>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

DateTime::DateTime()
{
    using namespace std::chrono;
    _microseconds = duration_cast<Microsec>(Clock::now().time_since_epoch()).count();
    _local_diff   = duration_cast<Microsec>(getCurrentLocalDuration()).count();
}

DateTime::DateTime(Rep microseconds, Rep local_diff) : _microseconds(microseconds), _local_diff(local_diff)
{
    // EMPTY.
}

DateTime::DateTime(int y, int m, int d, int hour, int min, int sec, int milli, int micro)
        : _microseconds(0), _local_diff(0)
{
    setAll(y, m, d, hour, min, sec, milli, micro);
}

DateTime::DateTime(DateTime const & datetime)
{
    (*this) = datetime;
}

DateTime::DateTime(DateTime && datetime)
{
    (*this) = std::move(datetime);
}

DateTime::~DateTime()
{
    // EMPTY.
}

DateTime & DateTime::operator =(DateTime const & datetime)
{
    if (this != &datetime) {
        _microseconds = datetime._microseconds;
        _local_diff   = datetime._local_diff;
    }
    return *this;
}

DateTime & DateTime::operator =(DateTime && datetime)
{
    if (this != &datetime) {
        _microseconds = datetime._microseconds;
        _local_diff   = datetime._local_diff;
    }
    return *this;
}

void DateTime::setNow()
{
    setDuration(Clock::now().time_since_epoch());
}

void DateTime::setLocalDiff()
{
    using namespace std::chrono;
    _local_diff = duration_cast<Microsec>(getCurrentLocalDuration()).count();
}

void DateTime::setAll(int y, int m, int d, int hour, int min, int sec, int milli, int micro)
{
    using namespace date;
    using namespace std::chrono;

    assert(y >= 1900); // years since 1900.
    auto date = year(y) / m / d;
    auto tp = sys_days(date) + hours(hour) + minutes(min) + seconds(sec) + milliseconds(milli) + microseconds(micro);
    _microseconds = duration_cast<Microsec>(tp.time_since_epoch()).count();
}

DateTime::TimePoint DateTime::getTimePoint() const
{
    return TimePoint(Microsec(_microseconds));
}

DateTime::TimePoint DateTime::getLocalTimePoint() const
{
    return TimePoint(Microsec(_microseconds + _local_diff));
}

DateTime::Microsec DateTime::getDuration() const
{
    return Microsec(_microseconds);
}

DateTime::Microsec DateTime::getLocalDuration() const
{
    return Microsec(_microseconds + _local_diff);
}

int DateTime::getYear() const
{
    return libtbag::time::getYear(getTimePoint());
}

int DateTime::getMonth() const
{
    return libtbag::time::getMonth(getTimePoint());
}

int DateTime::getDay() const
{
    return libtbag::time::getDay(getTimePoint());
}

int DateTime::getHours() const
{
    return libtbag::time::getHours(getTimePoint());
}

int DateTime::getMinutes() const
{
    return libtbag::time::getMinutes(getTimePoint());
}

int DateTime::getSeconds() const
{
    return libtbag::time::getSeconds(getTimePoint());
}

int DateTime::getMillisec() const
{
    return libtbag::time::getMillisec(getTimePoint());
}

int DateTime::getMicrosec() const
{
    return libtbag::time::getMicrosec(getTimePoint());
}

int DateTime::getLocalYear() const
{
    return libtbag::time::getYear(getLocalTimePoint());
}

int DateTime::getLocalMonth() const
{
    return libtbag::time::getMonth(getLocalTimePoint());
}

int DateTime::getLocalDay() const
{
    return libtbag::time::getDay(getLocalTimePoint());
}

int DateTime::getLocalHours() const
{
    return libtbag::time::getHours(getLocalTimePoint());
}

int DateTime::getLocalMinutes() const
{
    return libtbag::time::getMinutes(getLocalTimePoint());
}

int DateTime::getLocalSeconds() const
{
    return libtbag::time::getSeconds(getLocalTimePoint());
}

int DateTime::getLocalMillisec() const
{
    return libtbag::time::getMillisec(getLocalTimePoint());
}

int DateTime::getLocalMicrosec() const
{
    return libtbag::time::getMicrosec(getLocalTimePoint());
}

int DateTime::getDays() const
{
    return libtbag::time::getDays(getTimePoint());
}

int DateTime::getLocalDays() const
{
    return libtbag::time::getDays(getLocalTimePoint());
}

std::string DateTime::toString(std::string const & format)
{
    std::stringstream ss;
    date::to_stream(ss, format.c_str(), getTimePoint());
    return ss.str();
}

std::string DateTime::toLongString()
{
    return toString(TIMESTAMP_LONG_FORMAT);
}

std::string DateTime::toShortString()
{
    return toString(TIMESTAMP_SHORT_FORMAT);
}

std::string DateTime::toLocalString(std::string const & format)
{
    std::stringstream ss;
    date::to_stream(ss, format.c_str(), getLocalTimePoint());
    return ss.str();
}

std::string DateTime::toLocalLongString()
{
    return toLocalString(TIMESTAMP_LONG_FORMAT);
}

std::string DateTime::toLocalShortString()
{
    return toLocalString(TIMESTAMP_SHORT_FORMAT);
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

