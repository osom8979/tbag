/**
 * @file   UvBase.cpp
 * @brief  UvBase class implementation.
 * @author zer0
 * @date   2016-12-27
 * @date   2016-01-03 (Rename: Type -> UvCommon)
 */

#include <libtbag/uv/UvCommon.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// @formatter:off
#define _TBAG_XX(name, type) case UvType::name: return true;
#define _TBAG_NX(name, type)
bool isHandle (UvType type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) default: return false; } }
bool isRequest(UvType type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) default: return false; } }
bool isEtc    (UvType type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) default: return false; } }
#undef _TBAG_XX
#undef _TBAG_NX
// @formatter:on

bool isStream(UvType type)
{
    return (type == UvType::TCP || type == UvType::PIPE || type == UvType::TTY);
}

// ------------------
// Debugging methods.
// ------------------

// @formatter:off
std::string getUvErrorString(int uv_error_code) { return std::string(::uv_strerror(uv_error_code)); }
std::string getUvErrorName  (int uv_error_code) { return std::string(::uv_err_name(uv_error_code)); }
// @formatter:on

// ---------------------
// Native handle helper.
// ---------------------

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

binf defaultOnAlloc(std::vector<char> & buffer, std::size_t suggested_size)
{
    if (buffer.size() < suggested_size) {
        buffer.resize(suggested_size);
    }

    binf info;
    info.buffer = &buffer[0];
    info.size   =  buffer.size();
    return info;
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

