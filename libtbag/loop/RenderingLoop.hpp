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
#include <libtbag/Log.hpp>

#include <cassert>

#include <atomic>
#include <chrono>
#include <thread>

//#define ENABLE_RENDERING_LOOP_DEBUG_VERBOSE

#if defined(ENABLE_RENDERING_LOOP_DEBUG_VERBOSE)
# define __RENDERING_LOOP_DEBUG_VERBOSE_LOG(format, ...) \
    _DIRECT_CONSOLE_LOG(format, __VA_ARGS__)
#else
# define __RENDERING_LOOP_DEBUG_VERBOSE_LOG(format, ...)
#endif

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
        using Looper   = RenderingLoop<TimeUnit>;

        virtual void onStart (Looper &) { __EMPTY_BLOCK__ }
        virtual void onEnd   (Looper &) { __EMPTY_BLOCK__ }

        virtual bool update  (Looper &) { return true;    }
        virtual void render  (Looper &) { __EMPTY_BLOCK__ }
    };

// User's settings.
private:
    Callback & _callback;
    TimeUnit   _time_step;

// Sync object.
private:
    std::atomic_bool _exit;
    std::atomic_int  _result_code;

// Loop variables.
private:
    TimePoint  _pre_start;
    TimePoint  _now_start;
    TimeUnit   _duration;
    TimeUnit   _time_lag;

public:
    RenderingLoop(Callback & callback, TimeUnit step)
            : _callback(callback)
            , _time_step(step)
            , _exit(false)
            , _result_code(0) {
        __EMPTY_BLOCK__
    }

    ~RenderingLoop() {
        this->_exit.store(true);
    }

public:
    int run(bool enable_sleep_step = true) {
        this->init();
        this->_callback.onStart(*this);
        this->loop();
        this->_callback.onEnd(*this);
        return _result_code;
    }

private:
    void init() {
        this->_pre_start = SystemClock::now();
        this->_now_start = SystemClock::now();
        this->_duration  = TimeUnit(0);
        this->_time_lag  = TimeUnit(0);
    }

private:
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
    void loop(bool enable_sleep_step = true) {
        TimeUnit sleep_time;

        while (!_exit.load()) {
            _now_start = SystemClock::now();
            _duration  = _now_start - _pre_start;
            _pre_start = _now_start;
            _time_lag += _duration; // 직전 루프의 경과시간을 TIME_LAG에 더해 준다.
                                    // 1루프당 1TIME_STEP을 초과하였는지 확인할 수 있다.
                                    // TIME_LAG는 항상 양수를 보장받아야 한다.

            assert(_duration.count() >= 0);
            assert(_time_lag.count() >= 0);

            // 1루프 당 1회의 업데이트를 위한 중첩 LOOP.
            while (_time_lag >= _time_step) {

                // 1루프 당 1회의 업데이트를 보장하기 위해 아래와 같이 TIME_STEP 뺀다.
                _time_lag -= _time_step;

                __RENDERING_LOOP_DEBUG_VERBOSE_LOG(
                        "UPDATE: LAG[%lld] DUR[%lld]\n"
                        , _time_lag.count()
                        , _duration.count());

                // 만약 이벤트 확보를 위한 콜백이 필요하다면 UPDATE 콜백에 추가한다.
                if (!_callback.update(*this)) {
                    _exit.store(true);
                    break;
                }
            }

            assert(_time_lag.count() >= 0);

            __RENDERING_LOOP_DEBUG_VERBOSE_LOG(
                    "RENDER: LAG[%lld] DURA[%lld]\n"
                    , _time_lag.count()
                    , _duration.count());

            _callback.render(*this);

            if (enable_sleep_step) {
                // 1루프를 1TIME_STEP 보다 더 빠르게 끝냈을 경우
                // 강제로 1TIME_STEP을 맞추기 위한 SLEEP.
                sleep_time = SystemClock::now() - _now_start;
                if (sleep_time < _time_step) {
                    // std::this_thread::yield();
                    std::this_thread::sleep_for(_time_step - sleep_time);
                }
            }
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

    inline void setResultCode(int code) noexcept {
        this->_result_code = code;
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

