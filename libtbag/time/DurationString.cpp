/**
 * @file   DurationString.cpp
 * @brief  DurationString class implementation.
 * @author zer0
 * @date   2019-07-15
 */

#include <libtbag/time/DurationString.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cctype>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

template <typename DefaultDurationT>
static std::size_t __to_chrono_duration(std::string const & str)
{
    auto const lower_str = libtbag::string::trim(libtbag::string::lower(str));
    auto const size = lower_str.size();

    std::size_t i = 0;
    for (; i < size; ++i) {
        if (!std::isdigit(lower_str[i])) {
            break;
        }
    }
    // Skip space;
    for (; i < size; ++i) {
        if (lower_str[i] != ' ') {
            break;
        }
    }

    auto const value = libtbag::string::toValue<std::size_t>(lower_str.substr(0, i));
    auto const suffix = lower_str.substr(i);
    auto const suffix_size = suffix.size();

    using namespace std::chrono;
    if (suffix_size <= 0) {
        return value;
    }

    assert(suffix_size >= 1);
    if (suffix[0] == 'h') {
        return duration_cast<DefaultDurationT>(hours(value)).count();
    } else if (suffix[0] == 's') {
        return duration_cast<DefaultDurationT>(seconds(value)).count();
    } else if (suffix[0] == 'n') {
        return duration_cast<DefaultDurationT>(nanoseconds(value)).count();
    } else if (suffix[0] == 'm') {
        if (suffix_size >= 2 && suffix[1] == 'i') {
            if (suffix_size >= 3) {
                if (suffix[2] == 'c') { // mic
                    return duration_cast<DefaultDurationT>(microseconds(value)).count();
                } else if (suffix[2] == 'l') { // mil
                    return duration_cast<DefaultDurationT>(milliseconds(value)).count();
                } else if (suffix[2] == 'n') { // min
                    return duration_cast<DefaultDurationT>(minutes(value)).count();
                }
            }
        }
    }

    return value;
}

std::size_t toNanoseconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::nanoseconds>(str);
}

std::size_t toMicroseconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::microseconds>(str);
}

std::size_t toMilliseconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::milliseconds>(str);
}

std::size_t toSeconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::seconds>(str);
}

std::size_t toMinutes(std::string const & str)
{
    return __to_chrono_duration<std::chrono::minutes>(str);
}

std::size_t toHours(std::string const & str)
{
    return __to_chrono_duration<std::chrono::hours>(str);
}

std::string getDayText(int day)
{
    return std::to_string(day) + TBAG_DAY_DURATION_STRING;
}

std::string getMonthText(int month)
{
    return std::to_string(month) + TBAG_MONTH_DURATION_STRING;
}

std::string getYearText(int year)
{
    return std::to_string(year) + TBAG_YEAR_DURATION_STRING;
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
    auto const hours = duration.count();
    auto const days = static_cast<int>(hours/24);
    if (days >= 1) {
        return getUpperTimeTextByDay(days);
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

std::string getUpperTimeTextByDay(std::size_t count)
{
    if (count >= 365) {
        return getYearText(static_cast<int>(count/365));
    } else if (count >= 30) {
        return getMonthText(static_cast<int>(count/30));
    } else {
        return getDayText(static_cast<int>(count));
    }
}

std::string getUpperTimeTextByMonth(std::size_t count)
{
    if (count >= 12) {
        return getYearText(static_cast<int>(count/12));
    } else {
        return getMonthText(static_cast<int>(count));
    }
}

std::string getUpperTimeTextByYear(std::size_t count)
{
    return getYearText(static_cast<int>(count));
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

