/**
 * @file   Handle.cpp
 * @brief  Handle class implementation.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Handle.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <uv.h>

//#define DISABLE_SAFETY_ERASE_HANDLE

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_close_cb__(uv_handle_t * handle)
{
    Handle * h = static_cast<Handle*>(handle->data);
    if (h == nullptr) {
        tDLogE("__global_uv_close_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        tDLogE("__global_uv_close_cb__() handle.data is deleted.");
    } else {
        Loop * loop = nullptr;
        if (handle->loop == nullptr) {
            tDLogE("__global_uv_close_cb__() handle.loop is nullptr.");
        } else if (isDeletedAddress(handle->loop)) {
            tDLogE("__global_uv_close_cb__() handle.loop is deleted.");
        } else {
            loop = static_cast<Loop*>(handle->loop->data);
        }

        // @formatter:off
        if (loop != nullptr) { loop->onClosing(h); }
        h->onClose();
        if (loop != nullptr) { loop->onClosed(h); loop->eraseChildHandle(*h); }
        // @formatter:on

        UNUSED_PARAM(h); // Do not call this handle any more.
    }
}

// ----------------------
// Handle implementation.
// ----------------------

Handle::Handle(uhandle type) : Native(static_cast<utype>(type))
{
    if (isHandle() == false) {
        tDLogE("Handle::Handle({}) type is not handle type", static_cast<int>(type));
        throw std::bad_alloc();
    }

    uv_handle_t * native = Parent::cast<uv_handle_t>();
    assert(native != nullptr);
    native->data = this; // IMPORTANT!!
}

Handle::~Handle()
{
    if (isInit() && isClosing() == false) {
        tryClose();
    }

#if !defined(DISABLE_SAFETY_ERASE_HANDLE)
    eraseFromLoop();
#endif
}

void Handle::eraseFromLoop()
{
    Loop * loop = getLoop();
    if (loop != nullptr && isDeletedAddress(loop) == false) {
        auto weak = loop->findChildHandle(*this);
        if (weak.expired() == false) {
            loop->eraseChildHandle(*this);
        }
    }
}

bool Handle::isInit() const TBAG_NOEXCEPT
{
    return Parent::cast<uv_handle_t>()->type != UV_UNKNOWN_HANDLE/*0*/;
}

Loop * Handle::getLoop()
{
    uv_loop_t * loop = Parent::cast<uv_handle_t>()->loop;
    if (loop == nullptr || isDeletedAddress(loop)) {
        return nullptr;
    }
    return static_cast<Loop*>(loop->data);
}

Loop const * Handle::getLoop() const
{
    uv_loop_t * loop = Parent::cast<uv_handle_t>()->loop;
    if (loop == nullptr || isDeletedAddress(loop)) {
        return nullptr;
    }
    return static_cast<Loop*>(loop->data);
}

void Handle::close()
{
    // Request handle to be closed. close_cb will be called asynchronously after this call.
    // This MUST be called on each handle before memory is released.
    //
    // Handles that wrap file descriptors are closed immediately but close_cb will still be
    // deferred to the next iteration of the event loop.
    // It gives you a chance to free up any resources associated with the handle.
    //
    // In-progress requests, like uv_connect_t or uv_write_t, are cancelled and have their
    // callbacks called asynchronously with status=UV_ECANCELED.
    ::uv_close(Parent::cast<uv_handle_t>(), __global_uv_close_cb__);
}

void Handle::tryClose()
{
    ::uv_close(Parent::cast<uv_handle_t>(), nullptr);
}

bool Handle::isActive() const TBAG_NOEXCEPT
{
    // What "active" means depends on the type of handle:
    //
    // - A uv_async_t handle is always active and cannot be deactivated,
    //   except by closing it with uv_close().
    // - A uv_pipe_t, uv_tcp_t, uv_udp_t, etc. handle (basically any handle that deals with i/o)
    //   is active when it is doing something that involves i/o,
    //   like reading, writing, connecting, accepting new connections, etc.
    // - A uv_check_t, uv_idle_t, uv_timer_t, etc. handle is active
    //   when it has been started with a call to uv_check_start(), uv_idle_start(), etc.
    //
    // Rule of thumb: if a handle of type uv_foo_t has a uv_foo_start() function,
    // then it’s active from the moment that function is called.
    // Likewise, uv_foo_stop() deactivates the handle again.
    return ::uv_is_active(Parent::cast<uv_handle_t>()) != 0;
}

bool Handle::isClosing() const TBAG_NOEXCEPT
{
    return ::uv_is_closing(this->cast<uv_handle_t>()) != 0;
}

void Handle::ref() TBAG_NOEXCEPT
{
    ::uv_ref(Parent::cast<uv_handle_t>());
}

void Handle::unref() TBAG_NOEXCEPT
{
    ::uv_unref(Parent::cast<uv_handle_t>());
}

bool Handle::hasRef() const TBAG_NOEXCEPT
{
    // Returns non-zero if the handle referenced, zero otherwise.
    return ::uv_has_ref(Parent::cast<uv_handle_t>()) != 0;
}

std::size_t Handle::getNativeSize() const TBAG_NOEXCEPT
{
    return ::uv_handle_size(Parent::cast<uv_handle_t>()->type);
}

// ----------------------------
// Miscellaneous API functions.
// ----------------------------

int Handle::getSendBufferSize() const
{
    int OBTAIN_VALUE_OF_FLAG = 0;
    return ::uv_send_buffer_size(Parent::cast<uv_handle_t>(), &OBTAIN_VALUE_OF_FLAG);
}

int Handle::getRecvBufferSize() const
{
    int OBTAIN_VALUE_OF_FLAG = 0;
    return ::uv_recv_buffer_size(Parent::cast<uv_handle_t>(), &OBTAIN_VALUE_OF_FLAG);
}

void Handle::setSendBufferSize(int size)
{
    if (size <= 0) {
        tDLogE("Handle::setSendBufferSize({}) size must be greater than zero.", size);
        return;
    }
    ::uv_send_buffer_size(Parent::cast<uv_handle_t>(), &size);
}

void Handle::setRecvBufferSize(int size)
{
    if (size <= 0) {
        tDLogE("Handle::setRecvBufferSize({}) size must be greater than zero.", size);
        return;
    }
    ::uv_recv_buffer_size(Parent::cast<uv_handle_t>(), &size);
}

// --------------
// Event methods.
// --------------

void Handle::onClose()
{
    tDLogD("Handle::onClose() @{} called.", static_cast<void*>(this));
}

void Handle::onWalk(void * arg)
{
    tDLogD("Handle::onWalk() @{} called.", static_cast<void*>(this));
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

