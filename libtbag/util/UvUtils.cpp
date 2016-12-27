/**
 * @file   UvUtils.cpp
 * @brief  UvUtils class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/util/UvUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <cstring>
#include <exception>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

static void initUv()
{
    uv_loop_t loop = {0,};
    ::uv_loop_init(&loop);
    ::uv_loop_close(&loop);
}

#ifndef UV_NATIVE_HANDLE_MAP
#define UV_NATIVE_HANDLE_MAP(_TBAG_XX) \
    _TBAG_XX(UV_UNKNOWN_HANDLE) \
    _TBAG_XX(UV_NAMED_PIPE)     \
    _TBAG_XX(UV_TTY)            \
    _TBAG_XX(UV_TCP)            \
    _TBAG_XX(UV_UDP)            \
    _TBAG_XX(UV_PREPARE)        \
    _TBAG_XX(UV_CHECK)          \
    _TBAG_XX(UV_IDLE)           \
    _TBAG_XX(UV_ASYNC)          \
    _TBAG_XX(UV_TIMER)          \
    _TBAG_XX(UV_PROCESS)        \
    _TBAG_XX(UV_FS_EVENT)       \
    _TBAG_XX(UV_POLL)           \
    _TBAG_XX(UV_FS_POLL)        \
    _TBAG_XX(UV_SIGNAL)         \
    _TBAG_XX(UV_FILE)
#endif

char const * getUvNativeHandleName(void const * handle)
{
    switch (static_cast<uv_handle_t const *>(handle)->type) {
#define _TBAG_XX(name) case name: return #name;
    UV_NATIVE_HANDLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "UNKNOWN_HANDLE";
    }
}

bool isUvNativeHandleType(void const * handle)
{
    switch (static_cast<uv_handle_t const *>(handle)->type) {
#define _TBAG_XX(name) case name: return true;
    UV_NATIVE_HANDLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return false;
    }
}

// @formatter:off
std::string getUvErrorString(int uv_error_code) { return std::string(::uv_strerror(uv_error_code)); }
std::string getUvErrorName  (int uv_error_code) { return std::string(::uv_err_name(uv_error_code)); }
// @formatter:on

// @formatter:off
#define _TBAG_XX(name, type) case UvType::name: return true;
#define _TBAG_NX(name, type)
bool isHandle (UvType type) { switch (type) { TBAG_UTIL_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) default: return false; } }
bool isRequest(UvType type) { switch (type) { TBAG_UTIL_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) default: return false; } }
bool isEtc    (UvType type) { switch (type) { TBAG_UTIL_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) default: return false; } }
#undef _TBAG_XX
#undef _TBAG_NX
// @formatter:on

// ------------------------
// UvNative implementation.
// ------------------------

UvNative::UvNative(Type t) : TYPE(t), _native(nullptr)
{
    switch (TYPE) {
#define _TBAG_XX(name, type)                    \
    case UvType::name:                          \
        _native = ::malloc(sizeof(type));       \
        ::memset(_native, 0x00, sizeof(type));  \
        break;
    TBAG_UTIL_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    default:
        __tbag_error("UvNative::UvNative({}) Unknown uv type.", static_cast<int>(TYPE));
        _native = nullptr;
        break;
    }
}

UvNative::~UvNative()
{
    if (_native != nullptr) {
        ::free(_native);
        _native = nullptr;
    }
}

bool UvNative::isInit() const
{
    return castNative<uv_handle_t>()->type != UV_UNKNOWN_HANDLE /* 0 */ ? true : false;
}

// ------------------------
// UvHandle implementation.
// ------------------------

// Global close event.
static void __global_uv_handle_close__(uv_handle_t * native_handle)
{
    UvHandle * handle = static_cast<UvHandle*>(native_handle->data);
    if (handle != nullptr) {
        handle->onClose();
    }
}

UvHandle::UvHandle(UvHandleType type) : UvNative(static_cast<UvType>(type))
{
    if (isHandle() == false) {
        __tbag_error("UvHandle::UvHandle({}) type is not handle type", static_cast<int>(type));
        throw std::bad_alloc();
    }
    Parent::castNative<uv_handle_t>()->data = this; // IMPORTANT!!
}

UvHandle::~UvHandle()
{
    // EMPTY.
}

bool UvHandle::isClosing() const TBAG_NOEXCEPT
{
    int const ENABLE = ::uv_is_closing(this->castNative<uv_handle_t>());
    return (ENABLE == 0 ? false : true);
}

void UvHandle::close()
{
    ::uv_close(Parent::castNative<uv_handle_t>(), __global_uv_handle_close__);
}

bool UvHandle::isActive()
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

void UvHandle::ref()
{
    ::uv_ref(Parent::castNative<uv_handle_t>());
}

void UvHandle::unref()
{
    ::uv_unref(Parent::castNative<uv_handle_t>());
}

bool UvHandle::hasRef()
{
    // Returns non-zero if the handle referenced, zero otherwise.
    return ::uv_has_ref(Parent::castNative<uv_handle_t>()) != 0;
}

std::size_t UvHandle::getNativeSize()
{
    return ::uv_handle_size(Parent::castNative<uv_handle_t>()->type);
}

void UvHandle::onClose()
{
    __tbag_error("UvHandle::onClose({}) type is not handle type", getName());
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

