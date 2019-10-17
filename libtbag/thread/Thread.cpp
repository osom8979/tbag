/**
 * @file   Thread.cpp
 * @brief  Thread class implementation.
 * @author zer0
 * @date   2019-07-17
 */

#include <libtbag/thread/Thread.hpp>
#include <libtbag/thread/ThreadKill.hpp>

#include <cassert>
#include <utility>

#include <uv.h>

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

Thread::Thread() TBAG_NOEXCEPT
        : _thread(), _id(NO_ASSIGN_ID), _state(State::S_READY), _exception(),
          _once_joined(false)
{
    // EMPTY.
}

Thread::~Thread()
{
    join(false); // Note: Destructor is an implicit 'NOEXCEPT' state.
}

uthread Thread::tid() const
{
    UvGuard const G(_lock);
    return _thread;
}

std::size_t Thread::id() const
{
    UvGuard const G(_lock);
    return _id;
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
        _id = reinterpret_cast<std::size_t>(_thread);
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
        _id = reinterpret_cast<std::size_t>(_thread);
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
    auto const state = _state;
    auto const joined = _once_joined;
    _once_joined = true;
    _lock.unlock();

    if (state == State::S_READY) {
        return E_ILLSTATE;
    }
    if (joined) {
        return E_ALREADY;
    }

    // [WARNING]
    //  The thread resource is freed at this point.
    //  After joining in Windows, the corresponding handle value disappears.
    auto const code = ::uv_thread_join(&_thread);
    if (code == 0) {
        // After joining, it is accessible without thread contention.
        if (rethrow && _exception) {
            std::rethrow_exception(_exception);
        }
    }
    return convertUvErrorToErr(code);
}

Err Thread::waitForRunningOrDone(unsigned long timeout_ms, unsigned long tick_ms)
{
    return waitForRunningOrDone(*this, std::chrono::milliseconds(timeout_ms), std::chrono::milliseconds(tick_ms));
}

Err Thread::kill() const
{
    return killThread(_thread);
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

