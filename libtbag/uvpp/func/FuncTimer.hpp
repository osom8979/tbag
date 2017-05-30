/**
 * @file   FuncTimer.hpp
 * @brief  FuncTimer class prototype.
 * @author zer0
 * @date   2017-05-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIMER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIMER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
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
 * FuncTimer class prototype.
 *
 * @author zer0
 * @date   2017-05-30
 */
template <typename TimerType, typename MutexType = lock::FakeLock>
class FuncTimer : public TimerType
{
public:
    using Parent = TimerType;
    using Mutex  = MutexType;
    using Guard  = std::lock_guard<Mutex>;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::uvpp::Timer, Parent);

public:
    using OnClose = std::function<void(void)>;
    using OnTimer = std::function<void(void)>;

public:
    mutable Mutex _mutex;

private:
    OnClose _close_cb;
    OnTimer _timer_cb;

public:
    FuncTimer(Loop & loop) : Parent(loop)
    { /* EMPTY. */ }
    virtual ~FuncTimer()
    { /* EMPTY. */ }

public:
    // @formatter:off
    inline void setOnClose(OnClose const & cb) { Guard g(_mutex); _close_cb = cb; }
    inline void setOnTimer(OnTimer const & cb) { Guard g(_mutex); _timer_cb = cb; }
    // @formatter:on

public:
    virtual void onClose() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_close_cb)) {
            _close_cb();
        }
    }

    virtual void onTimer() override
    {
        Guard guard(_mutex);
        if (static_cast<bool>(_timer_cb)) {
            _timer_cb();
        }
    }
};

} // namespace func
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_FUNC_FUNCTIMER_HPP__

