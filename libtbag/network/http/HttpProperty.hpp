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
#include <sstream>
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

// Request
TBAG_CONSTEXPR char const * const HEADER_HOST       = "Host";
TBAG_CONSTEXPR char const * const HEADER_USER_AGENT = "User-Agent";
TBAG_CONSTEXPR char const * const HEADER_ACCEPT     = "Accept";

TBAG_CONSTEXPR char const * const HEADER_DEFAULT_USER_AGENT = LIBTBAG_TITLE_STRING LIBTBAG_VERSION_STRING;
TBAG_CONSTEXPR char const * const HEADER_DEFAULT_ACCEPT     = "*/*";

// Response
TBAG_CONSTEXPR char const * const HEADER_SERVER         = "Server";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_TYPE   = "Content-Type";
TBAG_CONSTEXPR char const * const HEADER_CONTENT_LENGTH = "Content-Length";

TBAG_CONSTEXPR char const * const HEADER_DEFAULT_SERVER       = LIBTBAG_TITLE_STRING LIBTBAG_VERSION_STRING;
TBAG_CONSTEXPR char const * const HEADER_DEFAULT_CONTENT_TYPE = "text/html; charset=utf-8";

struct HttpVersionProperty
{
    int maj;
    int min;

    inline void setVersion(int maj, int min)
    { this->maj = maj; this->min = min; }
};

struct HttpCommonProperty
{
    using HeaderMap  = std::map<std::string, std::string>;
    using HeaderPair = HeaderMap::value_type;

    HeaderMap headers;
    std::string body;

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

struct HttpRequestProperty
{
    std::string method;
    std::string url;
};

struct HttpResponseProperty
{
    int status;
    std::string reason;

    inline std::string getStatus() const
    {
        return std::to_string(status);
    }

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

