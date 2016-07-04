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

void FrameInterpolator::set(long  start_millisec
                          , long  fps_millisec
                          , int   frame_size) noexcept
{
    this->_start_millisec = start_millisec;
    this->_fps_millisec   = fps_millisec;
    this->_frame_size     = frame_size;
}

FrameInterpolator::ValueType FrameInterpolator::getInterpolation(FrameInterpolator::RepType rep) noexcept
{
    if (_fps_millisec == 0 || _frame_size == 0) {
        return 0;
    }

    return std::abs((rep - _start_millisec) / _fps_millisec % _frame_size /* + 0.5f */);
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

