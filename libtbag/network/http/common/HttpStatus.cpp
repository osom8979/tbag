/**
 * @file   HttpStatus.cpp
 * @brief  HttpStatus utility implementation.
 * @author zer0
 * @date   2017-09-29
 */

#include <libtbag/network/http/common/HttpStatus.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

char const * getHttpStatusName(HttpStatus status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case HttpStatus::SC_##name: return #name;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown status code";
    }
}

char const * getHttpStatusReason(HttpStatus status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case HttpStatus::SC_##name: return reason;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "Unknown status code";
    }
}

int getHttpStatusNumber(HttpStatus status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case HttpStatus::SC_##name: return code;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return static_cast<int>(HttpStatus::SC_UNKNOWN);
    }
}

HttpStatus getHttpStatus(int status) TBAG_NOEXCEPT
{
    switch (status) {
#define _TBAG_XX(code, name, reason) case static_cast<int>(HttpStatus::SC_##name): return HttpStatus::SC_##name;
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return HttpStatus::SC_UNKNOWN;
    }
}

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

