/**
 * @file   DecelerateInterpolator.cpp
 * @brief  DecelerateInterpolator class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/interpolator/DecelerateInterpolator.hpp>
#include <libtbag/log/Log.hpp>

#include <cmath>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

DecelerateInterpolator::DecelerateInterpolator(float factor) : _factor(factor)
{
    // EMPTY.
}

DecelerateInterpolator::~DecelerateInterpolator()
{
    // EMPTY.
}

float DecelerateInterpolator::getInterpolation(float input)
{
    if (_factor == 1.0f) {
        return (1.0f - (1.0f - input) * (1.0f - input));
    } else {
        return (1.0f - std::pow((1.0f - input), 2 * _factor));
    }
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

