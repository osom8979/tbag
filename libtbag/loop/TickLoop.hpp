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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/time/Time.hpp>

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
template <typename Unit = std::chrono::milliseconds>
class TickLoop : public Noncopyable
{
public:
    using SystemClock = std::chrono::system_clock;
    using TimePoint   = SystemClock::time_point;
    using TimeUnit    = Unit;
    using Callback    = std::function<void(void)>;

protected:
    Callback _callback;
    TimeUnit _time_step;

// Sync object.
private:
    std::atomic_bool _exit;
    std::atomic_int  _result_code;

public:
    TickLoop(Callback const & callback, TimeUnit step)
            : _callback(callback)
            , _time_step(step)
            , _exit(false)
            , _result_code(0) {
        // EMPTY.
    }

    ~TickLoop() {
        // EMPTY.
    }

public:
    int run() {
        TimePoint start;
        TimeUnit  duration;

        while (_exit.load() == false) {
            start = SystemClock::now();
            _callback();
            duration = time::getElapsedTime(start);

            if (duration < _time_step) {
                std::this_thread::yield(); // hint of thread schedule.
                std::this_thread::sleep_for(_time_step - duration);
            }
        }

        return _result_code;
    }

public:
    inline TimeUnit getTimeStep() const noexcept {
        return this->_time_step;
    }

// Sync object.
public:
    inline void setExit(bool exit = true) noexcept {
        this->_exit = exit;
    }

    inline void exit() noexcept {
        this->setExit(true);
    }

    inline void setResultCode(int code) noexcept {
        this->_result_code = code;
    }
};

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_TICKLOOP_HPP__

