/**
 * @file   AlphaAnimation.hpp
 * @brief  AlphaAnimation class prototype.
 * @author zer0
 * @date   2017-11-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ALPHAANIMATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ALPHAANIMATION_HPP__

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
 * AlphaAnimation class prototype.
 *
 * @author zer0
 * @date   2017-11-30
 */
class TBAG_API AlphaAnimation : public Animation
{
private:
    float _from_alpha;  ///< 0.0 ~ 1.0
    float _to_alpha;    ///< 0.0 ~ 1.0

private:
    float _alpha;   ///< 0.0 ~ 1.0

public:
    AlphaAnimation(float from_alpha = 0, float to_alpha = 1);
    virtual ~AlphaAnimation();

public:
    inline float getAlpha() const TBAG_NOEXCEPT { return _alpha; }

public:
    virtual void onUpdate(std::chrono::milliseconds const & duration, float ratio, float interpolated) override;
    virtual void onEnd() override;
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ALPHAANIMATION_HPP__

