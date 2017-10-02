/**
 * @file   HttpStatus.hpp
 * @brief  HttpMethod enum class prototype.
 * @author zer0
 * @date   2017-09-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPSTATUS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPSTATUS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

// HTTP Status Code Definitions.
// Reference: https://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html

#ifndef TBAG_HTTP_STATUS_MAP
#define TBAG_HTTP_STATUS_MAP(_TBAG_XX)                                                 \
    /* 1xx: Informational */                                                           \
    _TBAG_XX(100, CONTINUE                       , "Continue"                        ) \
    _TBAG_XX(101, SWITCHING_PROTOCOLS            , "Switching Protocols"             ) \
    /* 2xx: Successful */                                                              \
    _TBAG_XX(200, OK                             , "OK"                              ) \
    _TBAG_XX(201, CREATED                        , "Created"                         ) \
    _TBAG_XX(202, ACCEPTED                       , "Accepted"                        ) \
    _TBAG_XX(203, NON_AUTHORITATIVE_INFORMATION  , "Non-Authoritative Information"   ) \
    _TBAG_XX(204, NO_CONTENT                     , "No Content"                      ) \
    _TBAG_XX(205, RESET_CONTENT                  , "Reset Content"                   ) \
    _TBAG_XX(206, PARTIAL_CONTENT                , "Partial Content"                 ) \
    /* 3xx: Redirection */                                                             \
    _TBAG_XX(300, MULTIPLE_CHOICES               , "Multiple Choices"                ) \
    _TBAG_XX(301, MOVED_PERMANENTLY              , "Moved Permanently"               ) \
    _TBAG_XX(302, FOUND                          , "Found"                           ) \
    _TBAG_XX(303, SEE_OTHER                      , "See Other"                       ) \
    _TBAG_XX(304, NOT_MODIFIED                   , "Not Modified"                    ) \
    _TBAG_XX(305, USE_PROXY                      , "Use Proxy"                       ) \
    _TBAG_XX(306, _UNUSED_                       , "(Unused)"                        ) \
    _TBAG_XX(307, TEMPORARY_REDIRECT             , "Temporary Redirect"              ) \
    /* 4xx: Client Error */                                                            \
    _TBAG_XX(400, BAD_REQUEST                    , "Bad Request"                     ) \
    _TBAG_XX(401, UNAUTHORIZED                   , "Unauthorized"                    ) \
    _TBAG_XX(402, PAYMENT_REQUIRED               , "Payment Required"                ) \
    _TBAG_XX(403, FORBIDDEN                      , "Forbidden"                       ) \
    _TBAG_XX(404, NOT_FOUND                      , "Not Found"                       ) \
    _TBAG_XX(405, METHOD_NOT_ALLOWED             , "Method Not Allowed"              ) \
    _TBAG_XX(406, NOT_ACCEPTABLE                 , "Not Acceptable"                  ) \
    _TBAG_XX(407, PROXY_AUTHENTICATION_REQUIRED  , "Proxy Authentication Required"   ) \
    _TBAG_XX(408, REQUEST_TIMEOUT                , "Request Timeout"                 ) \
    _TBAG_XX(409, CONFLICT                       , "Conflict"                        ) \
    _TBAG_XX(410, GONE                           , "Gone"                            ) \
    _TBAG_XX(411, LENGTH_REQUIRED                , "Length Required"                 ) \
    _TBAG_XX(412, PRECONDITION_FAILED            , "Precondition Failed"             ) \
    _TBAG_XX(413, REQUEST_ENTITY_TOO_LARGE       , "Request Entity Too Large"        ) \
    _TBAG_XX(414, REQUEST_URI_TOO_LONG           , "Request-URI Too Long"            ) \
    _TBAG_XX(415, UNSUPPORTED_MEDIA_TYPE         , "Unsupported Media Type"          ) \
    _TBAG_XX(416, REQUESTED_RANGE_NOT_SATISFIABLE, "Requested Range Not Satisfiable" ) \
    _TBAG_XX(417, EXPECTATION_FAILED             , "Expectation Failed"              ) \
    /* 5xx: Server Error */                                                            \
    _TBAG_XX(500, INTERNAL_SERVER_ERROR          , "Internal Server Error"           ) \
    _TBAG_XX(501, NOT_IMPLEMENTED                , "Not Implemented"                 ) \
    _TBAG_XX(502, BAD_GATEWAY                    , "Bad Gateway"                     ) \
    _TBAG_XX(503, SERVICE_UNAVAILABLE            , "Service Unavailable"             ) \
    _TBAG_XX(504, GATEWAY_TIMEOUT                , "Gateway Timeout"                 ) \
    _TBAG_XX(505, HTTP_VERSION_NOT_SUPPORTED     , "HTTP Version Not Supported"      ) \
    /* END */
#endif

TBAG_CONSTEXPR int const TBAG_UNKNOWN_HTTP_STATUS = 0;

/**
 * List of http status.
 *
 * @author zer0
 * @date 2017-05-25
 */
enum class HttpStatus : int
{
    SC_UNKNOWN = TBAG_UNKNOWN_HTTP_STATUS,
#define _TBAG_XX(code, name, reason) SC_##name = code,
    TBAG_HTTP_STATUS_MAP(_TBAG_XX)
#undef _TBAG_XX
};

// @formatter:off
TBAG_API char const * getHttpStatusName  (HttpStatus status) TBAG_NOEXCEPT;
TBAG_API char const * getHttpStatusReason(HttpStatus status) TBAG_NOEXCEPT;
TBAG_API int          getHttpStatusNumber(HttpStatus status) TBAG_NOEXCEPT;
// @formatter:on

TBAG_API HttpStatus getHttpStatus(int status) TBAG_NOEXCEPT;
TBAG_API HttpStatus getHttpStatus(std::string const & status);

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPSTATUS_HPP__

