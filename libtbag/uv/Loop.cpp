/**
 * @file   Loop.cpp
 * @brief  BaseLoop & Loop class implementation.
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
    if (handle->loop == nullptr) {
        __tbag_error("__global_uv_walk_cb__() handle.loop is nullptr.");
        return;
    }

    BaseLoop * loop = static_cast<BaseLoop*>(handle->loop->data);
    if (loop == nullptr) {
        __tbag_error("__global_uv_walk_cb__() handle.loop.data is nullptr.");
        return;
    }

    loop->onWalk(handle, arg);
}

// ------------------------
// BaseLoop implementation.
// ------------------------

BaseLoop::BaseLoop() : Native(UvType::LOOP), _running(false)
{
    // Initializes the given uv_loop_t structure.
    int const CODE = ::uv_loop_init(Parent::cast<uv_loop_t>());
    if (CODE != 0) {
        __tbag_debug("BaseLoop::BaseLoop() error [{}] {}", CODE, getUvErrorName(CODE));
        throw std::bad_alloc();
    }

    // Space for user-defined arbitrary data. libuv does not use this field.
    // libuv does, however, initialize it to NULL in uv_loop_init(),
    // and it poisons the value (on debug builds) on uv_loop_close().
    uv_loop_t * loop = Parent::cast<uv_loop_t>();
    assert(loop != nullptr);
    loop->data = this; // IMPORTANT!!
}

BaseLoop::~BaseLoop()
{
    // EMPTY.
}

bool BaseLoop::close(Err * result)
{
    // Releases all internal loop resources.
    //
    // Call this function only when the loop has finished executing
    // and all open handles and requests have been closed, or it will return UV_EBUSY.
    // After this function returns, the user can free the memory allocated for the loop.
    int const CODE = ::uv_loop_close(Parent::cast<uv_loop_t>());
    if (CODE != 0) {
        if (result != nullptr) {
            if (CODE == UV_EBUSY) {
                *result = Err::BUSY_ERROR;
            } else {
                *result = Err::FAILURE;
            }
        } else {
            __tbag_debug("BaseLoop:close() error [{}] {}", CODE, getUvErrorName(CODE));
        }
        return false;
    }

    if (result != nullptr) {
        *result = Err::SUCCESS;
    }
    return true;
}

bool BaseLoop::run(RunMode mode)
{
    if (_running.load() == true) {
        __tbag_error("BaseLoop::run() duplicated calls.");
        return false;
    }

    uv_run_mode uv_mode = UV_RUN_DEFAULT;
    // @formatter:off
    switch (mode) {
    case RunMode::RUN_DEFAULT: uv_mode = UV_RUN_DEFAULT; break;
    case RunMode::RUN_ONCE:    uv_mode = UV_RUN_ONCE;    break;
    case RunMode::RUN_NOWAIT:  uv_mode = UV_RUN_NOWAIT;  break;
    }
    // @formatter:on

    // Update owner thread id.
    _owner_thread_id = std::this_thread::get_id();

    // It will act differently depending on the specified mode:
    // - UV_RUN_DEFAULT: Runs the event loop until there are no more active and referenced handles or requests. Returns non-zero if uv_stop() was called and there are still active handles or requests. Returns zero in all other cases.
    // - UV_RUN_ONCE: Poll for i/o once. Note that this function blocks if there are no pending callbacks. Returns zero when done (no active handles or requests left), or non-zero if more callbacks are expected (meaning you should run the event loop again sometime in the future).
    // - UV_RUN_NOWAIT: Poll for i/o once but don’t block if there are no pending callbacks. Returns zero if done (no active handles or requests left), or non-zero if more callbacks are expected (meaning you should run the event loop again sometime in the future).
    _running.store(true);
    int const CODE = ::uv_run(Parent::cast<uv_loop_t>(), uv_mode);
    _running.store(false);

    if (CODE != 0) {
        __tbag_error("BaseLoop::run() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool BaseLoop::isAlive() const
{
    // Non-zero is alive.
    return ::uv_loop_alive(Parent::cast<uv_loop_t>()) != 0;
}

void BaseLoop::stop()
{
    // Stop the event loop, causing uv_run() to end as soon as possible.
    // This will happen not sooner than the next loop iteration.
    // If this function was called before blocking for i/o, the loop won’t block for i/o on this iteration.
    ::uv_stop(Parent::cast<uv_loop_t>());
}

int BaseLoop::getPollTimeoutMilliseconds() const
{
    return ::uv_backend_timeout(Parent::cast<uv_loop_t>());
}

uint64_t BaseLoop::getNowMilliseconds() const
{
    // Return the current timestamp in milliseconds.
    // The timestamp is cached at the start of the event loop tick,
    // see uv_update_time() for details and rationale.
    //
    // The timestamp increases monotonically from some arbitrary point in time.
    // Don’t make assumptions about the starting point, you will only get disappointed.
    return ::uv_now(Parent::cast<uv_loop_t>());
}

uint64_t BaseLoop::getNowNanoseconds() const
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

void BaseLoop::updateTime()
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

void BaseLoop::walk(void * arg)
{
    ::uv_walk(Parent::cast<uv_loop_t>(), __global_uv_walk_cb__, arg);
}

// --------------
// Event methods.
// --------------

void BaseLoop::onWalk(void * native_handle, void * arg)
{
    if (native_handle == nullptr) {
        __tbag_error("BaseLoop::onWalk() handle is nullptr.");
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

void BaseLoop::printAllHandles(FILE * file)
{
    // The format is [flags] handle-type handle-address. For flags:
    //
    // - R is printed for a handle that is referenced
    // - A is printed for a handle that is active
    // - I is printed for a handle that is internal
    ::uv_print_all_handles(Parent::cast<uv_loop_t>(), file);
}

void BaseLoop::printActiveHandles(FILE * file)
{
    ::uv_print_active_handles(Parent::cast<uv_loop_t>(), file);
}

// ---------------
// Static methods.
// ---------------

std::size_t BaseLoop::getNativeSize() TBAG_NOEXCEPT
{
    return ::uv_loop_size();
}

// --------------------
// Loop implementation.
// --------------------

Loop::Loop()
{
    // EMPTY.
}

Loop::~Loop()
{
    Err code;
    if (Parent::close(&code) == false) {
        runCloseAllHandles();

        if (Parent::isAlive()) {
            stop();
        }

        // RE-TRY.
        if (Parent::close(&code) == false) {
            __tbag_error("Loop::~Loop() error [{}] {}", static_cast<ErrType>(code), debug::getErrorMessage(code));
        }
    }
}

std::size_t Loop::closeAllHandles()
{
    std::size_t close_count = 0;

    for (auto & handle : _handles) {
        if (static_cast<bool>(handle) && handle->isInit() && handle->isClosing() == false) {
            handle->close();
            ++close_count;
        }
    }

    return close_count;
}

void Loop::runCloseAllHandles()
{
    if (closeAllHandles() > 0 && isRunning() == false) {
        run(RunMode::RUN_DEFAULT);
    }
}

Loop::WeakHandle Loop::findChildHandle(Handle * handle)
{
    for (auto & cursor : _handles) {
        if (cursor.get() == handle) {
            return WeakHandle(cursor);
        }
    }
    return WeakHandle();
}

Loop::WeakHandle Loop::insertChildHandle(SharedHandle handle)
{
    auto itr = _handles.insert(handle);
    if (itr.second) {
        return WeakHandle(*(itr.first));
    }
    return WeakHandle();
}

Loop::WeakHandle Loop::insertChildHandle(Handle * handle)
{
    return insertChildHandle(SharedHandle(handle));
}

bool Loop::eraseChildHandle(WeakHandle handle)
{
    return _handles.erase(handle.lock()) == 1;
}

bool Loop::eraseChildHandle(Handle * handle)
{
    auto weak = findChildHandle(handle);
    if (auto shared = weak.lock()) {
        return eraseChildHandle(weak);
    }
    return false;
}

void Loop::forceClear()
{
    runCloseAllHandles();
    _handles.clear();
}

void Loop::onClosing(Handle * handle)
{
    // EMPTY.
}

void Loop::onClosed(Handle * handle)
{
    if (eraseChildHandle(handle) == false) {
        __tbag_error("Loop::onClosed() erase child handle error.");
    }
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

