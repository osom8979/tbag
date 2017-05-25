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

#include <utility>
#include <algorithm>
#include <string>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// --------------
// @formatter:off
// --------------

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
    int http_major;
    int http_minor;

    HttpVersionProperty() TBAG_NOEXCEPT : http_major(1), http_minor(1)
    { /* EMPTY. */ }
    HttpVersionProperty(int v1, int v2) TBAG_NOEXCEPT : http_major(v1), http_minor(v2)
    { /* EMPTY. */ }
    HttpVersionProperty(HttpVersionProperty const & obj) TBAG_NOEXCEPT
    { (*this) = obj; }
    HttpVersionProperty(HttpVersionProperty && obj) TBAG_NOEXCEPT
    { (*this) = std::move(obj); }
    ~HttpVersionProperty()
    { /* EMPTY. */ }

    inline HttpVersionProperty & operator =(HttpVersionProperty const & obj) TBAG_NOEXCEPT
    { if (this != &obj) { http_major = obj.http_major; http_minor = obj.http_minor; } return *this; }

    inline HttpVersionProperty & operator =(HttpVersionProperty && obj) TBAG_NOEXCEPT
    { if (this != &obj) { std::swap(http_major, obj.http_major); std::swap(http_minor, obj.http_minor); } return *this; }

    inline void setVersion(int v1, int v2) TBAG_NOEXCEPT
    { this->http_major = v1; this->http_minor = v2; }

    inline void setMajor(int val) TBAG_NOEXCEPT
    { http_major = val; }
    inline void setMinor(int val) TBAG_NOEXCEPT
    { http_minor = val; }

    inline int getMajor() const TBAG_NOEXCEPT
    { return http_major; }
    inline int getMinor() const TBAG_NOEXCEPT
    { return http_minor; }

    inline friend bool operator ==(HttpVersionProperty & lh, HttpVersionProperty & rh) TBAG_NOEXCEPT
    { return lh.http_major == rh.http_major && lh.http_minor == rh.http_minor; }
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
    HttpCommonProperty(HttpCommonProperty const & obj)
    { (*this) = obj; }
    HttpCommonProperty(HttpCommonProperty && obj)
    { (*this) = std::move(obj); }
    ~HttpCommonProperty()
    { /* EMPTY. */ }

    inline HttpCommonProperty & operator =(HttpCommonProperty const & obj)
    { if (this != &obj) { headers = obj.headers; body = obj.body; } return *this; }

    inline HttpCommonProperty & operator =(HttpCommonProperty && obj)
    { if (this != &obj) { headers.swap(obj.headers); body.swap(obj.body); } return *this; }

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
    HttpRequestProperty(HttpRequestProperty const & obj)
    { (*this) = obj; }
    HttpRequestProperty(HttpRequestProperty && obj)
    { (*this) = std::move(obj); }
    ~HttpRequestProperty()
    { /* EMPTY. */ }

    inline HttpRequestProperty & operator =(HttpRequestProperty const & obj)
    { if (this != &obj) { method = obj.method; url = obj.url; } return *this; }

    inline HttpRequestProperty & operator =(HttpRequestProperty && obj)
    { if (this != &obj) { method.swap(obj.method); url.swap(obj.url); } return *this; }

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
    HttpResponseProperty(HttpResponseProperty const & obj)
    { (*this) = obj; }
    HttpResponseProperty(HttpResponseProperty && obj)
    { (*this) = std::move(obj); }
    ~HttpResponseProperty()
    { /* EMPTY. */ }

    inline HttpResponseProperty & operator =(HttpResponseProperty const & obj)
    { if (this != &obj) { status = obj.status; reason = obj.reason; } return *this; }

    inline HttpResponseProperty & operator =(HttpResponseProperty && obj)
    { if (this != &obj) { std::swap(status, obj.status); reason.swap(obj.reason); } return *this; }

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

struct HttpCommon   : public HttpVersionProperty, public HttpCommonProperty { /* EMPTY. */ };
struct HttpRequest  : public HttpCommon, public HttpRequestProperty { /* EMPTY. */ };
struct HttpResponse : public HttpCommon, public HttpResponseProperty { /* EMPTY. */ };
struct HttpProperty : public HttpCommon, public HttpRequestProperty, public HttpResponseProperty { /* EMPTY. */ };

// -------------
// @formatter:on
// -------------

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

