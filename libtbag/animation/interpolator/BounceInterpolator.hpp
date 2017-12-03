/**
 * @file   BounceInterpolator.hpp
 * @brief  BounceInterpolator class prototype.
 * @author zer0
 * @date   2017-12-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_BOUNCEINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_BOUNCEINTERPOLATOR_HPP__

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
 * BounceInterpolator class prototype.
 *
 * @author zer0
 * @date   2017-12-03
 *
 * @remarks
 *  An interpolator where the change bounces at the end.
 */
class TBAG_API BounceInterpolator : public Interpolator
{
public:
    BounceInterpolator();
    virtual ~BounceInterpolator();

public:
    virtual float getInterpolation(float input) override;

public:
    inline static float bounce(float t) TBAG_NOEXCEPT { return t * t * 8.0f; }
};

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_BOUNCEINTERPOLATOR_HPP__

