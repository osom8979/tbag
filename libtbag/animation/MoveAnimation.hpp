/**
 * @file   MoveAnimation.hpp
 * @brief  MoveAnimation class prototype.
 * @author zer0
 * @date   2017-11-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_MOVEANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_MOVEANIMATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/animation/Animation.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

/**
 * MoveAnimation class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class TBAG_API MoveAnimation : public Animation
{
private:
    float _from_x; ///< from x-coordinate.
    float   _to_x; ///< to x-coordinate.

    float _from_y; ///< from y-coordinate.
    float   _to_y; ///< to y-coordinate.

private:
    float _x;
    float _y;

public:
    MoveAnimation(float from_x, float to_x, float from_y, float to_y);
    virtual ~MoveAnimation();

public:
    inline float getX() const TBAG_NOEXCEPT { return _x; }
    inline float getY() const TBAG_NOEXCEPT { return _y; }

public:
    virtual void onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated) override;
    virtual void onEnd() override;
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_MOVEANIMATION_HPP__

