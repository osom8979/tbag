/**
 * @file   HttpProperty.cpp
 * @brief  HttpProperty class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <http_parser.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

#define _TBAG_XX(num, name, str) static_assert( \
        static_cast<int>(::http_method::HTTP_##name) == \
        static_cast<int>(HttpMethod::M_##name), \
        "Mismatch HTTP Method number.");
TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX

char const * getHttpMethodName(HttpMethod method) TBAG_NOEXCEPT
{
    switch (method) {
#define _TBAG_XX(num, name, str) case HttpMethod::M_##name: return #str;
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return "M_UNKNOWN";
    }
}

int getHttpMethodNumber(HttpMethod method) TBAG_NOEXCEPT
{
    switch (method) {
#define _TBAG_XX(num, name, str) case HttpMethod::M_##name: return num;
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
    default: return TBAG_UNKNOWN_HTTP_METHOD;
    }
}

HttpMethod getHttpMethod(std::string const & name)
{
    std::string const UPPER_NAME = string::upper(name);
#define _TBAG_XX(num, name, str) \
    if (UPPER_NAME == getHttpMethodName(HttpMethod::M_##name)) \
    { return HttpMethod::M_##name; }
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
    return HttpMethod::M_UNKNOWN;
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

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------
