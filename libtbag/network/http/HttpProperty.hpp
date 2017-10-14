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

#include <libtbag/network/http/common/HttpCommon.hpp>
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
TBAG_API std::string toDebugString(http::HttpCommonProperty const & obj);
TBAG_API std::string toDebugString(http::HttpCommon const & obj);
TBAG_API std::string toDebugString(http::HttpRequest const & obj);
TBAG_API std::string toDebugString(http::HttpResponse const & obj);

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API std::string convertRegularString(std::string const & source);
TBAG_API std::set<std::string> convertRegularSet(std::vector<std::string> const & sources);

TBAG_API std::vector<std::string> findAccept(std::vector<std::string> const & sources,
                                             std::vector<std::string> const & accepts);
TBAG_API std::vector<std::string> findAccept(std::string const & sources,
                                             std::vector<std::string> const & accepts);

TBAG_API std::vector<std::string> fromDelimiterString(std::string const & sources, std::string const & delimiter = DEFAULT_VALUE_DELIMITER);
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

