/**
 * @file   UvUtils.cpp
 * @brief  UvUtils class implementation.
 * @author zer0
 * @date   2016-11-03
 */

#include <libtbag/util/UvUtils.hpp>
#include <cstdlib>
#include <cstring>
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

// ------------------------
// UvHandle implementation.
// ------------------------

UvNative::UvNative(UvType type) : TYPE(type), _native(nullptr)
{

    switch (TYPE) {
#define _TBAG_XX(name, type)                    \
    case UvType::name:                          \
        _native = ::malloc(sizeof(type));       \
        ::memset(_native, 0x00, sizeof(type));  \
        break;
    TBAG_UTIL_UV_HANDLE_MAP_ALL(_TBAG_XX)
#undef _TBAG_XX
    default: _native = nullptr; break;
    }
}

UvNative::~UvNative()
{
    if (_native != nullptr) {
        ::free(_native);
        _native = nullptr;
    }
}

// --------------------------
// UvInStream implementation.
// --------------------------

UvInStream::UvInStream(UvType type) : UvNative(type)
{
}

UvInStream::~UvInStream()
{
}

// ---------------------------
// UvOutStream implementation.
// ---------------------------

UvOutStream::UvOutStream(UvType type) : UvNative(type)
{
}

UvOutStream::~UvOutStream()
{
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

