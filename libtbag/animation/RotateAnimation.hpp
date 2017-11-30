/**
 * @file   RotateAnimation.hpp
 * @brief  RotateAnimation class prototype.
 * @author zer0
 * @date   2017-11-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ROTATEANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ROTATEANIMATION_HPP__

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
 * RotateAnimation class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class TBAG_API RotateAnimation : public Animation
{
private:
    float _from_degrees;
    float _to_degrees;

    float _pivot_x;
    float _pivot_y;

public:
    RotateAnimation();
    virtual ~RotateAnimation();

public:
    virtual void onUpdate() override;
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ROTATEANIMATION_HPP__

