/**
 * @file   FrameAnimation.cpp
 * @brief  FrameAnimation class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/FrameAnimation.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

FrameAnimation::FrameAnimation() : Animation(AnimationType::AT_FRAME)
{
    // EMPTY.
}

FrameAnimation::~FrameAnimation()
{
    // EMPTY.
}

void FrameAnimation::onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated)
{
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

