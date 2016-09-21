/**
 * @file   Profile.cpp
 * @brief  Profile class implementation.
 * @author zer0
 * @date   2016-09-21
 */

#include <libtbag/debug/Profile.hpp>

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

Profile::Profile(std::size_t cycle, RepeatCallback const & callback)
        : _cycle_count(cycle), _current_cycle(0), _total_duration(0), _callback(callback)
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
    return SharedTimer(new Timer(*this));
}

} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

