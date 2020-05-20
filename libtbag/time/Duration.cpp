/**
 * @file   Duration.cpp
 * @brief  Duration class implementation.
 * @author zer0
 * @date   2020-05-15
 */

#include <libtbag/time/Duration.hpp>
#include <libtbag/time/DurationString.hpp>

#include <utility>

#include <date/date.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

Duration::Duration() : _nano()
{
    // EMPTY.
}

Duration::Duration(std::size_t nano)
        : _nano(nano)
{
    // EMPTY.
}

Duration::Duration(std::string const & text)
{
    fromString(text);
}

Duration::Duration(Duration const & obj)
        : _nano(obj._nano)
{
    // EMPTY.
}

Duration::Duration(Duration && obj) TBAG_NOEXCEPT
        : _nano(std::move(obj._nano))
{
    // EMPTY.
}

Duration::~Duration()
{
    // EMPTY.
}

Duration & Duration::operator =(std::size_t value)
{
    _nano = decltype(_nano)(value);
    return *this;
}

Duration & Duration::operator =(std::string const & text)
{
    fromString(text);
    return *this;
}

Duration & Duration::operator =(Duration const & obj)
{
    if (this != &obj) {
        _nano = obj._nano;
    }
    return *this;
}

Duration & Duration::operator =(Duration && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _nano = std::move(obj._nano);
    }
    return *this;
}

void Duration::swap(Duration & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_nano, obj._nano);
    }
}

std::size_t Duration::toNanoseconds() const
{
    return _nano.count();
}

std::size_t Duration::toMicroseconds() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(_nano).count();
}

std::size_t Duration::toMilliseconds() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(_nano).count();
}

std::size_t Duration::toSeconds() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(_nano).count();
}

std::size_t Duration::toMinutes() const
{
    return std::chrono::duration_cast<std::chrono::minutes>(_nano).count();
}

std::size_t Duration::toHours() const
{
    return std::chrono::duration_cast<std::chrono::hours>(_nano).count();
}

std::size_t Duration::toDays() const
{
    return date::floor<date::days>(_nano).count();
}

std::string Duration::toNanosecondsText() const
{
    return getTimeText(_nano);
}

std::string Duration::toMicrosecondsText() const
{
    return getTimeText(std::chrono::duration_cast<std::chrono::microseconds>(_nano));
}

std::string Duration::toMillisecondsText() const
{
    return getTimeText(std::chrono::duration_cast<std::chrono::milliseconds>(_nano));
}

std::string Duration::toSecondsText() const
{
    return getTimeText(std::chrono::duration_cast<std::chrono::seconds>(_nano));
}

std::string Duration::toMinutesText() const
{
    return getTimeText(std::chrono::duration_cast<std::chrono::minutes>(_nano));
}

std::string Duration::toHoursText() const
{
    return getTimeText(std::chrono::duration_cast<std::chrono::hours>(_nano));
}

std::string Duration::toDaysText() const
{
    return getDayText(toDays());
}

std::string Duration::toUpperTimeText() const
{
    return libtbag::time::getUpperTimeText(_nano);
}

Err Duration::fromString(std::string const & text)
{
    auto const result = libtbag::time::parseNanoseconds(text);
    if (!result) {
        return result.code;
    }
    _nano = std::chrono::nanoseconds(result.value);
    return E_SUCCESS;
}

std::string Duration::toString() const
{
    return toUpperTimeText();
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

