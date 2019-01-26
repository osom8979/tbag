/**
 * @file   HttpCommon.hpp
 * @brief  HttpCommon class prototype.
 * @author zer0
 * @date   2018-12-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCOMMON_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCOMMON_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/algorithm/Swap.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

#include <cstdint>
#include <functional>
#include <regex>
#include <string>
#include <vector>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

TBAG_CONSTEXPR char const * const HTTP_SCHEMA_LOWER  = "http";
TBAG_CONSTEXPR char const * const HTTPS_SCHEMA_LOWER = "https";

TBAG_CONSTEXPR char const * const HTTP = "HTTP";
TBAG_CONSTEXPR char const * const   SP = " ";
TBAG_CONSTEXPR char const * const CRLF = "\r\n";
TBAG_CONSTEXPR char const * const   CR = "\r";
TBAG_CONSTEXPR char const * const   LF = "\n";
TBAG_CONSTEXPR char const * const   HT = "\t";

TBAG_CONSTEXPR int const DEFAULT_HTTP_VERSION_MAJOR = 1;
TBAG_CONSTEXPR int const DEFAULT_HTTP_VERSION_MINOR = 1;

/**
 * @defgroup __DOXYGEN_GROUP__HTTP_HEADER__ List of http headers.
 * @see <https://en.wikipedia.org/wiki/List_of_HTTP_header_fields>
 * @{
 */

TBAG_CONSTEXPR char const * const HEADER_HOST               = "Host";
TBAG_CONSTEXPR char const * const HEADER_UPGRADE            = "Upgrade";
TBAG_CONSTEXPR char const * const HEADER_SERVER             = "Server";
TBAG_CONSTEXPR char const * const HEADER_USER_AGENT         = "User-Agent";
TBAG_CONSTEXPR char const * const HEADER_ACCEPT             = "Accept";
TBAG_CONSTEXPR char const * const HEADER_TRANSFER_ENCODING  = "Transfer-Encoding";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_TYPE       = "Content-Type";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_LENGTH     = "Content-Length";
TBAG_CONSTEXPR char const * const HEADER_ORIGIN             = "Origin";
TBAG_CONSTEXPR char const * const HEADER_CONNECTION         = "Connection";

// WebSocket

TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_KEY      = "Sec-WebSocket-Key";
TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_ACCEPT   = "Sec-WebSocket-Accept";
TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_PROTOCOL = "Sec-WebSocket-Protocol";
TBAG_CONSTEXPR char const * const HEADER_SEC_WEBSOCKET_VERSION  = "Sec-WebSocket-Version";

/**
 * @}
 */

TBAG_CONSTEXPR char const * const VALUE_WEBSOCKET        = "WebSocket";
TBAG_CONSTEXPR char const * const VALUE_UPGRADE          = "Upgrade";
TBAG_CONSTEXPR char const * const VALUE_TBAG_PROTOCOL    = "Tbag";
TBAG_CONSTEXPR char const * const VALUE_TBAG_SERVER_INFO = LIBTBAG_TITLE_STRING "/" LIBTBAG_VERSION_STRING;

TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_SERVER            = VALUE_TBAG_SERVER_INFO;
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_CONTENT_TYPE      = "text/html; charset=utf-8";
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_USER_AGENT        = VALUE_TBAG_SERVER_INFO;
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_ACCEPT            = "*/*";
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_TRANSFER_ENCODING = "identity";

TBAG_CONSTEXPR char const * const HEADER_DELIMITER = ":";
TBAG_CONSTEXPR char const * const VALUE_DELIMITER  = ",";
TBAG_CONSTEXPR char const * const PATH_DELIMITER   = "/";
TBAG_CONSTEXPR char const * const ROOT_PATH        = "/";

TBAG_CONSTEXPR uint64_t const DEFAULT_HTTP_TIMEOUT_MILLISEC = 8 * 1000;

TBAG_CONSTEXPR int const DEFAULT_HTTP_PORT  = 80;
TBAG_CONSTEXPR int const DEFAULT_HTTPS_PORT = 443;

// ------------------
// WebSocket section.
// ------------------

TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI00 =  0; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI07 =  7; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI08 =  8; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI13 = 13; ///< RFC 6455 (Standard)

TBAG_CONSTEXPR char const * const WEBSOCKET_HANDSHAKE_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

TBAG_CONSTEXPR uint8_t const WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE = 2;

enum class EventType
{
    ET_CONNECT,
    ET_START,
    ET_READ
};

inline char const * const getEventTypeName(EventType type) TBAG_NOEXCEPT
{
    switch (type) {
    case EventType::ET_CONNECT: return "CONNECT";
    case EventType::ET_START:   return "START";
    case EventType::ET_READ:    return "READ";
    default:                    return "UNKNOWN";
    }
}

