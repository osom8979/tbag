/**
 * @file   HttpProperty.hpp
 * @brief  HttpProperty class prototype.
 * @author zer0
 * @date   2017-05-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Uri.hpp>

#include <algorithm>
#include <string>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

TBAG_CONSTEXPR char const * const HTTP = "HTTP";
TBAG_CONSTEXPR char const * const   SP = " ";
TBAG_CONSTEXPR char const * const CRLF = "\r\n";

/**
 * @defgroup __DOXYGEN_GROUP__HTTP_HEADER__ List of http headers.
 * @{
 */
TBAG_CONSTEXPR char const * const HEADER_HOST = "Host";
TBAG_CONSTEXPR char const * const HEADER_SERVER = "Server";
TBAG_CONSTEXPR char const * const HEADER_USER_AGENT = "User-Agent";
TBAG_CONSTEXPR char const * const HEADER_ACCEPT = "Accept";
TBAG_CONSTEXPR char const * const HEADER_TRANSFER_ENCODING = "Transfer-Encoding";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_TYPE = "Content-Type";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_LENGTH = "Content-Length";
/**
 * @}
 */

TBAG_CONSTEXPR char const * const HEADER_DEFAULT_SERVER = LIBTBAG_TITLE_STRING LIBTBAG_VERSION_STRING;
TBAG_CONSTEXPR char const * const HEADER_DEFAULT_CONTENT_TYPE = "text/html; charset=utf-8";
TBAG_CONSTEXPR char const * const HEADER_DEFAULT_USER_AGENT = LIBTBAG_TITLE_STRING LIBTBAG_VERSION_STRING;
TBAG_CONSTEXPR char const * const HEADER_DEFAULT_ACCEPT = "*/*";
TBAG_CONSTEXPR char const * const HEADER_DEFAULT_TRANSFER_ENCODING = "identity";

TBAG_CONSTEXPR int const TBAG_UNKNOWN_HTTP_METHOD = -1;

#ifndef TBAG_HTTP_METHOD_MAP
#define TBAG_HTTP_METHOD_MAP(_TBAG_XX)      \
    _TBAG_XX(0,  DELETE,      DELETE)       \
    _TBAG_XX(1,  GET,         GET)          \
    _TBAG_XX(2,  HEAD,        HEAD)         \
    _TBAG_XX(3,  POST,        POST)         \
    _TBAG_XX(4,  PUT,         PUT)          \
    /* pathological */                      \
    _TBAG_XX(5,  CONNECT,     CONNECT)      \
    _TBAG_XX(6,  OPTIONS,     OPTIONS)      \
    _TBAG_XX(7,  TRACE,       TRACE)        \
    /* WebDAV */                            \
    _TBAG_XX(8,  COPY,        COPY)         \
    _TBAG_XX(9,  LOCK,        LOCK)         \
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

/**
 * List of http method.
 *
 * @author zer0
 * @date 2017-05-25
 */
enum class HttpMethod : int
{
    M_UNKNOWN = -1,
#define _TBAG_XX(num, name, str) M_##name = num,
    TBAG_HTTP_METHOD_MAP(_TBAG_XX)
#undef _TBAG_XX
};

static_assert(static_cast<int>(HttpMethod::M_UNKNOWN) == TBAG_UNKNOWN_HTTP_METHOD,
              "Unknown HTTP Method number is not equals.");

TBAG_API char const * getHttpMethodName(HttpMethod method) TBAG_NOEXCEPT;
TBAG_API int getHttpMethodNumber(HttpMethod method) TBAG_NOEXCEPT;
TBAG_API HttpMethod getHttpMethod(std::string const & name);

/**
 * Http version structure.
 *
 * @author zer0
 * @date   2017-05-20
 */
struct HttpVersionProperty
{
    int maj;
    int min;

    HttpVersionProperty() : maj(1), min(1)
    { /* EMPTY. */ }
    HttpVersionProperty(int v1, int v2) : maj(v1), min(v2)
    { /* EMPTY. */ }

    inline void setVersion(int maj, int min) TBAG_NOEXCEPT
    { this->maj = maj; this->min = min; }

    inline void setMajor(int val) TBAG_NOEXCEPT
    { maj = val; }
    inline void setMinor(int val) TBAG_NOEXCEPT
    { min = val; }

    inline int getMajor() const TBAG_NOEXCEPT
    { return maj; }
    inline int getMinor() const TBAG_NOEXCEPT
    { return min; }

    inline friend bool operator ==(HttpVersionProperty & lh, HttpVersionProperty & rh) TBAG_NOEXCEPT
    { return lh.maj == rh.maj && lh.min == rh.min; }
};

/**
 * Http header & body structure.
 *
 * @author zer0
 * @date   2017-05-20
 */
struct HttpCommonProperty
{
    using HeaderMap  = std::map<std::string, std::string>;
    using HeaderPair = HeaderMap::value_type;

    HeaderMap headers;
    std::string body;

    HttpCommonProperty()
    { /* EMPTY. */ }
    HttpCommonProperty(HeaderMap const & h) : headers(h), body()
    { /* EMPTY. */ }
    HttpCommonProperty(HeaderMap const & h, std::string const & b) : headers(h), body(b)
    { /* EMPTY. */ }

    inline bool insertHeader(std::string const & key, std::string const & val)
    { return headers.insert(HeaderPair(key, val)).second; }

    inline bool eraseHeader(std::string const & key)
    { return headers.erase(key) == 1U; }

    inline bool existsHeader(std::string const & key) const
    { return headers.find(key) != headers.end(); }

    std::string getHeader(std::string const & key) const
    {
        auto itr = headers.find(key);
        if (itr != headers.end()) {
            return itr->second;
        }
        return std::string();
    }

    inline void appendBody(std::string const & content)
    { body.append(content); }
};

/**
 * Http request structure.
 *
 * @author zer0
 * @date   2017-05-20
 */
struct HttpRequestProperty
{
    std::string method;
    std::string url;

    HttpRequestProperty()
    { /* EMPTY. */ }
    HttpRequestProperty(std::string const & m, std::string const & u) : method(m), url(u)
    { /* EMPTY. */ }

    inline Uri getUri() const
    { return Uri(url); }
};

/**
 * Http response structure.
 *
 * @author zer0
 * @date   2017-05-20
 */
struct HttpResponseProperty
{
    int status;
    std::string reason;

    HttpResponseProperty()
    { /* EMPTY. */ }
    HttpResponseProperty(int s) : status(s)
    { /* EMPTY. */ }
    HttpResponseProperty(int s, std::string const & r) : status(s), reason(r)
    { /* EMPTY. */ }

    inline std::string getStatus() const
    { return std::to_string(status); }

    void setStatus(std::string const & str)
    {
        try {
            status = std::stoi(str);
        } catch (...) {
            status = 0;
        }
    }
};

// @formatter:off
struct HttpCommon   : public HttpVersionProperty, public HttpCommonProperty { /* EMPTY. */ };
struct HttpRequest  : public HttpCommon, public HttpRequestProperty { /* EMPTY. */ };
struct HttpResponse : public HttpCommon, public HttpResponseProperty { /* EMPTY. */ };
struct HttpProperty : public HttpCommon, public HttpRequestProperty, public HttpResponseProperty { /* EMPTY. */ };
// @formatter:on

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

