/**
 * @file   FrameInterpolator.hpp
 * @brief  FrameInterpolator class prototype.
 * @author zer0
 * @date   2016-05-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEINTERPOLATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/animation/TimeInterpolator.hpp>

#include <cmath>
#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

/**
 * Obtaining the changed frame number for a period of time.
 *
 * @translate{ko, 시간 경과에 대한 변화된 프레임(Frame)번호를 획득할 수 있도록 도와주는 클래스.}
 *
 * @author zer0
 * @date   2015-06-28
 * @date   2015-08-22 (Move the world library)
 * @date   2016-05-23 (Move the tbag library)
 */
class FrameInterpolator : public TimeInterpolator<int, std::chrono::milliseconds>
{
public:
    using BaseInterpolator = TimeInterpolator<int, std::chrono::milliseconds>;
    using ValueType        = typename BaseInterpolator::ValueType;
    using PeriodType       = typename BaseInterpolator::PeriodType;
    using RepType          = typename BaseInterpolator::RepType;

private:
    /**
     * Based on time to start the operation.
     *
     * @translate{ko, 연산을 시작할 기준 시간.}
     */
    RepType _start_millisec = 0;

    /** Frames per milliseconds (FPS). */
    RepType _fps_millisec = 1;

    /** Total frame count. */
    ValueType _frame_size = 1;

public:
    FrameInterpolator() noexcept = default;
    virtual ~FrameInterpolator() noexcept = default;

public:
    inline void set(long  start_millisec
                  , long  fps_millisec
                  , int   frame_size) noexcept {
        this->_start_millisec = start_millisec;
        this->_fps_millisec   = fps_millisec;
        this->_frame_size     = frame_size;
    }

public:
    /**
     * Return the Frame Number.
     *
     * @translate{ko, 보간된 결과값은 Frame 번호가 된다.}
     *
     * @remarks
     *  Example code:
     *  @code{.cpp}
     *  int frame_number = static_cast<int>(animation.getInterpolation(millisec));
     *  @endcode
     */
    virtual ValueType getInterpolation(RepType rep) noexcept override {
        if (_fps_millisec == 0 || _frame_size == 0) {
            return 0;
        }
        return std::abs((rep - _start_millisec) / _fps_millisec % _frame_size /* + 0.5f */);
    }
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEINTERPOLATOR_HPP__

