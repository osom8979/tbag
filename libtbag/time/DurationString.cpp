/**
 * @file   DurationString.cpp
 * @brief  DurationString class implementation.
 * @author zer0
 * @date   2019-07-15
 */

#include <libtbag/time/DurationString.hpp>
#include <libtbag/time/DurationStringParser.hpp.inl>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/Unit.hpp>

#include <cctype>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

ErrSize parseNanoseconds(std::string const & str)
{
    return DurationStringParser<std::chrono::nanoseconds>::parse(str);
}

ErrSize parseMicroseconds(std::string const & str)
{
    return DurationStringParser<std::chrono::microseconds>::parse(str);
}

ErrSize parseMilliseconds(std::string const & str)
{
    return DurationStringParser<std::chrono::milliseconds>::parse(str);
}

ErrSize parseSeconds(std::string const & str)
{
    return DurationStringParser<std::chrono::seconds>::parse(str);
}

ErrSize parseMinutes(std::string const & str)
{
    return DurationStringParser<std::chrono::minutes>::parse(str);
}

ErrSize parseHours(std::string const & str)
{
    return DurationStringParser<std::chrono::hours>::parse(str);
}

std::string getDayText(std::size_t day)
{
    return std::to_string(day) + DAY_DURATION_STRING;
}

std::string getUpperTimeText(std::chrono::nanoseconds const & duration)
{
    auto const microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    if (microseconds.count() >= 1) {
        return getUpperTimeText(microseconds);
    } else {
        return getTimeText(duration);
    }
}

std::string getUpperTimeText(std::chrono::microseconds const & duration)
{
    auto const milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    if (milliseconds.count() >= 1) {
        return getUpperTimeText(milliseconds);
    } else {
        return getTimeText(duration);
    }
}

std::string getUpperTimeText(std::chrono::milliseconds const & duration)
{
    auto const seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    if (seconds.count() >= 1) {
        return getUpperTimeText(seconds);
    } else {
        return getTimeText(duration);
    }
}

std::string getUpperTimeText(std::chrono::seconds const & duration)
{
    auto const minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    if (minutes.count() >= 1) {
        return getUpperTimeText(minutes);
    } else {
        return getTimeText(duration);
    }
}

std::string getUpperTimeText(std::chrono::minutes const & duration)
{
    auto const hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    if (hours.count() >= 1) {
        return getUpperTimeText(hours);
    } else {
        return getTimeText(duration);
    }
}

std::string getUpperTimeText(std::chrono::hours const & duration)
{
    auto const days = duration.count()/DAY_TO_HOURS;
    if (days >= 1) {
        return getUpperTimeTextByDays(days);
    } else {
        return getTimeText(duration);
    }
}

std::string getUpperTimeTextByNanoseconds(std::size_t count)
{
    return getUpperTimeText(std::chrono::nanoseconds(count));
}

std::string getUpperTimeTextByMicroseconds(std::size_t count)
{
    return getUpperTimeText(std::chrono::microseconds(count));
}

std::string getUpperTimeTextByMilliseconds(std::size_t count)
{
    return getUpperTimeText(std::chrono::milliseconds(count));
}

std::string getUpperTimeTextBySeconds(std::size_t count)
{
    return getUpperTimeText(std::chrono::seconds(count));
}

std::string getUpperTimeTextByMinutes(std::size_t count)
{
    return getUpperTimeText(std::chrono::minutes(count));
}

std::string getUpperTimeTextByHours(std::size_t count)
{
    return getUpperTimeText(std::chrono::hours(count));
}

std::string getUpperTimeTextByDays(std::size_t count)
{
    return getDayText(count);
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

