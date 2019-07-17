/**
 * @file   Thread.cpp
 * @brief  Thread class implementation.
 * @author zer0
 * @date   2019-07-17
 */

#include <libtbag/thread/Thread.hpp>
#include <uv.h>
#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

using uthread = Thread::uthread;
using State = Thread::State;

void __global_uv_thread_cb__(void * args) TBAG_NOEXCEPT
{
    assert(args != nullptr);
    auto * thread = (Thread*)args;

    thread->_lock.lock();
    thread->_state = Thread::State::S_RUNNING;
    thread->_lock.unlock();

    try {
        thread->onRunner();
    } catch (...) {
        thread->_exception = std::current_exception();
    }

    thread->_lock.lock();
    thread->_state = Thread::State::S_DONE;
    thread->_lock.unlock();
}

Thread::Thread(bool join_in_destructors) TBAG_NOEXCEPT
        : JOIN_IN_DESTRUCTORS(join_in_destructors), _thread(), _state(State::S_READY), _exception()
{
    // EMPTY.
}

Thread::Thread(start_t, bool join_in_destructors) : Thread(join_in_destructors)
{
    auto const code = run();
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

Thread::~Thread()
{
    if (JOIN_IN_DESTRUCTORS && joinable()) {
        join(false); // Destructor is an implicit 'NOEXCEPT' state.
    }
    // S_READY is E_ESRCH
    // S_DONE is no need to join.
}

State Thread::state() const
{
    UvGuard const G(_lock);
    return _state;
}

bool Thread::operator ==(Thread const & obj) const TBAG_NOEXCEPT
{
    return equal(obj);
}

bool Thread::operator !=(Thread const & obj) const TBAG_NOEXCEPT
{
    return !equal(obj);
}

bool Thread::equal(uthread const & lh, uthread const & rh) TBAG_NOEXCEPT
{
    return ::uv_thread_equal(&lh, &rh);
}

bool Thread::equal(uthread const & t) const TBAG_NOEXCEPT
{
    UvGuard const G(_lock);
    return Thread::equal(_thread, t);
}

bool Thread::equal(Thread const & t) const TBAG_NOEXCEPT
{
    return equal(t._thread);
}

uthread Thread::getCurrentThreadId() TBAG_NOEXCEPT
{
    return ::uv_thread_self();
}

bool Thread::isCurrentThread() TBAG_NOEXCEPT
{
    return equal(getCurrentThreadId());
}

Err Thread::run()
{
    UvGuard const G(_lock);
    if (_state != State::S_READY) {
        return E_ILLSTATE;
    }
    auto const code = ::uv_thread_create(&_thread, &__global_uv_thread_cb__, this);
    if (code == 0) {
        _state = State::S_CREATED;
    }
    return convertUvErrorToErr(code);
}

Err Thread::run(std::size_t stack_size)
{
    UvGuard const G(_lock);
    if (_state != State::S_READY) {
        return E_ILLSTATE;
    }

    // If UV_THREAD_HAS_STACK_SIZE is set, stack_size specifies a stack size for the new thread.
    // 0 indicates that the default value should be used, i.e. behaves as if the flag was not set.
    // Other values will be rounded up to the nearest page boundary.
    uv_thread_options_t options;
    options.flags = UV_THREAD_HAS_STACK_SIZE;
    options.stack_size = stack_size;

    auto const code = ::uv_thread_create_ex(&_thread, &options, &__global_uv_thread_cb__, this);
    if (code == 0) {
        _state = State::S_CREATED;
    }
    return convertUvErrorToErr(code);
}

bool Thread::joinable() const
{
    UvGuard const G(_lock);
    return _state != State::S_READY;
}

Err Thread::join(bool rethrow)
{
    _lock.lock();
    auto const STATE = _state;
    _lock.unlock();

    if (STATE == State::S_READY) {
        return E_ILLSTATE;
    }

    auto const code = ::uv_thread_join(&_thread);
    if (rethrow && code == 0) {
        if (_exception) {
            std::rethrow_exception(_exception);
        }
    }
    return convertUvErrorToErr(code);
}

Err Thread::waitForRunningOrDone(unsigned long timeout_ms, unsigned long tick_ms)
{
    return waitForRunningOrDone(*this, std::chrono::milliseconds(timeout_ms), std::chrono::milliseconds(tick_ms));
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

