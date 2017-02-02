/**
 * @file   UvCommon.cpp
 * @brief  UvCommon class implementation.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-01-03 (Rename: Type -> UvCommon)
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/UvCommon.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// @formatter:off
#define _TBAG_XX(name, type) case UvType::name: return true;
#define _TBAG_NX(name, type)
bool isHandle (utype type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_XX, _TBAG_NX, _TBAG_NX) default: return false; } }
bool isRequest(utype type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_XX, _TBAG_NX) default: return false; } }
bool isEtc    (utype type) { switch (type) { TBAG_UV_HANDLE_MAP(_TBAG_NX, _TBAG_NX, _TBAG_XX) default: return false; } }
#undef _TBAG_XX
#undef _TBAG_NX
// @formatter:on

bool isStream(utype type)
{
    return (type == utype::TCP || type == utype::PIPE || type == utype::TTY);
}

// ------------------
// Debugging methods.
// ------------------

// @formatter:off
std::string getUvErrorString(int uv_error_code) { return std::string(::uv_strerror(uv_error_code)); }
std::string getUvErrorName  (int uv_error_code) { return std::string(::uv_err_name(uv_error_code)); }
// @formatter:on

uerr getUvppErrorCode(int uv_error_code)
{
    // @formatter:off
    switch (uv_error_code) {
    case 0: return uerr::UVPP_SUCCESS;
#define _TBAG_XX(name, msg) case UV##name: return uerr::UVPP##name;
    TBAG_UV_ERROR_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return uerr::UVPP_UNKNOWN;
    }
    // @formatter:on
}

std::string getErrorName(ErrorCode err)
{
    // @formatter:off
    switch (err) {
    case uerr::UVPP_SUCCESS: return std::string("UVPP_SUCCESS");
#define _TBAG_XX(name, msg) case uerr::UVPP##name: return std::string("UVPP"#name);
    TBAG_UV_ERROR_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return std::string("UVPP_UNKNOWN");
    }
    // @formatter:on
}

std::string getErrorDetail(ErrorCode err)
{
    // @formatter:off
    switch (err) {
    case uerr::UVPP_SUCCESS: return std::string("No error");
#define _TBAG_XX(name, msg) case uerr::UVPP##name: return std::string(msg);
    TBAG_UV_ERROR_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return std::string("Unknown error");
    }
    // @formatter:on
}

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

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

