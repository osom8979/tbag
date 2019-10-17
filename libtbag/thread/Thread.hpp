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
#include <libtbag/lock/UvLock.hpp>

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
    using UvLock = libtbag::lock::UvLock;
    using UvGuard = libtbag::lock::UvLockGuard<UvLock>;

public:
    TBAG_CONSTEXPR static std::size_t NO_ASSIGN_ID = 0;

public:
    /**
     * Thread state enumeration.
     *
     * @remarks
     *  The state is moved from <code>READY -> CREATED -> RUNNING</code> -> DONE.
     *
     * @warning
     *  It is not converted in the reverse direction.
     *
     * @translate{ko, 상태는 READY -> CREATED -> RUNNING -> DONE 순서로 이동한다. 역방향으로 변환되지 않는다.}
     */
    enum class State
    {
        S_READY,
        S_CREATED, ///< It will be running soon.
        S_RUNNING,
        S_DONE,
    };

private:
    UvLock mutable _lock;

    /**
     * @warning
     *  After joining, the corresponding handle can not be used.
     */
    uthread _thread;

    /**
     * The handle ID that can be used after joining.
     */
    std::size_t _id;

    /**
     * Current thread state.
     */
    State _state;

    /**
     * Flag variable to prevent multiple joins.
     */
    bool _once_joined;

private:
    std::exception_ptr _exception;

private:
    void * _opaque; ///< Internally, this value is not modified.

public:
    Thread() TBAG_NOEXCEPT;
    virtual ~Thread();

public:
    inline void * getOpaque() const TBAG_NOEXCEPT
    { return _opaque; }
    inline void setOpaque(void * opaque) TBAG_NOEXCEPT
    { _opaque = opaque; }

public:
    /**
     * @warning
     *  After the join, the value may change.
     */
    uthread tid() const;
    std::size_t id() const;
    State state() const;

public:
    /**
     * @warning
     *  After the join, you should not use the comparison.
     */
    static bool equal(uthread const & lh, uthread const & rh) TBAG_NOEXCEPT;

public:
    bool equal(uthread const & tid) const TBAG_NOEXCEPT;
    bool equal(Thread const & thread) const TBAG_NOEXCEPT;

public:
    bool operator ==(Thread const & obj) const TBAG_NOEXCEPT;
    bool operator !=(Thread const & obj) const TBAG_NOEXCEPT;

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

public:
    Err kill() const;
};

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREAD_HPP__

