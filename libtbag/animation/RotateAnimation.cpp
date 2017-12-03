/**
 * @file   RotateAnimation.cpp
 * @brief  RotateAnimation class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/RotateAnimation.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

RotateAnimation::RotateAnimation() : Animation(AnimationType::AT_ROTATE)
{
    // EMPTY.
}

RotateAnimation::~RotateAnimation()
{
    // EMPTY.
}

void RotateAnimation::onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated)
{
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

