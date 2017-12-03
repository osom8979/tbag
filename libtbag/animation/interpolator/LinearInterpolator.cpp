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

LinearInterpolator::LinearInterpolator()
{
    // EMPTY.
}

LinearInterpolator::~LinearInterpolator()
{
    // EMPTY.
}

float LinearInterpolator::getInterpolation(float input)
{
    return input;
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

