/**
 * @file   Interpolator.hpp
 * @brief  Interpolator class prototype.
 * @author zer0
 * @date   2016-05-23
 * @date   2017-11-30 (Rename: TimeInterpolator -> Interpolator)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_INTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_INTERPOLATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation    {
namespace interpolator {

/**
 * Interpolator interface.
 *
 * @translate{ko, 값을 보간하는 인터페이스를 제공한다.}
 *
 * @author zer0
 * @date   2015-06-27
 * @date   2015-08-22 (Move the world library)
 * @date   2016-05-23 (Move the tbag library)
 * @date   2017-11-22 (Remove template)
 * @date   2017-11-30 (Rename: TimeInterpolator -> Interpolator)
 */
struct Interpolator
{
    /**
     * Virtual function that returns an interpolated value.
     *
     * @translate{ko, 보간된 값을 반환하는 가상함수.}
     *
     * @return Interpolated value.
     */
    virtual float getInterpolation(float input) = 0;
};

} // namespace interpolator
} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_INTERPOLATOR_INTERPOLATOR_HPP__

