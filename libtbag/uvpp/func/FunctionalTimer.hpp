/**
 * @file   FunctionalTimer.hpp
 * @brief  FunctionalTimer class prototype.
 * @author zer0
 * @date   2017-05-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTIMER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTIMER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/func/FunctionalHandle.hpp>
#include <libtbag/uvpp/Timer.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace func {

/**
 * FunctionalTimer class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename TimerType>
struct FunctionalTimer : public FunctionalHandle<TimerType>
{
    using Parent  = FunctionalHandle<TimerType>;
    using OnTimer = std::function<void(void)>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Timer, Parent);

    OnTimer timer_cb;

    template <typename ... Args>
    FunctionalTimer(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }

    virtual ~FunctionalTimer()
    { /* EMPTY. */ }

    virtual void onTimer() override
    {
        if (timer_cb) {
            timer_cb();
        } else {
            Parent::onTimer();
        }
    }
};

/**
 * FuncTimer typedef.
 *
 * @author zer0
 * @date   2017-05-30
 */
using FuncTimer = FunctionalTimer<libtbag::uvpp::Timer>;

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIONALTIMER_HPP__

