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

MoveAnimation::MoveAnimation(float from_x, float to_x, float from_y, float to_y)
        : Animation(AnimationType::AT_MOVE),
          _from_x(from_x), _to_x(to_x),
          _from_y(from_y), _to_y(to_y),
          _x(from_x), _y(from_y)
{
    // EMPTY.
}

MoveAnimation::~MoveAnimation()
{
    // EMPTY.
}

void MoveAnimation::onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated)
{
    _x = _from_x + (_to_x - _from_x) * interpolated;
    _y = _from_y + (_to_y - _from_y) * interpolated;
}

void MoveAnimation::onEnd()
{
    _x = _to_x;
    _y = _to_y;
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

