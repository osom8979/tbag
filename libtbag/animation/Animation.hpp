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
#include <libtbag/animation/Interpolator.hpp>

#include <cassert>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

/**
 * Animation class prototype.
 *
 * @author zer0
 * @date   2017-11-22
 */
class TBAG_API Animation
{
public:
    using SharedInterpolator = std::shared_ptr<Interpolator>;

private:
    SharedInterpolator _interpolator;

public:
    Animation();
    Animation(Animation const & obj);
    Animation(Animation && obj);
    virtual ~Animation();

public:
    Animation & operator =(Animation const & obj);
    Animation & operator =(Animation && obj);

public:
    void swap(Animation & obj);

public:
    inline friend void swap(Animation & lh, Animation & rh) { lh.swap(rh); }

public:
    bool isFinished() const;

public:
    float getInterpolation(float input);
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

