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

TBAG_CONSTEXPR char const * const METHOD_DELETE = "DELETE";
TBAG_CONSTEXPR char const * const METHOD_GET    = "GET";
TBAG_CONSTEXPR char const * const METHOD_HEAD   = "HEAD";
TBAG_CONSTEXPR char const * const METHOD_POST   = "POST";
TBAG_CONSTEXPR char const * const METHOD_PUT    = "PUT";

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

