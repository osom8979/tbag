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

char const * getUvHandleName(void const * handle)
{
    switch (static_cast<uv_handle_t const *>(handle)->type) {
#define _TBAG_XX(name) case name: return #name;
    UV_NATIVE_HANDLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "UNKNOWN_HANDLE";
    }
}

bool isUvHandleType(void const * handle)
{
    switch (static_cast<uv_handle_t const *>(handle)->type) {
#define _TBAG_XX(name) case name: return true;
    UV_NATIVE_HANDLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return false;
    }
}

std::string getUvErrorString(int uv_error_code)
{
    return std::string(::uv_strerror(uv_error_code));
}

std::string getUvErrorName(int uv_error_code)
{
    return std::string(::uv_err_name(uv_error_code));
}

bool isHandle(UvType type)
{
    switch (type) {
#define _TBAG_XX(name, type) case UvType::name: return true;
#define _TBAG_NX(name, type)
        TBAG_UTIL_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX)
#undef _TBAG_XX
#undef _TBAG_NX
        default: return false;
    }
}

bool isRequest(UvType type)
{
    switch (type) {
#define _TBAG_XX(name, type) case UvType::name: return true;
#define _TBAG_NX(name, type)
        TBAG_UTIL_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX)
#undef _TBAG_XX
#undef _TBAG_NX
        default: return false;
    }
}

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

UvHandle::UvHandle(UvHandleType type) : UvNative(static_cast<UvType>(type)), _on_close_cb(nullptr)
{
    if (isHandle() == false) {
        __tbag_error("UvHandle::UvHandle({}) type is not handle type", static_cast<int>(type));
        throw std::bad_alloc();
    }
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

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

