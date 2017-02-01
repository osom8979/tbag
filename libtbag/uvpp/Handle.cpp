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
#include <uv.h>

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
        __tbag_error("__global_uv_close_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        __tbag_error("__global_uv_close_cb__() handle.data is deleted.");
    } else {
        Loop * loop = nullptr;
        if (handle->loop == nullptr) {
            __tbag_error("__global_uv_close_cb__() handle.loop is nullptr.");
        } else if (isDeletedAddress(handle->loop)) {
            __tbag_error("__global_uv_close_cb__() handle.loop is deleted.");
        } else {
            loop = static_cast<Loop*>(handle->loop->data);
        }

        // @formatter:off
        if (loop != nullptr) { loop->onClosing(h); }
        h->onClose();
        if (loop != nullptr) { loop->onClosed(h); loop->eraseChildHandle(h); }
        // @formatter:on

        UNUSED_PARAM(h); // Do not call this handle any more.
    }
}

// ----------------------
// Handle implementation.
// ----------------------

Handle::Handle(UvHandleType type) : Native(static_cast<UvType>(type))
{
    if (isHandle() == false) {
        __tbag_error("Handle::Handle({}) type is not handle type", static_cast<int>(type));
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
}

bool Handle::isInit() const TBAG_NOEXCEPT
{
    return Parent::cast<uv_handle_t>()->type != UV_UNKNOWN_HANDLE/*0*/;
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
    return ::uv_is_active(Parent::cast<uv_handle_t>());
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
        __tbag_error("Handle::setSendBufferSize({}) size must be greater than zero.", size);
        return;
    }
    ::uv_send_buffer_size(Parent::cast<uv_handle_t>(), &size);
}

void Handle::setRecvBufferSize(int size)
{
    if (size <= 0) {
        __tbag_error("Handle::setRecvBufferSize({}) size must be greater than zero.", size);
        return;
    }
    ::uv_recv_buffer_size(Parent::cast<uv_handle_t>(), &size);
}

// --------------
// Event methods.
// --------------

void Handle::onClose()
{
    __tbag_debug("Handle::onClose() called.");
}

void Handle::onWalk(void * arg)
{
    __tbag_debug("Handle::onWalk() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

