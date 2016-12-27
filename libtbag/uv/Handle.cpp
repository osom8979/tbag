/**
 * @file   Handle.cpp
 * @brief  Handle class implementation.
 * @author zer0
 * @date   2016-12-27
 */

#include <libtbag/uv/Handle.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Global close event.
static void __global_uv_handle_close__(uv_handle_t * native_handle)
{
    Handle * handle = static_cast<Handle*>(native_handle->data);
    if (handle != nullptr) {
        handle->onClose();
    }
}

Handle::Handle(UvHandleType type) : Native(static_cast<UvType>(type))
{
    if (isHandle() == false) {
        __tbag_error("Handle::Handle({}) type is not handle type", static_cast<int>(type));
        throw std::bad_alloc();
    }
    Parent::castNative<uv_handle_t>()->data = this; // IMPORTANT!!
}

Handle::~Handle()
{
    // EMPTY.
}

void Handle::close()
{
    ::uv_close(Parent::castNative<uv_handle_t>(), __global_uv_handle_close__);
}

bool Handle::isActive() const TBAG_NOEXCEPT
{
    /* What “active” means depends on the type of handle:
     *
     * - A uv_async_t handle is always active and cannot be deactivated,
     *   except by closing it with uv_close().
     * - A uv_pipe_t, uv_tcp_t, uv_udp_t, etc. handle (basically any handle that deals with i/o)
     *   is active when it is doing something that involves i/o,
     *   like reading, writing, connecting, accepting new connections, etc.
     * - A uv_check_t, uv_idle_t, uv_timer_t, etc. handle is active
     *   when it has been started with a call to uv_check_start(), uv_idle_start(), etc.
     *
     * Rule of thumb: if a handle of type uv_foo_t has a uv_foo_start() function,
     * then it’s active from the moment that function is called.
     * Likewise, uv_foo_stop() deactivates the handle again.
     */
    return ::uv_is_active(Parent::castNative<uv_handle_t>());
}

bool Handle::isClosing() const TBAG_NOEXCEPT
{
    int const ENABLE = ::uv_is_closing(this->castNative<uv_handle_t>());
    return (ENABLE == 0 ? false : true);
}

void Handle::ref() TBAG_NOEXCEPT
{
    ::uv_ref(Parent::castNative<uv_handle_t>());
}

void Handle::unref() TBAG_NOEXCEPT
{
    ::uv_unref(Parent::castNative<uv_handle_t>());
}

bool Handle::hasRef() const TBAG_NOEXCEPT
{
    // Returns non-zero if the handle referenced, zero otherwise.
    return ::uv_has_ref(Parent::castNative<uv_handle_t>()) != 0;
}

std::size_t Handle::getNativeSize() const TBAG_NOEXCEPT
{
    return ::uv_handle_size(Parent::castNative<uv_handle_t>()->type);
}

void Handle::onClose()
{
    __tbag_error("Handle::onClose({}) type is not handle type", getName());
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

