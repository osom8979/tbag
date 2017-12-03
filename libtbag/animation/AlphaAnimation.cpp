/**
 * @file   AlphaAnimation.cpp
 * @brief  AlphaAnimation class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/AlphaAnimation.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

AlphaAnimation::AlphaAnimation(float from_alpha, float to_alpha)
        : Animation(AnimationType::AT_ALPHA), _from_alpha(from_alpha), _to_alpha(to_alpha), _alpha(from_alpha)
{
    // EMPTY.
}

AlphaAnimation::~AlphaAnimation()
{
    // EMPTY.
}

void AlphaAnimation::onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated)
{
    _alpha = _from_alpha + (_to_alpha - _from_alpha) * interpolated;
}

void AlphaAnimation::onEnd()
{
    _alpha = _to_alpha;
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

