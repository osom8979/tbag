/**
 * @file   FrameNumberAnimation.hpp
 * @brief  FrameNumberAnimation class prototype.
 * @author zer0
 * @date   2017-11-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMENUMBERANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMENUMBERANIMATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/animation/Animation.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

/**
 * FrameNumberAnimation class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class TBAG_API FrameNumberAnimation : public Animation
{
private:
    int _total_frame;   ///< Total frame count.
    int _current_frame; ///< Current frame index: 0 ~ total_frame-1

public:
    FrameNumberAnimation(int total_frame = 1, int current_frame = 0);
    virtual ~FrameNumberAnimation();

public:
    inline int   getTotalFrame() const TBAG_NOEXCEPT { return   _total_frame; }
    inline int getCurrentFrame() const TBAG_NOEXCEPT { return _current_frame; }

    inline void   setTotalFrame(int frame) TBAG_NOEXCEPT {   _total_frame = frame; }
    inline void setCurrentFrame(int frame) TBAG_NOEXCEPT { _current_frame = frame; }

public:
    virtual void onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated) override;
    virtual void onEnd() override;
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMENUMBERANIMATION_HPP__

