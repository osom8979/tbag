/**
 * @file   BounceInterpolator.cpp
 * @brief  BounceInterpolator class implementation.
 * @author zer0
 * @date   2017-12-03
 */

#include <libtbag/animation/interpolator/BounceInterpolator.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

BounceInterpolator::BounceInterpolator()
{
    // EMPTY.
}

BounceInterpolator::~BounceInterpolator()
{
    // EMPTY.
}

float BounceInterpolator::getInterpolation(float input)
{
    // _b(t) = t * t * 8
    // bs(t) = _b(t) for t < 0.3535
    // bs(t) = _b(t - 0.54719) + 0.7 for t < 0.7408
    // bs(t) = _b(t - 0.8526) + 0.9 for t < 0.9644
    // bs(t) = _b(t - 1.0435) + 0.95 for t <= 1.0
    // b(t) = bs(t * 1.1226)
    input *= 1.1226f;
    if (input < 0.3535f) {
        return bounce(input);
    } else if (input < 0.7408f) {
        return bounce(input - 0.54719f) + 0.7f;
    } else if (input < 0.9644f) {
        return bounce(input - 0.8526f) + 0.9f;
    }
    return bounce(input - 1.0435f) + 0.95f;
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

