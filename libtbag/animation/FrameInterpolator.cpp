/**
 * @file   FrameInterpolator.cpp
 * @brief  FrameInterpolator class implementation.
 * @author zer0
 * @date   2016-07-04
 */

#include <libtbag/animation/FrameInterpolator.hpp>
#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

float FrameInterpolator::getInterpolation(float input)
{
    if (_fps == 0 || _fcount == 0) {
        return 0;
    }
    return (int)((input - _start) / _fps) % _fcount;
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

