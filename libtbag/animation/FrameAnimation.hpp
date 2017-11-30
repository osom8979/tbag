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

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_FRAMEANIMATION_HPP__

