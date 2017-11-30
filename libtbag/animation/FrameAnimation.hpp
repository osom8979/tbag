/**
 * @file   FrameAnimation.hpp
 * @brief  FrameAnimation class prototype.
 * @author zer0
 * @date   2017-11-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEANIMATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/animation/Animation.hpp>

#include <cassert>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

/**
 * FrameAnimation class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class TBAG_API FrameAnimation : public Animation
{
public:
    struct FrameInfo
    {
        int _index;
        int _duration;
    };

public:
    using FrameInfos = std::vector<FrameInfo>;

private:
    FrameInfos _infos;

public:
    FrameAnimation();
    virtual ~FrameAnimation();

public:
    virtual void onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated) override;
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEANIMATION_HPP__

