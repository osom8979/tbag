/**
 * @file   Animation.hpp
 * @brief  Animation class prototype.
 * @author zer0
 * @date   2017-11-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ANIMATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/animation/interpolator/Interpolator.hpp>
#include <libtbag/time/TimePoint.hpp>

#include <cassert>
#include <memory>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

struct AnimationCallback
{
    virtual void    onEnd() = 0;
    virtual void onRepeat() = 0;
    virtual void  onStart() = 0;
};

/**
 * Animation class prototype.
 *
 * @author zer0
 * @date   2017-11-22
 */
class TBAG_API Animation : public AnimationCallback
{
public:
    TBAG_CONSTEXPR static int const INFINITE_REPEAT = 0;

public:
    TBAG_CONSTEXPR static int getInfiniteRepeat() TBAG_NOEXCEPT { return INFINITE_REPEAT; }

public:
    using Interpolator       = interpolator::Interpolator;
    using SharedInterpolator = std::shared_ptr<Interpolator>;

    using TimePoint = time::TimePoint;
    using Rep       = std::chrono::milliseconds::rep;

public:
    enum class RepeatMode
    {
        RM_RESTART,
        RM_REVERSE,
    };

protected:
    SharedInterpolator _interpolator; ///< Defines the interpolator used to smooth the animation movement in time.

protected:
    bool _fill_after;   ///< When set to true, the animation transformation is applied after the animation is over.
    bool _fill_before;  ///< When set to true or when fillEnabled is not set to true, the animation transformation is applied before the animation has started.

    RepeatMode _repeat_mode;  ///< Defines the animation behavior.
    int        _repeat_count; ///< Defines how many times the animation should repeat. (0 is infinite)

    Rep  _start_offset; ///< Delay in milliseconds before the animation runs, once start time is reached.
    Rep  _duration;     ///< Amount of time (in milliseconds) for the animation to run.

protected:
    bool _is_started;

protected:
    TimePoint _start;  ///< Start time.
    int       _repeat; ///< Current repeat count.

public:
    Animation();
    virtual ~Animation();

public:
    inline bool  isStarted() const TBAG_NOEXCEPT { return _is_started; }
    inline bool isFinished() const TBAG_NOEXCEPT { return !isStarted(); }

public:
    void clear();

public:
    void update(TimePoint const & tp);

public:
    virtual void    onEnd() override { /* EMPTY. */ }
    virtual void onRepeat() override { /* EMPTY. */ }
    virtual void  onStart() override { /* EMPTY. */ }

public:
    virtual void onUpdate() = 0;
};

/**
 * Obtaining the changed frame number for a period of time.
 *
 * @translate{ko, 시간 경과에 대한 변화된 프레임(Frame)번호를 획득할 수 있도록 도와주는 클래스.}
 *
 * @param[in] fps
 *      Frames per milliseconds (FPS).
 * @param[in] frames
 *      Total frame count.
 * @param[in] milliseconds
 *      Current milliseconds.
 *
 * @return
 *  Result frame number.
 */
template <typename T, typename FloatingType = float>
inline T getNextFrameNumber(T frames, T fps, T milliseconds)
{
    assert(fps != 0);
    assert(frames != 0);
    return static_cast<T>(static_cast<FloatingType>(milliseconds) / static_cast<FloatingType>(fps)) % frames;
}

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ANIMATION_HPP__

