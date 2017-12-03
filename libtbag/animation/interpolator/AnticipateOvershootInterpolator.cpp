/**
 * @file   AnticipateOvershootInterpolator.cpp
 * @brief  AnticipateOvershootInterpolator class implementation.
 * @author zer0
 * @date   2017-12-03
 */

#include <libtbag/animation/interpolator/AnticipateOvershootInterpolator.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

AnticipateOvershootInterpolator::AnticipateOvershootInterpolator(float tension, float extra_tension)
        : _tension(tension * extra_tension)
{
    // EMPTY.
}

AnticipateOvershootInterpolator::~AnticipateOvershootInterpolator()
{
    // EMPTY.
}

float AnticipateOvershootInterpolator::getInterpolation(float input)
{
    // a(t, s) = t * t * ((s + 1) * t - s)
    // o(t, s) = t * t * ((s + 1) * t + s)
    // f(t) = 0.5 * a(t * 2, tension * extraTension), when t < 0.5
    // f(t) = 0.5 * (o(t * 2 - 2, tension * extraTension) + 2), when t <= 1.0
    if (input < 0.5f) {
        return 0.5f * a(input * 2.0f, _tension);
    }
    return 0.5f * (o(input * 2.0f - 2.0f, _tension) + 2.0f);
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

