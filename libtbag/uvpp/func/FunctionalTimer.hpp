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
#include <libtbag/lock/FakeLock.hpp>

#include <functional>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace func {

/**
 * FunctionalTimer class prototype.
 *
 * @author zer0
 * @date   2017-05-31
 */
template <typename TimerType, typename MutexType = lock::FakeLock>
class FunctionalTimer : public TimerType
{
public:
    using Parent = TimerType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Timer, Parent);
    TBAG_UVPP_FUNCTIONAL_HANDLE_DEFAULT(Guard, _mutex);

public:
    using OnTimer = std::function<void(void)>;

private:
    Mutex _mutex;
    OnTimer _timer_cb;

public:
    FunctionalTimer(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }
    virtual ~FunctionalTimer()
    { /* EMPTY. */ }

public:
    void setOnTimer(OnTimer const & cb)
    {
        Guard guard(_mutex);
        _timer_cb = cb;
    }

public:
    virtual void onTimer() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_timer_cb)) {
            _timer_cb();
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

