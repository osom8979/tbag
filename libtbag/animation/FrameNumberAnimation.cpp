/**
 * @file   FrameNumberAnimation.cpp
 * @brief  FrameNumberAnimation class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/FrameNumberAnimation.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

FrameNumberAnimation::FrameNumberAnimation(int total_frame, int current_frame)
        : _total_frame(total_frame), _current_frame(current_frame)
{
    // EMPTY.
}

FrameNumberAnimation::~FrameNumberAnimation()
{
    // EMPTY.
}

void FrameNumberAnimation::onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated)
{
    _current_frame = static_cast<int>(_total_frame * interpolated + 0.5f);
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

