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
#include <lmdb.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

char const * getErrName(Err code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(name, message) case Err::E##name: return "E"#name;
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    default: return "E_UNKNOWN";
    }
}

char const * getErrDetail(Err code) TBAG_NOEXCEPT
{
    switch (code) {
#define _TBAG_XX(name, message) case Err::E##name: return message;
    TBAG_ERROR_INFO_MAP(_TBAG_XX, _TBAG_XX, _TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown error.";
    }
}

Err convertUvErrorToErr(int uv_error_code) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (uv_error_code) {
    case 0: return Err::E_SUCCESS;
#define _TBAG_ERASE_XX(name, msg)
#define _TBAG_XX(name, msg) case UV##name: return Err::E##name;
    TBAG_ERROR_INFO_MAP(_TBAG_ERASE_XX, _TBAG_XX, _TBAG_ERASE_XX)
#undef _TBAG_ERASE_XX
#undef _TBAG_XX
    default: return Err::E_UNKNOWN;
    }
    // @formatter:on
}

Err convertUvErrorToErrWithLogging(char const * prefix, int uv_error_code)
{
    if (uv_error_code != 0) {
        tDLogE("uv {} error [{}] {}", prefix, uv_error_code, getUvErrorName(uv_error_code));
        return convertUvErrorToErr(uv_error_code);
    }
    return Err::E_SUCCESS;
}

Err convertMdbErrorToErr(int mdb_error_code) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (mdb_error_code) {
    case 0: return Err::E_SUCCESS;
#define _TBAG_ERASE_XX(name, msg)
#define _TBAG_XX(name, msg) case MDB##name: return Err::E##name;
    TBAG_ERROR_INFO_MAP(_TBAG_ERASE_XX, _TBAG_ERASE_XX, _TBAG_XX)
#undef _TBAG_ERASE_XX
#undef _TBAG_XX
    default: return Err::E_UNKNOWN;
    }
    // @formatter:on
}

Err convertMdbErrorToErrWithLogging(char const * prefix, int mdb_error_code)
{
    if (mdb_error_code != MDB_SUCCESS) {
        tDLogE("mdb {} error [{}] {}", prefix, mdb_error_code, getUvErrorName(mdb_error_code));
        return convertMdbErrorToErr(mdb_error_code);
    }
    return Err::E_SUCCESS;
}

// ----------------
// libuv debugging.
// ----------------

char const * getUvErrorDetail(int uv_error_code)
{
    return ::uv_strerror(uv_error_code);
}

char const * getUvErrorName(int uv_error_code)
{
    return ::uv_err_name(uv_error_code);
}

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

