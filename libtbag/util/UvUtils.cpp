/**
 * @file   UvUtils.cpp
 * @brief  UvUtils class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/util/UvUtils.hpp>
#include <libtbag/loop/UvEventDispatcher.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <cstring>
#include <exception>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

void initUv()
{
    uv_loop_t loop = {0,};
    ::uv_loop_init(&loop);
    ::uv_loop_close(&loop);
}

char const * getUvHandleName(void * handle)
{
#ifndef _CASE_TYPE_NAME_RETURN
#define _CASE_TYPE_NAME_RETURN(name) case name: return #name
#endif

    switch (static_cast<uv_handle_t*>(handle)->type) {
    _CASE_TYPE_NAME_RETURN(UV_UNKNOWN_HANDLE);
    _CASE_TYPE_NAME_RETURN(UV_NAMED_PIPE);
    _CASE_TYPE_NAME_RETURN(UV_TTY);
    _CASE_TYPE_NAME_RETURN(UV_TCP);
    _CASE_TYPE_NAME_RETURN(UV_UDP);
    _CASE_TYPE_NAME_RETURN(UV_PREPARE);
    _CASE_TYPE_NAME_RETURN(UV_CHECK);
    _CASE_TYPE_NAME_RETURN(UV_IDLE);
    _CASE_TYPE_NAME_RETURN(UV_ASYNC);
    _CASE_TYPE_NAME_RETURN(UV_TIMER);
    _CASE_TYPE_NAME_RETURN(UV_PROCESS);
    _CASE_TYPE_NAME_RETURN(UV_FS_EVENT);
    _CASE_TYPE_NAME_RETURN(UV_POLL);
    _CASE_TYPE_NAME_RETURN(UV_FS_POLL);
    _CASE_TYPE_NAME_RETURN(UV_SIGNAL);
    _CASE_TYPE_NAME_RETURN(UV_FILE);
    default: return "Undefined handle type.";
    }

#undef _CASE_TYPE_NAME_RETURN
}

std::string getUvErrorString(int uv_error_code)
{
    return std::string(uv_strerror(uv_error_code));
}

std::string getUvErrorName(int uv_error_code)
{
    return std::string(uv_err_name(uv_error_code));
}

bool isUvHandle(UvType type)
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

bool isUvRequest(UvType type)
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

// ------------------------
// UvHandle implementation.
// ------------------------

TBAG_UV_EVENT_DEFAULT_IMPLEMENT_OPEN(UvHandle);
//{
    TBAG_UV_EVENT_CLOSE(onClose);
//}
TBAG_UV_EVENT_DEFAULT_IMPLEMENT_CLOSE(UvHandle);

UvHandle::UvHandle(UvHandleType type) : UvNative(static_cast<UvType>(type)), _on_close_cb(nullptr)
{
    if (isHandle() == false) {
        __tbag_error("UvHandle::UvHandle({}) type is not handle type", static_cast<int>(type));
        throw std::bad_alloc();
    }
    TBAG_UV_EVENT_DEFAULT_REGISTER(this->getNative(), this);
}

UvHandle::~UvHandle()
{
    TBAG_UV_EVENT_DEFAULT_UNREGISTER(this->getNative());
}

bool UvHandle::isClosing() const TBAG_NOEXCEPT
{
    return static_cast<bool>(::uv_is_closing(this->castNative<uv_handle_t>()));
}

ErrorCode UvHandle::close()
{
    if (isHandle() == false) {
        __tbag_error("UvNative::close() Unsupported uv type: {}", static_cast<int>(this->getType()));
        return ErrorCode::UNSUPPORTED;
    }
    ::uv_close(this->castNative<uv_handle_t>(), TBAG_UV_EVENT_DEFAULT_CALLBACK_CLOSE(onClose));
    return ErrorCode::SUCCESS;
}

void UvHandle::onClose(void * handle)
{
    if (_on_close_cb != nullptr) {
        _on_close_cb->onClose();
    }
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

