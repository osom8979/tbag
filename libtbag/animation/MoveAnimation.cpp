/**
 * @file   MoveAnimation.cpp
 * @brief  MoveAnimation class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/MoveAnimation.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

MoveAnimation::MoveAnimation() : Animation(AnimationType::AT_MOVE)
{
    // EMPTY.
}

MoveAnimation::~MoveAnimation()
{
    // EMPTY.
}

void MoveAnimation::onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated)
{
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

