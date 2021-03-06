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
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

#include <mutex>
#include <atomic>
#include <chrono>
#include <thread>

// Developer only.
//#define ENABLE_RENDERING_LOOP_DEBUG_VERBOSE

#if defined(ENABLE_RENDERING_LOOP_DEBUG_VERBOSE)
# define __RENDERING_LOOP_DEBUG_VERBOSE_LOG(format, ...)
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
class RenderingLoop : private Noncopyable
{
public:
    using SystemClock = std::chrono::system_clock;
    using TimePoint   = SystemClock::time_point;
    using TimeUnit    = Unit;

public:
    struct Callback
    {
        using Looper = RenderingLoop<TimeUnit>;

        virtual void onStart (Looper &) { /* EMPTY. */ }
        virtual void onEnd   (Looper &) { /* EMPTY. */ }

        virtual void update  (Looper &) { /* EMPTY. */ }
        virtual void render  (Looper &) { /* EMPTY. */ }

        virtual bool isExit  (Looper &) { return false;   }
    };

// User's settings.
private:
    Callback & _callback;
    TimeUnit   _time_step;

// Sync object.
private:
    mutable std::mutex _mutex;
    std::atomic_bool   _exit;
    std::atomic_int    _result_code;

// Loop variables.
private:
    TimePoint  _pre_start;
    TimePoint  _now_start;
    TimeUnit   _duration;
    TimeUnit   _time_lag;

public:
    RenderingLoop(Callback & callback, TimeUnit step) : _callback(callback), _time_step(step)
    {
        // The 'atomic_bool(bool) & atomic_int(int) constructor' does not exist in MSVC 2013.
        _exit = false;
        _result_code = 0;
    }

    ~RenderingLoop()
    {
        this->_exit.store(true);
    }

public:
    int run(bool enable_sleep_step = true)
    {
        std::lock_guard<std::mutex> guard(_mutex);
        this->init();
        this->_callback.onStart(*this);
        this->loop(enable_sleep_step);
        this->_callback.onEnd(*this);
        return _result_code;
    }

    inline bool isRunning() const TBAG_NOEXCEPT
    {
        if (_mutex.try_lock()) {
            _mutex.unlock();
            return false;
        }
        return true;
    }

private:
    void init()
    {
        this->_pre_start = SystemClock::now();
        this->_now_start = this->_pre_start;
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
    void loop(bool enable_sleep_step)
    {
        TimeUnit sleep_time;

        while (!_exit.load()) {
            _pre_start = _now_start;
            _now_start = SystemClock::now();
            _duration  = std::chrono::duration_cast<TimeUnit>(_now_start - _pre_start);
            _time_lag += _duration; // 직전 루프의 경과시간을 TIME_LAG에 더해 준다.
                                    // 1루프당 1TIME_STEP을 초과하였는지 확인할 수 있다.
                                    // TIME_LAG는 항상 양수를 보장받아야 한다.

            __RENDERING_LOOP_DEBUG_VERBOSE_LOG(
                    "START LOOP: PRE[%09lld] NOW[%09lld] LAG[%09lld] DUR[%09lld] STEP[%09lld]\n",
                    _pre_start.time_since_epoch().count(),
                    _now_start.time_since_epoch().count(),
                    _time_lag.count(),
                    _duration.count(),
                    _time_step.count());

#if (defined(TBAG_COMP_GNUC_CXX) && (TBAG_COMP_GNUC_VERSION > 50401)) || !defined(TBAG_COMP_GNUC_CXX) || defined(TBAG_COMP_CLANG)
            // Unknown std::chrono bug (GCC<=50401): {cmp rax, rbx}
            assert(_pre_start.time_since_epoch().count() <= _now_start.time_since_epoch().count());
#endif
            assert(_duration.count() >= 0);
            assert(_time_lag.count() >= 0);

            // 1루프 당 1회의 업데이트를 위한 중첩 LOOP.
            while (_time_lag >= _time_step) {

                // 1루프 당 1회의 업데이트를 보장하기 위해 아래와 같이 TIME_STEP 뺀다.
                _time_lag -= _time_step;

                __RENDERING_LOOP_DEBUG_VERBOSE_LOG(
                        "UPDATE LOOP: LAG[%09lld] DUR[%09lld]\n",
                        _time_lag.count(),
                        _duration.count());

                // 만약 이벤트 확보를 위한 EVENT 콜백이 필요하다면 UPDATE 콜백에 추가한다.
                _callback.update(*this);
            }

            assert(_time_lag.count() >= 0);

            __RENDERING_LOOP_DEBUG_VERBOSE_LOG(
                    "RENDER: LAG[%09lld] DURA[%09lld]\n",
                    _time_lag.count(),
                    _duration.count());

            _callback.render(*this);

            if (enable_sleep_step) {
                // 1루프를 1TIME_STEP 보다 더 빠르게 끝냈을 경우
                // 강제로 1TIME_STEP을 맞추기 위한 SLEEP.
                sleep_time = std::chrono::duration_cast<TimeUnit>(SystemClock::now() - _now_start);
                if (sleep_time < _time_step) {

                    __RENDERING_LOOP_DEBUG_VERBOSE_LOG(
                            "SLEEP TIME: %09lld\n",
                            sleep_time.count());

                    // std::this_thread::yield();
                    std::this_thread::sleep_for(_time_step - sleep_time);
                }
            }

            if (_callback.isExit(*this) == true) {
                _exit.store(true);
                break;
            }
        }
    }

public:
    inline TimeUnit getTimeStep() const TBAG_NOEXCEPT { return this->_time_step; }

// Sync object.
public:
    inline void setExit(bool exit = true) TBAG_NOEXCEPT { this->_exit = exit; }
    inline void exit() TBAG_NOEXCEPT { this->setExit(true); }
    inline void setResultCode(int code) TBAG_NOEXCEPT { this->_result_code = code; }

// Loop variables.
public:
    inline TimePoint getPreStart() const TBAG_NOEXCEPT { return this->_pre_start; }
    inline TimePoint getNowStart() const TBAG_NOEXCEPT { return this->_now_start; }
    inline TimeUnit  getDuration() const TBAG_NOEXCEPT { return this->_duration; }
    inline TimeUnit   getTimeLag() const TBAG_NOEXCEPT { return this->_time_lag; }
};

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_RENDERINGLOOP_HPP__

