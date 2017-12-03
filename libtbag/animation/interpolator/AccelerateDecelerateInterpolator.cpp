/**
 * @file   AccelerateDecelerateInterpolator.cpp
 * @brief  AccelerateDecelerateInterpolator class implementation.
 * @author zer0
 * @date   2017-11-30
 */

#include <libtbag/animation/interpolator/AccelerateDecelerateInterpolator.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/math/Constant.hpp>

#include <cmath>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

AccelerateDecelerateInterpolator::AccelerateDecelerateInterpolator()
{
    // EMPTY.
}

AccelerateDecelerateInterpolator::~AccelerateDecelerateInterpolator()
{
    // EMPTY.
}

float AccelerateDecelerateInterpolator::getInterpolation(float input)
{
    return static_cast<float>(std::cos((input + 1.0f) * MATH_CONSTANT_PI) / 2.0f) + 0.5f;
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

