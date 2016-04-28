/**
 * @file   RenderingLoop.hpp
 * @brief  RenderingLoop class prototype.
 * @author zer0
 * @date   2016-04-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_RENDERINGLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_RENDERINGLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Time.hpp>

#include <atomic>
#include <mutex>
#include <chrono>

#include <iostream>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

/**
 * RenderingLoop class prototype.
 *
 * @author zer0
 * @date   2016-04-28
 */
template <typename Unit = std::chrono::nanoseconds>
class RenderingLoop : public Noncopyable
{
private:
    using SystemClock = std::chrono::system_clock;
    using TimePoint   = SystemClock::time_point;
    using TimeUnit    = Unit;

public:
    struct Callback
    {
        using Looper = RenderingLoop<TimeUnit>;

        virtual void onStart (Looper &) { __EMPTY_BLOCK__ }
        virtual void onEnd   (Looper &) { __EMPTY_BLOCK__ }

        virtual bool update  (Looper &) { return true;    }
        virtual void render  (Looper &) { __EMPTY_BLOCK__ }
    };

private:
    Callback & _callback;
    TimeUnit  _time_step;

// Sync object.
private:
    std::atomic_bool _exit;
    std::mutex _locker;

// Loop variables.
private:
    TimePoint _pre_start;
    TimePoint _now_start;
    TimeUnit  _duration;
    TimeUnit  _time_lag;

public:
    RenderingLoop(Callback & callback, TimeUnit step)
            : _callback(callback)
            , _time_step(step)
            , _exit(false) {
        __EMPTY_BLOCK__
    }

    ~RenderingLoop() {
        this->_exit.store(true);
        this->_locker.lock();
        this->_locker.unlock();
    }

public:
    int run() {
        std::lock_guard<std::mutex> guard(this->_locker);
        this->init();
        this->_callback.onStart(*this);
        this->loop();
        this->_callback.onEnd(*this);
        return 0;
    }

private:
    void init() {
        this->_pre_start = SystemClock::now();
        this->_now_start = SystemClock::now();
        this->_duration  = TimeUnit(0);
        this->_time_lag  = TimeUnit(0);
    }

    /**
     * Main loop.
     *
     * @remarks
     *  Graphviz code:
     *   @code
     *     digraph MainLoop {
     *         now_time -> duration -> pre_time -> lag -> check_lag;
     *         check_lag -> update_loop;
     *         check_lag -> render;
     *         update_loop -> render;
     *     }
     *   @endcode
     */
    void loop() {
        while (!_exit.load()) {
            _now_start = SystemClock::now();
            _duration  = _now_start - _pre_start;
            _pre_start = _now_start;
            _time_lag += _duration;

            while (_time_lag >= _time_step) {
                _time_lag -= _time_step;
                if (!_callback.update(*this)) {
                    this->_exit.store(true);
                }
            }

            _callback.render(*this);
        }
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

// Loop variables.
public:
    inline TimePoint getPreStart() const noexcept {
        return this->_pre_start;
    }

    inline TimePoint getNowStart() const noexcept {
        return this->_now_start;
    }

    inline TimeUnit  getDuration() const noexcept {
        return this->_duration;
    }

    inline TimeUnit  getTimeLag() const noexcept {
        return this->_time_lag;
    }
};

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_RENDERINGLOOP_HPP__

