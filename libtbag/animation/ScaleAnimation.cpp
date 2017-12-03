/**
 * @file   ScaleAnimation.cpp
 * @brief  ScaleAnimation class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/ScaleAnimation.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

ScaleAnimation::ScaleAnimation() : Animation(AnimationType::AT_SCALE)
{
    // EMPTY.
}

ScaleAnimation::~ScaleAnimation()
{
    // EMPTY.
}

void ScaleAnimation::onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated)
{
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

