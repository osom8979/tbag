/**
 * @file   SyncedWait.cpp
 * @brief  SyncedWait class implementation.
 * @author zer0
 * @date   2019-09-11
 */

#include <libtbag/time/SyncedWait.hpp>
#include <libtbag/time/Time.hpp>

#include <cassert>
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
    try {
        done();
    } catch (...) {
        COMMENT("Remove the exception")
    }
}

void SyncedWait::__set_running()
{
    Guard const G(_mutex);
    _state = State::S_RUNNING;
}

SyncedWait::State SyncedWait::state() const
{
    Guard const G(_mutex);
    return _state;
}

char const * SyncedWait::getStateName(State s) TBAG_NOEXCEPT
{
    // clang-format off
    switch (s) {
    case State::S_INITIALIZING: return STATE_INITIALIZING;
    case State::S_RUNNING:      return STATE_RUNNING;
    case State::S_DONE:         return STATE_DONE;
    case State::S_ERROR:        return STATE_ERROR;
    default:                    return STATE_READY;
    }
    // clang-format on
}

SyncedWait::State SyncedWait::getState(std::string const & s) TBAG_NOEXCEPT
{
    if (s == STATE_INITIALIZING) {
        return State::S_INITIALIZING;
    } else if (s == STATE_RUNNING) {
        return State::S_RUNNING;
    } else if (s == STATE_DONE) {
        return State::S_DONE;
    } else if (s == STATE_ERROR) {
        return State::S_ERROR;
    }
    assert(s == STATE_READY);
    return State::S_READY;
}

Err SyncedWait::run(Callback const & cb, unsigned timeout_ms, unsigned tick_ms)
{
    _mutex.lock();
    bool illegal_state;
    if (_state != State::S_READY) {
        illegal_state = true;
    } else {
        illegal_state = false;
        _state = State::S_INITIALIZING;
        _future = std::async(std::launch::async, [&, cb]() -> Err {
            RunningSignal signal(*this);
            auto const code = cb ? cb(signal) : E_ILLARGS;
            Guard const G(_mutex);
            _state = isSuccess(code) ? State::S_DONE : State::S_ERROR;
            return code;
        });
    }
    _mutex.unlock();

    if (illegal_state) {
        return E_ILLSTATE;
    }

    using namespace std::chrono;
    auto const begin = system_clock::now();
    auto const timeout = milliseconds(timeout_ms);
    auto const tick = milliseconds(tick_ms);
    auto const result = syncedWait(begin, timeout, tick, [&]() -> bool {
        Guard const G(_mutex);
        return _state != State::S_INITIALIZING;
    });
    return result ? E_SUCCESS : E_TIMEOUT;
}

Err SyncedWait::done()
{
    _mutex.lock();
    bool illegal_state;
    std::future<Err> future;
    if (_state == State::S_READY) {
        illegal_state = true;
    } else {
        illegal_state = false;
        future.swap(_future);
        assert(!_future.valid());
    }
    _mutex.unlock();

    if (illegal_state) {
        return E_ILLSTATE;
    }

    assert(future.valid());
    auto const code = future.get();

    _mutex.lock();
    assert(_state == State::S_DONE || _state == State::S_ERROR);
    assert(!_future.valid());
    _state = State::S_READY;
    _mutex.unlock();

    return code;
}

std::string SyncedWait::toString() const
{
    return getStateName(state());
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

