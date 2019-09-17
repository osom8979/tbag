/**
 * @file   SyncedWait.hpp
 * @brief  SyncedWait class prototype.
 * @author zer0
 * @date   2019-09-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TIME_SYNCEDWAIT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TIME_SYNCEDWAIT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>
#include <functional>
#include <future>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/**
 * SyncedWait class prototype.
 *
 * @author zer0
 * @date   2019-09-11
 */
class TBAG_API SyncedWait : private Noncopyable
{
public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    class RunningSignal TBAG_FINAL : private Noncopyable
    {
    private:
        SyncedWait & _parent;
    public:
        RunningSignal(SyncedWait & parent) : _parent(parent)
        { /* EMPTY. */ }
        ~RunningSignal()
        { /* EMPTY. */ }
    public:
        inline void nowRunning()
        { _parent.__set_running(); }
    };
    friend class RunningSignal;

public:
    TBAG_CONSTEXPR static char const * const STATE_READY = "READY";
    TBAG_CONSTEXPR static char const * const STATE_INITIALIZING = "INITIALIZING";
    TBAG_CONSTEXPR static char const * const STATE_RUNNING = "RUNNING";
    TBAG_CONSTEXPR static char const * const STATE_DONE = "DONE";
    TBAG_CONSTEXPR static char const * const STATE_ERROR = "ERROR";

public:
    enum class State
    {
        S_READY,
        S_INITIALIZING,
        S_RUNNING,
        S_DONE,
        S_ERROR,
    };

public:
    using Callback = std::function<Err(RunningSignal&)>;

private:
    Mutex mutable _mutex;
    std::future<Err> _future;
    State _state;

public:
    SyncedWait();
    ~SyncedWait();

public:
    static char const * getStateName(State s) TBAG_NOEXCEPT;
    static State getState(std::string const & s) TBAG_NOEXCEPT;

private:
    void __set_running();

public:
    State state() const;

public:
    TBAG_CONSTEXPR static unsigned const DEFAULT_TIMEOUT_MS = 1000;
    TBAG_CONSTEXPR static unsigned const DEFAULT_TICK_MS = 1;

public:
    Err run(Callback const & cb,
            unsigned timeout_ms = DEFAULT_TIMEOUT_MS,
            unsigned tick_ms = DEFAULT_TICK_MS);
    Err done();

public:
    std::string toString() const;
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_SYNCEDWAIT_HPP__

