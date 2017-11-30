/**
 * @file   AccelerateInterpolator.hpp
 * @brief  AccelerateInterpolator class prototype.
 * @author zer0
 * @date   2017-11-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ACCELERATEINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ACCELERATEINTERPOLATOR_HPP__

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
 * AccelerateInterpolator class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class TBAG_API AccelerateInterpolator : public Interpolator
{
public:
    AccelerateInterpolator();
    virtual ~AccelerateInterpolator();

public:
    virtual float getInterpolation(float input) override;
};

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ACCELERATEINTERPOLATOR_HPP__

