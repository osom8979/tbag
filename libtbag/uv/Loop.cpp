/**
 * @file   Loop.cpp
 * @brief  Loop class implementation.
 * @author zer0
 * @date   2016-12-28
 */

#include <libtbag/uv/Loop.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Handle.hpp>

#include <cassert>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_walk_cb__(uv_handle_t * handle, void * arg)
{
    uv_loop_t * native_loop = static_cast<uv_loop_t*>(handle->loop);
    if (native_loop == nullptr) {
        __tbag_error("__global_uv_walk_cb__() handle loop is nullptr.");
        return;
    }

    Loop * loop = static_cast<Loop*>(native_loop->data);
    if (loop == nullptr) {
        __tbag_error("__global_uv_walk_cb__() loop data is nullptr.");
        return;
    }
    loop->onWalk(handle, arg);
}

static void __global_close_all_uv_walk_cb__(uv_handle_t * handle, void * arg)
{
    Handle * h = static_cast<Handle*>(handle->data);
    if (h != nullptr && h->isInit() && h->isClosing() == false) {
        h->close();
    }
}

// --------------------
// Loop implementation.
// --------------------

Loop::Loop() : Native(UvType::LOOP)
{
    // Initializes the given uv_loop_t structure.
    int const CODE = ::uv_loop_init(Parent::cast<uv_loop_t>());
    if (CODE != 0) {
        __tbag_error("Loop::Loop() error [{}] {}", CODE, getUvErrorName(CODE));
        throw std::bad_alloc();
    }

    // Space for user-defined arbitrary data. libuv does not use this field.
    // libuv does, however, initialize it to NULL in uv_loop_init(),
    // and it poisons the value (on debug builds) on uv_loop_close().
    uv_loop_t * loop = Parent::cast<uv_loop_t>();
    assert(loop != nullptr);
    loop->data = this; // IMPORTANT!!
}

Loop::~Loop()
{
    // Releases all internal loop resources.
    //
    // Call this function only when the loop has finished executing
    // and all open handles and requests have been closed, or it will return UV_EBUSY.
    // After this function returns, the user can free the memory allocated for the loop.
    int code = ::uv_loop_close(Parent::cast<uv_loop_t>());
    if (code == UV_EBUSY) {
        if (isAlive()) { stop(); }
        runCloseAllHandles();
        code = ::uv_loop_close(Parent::cast<uv_loop_t>()); // RE-TRY.
    }

    if (code != 0) {
        __tbag_error("Loop::~Loop() error [{}] {}", code, getUvErrorName(code));
    }
}

void Loop::runCloseAllHandles()
{
    ::uv_walk(Parent::cast<uv_loop_t>(), __global_close_all_uv_walk_cb__, nullptr);
    run(RunMode::RUN_DEFAULT);
}

bool Loop::run(RunMode mode)
{
    uv_run_mode uv_mode = UV_RUN_DEFAULT;
    // @formatter:off
    switch (mode) {
    case RunMode::RUN_DEFAULT: uv_mode = UV_RUN_DEFAULT; break;
    case RunMode::RUN_ONCE:    uv_mode = UV_RUN_ONCE;    break;
    case RunMode::RUN_NOWAIT:  uv_mode = UV_RUN_NOWAIT;  break;
    }
    // @formatter:on

    // It will act differently depending on the specified mode:
    // - UV_RUN_DEFAULT: Runs the event loop until there are no more active and referenced handles or requests. Returns non-zero if uv_stop() was called and there are still active handles or requests. Returns zero in all other cases.
    // - UV_RUN_ONCE: Poll for i/o once. Note that this function blocks if there are no pending callbacks. Returns zero when done (no active handles or requests left), or non-zero if more callbacks are expected (meaning you should run the event loop again sometime in the future).
    // - UV_RUN_NOWAIT: Poll for i/o once but don’t block if there are no pending callbacks. Returns zero if done (no active handles or requests left), or non-zero if more callbacks are expected (meaning you should run the event loop again sometime in the future).

    int const CODE = ::uv_run(Parent::cast<uv_loop_t>(), uv_mode);
    if (CODE != 0) {
        __tbag_error("Loop::run() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Loop::isAlive() const
{
    // Non-zero is alive.
    return ::uv_loop_alive(Parent::cast<uv_loop_t>()) != 0;
}

void Loop::stop()
{
    // Stop the event loop, causing uv_run() to end as soon as possible.
    // This will happen not sooner than the next loop iteration.
    // If this function was called before blocking for i/o, the loop won’t block for i/o on this iteration.
    ::uv_stop(Parent::cast<uv_loop_t>());
}

int Loop::getPollTimeoutMilliseconds() const
{
    return ::uv_backend_timeout(Parent::cast<uv_loop_t>());
}

uint64_t Loop::getNowMilliseconds() const
{
    // Return the current timestamp in milliseconds.
    // The timestamp is cached at the start of the event loop tick,
    // see uv_update_time() for details and rationale.
    //
    // The timestamp increases monotonically from some arbitrary point in time.
    // Don’t make assumptions about the starting point, you will only get disappointed.
    return ::uv_now(Parent::cast<uv_loop_t>());
}

uint64_t Loop::getNowNanoseconds() const
{
    // It is relative to an arbitrary time in the past.
    // It is not related to the time of day and therefore
    // not subject to clock drift.
    // The primary use is for measuring performance between intervals.
    //
    // Note: Not every platform can support nanosecond resolution;
    // however, this value will always be in nanoseconds.
    return ::uv_hrtime();
}

void Loop::updateTime()
{
    // Update the event loop’s concept of "now".
    // Libuv caches the current time at the start of the event loop tick
    // in order to reduce the number of time-related system calls.
    //
    // You won’t normally need to call this function unless you have
    // callbacks that block the event loop for longer periods of
    // time, where "longer" is somewhat subjective but probably
    // on the order of a millisecond or more.
    ::uv_update_time(Parent::cast<uv_loop_t>());
}

void Loop::walk(void * arg)
{
    ::uv_walk(Parent::cast<uv_loop_t>(), __global_uv_walk_cb__, arg);
}

// --------------
// Event methods.
// --------------

void Loop::onWalk(void * native_handle, void * arg)
{
    if (native_handle == nullptr) {
        __tbag_error("Loop::onWalk() handle is nullptr.");
        return;
    }

    Handle * handle = static_cast<Handle*>(static_cast<uv_handle_t*>(native_handle)->data);
    if (handle != nullptr) {
        handle->onWalk(arg);
    }
}

// ----------
// Debugging.
// ----------

void Loop::printAllHandles(FILE * stream)
{
    uv_print_all_handles(Parent::cast<uv_loop_t>(), stream);
}

void Loop::printActiveHandles(FILE * stream)
{
    uv_print_active_handles(Parent::cast<uv_loop_t>(), stream);
}

std::size_t Loop::getNativeSize() TBAG_NOEXCEPT
{
    return ::uv_loop_size();
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

