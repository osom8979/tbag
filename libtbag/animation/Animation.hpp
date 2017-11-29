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
#include <libtbag/animation/TimeInterpolator.hpp>

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
    using SharedTimeInterpolator = std::shared_ptr<TimeInterpolator>;

private:
    SharedTimeInterpolator _interpolator;

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
    friend void swap(Animation & lh, Animation & rh)
    {
        lh.swap(rh);
    }

public:
    bool isFinished() const;

public:
    float getInterpolation(float input);
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_ANIMATION_HPP__

