/**
 * @file   Err.cpp
 * @brief  Err class implementation.
 * @author zer0
 * @date   2016-12-14
 * @date   2017-05-18 (Rename: ErrorCode -> Err)
 */

#include <libtbag/Err.hpp>
#include <libtbag/log/Log.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

// @formatter:off
std::string getUvErrorString(int uv_error_code) { return std::string(::uv_strerror(uv_error_code)); }
std::string getUvErrorName  (int uv_error_code) { return std::string(::uv_err_name(uv_error_code)); }
// @formatter:on

char const * getErrorMessage(Err code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(name, message) case Err::E##name: return message;
#define _TBAG_UV_XX(name, message)
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_UV_XX
#undef _TBAG_XX
    default: return "Unknown error code.";
    }
}

Err getUerr(int uv_error_code)
{
    // @formatter:off
    switch (uv_error_code) {
    case 0: return Err::E_SUCCESS;
#define _TBAG_ERASE_XX(name, msg)
#define _TBAG_XX(name, msg) case UV##name: return Err::E##name;
    TBAG_ERROR_INFO_MAP(_TBAG_ERASE_XX, _TBAG_XX)
#undef _TBAG_ERASE_XX
#undef _TBAG_XX
    default: return Err::E_UNKNOWN;
    }
    // @formatter:on
}

Err getUerr2(char const * prefix, int uv_error_code)
{
    if (uv_error_code != 0) {
        tDLogE("{} error [{}] {}", prefix, uv_error_code, getUvErrorName(uv_error_code));
        return getUerr(uv_error_code);
    }
    return Err::E_SUCCESS;
}

std::string getErrorName(Err err)
{
    // @formatter:off
    switch (err) {
#define _TBAG_XX(name, msg) case Err::E##name: return std::string("E"#name);
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    default: return std::string("E_UNKNOWN");
    }
    // @formatter:on
}

std::string getErrorDetail(Err err)
{
    // @formatter:off
    switch (err) {
#define _TBAG_XX(name, msg) case Err::E##name: return std::string(msg);
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    default: return std::string("Unknown error");
    }
    // @formatter:on
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

