/**
 * @file   LinearInterpolator.cpp
 * @brief  LinearInterpolator class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/interpolator/LinearInterpolator.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

LinearInterpolator::LinearInterpolator() : LinearInterpolator(1, 0)
{
    // EMPTY.
}

LinearInterpolator::LinearInterpolator(float gradient, float y_intercept)
        : _gradient(gradient), _y_intercept(y_intercept)
{
}

LinearInterpolator::~LinearInterpolator()
{
    // EMPTY.
}

float LinearInterpolator::getInterpolation(float input)
{
    return input * _gradient + _y_intercept;
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

