/**
 * @file   AnticipateInterpolator.cpp
 * @brief  AnticipateInterpolator class implementation.
 * @author zer0
 * @date   2017-12-03
 */

#include <libtbag/animation/interpolator/AnticipateInterpolator.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

AnticipateInterpolator::AnticipateInterpolator(float tension) : _tension(tension)
{
    // EMPTY.
}

AnticipateInterpolator::~AnticipateInterpolator()
{
    // EMPTY.
}

float AnticipateInterpolator::getInterpolation(float input)
{
    return input * input * ((_tension + 1.0f) * input - _tension);
}

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

