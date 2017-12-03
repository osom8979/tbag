/**
 * @file   OvershootInterpolator.hpp
 * @brief  OvershootInterpolator class prototype.
 * @author zer0
 * @date   2017-12-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_OVERSHOOTINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_OVERSHOOTINTERPOLATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/animation/interpolator/Interpolator.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

/**
 * OvershootInterpolator class prototype.
 *
 * @author zer0
 * @date   2017-12-03
 *
 * @remarks
 *  An interpolator where the change flings forward and overshoots the last value then comes back.
 */
class TBAG_API OvershootInterpolator : public Interpolator
{
private:
    /**
     * @remarks
     *  Amount of overshoot. When tension equals 0.0f, there is @n
     *  no overshoot and the interpolator becomes a simple      @n
     *  deceleration interpolator.
     */
    float _tension;

public:
    OvershootInterpolator(float tension = 2.0f);
    virtual ~OvershootInterpolator();

public:
    virtual float getInterpolation(float input) override;
};

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_OVERSHOOTINTERPOLATOR_HPP__

