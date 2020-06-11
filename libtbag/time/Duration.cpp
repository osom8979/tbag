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

// clang-format off
Duration Duration::nano (std::size_t val) { return Duration(Nanoseconds (val)); }
Duration Duration::micro(std::size_t val) { return Duration(Microseconds(val)); }
Duration Duration::milli(std::size_t val) { return Duration(Milliseconds(val)); }
Duration Duration::sec  (std::size_t val) { return Duration(Seconds     (val)); }
Duration Duration::min  (std::size_t val) { return Duration(Minutes     (val)); }
Duration Duration::hour (std::size_t val) { return Duration(Hours       (val)); }
// clang-format off

// clang-format on
Duration Duration::nano (std::string const & text, Nanoseconds  const & def) { return Duration(text, def); }
Duration Duration::micro(std::string const & text, Microseconds const & def) { return Duration(text, def); }
Duration Duration::milli(std::string const & text, Milliseconds const & def) { return Duration(text, def); }
Duration Duration::sec  (std::string const & text, Seconds      const & def) { return Duration(text, def); }
Duration Duration::min  (std::string const & text, Minutes      const & def) { return Duration(text, def); }
Duration Duration::hour (std::string const & text, Hours        const & def) { return Duration(text, def); }
// clang-format on

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

std::string Duration::toString() const
{
    return toUpperTimeText();
}

Err Duration::fromNanosecondsText(std::string const & text)
{
    auto const result = libtbag::time::parseNanoseconds(text);
    if (!result) {
        return result.code;
    }
    _nano = std::chrono::nanoseconds(result.val);
    return E_SUCCESS;
}

Err Duration::fromMicrosecondsText(std::string const & text)
{
    auto const result = libtbag::time::parseMicroseconds(text);
    if (!result) {
        return result.code;
    }
    using namespace std::chrono;
    _nano = duration_cast<decltype(_nano)>(microseconds(result.val));
    return E_SUCCESS;
}

Err Duration::fromMillisecondsText(std::string const & text)
{
    auto const result = libtbag::time::parseMilliseconds(text);
    if (!result) {
        return result.code;
    }
    using namespace std::chrono;
    _nano = duration_cast<decltype(_nano)>(milliseconds(result.val));
    return E_SUCCESS;
}

Err Duration::fromSecondsText(std::string const & text)
{
    auto const result = libtbag::time::parseSeconds(text);
    if (!result) {
        return result.code;
    }
    using namespace std::chrono;
    _nano = duration_cast<decltype(_nano)>(seconds(result.val));
    return E_SUCCESS;
}

Err Duration::fromMinutesText(std::string const & text)
{
    auto const result = libtbag::time::parseMinutes(text);
    if (!result) {
        return result.code;
    }
    using namespace std::chrono;
    _nano = duration_cast<decltype(_nano)>(minutes(result.val));
    return E_SUCCESS;
}

Err Duration::fromHoursText(std::string const & text)
{
    auto const result = libtbag::time::parseHours(text);
    if (!result) {
        return result.code;
    }
    using namespace std::chrono;
    _nano = duration_cast<decltype(_nano)>(hours(result.val));
    return E_SUCCESS;
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

