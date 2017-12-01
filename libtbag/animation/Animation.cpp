/**
 * @file   Animation.cpp
 * @brief  Animation class implementation.
 * @author zer0
 * @date   2017-11-22
 */

#include <libtbag/animation/Animation.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

Animation::Animation() : Animation(Params())
{
    // EMPTY.
}

Animation::Animation(Params const & params)
        : _params(params), _start(), _is_start(false), _is_finish(false)
{
    // EMPTY.
}

Animation::~Animation()
{
    // EMPTY.
}

void Animation::clear()
{
    _params.clear();
    _start     = 0;
    _update    = 0;
    _is_start  = false;
    _is_finish = false;
}

void Animation::start(TimePoint const & tp)
{
    _is_start = true;
    _start  = tp;
    _update = tp;
}

void Animation::reset()
{
    _is_start  = false;
    _is_finish = false;
}

void Animation::stop()
{
    _is_start = false;
}

void Animation::update(TimePoint const & tp)
{
    _update = tp;

    if (_is_start == false && _is_finish) {
        return;
    }

    Milliseconds const diff = std::chrono::duration_cast<Milliseconds>(tp - _start);
    if (diff < _params.start_offset) {
        return;
    }

    assert(diff >= _params.start_offset);
    assert(_params.duration > Milliseconds::zero());

    using namespace std::chrono;
    Milliseconds const total_duration = duration_cast<Milliseconds>(diff - _params.start_offset);
    Milliseconds::rep const current_repeat_count = total_duration.count() / _params.duration.count();

    Milliseconds const current_duration = duration_cast<Milliseconds>(total_duration - (current_repeat_count * _params.duration));
    float ratio = static_cast<float>(current_duration.count()) / static_cast<float>(_params.duration.count());

    assert(ratio >= 0.0f);
    assert(ratio <= 1.0f);

    if (_params.repeat_mode == RepeatMode::RM_REVERSE && current_repeat_count & 0x1 /* ODD Number */) {
        ratio = 1.0f - ratio;
    }

    if (static_cast<bool>(_params.interpolator)) {
        onUpdate(current_duration, ratio, _params.interpolator->getInterpolation(ratio));
    } else {
        onUpdate(current_duration, ratio, ratio);
    }

    // Check the repeat count.
    if (_params.repeat_count != INFINITE_REPEAT && current_repeat_count >= _params.repeat_count) {
        _is_finish = true;
        onEnd();
    }
}

void Animation::updateBySeconds(Seconds::rep seconds)
{
    updateByDelta(Seconds(seconds));
}

void Animation::updateByMilliseconds(Milliseconds::rep milliseconds)
{
    updateByDelta(Milliseconds(milliseconds));
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

