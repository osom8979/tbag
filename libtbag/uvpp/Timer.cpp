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
        __tbag_error("__global_uv_timer_cb__() handle data is nullptr.");
        return;
    }
    t->onTimer();
}

// ---------------------
// Timer implementation.
// ---------------------

Timer::Timer() : Stream(UvHandleType::TIMER)
{
    // EMPTY.
}

Timer::Timer(Loop & loop) : Timer()
{
    if (init(loop) == false) {
        throw std::bad_alloc();
    }
}

Timer::~Timer()
{
    // EMPTY.
}

bool Timer::init(Loop & loop)
{
    int const CODE = ::uv_timer_init(loop.cast<uv_loop_t>(), Parent::cast<uv_timer_t>());
    if (CODE != 0) {
        __tbag_error("Timer::init() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Timer::start(uint64_t timeout, uint64_t repeat)
{
    // If timeout is zero, the callback fires on the next event loop iteration.
    // If repeat is non-zero, the callback fires first after timeout milliseconds
    // and then repeatedly after repeat milliseconds.
    //
    // Note:
    // Does not update the event loop's concept of "now".
    // See uv_update_time() for more information.

    int const CODE = ::uv_timer_start(Parent::cast<uv_timer_t>(), __global_uv_timer_cb__, timeout, repeat);
    if (CODE != 0) {
        __tbag_error("Timer::start() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Timer::stop()
{
    int const CODE = ::uv_timer_stop(Parent::cast<uv_timer_t>());
    if (CODE != 0) {
        __tbag_error("Timer::stop() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Timer::again()
{
    // If the timer has never been started before it returns UV_EINVAL.
    int const CODE = ::uv_timer_again(Parent::cast<uv_timer_t>());
    if (CODE != 0) {
        __tbag_error("Timer::again() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
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
    __tbag_debug("Timer::onTimer() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

