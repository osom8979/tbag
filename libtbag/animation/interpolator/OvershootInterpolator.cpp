/**
 * @file   OvershootInterpolator.cpp
 * @brief  OvershootInterpolator class implementation.
 * @author zer0
 * @date   2017-12-03
 */

#include <libtbag/animation/interpolator/OvershootInterpolator.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

OvershootInterpolator::OvershootInterpolator(float tension) : _tension(tension)
{
    // EMPTY.
}

OvershootInterpolator::~OvershootInterpolator()
{
    // EMPTY.
}

float OvershootInterpolator::getInterpolation(float input)
{
    // _o(t) = t * t * ((tension + 1) * t + tension)
    // o(t) = _o(t - 1) + 1
    input -= 1.0f;
    return input * input * ((_tension + 1.0f) * input + _tension) + 1.0f;
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

