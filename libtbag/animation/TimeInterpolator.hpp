/**
 * @file   TimeInterpolator.hpp
 * @brief  TimeInterpolator class prototype.
 * @author zer0
 * @date   2016-05-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_TIMEINTERPOLATOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_TIMEINTERPOLATOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace animation {

/**
 * Time based interpolator interface.
 *
 * @translate{ko, 시간에 기반하여 값을 보간하는 인터페이스를 제공한다.}
 *
 * @author zer0
 * @date   2015-06-27
 * @date   2015-08-22 (Move the world library)
 * @date   2016-05-23 (Move the tbag library)
 */
template <typename RepType>
struct TimeInterpolator
{
    using Rep = RepType;

    /**
     * Virtual function that returns an interpolated value.
     *
     * @translate{ko, 보간된 값을 반환하는 가상함수.}
     *
     * @param rep
     *          [in] Current time.
     *
     * @return Interpolated value.
     */
    virtual Rep getInterpolation(Rep rep) TBAG_NOEXCEPT = 0;
};

} // namespace animation

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ANIMATION_TIMEINTERPOLATOR_HPP__

