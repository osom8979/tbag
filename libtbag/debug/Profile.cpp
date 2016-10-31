/**
 * @file   Profile.cpp
 * @brief  Profile class implementation.
 * @author zer0
 * @date   2016-09-21
 */

#include <libtbag/debug/Profile.hpp>
#include <libtbag/time/Time.hpp>
#include <libtbag/time/DurationString.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {

Profile::Timer::Timer(Profile & profile) : _profile(profile), _start(std::chrono::system_clock::now())
{
    // EMPTY.
}

Profile::Timer::~Timer()
{
    _profile.update(std::chrono::system_clock::now() - _start);
}

Profile::Profile(std::size_t cycle, RepeatCallback const & callback, bool enable)
        : _cycle_count(cycle), _current_cycle(0), _total_duration(0), _callback(callback)
{
    // The 'atomic_bool(bool) constructor' does not exist in MSVC 2013.
    _enable = enable;
}

Profile::Profile(std::size_t cycle, RepeatCallback const & callback) : Profile(cycle, callback, true)
{
    // EMPTY.
}

Profile::Profile(std::size_t cycle) : Profile(cycle, RepeatCallback())
{
    // EMPTY.
}

Profile::Profile() : Profile(1)
{
    // EMPTY.
}

Profile::~Profile()
{
    // EMPTY.
}

void Profile::update(Duration duration)
{
    if (_enable == false) {
        return;
    }

    // UPDATE
    _total_duration += duration.count();
    ++_current_cycle;

    if (_current_cycle >= _cycle_count) {
        // CALLBACK
        if (static_cast<bool>(_callback)) {
            _callback(_total_duration / _cycle_count);
        }

        // CLEAR
        _current_cycle  = 0;
        _total_duration = 0;
    }
}

Profile::SharedTimer Profile::tick()
{
    if (_enable) {
        return SharedTimer(new Timer(*this));
    }
    return SharedTimer();
}

char const * const Profile::getTimeUnitString() TBAG_NOEXCEPT
{
    return libtbag::time::getDurationString<Profile::Duration>();
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

