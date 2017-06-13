/**
 * @file   Loop.cpp
 * @brief  BaseLoop & Loop class implementation.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Handle.hpp>

#include <cassert>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// ----------------
// libuv utilities.
// ----------------

inline static uv_run_mode convertRunModeToUvRunMode(BaseLoop::RunMode mode) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (mode) {
    case BaseLoop::RunMode::RUN_DEFAULT: return UV_RUN_DEFAULT;
    case BaseLoop::RunMode::RUN_ONCE:    return UV_RUN_ONCE;
    case BaseLoop::RunMode::RUN_NOWAIT:  return UV_RUN_NOWAIT;
    }
    assert(false && "Inaccessible block.");
    return UV_RUN_DEFAULT;
    // @formatter:on
}

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_walk_cb__(uv_handle_t * handle, void * arg)
{
    if (handle->loop == nullptr) {
        tDLogE("__global_uv_walk_cb__() handle.loop is nullptr.");
    } else if (isDeletedAddress(handle->loop)) {
        tDLogE("__global_uv_walk_cb__() handle.loop is deleted.");
    } else {
        BaseLoop * loop = static_cast<BaseLoop*>(handle->loop->data);
        if (loop == nullptr) {
            tDLogE("__global_uv_walk_cb__() handle.loop.data is nullptr.");
        } else if (isDeletedAddress(loop)) {
            tDLogE("__global_uv_walk_cb__() handle.loop.data is deleted.");
        } else {
            loop->onWalk(handle, arg);
        }
    }
}

// ------------------------
// BaseLoop implementation.
// ------------------------

BaseLoop::BaseLoop() : Native(utype::LOOP), _running(false)
{
    // Initializes the given uv_loop_t structure.
    int const CODE = ::uv_loop_init(Parent::cast<uv_loop_t>());
    if (CODE != 0) {
        tDLogD("BaseLoop::BaseLoop() error [{}] {}", CODE, getUvErrorName(CODE));
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

Err BaseLoop::close()
{
    // Releases all internal loop resources.
    //
    // Call this function only when the loop has finished executing
    // and all open handles and requests have been closed, or it will return UV_EBUSY.
    // After this function returns, the user can free the memory allocated for the loop.
    int const CODE = ::uv_loop_close(Parent::cast<uv_loop_t>());
    return convertUvErrorToErrWithLogging("BaseLoop::close()", CODE);
}

Err BaseLoop::run(RunMode mode)
{
    bool NOW_WAITING = false;
    if (_running.compare_exchange_weak(NOW_WAITING, true) == false) {
        tDLogE("BaseLoop::run() already working");
        return Err::E_EALREADY;
    }

    // Update owner thread id.
    _owner_thread_id = std::this_thread::get_id();

    // It will act differently depending on the specified mode:
    // - UV_RUN_DEFAULT: Runs the event loop until there are no more active and referenced handles or requests.
    //                   Returns non-zero if uv_stop() was called and there are still active handles or requests.
    //                   Returns zero in all other cases.
    // - UV_RUN_ONCE: Poll for i/o once. Note that this function blocks if there are no pending callbacks.
    //                Returns zero when done (no active handles or requests left), or non-zero if more callbacks
    //                are expected (meaning you should run the event loop again sometime in the future).
    // - UV_RUN_NOWAIT: Poll for i/o once but don’t block if there are no pending callbacks. Returns zero if done
    //                  (no active handles or requests left), or non-zero if more callbacks are expected
    //                  (meaning you should run the event loop again sometime in the future).
    int const CODE = ::uv_run(Parent::cast<uv_loop_t>(), convertRunModeToUvRunMode(mode));

    _running.store(false);
    return convertUvErrorToErrWithLogging("BaseLoop::run()", CODE);
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
        tDLogE("BaseLoop::onWalk() native_handle is nullptr.");
    } else if (isDeletedAddress(native_handle)) {
        tDLogE("BaseLoop::onWalk() native_handle is deleted.");
    } else {
        Handle * handle = static_cast<Handle*>(static_cast<uv_handle_t*>(native_handle)->data);
        if (handle == nullptr) {
            tDLogE("BaseLoop::onWalk() handle is nullptr.");
        } else if (isDeletedAddress(handle)) {
            tDLogE("BaseLoop::onWalk() handle is deleted.");
        } else {
            handle->onWalk(arg);
        }
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
    if (Parent::close() != Err::E_SUCCESS) {
        runCloseAllHandles();

        if (Parent::isAlive()) {
            stop();
        }

        // RE-TRY.
        if (Parent::close() != Err::E_SUCCESS) {
            tDLogE("Loop::~Loop() error.");
        }
    }
}

std::size_t Loop::closeAllHandles()
{
    std::size_t close_count = 0;

    for (auto & cursor : _handles) {
        if (/**/static_cast<bool>(cursor.second) &&
                cursor.second->isInit()          &&
                cursor.second->isClosing() == false) {
            cursor.second->close();
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

Loop::WeakHandle Loop::findChildHandle(void * native_handle)
{
    if (_handles.empty()) {
        return WeakHandle();
    }

    auto itr = _handles.find(NativeHandle(native_handle));
    if (itr == _handles.end()) {
        return WeakHandle();
    }
    return WeakHandle(itr->second);
}

Loop::WeakHandle Loop::findChildHandle(Handle & h)
{
    return findChildHandle(h.get());
}

bool Loop::eraseChildHandle(void * native_handle)
{
    WeakHandle weak = findChildHandle(native_handle);
    SharedHandle shared = weak.lock();
    if (static_cast<bool>(shared) == false) {
        return false; // Not found handle.
    }

    void const * HANDLE_ADDRESS   = shared.get();
    std::string const HANDLE_NAME = shared->getName();

    if (_handles.erase(NativeHandle(native_handle)) != 1U) {
        tDLogE("Loop::eraseChildHandle(@{}[{}]) Failure (Handles: {})",
               HANDLE_ADDRESS, HANDLE_NAME, _handles.size());
        return false;
    }

    tDLogD("Loop::eraseChildHandle(@{}[{}]) Success (Handles: {})",
           HANDLE_ADDRESS, HANDLE_NAME, _handles.size());
    return true;
}

bool Loop::eraseChildHandle(Handle & h)
{
    return eraseChildHandle(h.get());
}

Loop::WeakHandle Loop::insertChildHandle(SharedHandle h)
{
    auto itr = _handles.insert(HandleMap::value_type(NativeHandle(h->get()), h));
    if (itr.second) {
        tDLogD("Loop::insertChildHandle(@{}[{}]) Success (Handles: {})",
               static_cast<void*>(h.get()), h->getName(), _handles.size());
        return WeakHandle(itr.first->second);
    }

    tDLogE("Loop::insertChildHandle(@{}[{}]) Failure (Handles: {})",
           static_cast<void*>(h.get()), h->getName(), _handles.size());
    return WeakHandle();
}

void Loop::forceClear()
{
    runCloseAllHandles();
    _handles.clear();
}

// --------------
// Event methods.
// --------------

void Loop::onClosing(Handle * handle)
{
    // EMPTY.
}

void Loop::onClosed(Handle * handle)
{
    // EMPTY.
}

// ----------
// Utilities.
// ----------

void busyWaitForAlive(Loop & loop)
{
    while (loop.isRunning() == false || loop.isAlive() == false) {
        // BUSY WAIT!
    }
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

