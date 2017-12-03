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

#include <cassert>
#include <memory>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

enum class AnimationType : int
{
    AT_UNKNOWN = 0,
    AT_ALPHA,
    AT_FRAME,
    AT_FRAME_NUMBER,
    AT_MOVE,
    AT_ROTATE,
    AT_SCALE,
    AT_FIRST_NUMBER_OF_USER_DEFINED,
};

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
    using Seconds      = std::chrono::seconds;
    using Milliseconds = std::chrono::milliseconds;

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

private:
    int const TYPE;

protected:
    Params _params;

private:
    TimePoint _start; ///< Start time of animation.
    TimePoint _update;
    bool _is_finish;
    bool _is_start;

public:
    Animation();
    explicit Animation(AnimationType type);
    explicit Animation(int type);
    explicit Animation(Params const & params);
    explicit Animation(Params const & params, AnimationType type);
    explicit Animation(Params const & params, int type);
    virtual ~Animation();

public:
    TBAG_CONSTEXPR static int getFirstAnimationTypeNumber() TBAG_NOEXCEPT { return static_cast<int>(AnimationType::AT_UNKNOWN); }
    TBAG_CONSTEXPR static int  getLastAnimationTypeNumber() TBAG_NOEXCEPT { return static_cast<int>(AnimationType::AT_SCALE); }

public:
    inline int getType() const TBAG_NOEXCEPT { return TYPE; }
    inline AnimationType getAnimationType() const TBAG_NOEXCEPT
    {
        if (getFirstAnimationTypeNumber() <= COMPARE_AND(TYPE) <= getLastAnimationTypeNumber()) {
            return static_cast<AnimationType>(TYPE);
        }
        return AnimationType::AT_UNKNOWN;
    }

public:
    inline Params       & atParams()       TBAG_NOEXCEPT { return _params; }
    inline Params const & atParams() const TBAG_NOEXCEPT { return _params; }

    inline TimePoint  getStartTimePoint() const { return _start; }
    inline TimePoint getUpdateTimePoint() const { return _update; }

    inline bool  isStarted() const TBAG_NOEXCEPT { return _is_start; }
    inline bool isFinished() const TBAG_NOEXCEPT { return _is_finish; }
    inline bool  isRunning() const TBAG_NOEXCEPT { return isStarted() && !isFinished(); }

public:
    void clear();

public:
    void start(TimePoint const & tp);
    void reset();
    void stop();

public:
    void update(TimePoint const & tp);

public:
    template <typename DurationType>
    void updateByDelta(DurationType const & dur)
    {
        update(_update + dur);
    }

public:
    void updateBySeconds(Seconds::rep seconds);
    void updateByMilliseconds(Milliseconds::rep milliseconds);

public:
    virtual void onEnd() override { /* EMPTY. */ };

public:
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
    inline static T getNextFrameNumber(T frames, T fps, T milliseconds)
    {
        assert(fps != 0);
        assert(frames != 0);
        return static_cast<T>(static_cast<FloatingType>(milliseconds) / static_cast<FloatingType>(fps)) % frames;
    }
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

