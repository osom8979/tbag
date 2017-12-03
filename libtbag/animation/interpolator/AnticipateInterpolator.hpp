/**
 * @file   AnticipateInterpolator.hpp
 * @brief  AnticipateInterpolator class prototype.
 * @author zer0
 * @date   2017-12-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ANTICIPATEINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ANTICIPATEINTERPOLATOR_HPP__

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
 * AnticipateInterpolator class prototype.
 *
 * @author zer0
 * @date   2017-12-03
 *
 * @remarks
 *  An interpolator where the change starts backward then flings forward.
 */
class TBAG_API AnticipateInterpolator : public Interpolator
{
private:
    /**
     * @remarks
     *  Amount of anticipation. When tension equals 0.0f,               @n
     *  there is no anticipation and the interpolator becomes a simple  @n
     *  acceleration interpolator.
     */
    float _tension;

public:
    AnticipateInterpolator(float tension = 2.0f);
    virtual ~AnticipateInterpolator();

public:
    virtual float getInterpolation(float input) override;
};

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_ANTICIPATEINTERPOLATOR_HPP__

