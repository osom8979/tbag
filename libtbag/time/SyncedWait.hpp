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
#include <atomic>
#include <functional>
#include <future>

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
    TBAG_CONSTEXPR static char const * const STATE_NONE = "NONE";
    TBAG_CONSTEXPR static char const * const STATE_READY = "READY";
    TBAG_CONSTEXPR static char const * const STATE_INITIALIZING = "INITIALIZING";
    TBAG_CONSTEXPR static char const * const STATE_RUNNING = "RUNNING";
    TBAG_CONSTEXPR static char const * const STATE_DONE = "DONE";
    TBAG_CONSTEXPR static char const * const STATE_ERROR = "ERROR";

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
        inline void nowRunning() TBAG_NOEXCEPT_SP_OP(_parent._state.store(State::S_RUNNING))
        { _parent._state.store(State::S_RUNNING); }
    };
    friend class RunningSignal;

public:
    enum class State
    {
        S_NONE,
        S_READY,
        S_INITIALIZING,
        S_RUNNING,
        S_DONE,
        S_ERROR,
    };

public:
    using AtomicState = std::atomic<State>;
    using ErrFuture = std::future<Err>;
    using Callback = std::function<Err(RunningSignal&)>;

private:
    AtomicState _state;
    ErrFuture _future;

public:
    SyncedWait();
    ~SyncedWait();

public:
    static char const * getStateName(State s) TBAG_NOEXCEPT;
    static State getState(std::string const & s) TBAG_NOEXCEPT;

public:
    inline State state() const TBAG_NOEXCEPT_SP_OP(_state.load())
    { return _state.load(); }

public:
    Err run(Callback const & cb, int timeout_ms, int tick_ms = 1);

public:
    Err get();
};

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TIME_SYNCEDWAIT_HPP__

