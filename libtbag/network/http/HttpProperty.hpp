/**
 * @file   HttpProperty.hpp
 * @brief  HttpProperty class prototype.
 * @author zer0
 * @date   2017-05-20
 *
 * @see HTTP: <https://tools.ietf.org/html/rfc2616>
 * @see WebSocket v13: <https://tools.ietf.org/html/rfc6455>
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
#include <libtbag/string/StringUtils.hpp>

#include <libtbag/network/http/common/HttpMethod.hpp>
#include <libtbag/network/http/common/HttpStatus.hpp>
#include <libtbag/network/http/common/HttpVersion.hpp>

#include <utility>
#include <algorithm>
#include <string>
#include <map>
#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

using namespace libtbag::network::http::common;
using HttpVersionProperty = HttpVersion;

// --------------
// @formatter:off
// --------------

TBAG_CONSTEXPR char const * const HTTP = "HTTP";
TBAG_CONSTEXPR char const * const   SP = " ";
TBAG_CONSTEXPR char const * const CRLF = "\r\n";
TBAG_CONSTEXPR char const * const   CR = "\r";
TBAG_CONSTEXPR char const * const   LF = "\n";

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

TBAG_CONSTEXPR char const * const VALUE_DELIMITER        = ",";
TBAG_CONSTEXPR char const * const VALUE_WEBSOCKET        = "WebSocket";
TBAG_CONSTEXPR char const * const VALUE_UPGRADE          = "Upgrade";
TBAG_CONSTEXPR char const * const VALUE_TBAG_PROTOCOL    = "Tbag";
TBAG_CONSTEXPR char const * const VALUE_TBAG_SERVER_INFO = LIBTBAG_TITLE_STRING "/" LIBTBAG_VERSION_STRING;

TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI00 =  0; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI07 =  7; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI08 =  8; ///< Interim
TBAG_CONSTEXPR int const WEBSOCKET_VERSION_HYBI13 = 13; ///< RFC 6455 (Standard)

TBAG_CONSTEXPR char const * const WEBSOCKET_HANDSHAKE_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_SERVER            = VALUE_TBAG_SERVER_INFO;
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_CONTENT_TYPE      = "text/html; charset=utf-8";
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_USER_AGENT        = VALUE_TBAG_SERVER_INFO;
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_ACCEPT            = "*/*";
TBAG_CONSTEXPR char const * const DEFAULT_VALUE_OF_TRANSFER_ENCODING = "identity";

using HttpHeaderMap  = std::multimap<std::string, std::string>;
using HttpHeaderPair = HttpHeaderMap::value_type;
using HttpBodyBuffer = std::vector<char>;

/**
 * Http header & body structure.
 *
 * @author zer0
 * @date   2017-05-20
 */
struct TBAG_API HttpCommonProperty
{
    using BodyBuffer = std::vector<char>;

    HttpHeaderMap headers;
    HttpBodyBuffer body;

    HttpCommonProperty()
    { /* EMPTY. */ }
    HttpCommonProperty(HttpHeaderMap const & h) : headers(h), body()
    { /* EMPTY. */ }
    explicit HttpCommonProperty(HttpHeaderMap const & h, std::string const & b) : headers(h), body(b.begin(), b.end())
    { /* EMPTY. */ }
    explicit HttpCommonProperty(HttpHeaderMap const & h, HttpBodyBuffer const & b) : headers(h), body(b)
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

    inline void insertHeader(std::string const & key, std::string const & val)
    { headers.insert(HttpHeaderPair(key, val)); }

    inline bool eraseHeader(std::string const & key)
    { return headers.erase(key) == 1U; }

    inline bool existsHeader(std::string const & key) const
    { return headers.find(key) != headers.end(); }

    inline void appendBody(char const * buffer, std::size_t size)
    { body.insert(body.end(), buffer, buffer + size); }
    inline void appendBody(std::string const & content)
    { body.insert(body.end(), content.begin(), content.end()); }

    void setBody(std::string const & val);
    std::string getBody() const;
    std::string getHeader(std::string const & key) const;

    bool existsHeaderValue(std::string const & key, std::string const & value, bool ignore_case = true) const;
    void insertIfNotExists(std::string const & key, std::string const & val);

    static bool existsHeaderValueFromHeaderMap(
            HttpHeaderMap const & headers,
            std::string const & key,
            std::string const & value,
            bool ignore_case = true);
    static void insertIfNotExists(HttpHeaderMap & headers, std::string const & key, std::string const & val);
};

/**
 * Http request structure.
 *
 * @author zer0
 * @date   2017-05-20
 */
struct TBAG_API HttpRequestProperty
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

    Uri getUri() const;
    void setMethod(HttpMethod m);
};

/**
 * Http response structure.
 *
 * @author zer0
 * @date   2017-05-20
 */
struct TBAG_API HttpResponseProperty
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

    void setStatus(HttpStatus s);
    void setStatus(std::string const & str);
};

struct HttpCommon   : public HttpVersionProperty, public HttpCommonProperty { /* EMPTY. */ };
struct HttpRequest  : public HttpCommon, public HttpRequestProperty { /* EMPTY. */ };
struct HttpResponse : public HttpCommon, public HttpResponseProperty { /* EMPTY. */ };
struct HttpProperty : public HttpCommon, public HttpRequestProperty, public HttpResponseProperty { /* EMPTY. */ };

// ------------------
// Debugging methods.
// ------------------

TBAG_API std::string toDebugString(HttpHeaderMap const & obj);
TBAG_API std::string toDebugString(HttpCommonProperty const & obj);
TBAG_API std::string toDebugString(HttpCommon const & obj);
TBAG_API std::string toDebugString(HttpRequest const & obj);
TBAG_API std::string toDebugString(HttpResponse const & obj);

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API std::string convertRegularString(std::string const & source);
TBAG_API std::set<std::string> convertRegularSet(std::vector<std::string> const & sources);

TBAG_API std::vector<std::string> findAccept(std::vector<std::string> const & sources,
                                             std::vector<std::string> const & accepts);
TBAG_API std::vector<std::string> findAccept(std::string const & sources,
                                             std::vector<std::string> const & accepts);

TBAG_API std::vector<std::string> fromDelimiterString(std::string const & sources,
                                                      std::string const & delimiter = VALUE_DELIMITER);
TBAG_API std::string toDelimiterString(std::vector<std::string> const & sources);

// -------------
// @formatter:on
// -------------

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

