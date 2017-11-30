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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/animation/interpolator/Interpolator.hpp>
#include <libtbag/time/TimePoint.hpp>

#include <memory>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

/**
 * Interface of animation.
 *
 * @author zer0
 * @date   2017-11-30
 */
struct AnimationInterface
{
    virtual void onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated) = 0;
    virtual void onEnd() = 0;
};

/**
 * Animation class prototype.
 *
 * @author zer0
 * @date   2017-11-22
 */
class TBAG_API Animation : public AnimationInterface, private Noncopyable
{
public:
    TBAG_CONSTEXPR static int const INFINITE_REPEAT = 0;

public:
    TBAG_CONSTEXPR static int getInfiniteRepeat() TBAG_NOEXCEPT { return INFINITE_REPEAT; }

public:
    using Interpolator       = interpolator::Interpolator;
    using SharedInterpolator = std::shared_ptr<Interpolator>;

    using TimePoint    = time::TimePoint;
    using Milliseconds = std::chrono::milliseconds;
    using Rep          = Milliseconds::rep;

public:
    enum class RepeatMode
    {
        RM_RESTART,
        RM_REVERSE,
    };

public:
    struct Params
    {
        SharedInterpolator interpolator; ///< Defines the interpolator used to smooth the animation movement in time.

        RepeatMode repeat_mode;  ///< Defines the animation behavior.
        int        repeat_count; ///< Defines how many times the animation should repeat. (0 is infinite)

        Milliseconds  start_offset; ///< Delay in milliseconds before the animation runs, once start time is reached.
        Milliseconds  duration;     ///< Amount of time (in milliseconds) for the animation to run.

        Params() : interpolator(), repeat_mode(RepeatMode::RM_RESTART),
                   repeat_count(INFINITE_REPEAT), start_offset(0), duration(1)
        { /* EMPTY. */ }
        ~Params() { /* EMPTY. */ }

        void clear()
        {
            interpolator.reset();
            repeat_mode  = RepeatMode::RM_RESTART;
            repeat_count = INFINITE_REPEAT;
            duration     = Milliseconds(1);
            start_offset = Milliseconds::zero();
        }

        inline void setRestartMode() TBAG_NOEXCEPT { repeat_mode = RepeatMode::RM_RESTART; }
        inline void setReverseMode() TBAG_NOEXCEPT { repeat_mode = RepeatMode::RM_REVERSE; }
    };

protected:
    Params _params;

private:
    TimePoint _start; ///< Start time of animation.
    bool _is_finish;
    bool _is_start;

public:
    Animation();
    Animation(Params const & params);
    virtual ~Animation();

public:
    inline Params       & atParams()       TBAG_NOEXCEPT { return _params; }
    inline Params const & atParams() const TBAG_NOEXCEPT { return _params; }

public:
    inline TimePoint getStart() const { return _start; }
    inline void setStart(TimePoint const & tp) { _start = tp; }

public:
    inline bool  isStarted() const TBAG_NOEXCEPT { return _is_start; }
    inline bool isFinished() const TBAG_NOEXCEPT { return _is_finish; }

public:
    void clear();

public:
    void start();
    void reset();
    void stop();

public:
    void update(TimePoint const & tp);

public:
    virtual void onEnd() override { /* EMPTY. */ };
};

/**
 * CachedAnimation class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class CachedAnimation : public Animation
{
protected:
    Milliseconds _duration;
    float        _ratio;
    float        _interpolated;

public:
    CachedAnimation() : _duration(0), _ratio(0), _interpolated(0) { /* EMPTY. */ }
    virtual ~CachedAnimation() { /* EMPTY. */ }

public:
    inline Milliseconds getDuration() const TBAG_NOEXCEPT { return _duration; }
    inline float           getRatio() const TBAG_NOEXCEPT { return _ratio; }
    inline float    getInterpolated() const TBAG_NOEXCEPT { return _interpolated; }

public:
    virtual void onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated) override
    {
        _duration     = duration;
        _ratio        = ratio;
        _interpolated = interpolated;
    }
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ANIMATION_HPP__

