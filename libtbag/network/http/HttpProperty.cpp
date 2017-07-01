/**
 * @file   HttpProperty.cpp
 * @brief  HttpProperty class implementation.
 * @author zer0
 * @date   2017-05-25
 */

#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <http_parser.h>
#include <sstream>

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

// ----------------------------------
// HttpCommonProperty implementation.
// ----------------------------------

std::string HttpCommonProperty::getHeader(std::string const & key) const
{
    auto itr = headers.find(key);
    if (itr != headers.end()) {
        return itr->second;
    }
    return std::string();
}

bool HttpCommonProperty::existsHeaderValue(std::string const & key, std::string const & value, bool ignore_case) const
{
    return existsHeaderValueFromHeaderMap(headers, key, value, ignore_case);
}

bool HttpCommonProperty::existsHeaderValueFromHeaderMap(
        HttpHeaderMap const & headers,
        std::string const & key,
        std::string const & value,
        bool ignore_case)
{
    auto itr = headers.find(key);
    if (itr != headers.end()) {
        std::string const VALUE = (ignore_case ? string::lower(value) : value);
        for (auto & cursor : string::splitTokens(itr->second, HEADER_VALUE_DELIMITER)) {
            if (VALUE == string::lower(string::trim(cursor))) {
                return true;
            }
        }
    }
    return false;
}

// ------------------------------------
// HttpResponseProperty implementation.
// ------------------------------------

void HttpResponseProperty::setStatus(std::string const & str)
{
    try {
        status = std::stoi(str);
    } catch (...) {
        status = 0;
    }
}

// ------------------
// Debugging methods.
// ------------------

std::string toDebugString(HttpHeaderMap const & obj)
{
    std::stringstream ss;
    std::size_t const SIZE = obj.size();
    auto itr = obj.begin();
    for (std::size_t i = 0; i < SIZE; ++i, ++itr) {
        ss << "[H] " << itr->first << ": " << itr->second;
        if (i + 1 < SIZE) {
            ss << std::endl;
        }
    }
    return ss.str();
}

std::string toDebugString(HttpCommonProperty const & obj)
{
    std::stringstream ss;
    ss << toDebugString(obj.headers) << std::endl;
    ss << "[BODY] " << obj.body;
    return ss.str();
}

std::string toDebugString(HttpCommon const & obj)
{
    std::stringstream ss;
    ss << "HTTP/" << obj.http_minor << "." << obj.http_minor << std::endl;
    ss << toDebugString(static_cast<HttpCommonProperty const &>(obj));
    return ss.str();
}

std::string toDebugString(HttpRequest const & obj)
{
    std::stringstream ss;
    ss << "REQUEST: " << obj.method << " " << obj.url << " ";
    ss << toDebugString(static_cast<HttpCommon const &>(obj));
    return ss.str();
}

std::string toDebugString(HttpResponse const & obj)
{
    std::stringstream ss;
    ss << "RESPONSE: " << obj.status << " " << obj.reason << " ";
    ss << toDebugString(static_cast<HttpCommon const &>(obj));
    return ss.str();
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

