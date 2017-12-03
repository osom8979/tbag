/**
 * @file   AccelerateInterpolator.cpp
 * @brief  AccelerateInterpolator class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/interpolator/AccelerateInterpolator.hpp>
#include <libtbag/log/Log.hpp>

#include <cmath>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

AccelerateInterpolator::AccelerateInterpolator(float factor)
        : _factor(factor), _double_factor(2.0f * factor)
{
    // EMPTY.
}

AccelerateInterpolator::~AccelerateInterpolator()
{
    // EMPTY.
}

float AccelerateInterpolator::getInterpolation(float input)
{
    if (_factor == 1.0f) {
        return input * input;
    } else {
        return std::pow(input, _double_factor);
    }
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