#ifndef TBAG_HTTP_METHOD_MAP
#define TBAG_HTTP_METHOD_MAP(_TBAG_XX)      \
    _TBAG_XX( 0, DELETE,      DELETE)       \
    _TBAG_XX( 1, GET,         GET)          \
    _TBAG_XX( 2, HEAD,        HEAD)         \
    _TBAG_XX( 3, POST,        POST)         \
    _TBAG_XX( 4, PUT,         PUT)          \
    /* pathological */                      \
    _TBAG_XX( 5, CONNECT,     CONNECT)      \
    _TBAG_XX( 6, OPTIONS,     OPTIONS)      \
    _TBAG_XX( 7, TRACE,       TRACE)        \
    /* WebDAV */                            \
    _TBAG_XX( 8, COPY,        COPY)         \
    _TBAG_XX( 9, LOCK,        LOCK)         \
    _TBAG_XX(10, MKCOL,       MKCOL)        \
    _TBAG_XX(11, MOVE,        MOVE)         \
    _TBAG_XX(12, PROPFIND,    PROPFIND)     \
    _TBAG_XX(13, PROPPATCH,   PROPPATCH)    \
    _TBAG_XX(14, SEARCH,      SEARCH)       \
    _TBAG_XX(15, UNLOCK,      UNLOCK)       \
    _TBAG_XX(16, BIND,        BIND)         \
    _TBAG_XX(17, REBIND,      REBIND)       \
    _TBAG_XX(18, UNBIND,      UNBIND)       \
    _TBAG_XX(19, ACL,         ACL)          \
    /* subversion */                        \
    _TBAG_XX(20, REPORT,      REPORT)       \
    _TBAG_XX(21, MKACTIVITY,  MKACTIVITY)   \
    _TBAG_XX(22, CHECKOUT,    CHECKOUT)     \
    _TBAG_XX(23, MERGE,       MERGE)        \
    /* upnp */                              \
    _TBAG_XX(24, MSEARCH,     M-SEARCH)     \
    _TBAG_XX(25, NOTIFY,      NOTIFY)       \
    _TBAG_XX(26, SUBSCRIBE,   SUBSCRIBE)    \
    _TBAG_XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
    /* RFC-5789 */                          \
    _TBAG_XX(28, PATCH,       PATCH)        \
    _TBAG_XX(29, PURGE,       PURGE)        \
    /* CalDAV */                            \
    _TBAG_XX(30, MKCALENDAR,  MKCALENDAR)   \
    /* RFC-2068, section 19.6.1.2 */        \
    _TBAG_XX(31, LINK,        LINK)         \
    _TBAG_XX(32, UNLINK,      UNLINK)       \
    /* EMD */
#endif

TBAG_CONSTEXPR int const TBAG_UNKNOWN_HTTP_METHOD = -1;

/**
 * List of http method.
 *
 * @author zer0
 * @date 2017-05-25
 */
enum class HttpMethod : int
{
    M_UNKNOWN = TBAG_UNKNOWN_HTTP_METHOD,
#define _TBAG_XX(num, name, str) M_##name = num,
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
};

