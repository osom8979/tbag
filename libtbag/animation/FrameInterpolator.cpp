/**
 * @file   FrameInterpolator.cpp
 * @brief  FrameInterpolator class implementation.
 * @author zer0
 * @date   2016-07-04
 */

#include <libtbag/animation/FrameInterpolator.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

FrameInterpolator::Rep FrameInterpolator::getInterpolation(FrameInterpolator::Rep rep) TBAG_NOEXCEPT
{
    if (_fps == 0 || _frame_count == 0) {
        return 0;
    }
    return std::abs((rep - _start) / _fps % _frame_count /* + 0.5f */);
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

