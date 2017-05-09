/**
 * @file   Timer.cpp
 * @brief  Timer class implementation.
 * @author zer0
 * @date   2017-01-08
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_timer_cb__(uv_timer_t * handle)
{
    Timer * t = static_cast<Timer*>(handle->data);
    if (t == nullptr) {
        tDLogE("__global_uv_timer_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(t)) {
        tDLogE("__global_uv_timer_cb__() handle.data is deleted.");
    } else {
        t->onTimer();
    }
}

// ---------------------
// Timer implementation.
// ---------------------

Timer::Timer() : Handle(uhandle::TIMER)
{
    // EMPTY.
}

Timer::Timer(Loop & loop) : Timer()
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

Timer::~Timer()
{
    // EMPTY.
}

uerr Timer::init(Loop & loop)
{
    int const CODE = ::uv_timer_init(loop.cast<uv_loop_t>(), Parent::cast<uv_timer_t>());
    return getUerr2("Timer::init()", CODE);
}

uerr Timer::start(uint64_t timeout, uint64_t repeat)
{
    // If timeout is zero, the callback fires on the next event loop iteration.
    // If repeat is non-zero, the callback fires first after timeout milliseconds
    // and then repeatedly after repeat milliseconds.
    //
    // Note:
    // Does not update the event loop's concept of "now".
    // See uv_update_time() for more information.

    int const CODE = ::uv_timer_start(Parent::cast<uv_timer_t>(), __global_uv_timer_cb__, timeout, repeat);
    return getUerr2("Timer::start()", CODE);
}

uerr Timer::stop()
{
    int const CODE = ::uv_timer_stop(Parent::cast<uv_timer_t>());
    return getUerr2("Timer::stop()", CODE);
}

uerr Timer::again()
{
    // If the timer has never been started before it returns UV_EINVAL.
    int const CODE = ::uv_timer_again(Parent::cast<uv_timer_t>());
    return getUerr2("Timer::again()", CODE);
}

void Timer::setRepeat(uint64_t repeat)
{
    // The timer will be scheduled to run on the given interval,
    // regardless of the callback execution duration,
    // and will follow normal timer semantics in the case of a time-slice overrun.
    //
    // For example, if a 50ms repeating timer first runs for 17ms,
    // it will be scheduled to run again 33ms later.
    // If other tasks consume more than the 33ms following the first timer callback,
    // then the callback will run as soon as possible.
    //
    // Note:
    // If the repeat value is set from a timer callback it does not immediately take effect.
    // If the timer was non-repeating before, it will have been stopped.
    // If it was repeating, then the old repeat value will have been used to schedule the next timeout.
    ::uv_timer_set_repeat(Parent::cast<uv_timer_t>(), repeat);
}

uint64_t Timer::getRepeat()
{
    return ::uv_timer_get_repeat(Parent::cast<uv_timer_t>());
}

// --------------
// Event methods.
// --------------

void Timer::onTimer()
{
    tDLogD("Timer::onTimer() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

