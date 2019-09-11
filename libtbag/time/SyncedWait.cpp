/**
 * @file   SyncedWait.cpp
 * @brief  SyncedWait class implementation.
 * @author zer0
 * @date   2019-09-11
 */

#include <libtbag/time/SyncedWait.hpp>
#include <libtbag/time/Time.hpp>

#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

SyncedWait::SyncedWait() : _state(State::S_READY)
{
    // EMPTY.
}

SyncedWait::~SyncedWait()
{
    // EMPTY.
}

char const * SyncedWait::getStateName(State s) TBAG_NOEXCEPT
{
    // clang-format off
    switch (s) {
    case State::S_NONE:         return STATE_NONE;
    case State::S_READY:        return STATE_READY;
    case State::S_INITIALIZING: return STATE_INITIALIZING;
    case State::S_RUNNING:      return STATE_RUNNING;
    case State::S_DONE:         return STATE_DONE;
    case State::S_ERROR:        return STATE_ERROR;
    default:                    return STATE_NONE;
    }
    // clang-format on
}

SyncedWait::State SyncedWait::getState(std::string const & s) TBAG_NOEXCEPT
{
    if (s == STATE_READY) {
        return State::S_READY;
    } else if (s == STATE_INITIALIZING) {
        return State::S_INITIALIZING;
    } else if (s == STATE_RUNNING) {
        return State::S_RUNNING;
    } else if (s == STATE_DONE) {
        return State::S_DONE;
    } else if (s == STATE_ERROR) {
        return State::S_ERROR;
    }
    return State::S_NONE;
}

Err SyncedWait::run(Callback const & cb, int timeout_ms, int tick_ms)
{
    if (!cb) {
        return E_ILLARGS;
    }
    if (timeout_ms <= 0 || tick_ms <= 0) {
        return E_ILLARGS;
    }

    _state = State::S_INITIALIZING;
    _future = std::async(std::launch::async, [&]() -> Err {
        RunningSignal signal(*this);
        auto const code = cb(signal);
        _state = isSuccess(code) ? State::S_DONE : State::S_ERROR;
        return code;
    });

    using namespace std::chrono;
    auto const begin = system_clock::now();
    auto const timeout = milliseconds(timeout_ms);
    auto const tick = milliseconds(tick_ms);
    auto const result = syncedWait(begin, timeout, tick, [&]() -> bool {
        return _state != State::S_INITIALIZING;
    });
    return result ? E_SUCCESS : E_TIMEOUT;
}

Err SyncedWait::get()
{
    return _future.get();
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

