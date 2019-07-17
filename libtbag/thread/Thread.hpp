/**
 * @file   Thread.hpp
 * @brief  Thread class prototype.
 * @author zer0
 * @date   2019-07-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREAD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREAD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <atomic>
#include <exception>
#include <thread>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * Thread class prototype.
 *
 * @author zer0
 * @date   2019-07-17
 */
class TBAG_API Thread : private Noncopyable
{
public:
    using uthread = libtbag::uvpp::uthread;

public:
    struct start_t { /* EMPTY. */ };

public:
    TBAG_CONSTEXPR static start_t const start = {};

public:
    enum class State
    {
        S_READY,
        S_RUNNING,
        S_DONE,
    };

private:
    bool const JOIN_IN_DESTRUCTORS;

private:
    uthread _thread;
    std::atomic<State> _state;

private:
    std::exception_ptr _exception;

public:
    Thread(bool join_in_destructors = true) TBAG_NOEXCEPT;
    Thread(start_t, bool join_in_destructors = true);
    virtual ~Thread();

public:
    inline State state() const TBAG_NOEXCEPT_SP_OP(_state.load())
    { return _state.load(); }
    inline uthread id() const TBAG_NOEXCEPT
    { return _thread; }

public:
    bool operator ==(Thread const & obj) const TBAG_NOEXCEPT;
    bool equal(uthread const & t) const TBAG_NOEXCEPT;
    bool equal(Thread const & t) const TBAG_NOEXCEPT;

public:
    static uthread getCurrentThreadId() TBAG_NOEXCEPT;

public:
    bool isCurrentThread() TBAG_NOEXCEPT;

public:
    Err run(std::size_t stack_size);
    Err run();

protected:
    virtual void onRunner()
    { /* EMPTY. */ }

public:
    void rethrowIfExists() const;

public:
    bool joinable() const;

public:
    Err join(bool rethrow = true);

private:
    friend TBAG_API void __global_uv_thread_cb__(void * args) TBAG_NOEXCEPT;

public:
    template <typename TimeoutDuration, typename TickDuration = TimeoutDuration>
    static Err waitForRunningOrDone(Thread const & t, TimeoutDuration const & timeout, TickDuration const & tick)
    {
        auto const begin = std::chrono::system_clock::now();
        while ((std::chrono::system_clock::now() - begin) < timeout) {
            if (t.state() != Thread::State::S_READY) {
                return E_SUCCESS;
            }
            std::this_thread::sleep_for(tick);
        }
        return E_TIMEOUT;
    }

public:
    Err waitForRunningOrDone(unsigned long timeout_ms, unsigned long tick_ms = 1);
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREAD_HPP__

