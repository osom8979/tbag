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

void __global_uv_thread_cb__(void * args) TBAG_NOEXCEPT
{
    assert(args != nullptr);
    auto * thread = (Thread*)args;
    thread->_state = Thread::State::S_RUNNING;
    try {
        thread->onRunner();
    } catch (...) {
        thread->_exception = std::current_exception();
    }
    thread->_state = Thread::State::S_DONE;
}

Thread::Thread() TBAG_NOEXCEPT : _thread(), _state(State::S_READY), _exception()
{
    // EMPTY.
}

Thread::~Thread()
{
    assert(_state == Thread::State::S_DONE);
}

bool Thread::operator ==(Thread const & obj) const TBAG_NOEXCEPT
{
    return equal(obj);
}

bool Thread::equal(uthread const & t) const TBAG_NOEXCEPT
{
    return ::uv_thread_equal(&_thread, &t) != 0;
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
    switch (_state.load()) {
    case State::S_READY:
        // OK!
        break;
    case State::S_DONE:
        return E_EXPIRED;
    case State::S_RUNNING:
        return E_ILLSTATE;
    default:
        return E_UNKNOWN;
    }
    return convertUvErrorToErr(::uv_thread_create(&_thread, &__global_uv_thread_cb__, this));
}

Err Thread::run(std::size_t stack_size)
{
    switch (_state.load()) {
    case State::S_READY:
        // OK!
        break;
    case State::S_DONE:
        return E_EXPIRED;
    case State::S_RUNNING:
        return E_ILLSTATE;
    default:
        return E_UNKNOWN;
    }

    // If UV_THREAD_HAS_STACK_SIZE is set, stack_size specifies a stack size for the new thread.
    // 0 indicates that the default value should be used, i.e. behaves as if the flag was not set.
    // Other values will be rounded up to the nearest page boundary.
    uv_thread_options_t options;
    options.flags = UV_THREAD_HAS_STACK_SIZE;
    options.stack_size = stack_size;
    return convertUvErrorToErr(::uv_thread_create_ex(&_thread, &options, &__global_uv_thread_cb__, this));
}

void Thread::rethrowIfExists() const
{
    if (_exception) {
        std::rethrow_exception(_exception);
    }
}

Err Thread::join(bool rethrow)
{
    auto const code = convertUvErrorToErr(::uv_thread_join(&_thread));
    if (rethrow) {
        rethrowIfExists();
    }
    return code;
}

Err Thread::waitForRunningOrDone(unsigned long timeout_ms, unsigned long tick_ms)
{
    return waitForRunningOrDone(*this, std::chrono::milliseconds(timeout_ms), std::chrono::milliseconds(tick_ms));
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

