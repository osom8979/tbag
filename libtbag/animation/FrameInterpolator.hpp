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
#include <libtbag/predef.hpp>
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
class TBAG_API FrameInterpolator : public TimeInterpolator<std::chrono::microseconds>
{
public:
    using Parent = TimeInterpolator<std::chrono::microseconds>;

private:
    /**
     * Based on time to start the operation.
     *
     * @translate{ko, 연산을 시작할 기준 시간.}
     */
    Rep _start = 0;

    /** Frames per milliseconds (FPS). */
    int _fps = 1;

    /** Total frame count. */
    int _frame_count = 1;

public:
    FrameInterpolator() TBAG_NOEXCEPT = default;
    virtual ~FrameInterpolator() TBAG_NOEXCEPT = default;

public:
    inline void setStart(Rep start) TBAG_NOEXCEPT
    { _start = start; }
    inline void setFps(int fps) TBAG_NOEXCEPT
    { _fps = fps; }
    inline void setFrameSize(int frame_count) TBAG_NOEXCEPT
    { _frame_count = frame_count; }
    inline void set(Rep start, int fps, int frame_count) TBAG_NOEXCEPT
    {
        _start = start;
        _fps = fps;
        _frame_count = frame_count;
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
     *   int frame_number = static_cast<int>(animation.getInterpolation(millisec));
     *  @endcode
     */
    virtual Rep getInterpolation(Rep rep) TBAG_NOEXCEPT override;
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEINTERPOLATOR_HPP__

