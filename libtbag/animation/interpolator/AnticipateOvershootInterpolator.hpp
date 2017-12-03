/**
 * @file   AnticipateOvershootInterpolator.hpp
 * @brief  AnticipateOvershootInterpolator class prototype.
 * @author zer0
 * @date   2017-12-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ANTICIPATEOVERSHOOTINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ANTICIPATEOVERSHOOTINTERPOLATOR_HPP__

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
 * AnticipateOvershootInterpolator class prototype.
 *
 * @author zer0
 * @date   2017-12-03
 *
 * @remarks
 *  An interpolator where the change starts backward then flings forward and overshoots @n
 *  the target value and finally goes back to the final value.
 */
class TBAG_API AnticipateOvershootInterpolator : public Interpolator
{
private:
    /**
     * Amount of anticipation/overshoot. When tension equals 0.0f,     @n
     * there is no anticipation/overshoot and the interpolator becomes @n
     * a simple acceleration/deceleration interpolator.
     */
    float _tension;

public:
    AnticipateOvershootInterpolator(float tension = 2.0f, float extra_tension = 1.5f);
    virtual ~AnticipateOvershootInterpolator();

public:
    virtual float getInterpolation(float input) override;

public:
    inline static float a(float t, float s) TBAG_NOEXCEPT { return t * t * ((s + 1) * t - s); }
    inline static float o(float t, float s) TBAG_NOEXCEPT { return t * t * ((s + 1) * t + s); }
};

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ANTICIPATEOVERSHOOTINTERPOLATOR_HPP__

