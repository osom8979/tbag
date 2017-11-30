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
    float _from_alpha;
    float _to_alpha;

public:
    AlphaAnimation();
    virtual ~AlphaAnimation();

public:
    virtual void onUpdate() override;
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ALPHAANIMATION_HPP__

