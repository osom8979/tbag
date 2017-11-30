/**
 * @file   AccelerateDecelerateInterpolator.hpp
 * @brief  AccelerateDecelerateInterpolator class prototype.
 * @author zer0
 * @date   2017-11-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ACCELERATEDECELERATEINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ACCELERATEDECELERATEINTERPOLATOR_HPP__

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
 * AccelerateDecelerateInterpolator class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class TBAG_API AccelerateDecelerateInterpolator : public Interpolator
{
public:
    AccelerateDecelerateInterpolator();
    virtual ~AccelerateDecelerateInterpolator();

public:
    virtual float getInterpolation(float input) override;
};

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ACCELERATEDECELERATEINTERPOLATOR_HPP__