#define _TBAG_XX(num, name, str)                                     \
    struct HttpMethod##name                                          \
    {                                                                \
        TBAG_CONSTEXPR static char const * getMethod() TBAG_NOEXCEPT \
        { return #name; }                                            \
    }; /* -- END -- */
TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX

static_assert(static_cast<int>(HttpMethod::M_UNKNOWN) == TBAG_UNKNOWN_HTTP_METHOD,
              "Unknown HTTP Method number is not equals.");

// @formatter:off
TBAG_API char const * getHttpMethodName  (HttpMethod method) TBAG_NOEXCEPT;
TBAG_API int          getHttpMethodNumber(HttpMethod method) TBAG_NOEXCEPT;
TBAG_API HttpMethod   getHttpMethod      (std::string const & name);
// @formatter:on

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
    /* [UNOFFICIAL] 6xx: libtbag exception */                                          \
    _TBAG_XX(600, TBAG_UNKNOWN_EXCEPTION         , "Unknown libtbag exception"       ) \
    _TBAG_XX(601, TBAG_LIBRARY_TIMEOUT           , "libtbag timeout exception"       ) \
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

using HttpBuffer = libtbag::util::Buffer;
using HttpHeader = std::multimap<std::string, std::string>;

/**
 * Lowercase and trim of all tokens.
 */
TBAG_API HttpHeader getRegularization(HttpHeader const & header);

TBAG_API std::string getHeaderValue(HttpHeader const & header, std::string const & key);
TBAG_API std::string getIgnoreCase(HttpHeader const & header, std::string const & key);

TBAG_API std::size_t insert(HttpHeader & header, std::string const & key, std::string const & val);
TBAG_API std::size_t insert(HttpHeader & header, HttpHeader const & insert_header);

TBAG_API std::size_t insertIfNotExists(HttpHeader & header, std::string const & key, std::string const & val);
TBAG_API std::size_t insertIfNotExists(HttpHeader & header, HttpHeader const & insert_header);

TBAG_API bool exists(HttpHeader const & header, std::string const & key);
TBAG_API bool exists(HttpHeader const & header, std::string const & key, std::string const & val);

TBAG_API bool existsInSplitValues(HttpHeader const & header, std::string const & key, std::string const & val);
TBAG_API bool existsInSplitValues(HttpHeader const & header, std::string const & key, std::string const & val, std::string const & delimiter);

/**
 * Generate HTTP Message Headers.
 *
 * @remarks
 *  @code
 *   message-header = field-name ":" [ field-value ]
 *   field-name     = token
 *   field-value    = *( field-content | LWS )
 *   field-content  = <the OCTETs making up the field-value
 *                    and consisting of either *TEXT or combinations
 *                    of token, separators, and quoted-string>
 *  @endcode
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-4.2>
 */
TBAG_API std::string toMessageHeader(HttpHeader const & header);

// Forward declarations.
struct HttpCommon;
struct HttpBaseRequest;
struct HttpBaseResponse;
struct HttpRequest;
struct HttpResponse;
struct HttpProperty;

/**
 * @remarks
 *  @code
 *   HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
 *  @endcode
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-3.1>
 */
TBAG_API std::string toVersionString(int http_major, int http_minor);
TBAG_API std::string toVersionString(HttpCommon const & common);

TBAG_API std::string toDebugHeaderString(HttpHeader const & header);
TBAG_API std::string toDebugHeaderString(HttpCommon const & common);

TBAG_API std::vector<int> getWsVersions(HttpHeader const & header);
TBAG_API bool testWsVersion(HttpHeader const & header, int test_version = WEBSOCKET_VERSION_HYBI13);

/**
 * HttpCommon structure.
 *
 * @author zer0
 * @date   2018-12-25
 */
struct HttpCommon
{
    /** Use 'http_' as a prefix to avoid variable name conflicts. */
    int http_major = DEFAULT_HTTP_VERSION_MAJOR;
    int http_minor = DEFAULT_HTTP_VERSION_MINOR;

    HttpHeader header;
    HttpBuffer body;

    HttpHeader getRegularization() const
    { return libtbag::http::getRegularization(header); }
    void regularization()
    { header = getRegularization(); }

    std::string getHeaderValue(std::string const & key) const
    { return libtbag::http::getHeaderValue(header, key); }
    std::string getIgnoreCase(std::string const & key) const
    { return libtbag::http::getIgnoreCase(header, key); }

    std::size_t insert(std::string const & key, std::string const & val)
    { return libtbag::http::insert(header, key, val); }
    std::size_t insert(HttpHeader const & insert_header)
    { return libtbag::http::insert(header, insert_header); }

    std::size_t insertIfNotExists(std::string const & key, std::string const & val)
    { return libtbag::http::insertIfNotExists(header, key, val); }
    std::size_t insertIfNotExists(HttpHeader const & insert_header)
    { return libtbag::http::insertIfNotExists(header, insert_header); }

    bool exists(std::string const & key) const
    { return libtbag::http::exists(header, key); }
    bool exists(std::string const & key, std::string const & val) const
    { return libtbag::http::exists(header, key, val); }

    bool existsInSplitValues(std::string const & key, std::string const & val) const
    { return libtbag::http::existsInSplitValues(header, key, val); }
    bool existsInSplitValues(std::string const & key, std::string const & val, std::string const & delimiter) const
    { return libtbag::http::existsInSplitValues(header, key, val, delimiter); }

    std::string getBodyString() const
    { return std::string(body.begin(), body.end()); }
    void setBodyString(std::string const & body_string)
    { body.assign(body_string.begin(), body_string.end()); }
    void appendBodyString(std::string const & body_string)
    { body.insert(body.end(), body_string.begin(), body_string.end()); }

    std::string toVersionString() const
    { return libtbag::http::toVersionString(http_major, http_minor); }
    std::string toDebugHeaderString() const
    { return libtbag::http::toDebugHeaderString(header); }

    std::vector<int> getWsVersions() const
    { return libtbag::http::getWsVersions(header); }
    bool testWsVersion(int test_version = WEBSOCKET_VERSION_HYBI13) const
    { return libtbag::http::testWsVersion(header, test_version); }
};

TBAG_API std::string generateRandomWebSocketKey();
TBAG_API std::string getUpgradeWebSocketKey(std::string const & key);

TBAG_API void clear(HttpCommon & common);
TBAG_API void clear(HttpBaseRequest & request);
TBAG_API void clear(HttpBaseResponse & response);
TBAG_API void clear(HttpRequest & request);
TBAG_API void clear(HttpResponse & response);
TBAG_API void clear(HttpProperty & property);

// @formatter:off
TBAG_API void swap(HttpCommon       & lh, HttpCommon       & rh) TBAG_NOEXCEPT;
TBAG_API void swap(HttpBaseRequest  & lh, HttpBaseRequest  & rh) TBAG_NOEXCEPT;
TBAG_API void swap(HttpBaseResponse & lh, HttpBaseResponse & rh) TBAG_NOEXCEPT;
TBAG_API void swap(HttpRequest      & lh, HttpRequest      & rh) TBAG_NOEXCEPT;
TBAG_API void swap(HttpResponse     & lh, HttpResponse     & rh) TBAG_NOEXCEPT;
TBAG_API void swap(HttpProperty     & lh, HttpProperty     & rh) TBAG_NOEXCEPT;
// @formatter:on

/**
 * Generally, Called from the server side.
 */
TBAG_API bool checkWsRequest(HttpMethod method, HttpHeader const & header, int test_version = WEBSOCKET_VERSION_HYBI13);
TBAG_API bool checkWsRequest(std::string const & method, HttpHeader const & header, int test_version = WEBSOCKET_VERSION_HYBI13);
TBAG_API bool checkWsRequest(HttpRequest const & request, int test_version = WEBSOCKET_VERSION_HYBI13);
TBAG_API bool checkWsRequest(HttpProperty const & property, int test_version = WEBSOCKET_VERSION_HYBI13);

TBAG_API void updateDefaultRequest(HttpCommon & common, HttpBaseRequest & request);
TBAG_API void updateDefaultRequest(HttpRequest & request);
TBAG_API void updateDefaultRequest(HttpProperty & property);

TBAG_API void updateDefaultWsRequest(HttpCommon & common, HttpBaseRequest & request, std::string const & ws_key);
TBAG_API void updateDefaultWsRequest(HttpRequest & request, std::string const & ws_key);
TBAG_API void updateDefaultWsRequest(HttpProperty & property, std::string const & ws_key);

/**
 * @remarks
 *  @code
 *   Request-Line = Method SP Request-URI SP HTTP-Version CRLF
 *  @endcode
 */
TBAG_API std::string toRequestLine(HttpCommon const & common, HttpBaseRequest const & request);
TBAG_API std::string toRequestLine(HttpRequest const & request);
TBAG_API std::string toRequestLine(HttpProperty const & property);

TBAG_API std::string toRequestString(HttpCommon const & common, HttpBaseRequest const & request);
TBAG_API std::string toRequestString(HttpRequest const & request);
TBAG_API std::string toRequestString(HttpProperty const & property);

TBAG_API std::string toDebugRequestString(HttpCommon const & common, HttpBaseRequest const & request);
TBAG_API std::string toDebugRequestString(HttpRequest const & request);
TBAG_API std::string toDebugRequestString(HttpProperty const & property);

/**
 * Http request information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-5.1>
 */
struct HttpBaseRequest
{
    std::string method;
    std::string path;
};

struct HttpRequest : public HttpCommon, public HttpBaseRequest
{
    void setMethod(HttpMethod m)
    { method = libtbag::http::getHttpMethodName(m); }
    bool checkWsRequest(int test_version = WEBSOCKET_VERSION_HYBI13) const
    { return libtbag::http::checkWsRequest(*this, test_version); }

    void updateDefaultRequest()
    { libtbag::http::updateDefaultRequest(*this); }
    void updateDefaultWsRequest(std::string const & ws_key)
    { libtbag::http::updateDefaultWsRequest(*this, ws_key); }

    std::string toRequestLine() const
    { return libtbag::http::toRequestLine(*this); }
    std::string toRequestString() const
    { return libtbag::http::toRequestString(*this); }
    std::string toDebugRequestString() const
    { return libtbag::http::toDebugRequestString(*this); }
};

TBAG_API void setHttpStatus(HttpBaseResponse & response, HttpStatus s);
TBAG_API void setHttpStatus(HttpBaseResponse & response, std::string const & name);

TBAG_API HttpStatus getHttpStatus(HttpBaseResponse const & response);

/**
 * Generally, Called from the client side.
 */
TBAG_API bool checkWsResponse(int code, HttpHeader const & header, std::string const & original_key);
TBAG_API bool checkWsResponse(int code, HttpCommon const & common, std::string const & original_key);
TBAG_API bool checkWsResponse(HttpResponse const & response, std::string const & original_key);
TBAG_API bool checkWsResponse(HttpProperty const & property, std::string const & original_key);

TBAG_API void updateDefaultResponse(HttpCommon & common, HttpBaseResponse & response);
TBAG_API void updateDefaultResponse(HttpResponse & response);
TBAG_API void updateDefaultResponse(HttpProperty & property);

TBAG_API void updateDefaultWsResponse(HttpCommon & common, HttpBaseResponse & response, std::string const & key);
TBAG_API void updateDefaultWsResponse(HttpCommon & common, HttpBaseResponse & response, HttpHeader const & request_header);
TBAG_API void updateDefaultWsResponse(HttpCommon & common, HttpBaseResponse & response, HttpCommon const & request_common);
TBAG_API void updateDefaultWsResponse(HttpResponse & response, std::string const & key);
TBAG_API void updateDefaultWsResponse(HttpResponse & response, HttpHeader const & request_header);
TBAG_API void updateDefaultWsResponse(HttpResponse & response, HttpCommon const & request_common);
TBAG_API void updateDefaultWsResponse(HttpProperty & property, std::string const & key);
TBAG_API void updateDefaultWsResponse(HttpProperty & property, HttpHeader const & request_header);
TBAG_API void updateDefaultWsResponse(HttpProperty & property, HttpCommon const & request_common);

/**
 * @remarks
 *  @code
 *   Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
 *  @endcode
 */
TBAG_API std::string toStatusLine(HttpCommon const & common, HttpBaseResponse const & response);
TBAG_API std::string toStatusLine(HttpResponse const & response);
TBAG_API std::string toStatusLine(HttpProperty const & property);

TBAG_API std::string toResponseString(HttpCommon const & common, HttpBaseResponse const & response);
TBAG_API std::string toResponseString(HttpResponse const & response);
TBAG_API std::string toResponseString(HttpProperty const & property);

TBAG_API std::string toDebugResponseString(HttpCommon const & common, HttpBaseResponse const & response);
TBAG_API std::string toDebugResponseString(HttpResponse const & response);
TBAG_API std::string toDebugResponseString(HttpProperty const & property);

/**
 * Http response information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-6.1>
 */
struct HttpBaseResponse
{
    int code = TBAG_UNKNOWN_HTTP_STATUS;
    std::string reason;

    void setHttpStatus(HttpStatus s)
    { libtbag::http::setHttpStatus(*this, s); }
    void setHttpStatus(std::string const & name)
    { libtbag::http::setHttpStatus(*this, name); }

    HttpStatus getHttpStatus() const
    { return libtbag::http::getHttpStatus(code); }
};

struct HttpResponse : public HttpCommon, public HttpBaseResponse
{
    bool checkWsResponse(std::string const & original_key) const
    { return libtbag::http::checkWsResponse(*this, original_key); }

    void updateDefaultResponse()
    { libtbag::http::updateDefaultResponse(*this); }
    void updateDefaultWsResponse(std::string const & key)
    { libtbag::http::updateDefaultWsResponse(*this, key); }
    void updateDefaultWsResponse(HttpHeader const & request_header)
    { libtbag::http::updateDefaultWsResponse(*this, request_header); }
    void updateDefaultWsResponse(HttpCommon const & request_common)
    { libtbag::http::updateDefaultWsResponse(*this, request_common); }

    std::string toStatusLine() const
    { return libtbag::http::toStatusLine(*this); }
    std::string toResponseString() const
    { return libtbag::http::toResponseString(*this); }
    std::string toDebugResponseString() const
    { return libtbag::http::toDebugResponseString(*this); }
};

struct HttpProperty : public HttpCommon, public HttpBaseRequest, public HttpBaseResponse
{
    operator HttpRequest() const
    {
        HttpRequest result;
        // @formatter:off
        result.http_minor = http_minor;
        result.http_major = http_major;
        result.header     = header;
        result.body       = body;
        result.method     = method;
        result.path       = path;
        // @formatter:on
        return result;
    }

    operator HttpResponse() const
    {
        HttpResponse result;
        // @formatter:off
        result.http_minor = http_minor;
        result.http_major = http_major;
        result.header     = header;
        result.body       = body;
        result.code       = code;
        result.reason     = reason;
        // @formatter:on
        return result;
    }

    // -------------------
    // HttpRequest Members
    // -------------------

    bool checkWsRequest(int test_version = WEBSOCKET_VERSION_HYBI13) const
    { return libtbag::http::checkWsRequest(*this, test_version); }

    void updateDefaultRequest()
    { libtbag::http::updateDefaultRequest(*this); }
    void updateDefaultWsRequest(std::string const & ws_key)
    { libtbag::http::updateDefaultWsRequest(*this, ws_key); }

    std::string toRequestLine() const
    { return libtbag::http::toRequestLine(*this); }
    std::string toRequestString() const
    { return libtbag::http::toRequestString(*this); }
    std::string toDebugRequestString() const
    { return libtbag::http::toDebugRequestString(*this); }

    // --------------------
    // HttpResponse Members
    // --------------------

    bool checkWsResponse(std::string const & original_key) const
    { return libtbag::http::checkWsResponse(*this, original_key); }

    void updateDefaultResponse()
    { libtbag::http::updateDefaultResponse(*this); }
    void updateDefaultWsResponse(std::string const & key)
    { libtbag::http::updateDefaultWsResponse(*this, key); }
    void updateDefaultWsResponse(HttpHeader const & request_header)
    { libtbag::http::updateDefaultWsResponse(*this, request_header); }
    void updateDefaultWsResponse(HttpCommon const & request_common)
    { libtbag::http::updateDefaultWsResponse(*this, request_common); }

    std::string toStatusLine() const
    { return libtbag::http::toStatusLine(*this); }
    std::string toResponseString() const
    { return libtbag::http::toResponseString(*this); }
    std::string toDebugResponseString() const
    { return libtbag::http::toDebugResponseString(*this); }
};

/**
 * Http filter interface.
 *
 * @author zer0
 * @date   2017-05-24
 */
struct HttpFilterInterface
{
    HttpFilterInterface() { /* EMPTY. */ }
    virtual ~HttpFilterInterface() { /* EMPTY. */ }

    virtual bool filter(HttpRequest const & request) = 0;
};

/**
 * HttpBaseFilter class prototype.
 *
 * @author zer0
 * @date   2017-05-24
 * @date   2017-10-16 (HttpDefaultFilter -> HttpBaseFilter)
 */
struct HttpBaseFilter : public HttpFilterInterface
{
    std::string method;
    std::regex regex;

    HttpBaseFilter()
    { /* EMPTY. */ }
    HttpBaseFilter(std::string const & m) : method(m)
    { /* EMPTY. */ }
    HttpBaseFilter(std::regex const & r) : regex(r)
    { /* EMPTY. */ }
    HttpBaseFilter(std::string const & m, std::regex const & r) : method(m), regex(r)
    { /* EMPTY. */ }
    HttpBaseFilter(std::string const & m, std::string const & r) : method(m), regex(r)
    { /* EMPTY. */ }

    virtual ~HttpBaseFilter()
    { /* EMPTY. */ }

    virtual bool filter(HttpRequest const & request) override
    {
        if (method.empty()) {
            return std::regex_match(request.path, regex);
        } else {
            return method == request.method && std::regex_match(request.path, regex);
        }
    }
};

enum class WsOpCode : uint8_t
{
    WSOC_CONTINUATION_FRAME           = 0x0,
    WSOC_TEXT_FRAME                   = 0x1,
    WSOC_BINARY_FRAME                 = 0x2,
    WSOC_RESERVED_NON_CONTROL_FRAME_1 = 0x3,
    WSOC_RESERVED_NON_CONTROL_FRAME_2 = 0x4,
    WSOC_RESERVED_NON_CONTROL_FRAME_3 = 0x5,
    WSOC_RESERVED_NON_CONTROL_FRAME_4 = 0x6,
    WSOC_RESERVED_NON_CONTROL_FRAME_5 = 0x7,
    WSOC_CONNECTION_CLOSE             = 0x8,
    WSOC_DENOTES_PING                 = 0x9,
    WSOC_DENOTES_PONG                 = 0xA,
    WSOC_RESERVED_CONTROL_FRAME_1     = 0xB,
    WSOC_RESERVED_CONTROL_FRAME_2     = 0xC,
    WSOC_RESERVED_CONTROL_FRAME_3     = 0xD,
    WSOC_RESERVED_CONTROL_FRAME_4     = 0xE,
    WSOC_RESERVED_CONTROL_FRAME_5     = 0xF,
};

TBAG_API char const * getWsOpCodeName(WsOpCode code) TBAG_NOEXCEPT;
TBAG_API bool isWsControlFrame(WsOpCode code) TBAG_NOEXCEPT;

inline void swap(WsOpCode & lh, WsOpCode & rh) TBAG_NOEXCEPT
{
    libtbag::algorithm::swapWithPod((uint8_t&)lh, (uint8_t&)rh);
}

enum class WsPayloadBit : uint8_t
{
    WSPL_BIT_7  =  7,
    WSPL_BIT_16 = 16,
    WSPL_BIT_64 = 64,
};

TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_7BIT_TYPE_SIZE  = 125;
TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_16BIT_TYPE_SIZE = 126;
TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_64BIT_TYPE_SIZE = 127;

/**
 * @remarks
 *  The length of the "Payload data", in bytes: if 0-125, that is the
 *  payload length.  If 126, the following 2 bytes interpreted as a
 *  16-bit unsigned integer are the payload length.  If 127, the
 *  following 8 bytes interpreted as a 64-bit unsigned integer (the
 *  most significant bit MUST be 0) are the payload length.  Multibyte
 *  length quantities are expressed in network byte order.  Note that
 *  in all cases, the minimal number of bytes MUST be used to encode
 *  the length, for example, the length of a 124-byte-long string
 *  can't be encoded as the sequence 126, 0, 124.  The payload length
 *  is the length of the "Extension data" + the length of the
 *  "Application data".  The length of the "Extension data" may be
 *  zero, in which case the payload length is the length of the
 *  "Application data".
 *
 * @see <https://tools.ietf.org/html/rfc6455#section-5.2>
 */
TBAG_API WsPayloadBit getWsPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT;
TBAG_API WsPayloadBit getWsPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT;

TBAG_API uint8_t getPayloadDataByteIndex(WsPayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT;
TBAG_API uint8_t getMaskingKeyByteIndex (WsPayloadBit payload_bit) TBAG_NOEXCEPT;

TBAG_API uint32_t copyMaskingKeyFromBuffer(char const * data) TBAG_NOEXCEPT;

TBAG_API std::string getPayloadData   (uint32_t mask, std::string const & data);
TBAG_API HttpBuffer  getPayloadData   (uint32_t mask, HttpBuffer const & data);
TBAG_API HttpBuffer  getPayloadData   (uint32_t mask, char const * data, std::size_t size);
TBAG_API void        updatePayloadData(uint32_t mask, char * result, std::size_t size);

TBAG_API uint64_t getPayloadLength(char const * total_data);
TBAG_API uint64_t getPayloadLength(char const * data, uint8_t payload_length_7bit, WsPayloadBit payload_bit);

TBAG_API std::size_t calculateBufferSize(uint64_t payload_length, bool is_mask);

/**
 * @def TBAG_WEB_SOCKET_STATUS_CODE_MAP
 *
 * WebSocket Status Codes
 * Reference: https://tools.ietf.org/html/rfc6455#section-7.4
 */
#ifndef TBAG_WEB_SOCKET_STATUS_CODE_MAP
#define TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX) \
    _TBAG_XX(1000, NORMAL_CLOSURE         , "Normal closure"         ) \
    _TBAG_XX(1001, GOING_AWAY             , "Going away"             ) \
    _TBAG_XX(1002, PROTOCOL_ERROR         , "Protocol error"         ) \
    _TBAG_XX(1003, CANNOT_ACCEPT          , "Cannot accept"          ) \
    _TBAG_XX(1004, RESERVED               , "Reserved"               ) \
    _TBAG_XX(1005, NO_STATUS              , "No status"              ) \
    _TBAG_XX(1006, ABNORMAL_CLOSE         , "Abnormal close"         ) \
    _TBAG_XX(1007, INVALID_PAYLOAD        , "Invalid payload"        ) \
    _TBAG_XX(1008, POLICY_VIOLATION       , "Policy violation"       ) \
    _TBAG_XX(1009, MESSAGE_TOO_BIG        , "Message too big"        ) \
    _TBAG_XX(1010, EXTENSION_REQUIRED     , "Extension required"     ) \
    _TBAG_XX(1011, INTERNAL_ENDPOINT_ERROR, "Internal endpoint error") \
    _TBAG_XX(1015, TLS_HANDSHAKE          , "TLS handshake"          ) \
    _TBAG_XX(3000, UNKNOWN_ERROR          , "Unknown error"          ) \
    _TBAG_XX(3001, CLIENT_TIMER_ERROR     , "Client timer error"     ) \
    /* END */
#endif

TBAG_CONSTEXPR uint16_t const TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE = 0;

/**
 * List of WebSocket status code.
 *
 * @author zer0
 * @date 2017-07-07
 * @date 2017-08-08 (Rename: WebSocketStatusCode -> WsStatusCode)
 */
enum class WsStatusCode : uint16_t
{
    WSSC_UNKNOWN = TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE,
#define _TBAG_XX(num, name, str) WSSC_##name = num,
    TBAG_WEB_SOCKET_STATUS_CODE_MAP(_TBAG_XX)
#undef _TBAG_XX
};

TBAG_API uint16_t     getWsStatusCodeNumber(WsStatusCode code) TBAG_NOEXCEPT;
TBAG_API char const * getWsStatusCodeName  (WsStatusCode code) TBAG_NOEXCEPT;
TBAG_API char const * getWsStatusCodeReason(WsStatusCode code) TBAG_NOEXCEPT;
TBAG_API WsStatusCode getWsStatusCode      (uint16_t     code) TBAG_NOEXCEPT;

// WebSocket - Reserved Status Code Ranges
// Reference: https://tools.ietf.org/html/rfc6455#section-7.4.2

/** Status codes in the range 0-999 are not used. */
inline bool isWsStatusCodeNotUsed(uint16_t code) TBAG_NOEXCEPT
{
    return 0 <= COMPARE_AND(code) <= 999;
}

/**
 * Status codes in the range 1000-2999 are reserved for definition by
 * this protocol, its future revisions, and extensions specified in a
 * permanent and readily available public specification.
 */
inline bool isWsStatusCodePublicSpecification(uint16_t code) TBAG_NOEXCEPT
{
    return 1000 <= COMPARE_AND(code) <= 2999;
}

/**
 * Status codes in the range 3000-3999 are reserved for use by
 * libraries, frameworks, and applications. These status codes are
 * registered directly with IANA. The interpretation of these codes
 * is undefined by this protocol.
 */
inline bool isWsStatusCodeIANA(uint16_t code) TBAG_NOEXCEPT
{
    return 3000 <= COMPARE_AND(code) <= 3999;
}

/**
 * Status codes in the range 4000-4999 are reserved for private use
 * and thus can't be registered.  Such codes can be used by prior
 * agreements between WebSocket applications. The interpretation of
 * these codes is undefined by this protocol.
 */
inline bool isWsStatusCodeUserDefined(uint16_t code) TBAG_NOEXCEPT
{
    return 4000 <= COMPARE_AND(code) <= 4999;
}

/**
 * WebSocket close result structure.
 *
 * @author zer0
 * @date   2017-07-07
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
 */
struct TBAG_API WsStatus
{
    uint16_t code = TBAG_UNKNOWN_WEBSOCKET_STATUS_CODE;
    std::string reason;

    WsStatus();
    WsStatus(uint16_t c);
    WsStatus(uint16_t c, std::string const & r);

    explicit WsStatus(WsStatusCode s);
    explicit WsStatus(char const * buffer, std::size_t size);
    explicit WsStatus(HttpBuffer const & payload);

    ~WsStatus();

    void setWsStatusCode(WsStatusCode s);
    WsStatusCode getWsStatusCode() const TBAG_NOEXCEPT;

    Err parse(char const * buffer, std::size_t size);
    Err parse(HttpBuffer const & payload);

    static Err getStatusCode(char const * payload_begin, std::size_t payload_length, uint16_t * result);
    static Err getReason(char const * payload_begin, std::size_t payload_length, std::string * result);

    std::string toString() const;
};

struct HttpParams : public libtbag::mq::details::MqParams
{
    /**
     * Enable TLS feature.
     */
    bool enable_tls = false;

    /**
     * Enable WebSocket feature.
     */
    bool enable_websocket = false;

    /**
     * WebSocket key.
     *
     * If the key is empty, use the random key.
     */
    std::string websocket_key;

    HttpParams()
    { /* EMPTY. */ }

    ~HttpParams()
    { /* EMPTY. */ }

    std::string getWebSocketKey() const
    {
        if (websocket_key.empty()) {
            return generateRandomWebSocketKey();
        } else {
            return websocket_key;
        }
    }
};

struct HttpClientParams : public HttpParams
{
    using OnBegin   = std::function<void(void)>;
    using OnEnd     = std::function<void(void)>;
    using OnHttp    = std::function<void(HttpResponse const &)>;
    using OnSwitch  = std::function<bool(HttpResponse const &)>;
    using OnMessage = std::function<void(WsOpCode, HttpBuffer const &)>;
    using OnError   = std::function<void(Err)>;

    OnBegin   begin_cb;
    OnEnd     end_cb;
    OnHttp    http_cb;
    OnSwitch  switch_cb;
    OnMessage message_cb;
    OnError   error_cb;

    HttpClientParams()
    {
        address = libtbag::net::LOOPBACK_IPV4;
        port = DEFAULT_HTTP_PORT;

        send_queue_size = 2;
        recv_queue_size = 4;

        wait_closing_millisec = 0;
        reconnect_delay_millisec = 0;
        wait_on_activation_timeout_millisec = 0;
    }

    ~HttpClientParams()
    { /* EMPTY. */ }
};

struct HttpServerParams : public HttpParams
{
    using OnBegin    = std::function<void(void)>;
    using OnEnd      = std::function<void(void)>;
    using OnAccept   = std::function<bool(std::intptr_t, std::string const &)>;
    using OnClose    = std::function<void(std::intptr_t)>;
    using OnContinue = std::function<void(std::intptr_t)>;
    using OnSwitch   = std::function<bool(std::intptr_t, HttpRequest const &)>;
    using OnMessage  = std::function<void(std::intptr_t, WsOpCode, HttpBuffer const &)>;
    using OnHttp     = std::function<HttpResponse(std::intptr_t, HttpRequest const &)>;
    using OnError    = std::function<void(std::intptr_t, Err)>;

    OnBegin    begin_cb;
    OnEnd      end_cb;
    OnAccept   accept_cb;
    OnClose    close_cb;
    OnContinue continue_cb;
    OnSwitch   switch_cb;
    OnMessage  message_cb;
    OnHttp     http_cb;
    OnError    error_cb;

    HttpServerParams()
    {
        address = libtbag::net::ANY_IPV4;
        port = DEFAULT_HTTP_PORT;
    }

    ~HttpServerParams()
    { /* EMPTY. */ }
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCOMMON_HPP__

