/**
 * @file   TickLoop.hpp
 * @brief  TickLoop class tester.
 * @author zer0
 * @date   2016-04-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_TICKLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_TICKLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/time/ElapsedTime.hpp>

#include <atomic>
#include <chrono>
#include <thread>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

/**
 * TickLoop class prototype.
 *
 * @author zer0
 * @date   2016-04-29
 */
template <typename DurationType = std::chrono::milliseconds>
class TickLoop : private Noncopyable
{
public:
    using Duration    = DurationType;
    using SystemClock = std::chrono::system_clock;
    using TimePoint   = typename SystemClock::time_point;
    using Callback    = std::function<void(void)>;

protected:
    Callback _callback;
    Duration _time_step;

// Sync object.
private:
    std::atomic_bool _exit;
    std::atomic_int  _result_code;

public:
    TickLoop() : _callback(), _time_step(DurationType(1))
    {
        // The 'atomic_bool(bool) & atomic_int(int) constructor' does not exist in MSVC 2013.
        _exit = false;
        _result_code = 0;
    }

    TickLoop(Callback const & callback, Duration step)
            : _callback(callback)
            , _time_step(step)
            , _exit(false)
            , _result_code(0)
    {
        // EMPTY.
    }

    ~TickLoop()
    {
        // EMPTY.
    }

public:
    inline void setCallback(Callback const & callback)
    { _callback = callback; }
    inline void setDuration(Duration const & step)
    { _time_step = step; }

public:
    int run()
    {
        TimePoint start;
        Duration  duration;

        while (_exit.load() == false) {
            start = SystemClock::now();
            _callback();
            duration = time::getDuration<Duration>(start);

            if (duration < _time_step) {
                std::this_thread::yield(); // hint of thread schedule.
                std::this_thread::sleep_for(_time_step - duration);
            }
        }

        return _result_code;
    }

public:
    inline Duration getTimeStep() const TBAG_NOEXCEPT
    { return _time_step; }

// Sync object.
public:
    inline void setExit(bool exit = true) TBAG_NOEXCEPT
    { _exit = exit; }
    inline void exit() TBAG_NOEXCEPT
    { setExit(true); }
    inline void setResultCode(int code) TBAG_NOEXCEPT
    { _result_code = code; }
};

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_TICKLOOP_HPP__

