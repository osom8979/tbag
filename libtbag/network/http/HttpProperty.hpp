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

struct HttpVersion
{
    int major_number;
    int minor_number;

    HttpVersion() : major_number(1), minor_number(1)
    { /* EMPTY. */ }
    HttpVersion(int maj, int min) : major_number(maj), minor_number(min)
    { /* EMPTY. */ }

    inline HttpVersion & operator =(HttpVersion const & obj)
    {
        if (this != &obj) {
            major_number = obj.major_number;
            minor_number = obj.minor_number;
        }
        return *this;
    }

    inline HttpVersion & operator =(HttpVersion && obj)
    {
        if (this != &obj) {
            swap(obj);
        }
        return *this;
    }

    inline friend bool operator ==(HttpVersion const & lh, HttpVersion const & rh) TBAG_NOEXCEPT
    {
        return lh.major_number == rh.major_number && lh.minor_number == rh.minor_number;
    }

    inline void set(int maj, int min) TBAG_NOEXCEPT
    {
        major_number = maj;
        minor_number = min;
    }

    inline void clear() TBAG_NOEXCEPT
    {
        major_number = 0;
        minor_number = 0;
    }

    inline void swap(HttpVersion & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(major_number, obj.minor_number);
            std::swap(major_number, obj.minor_number);
        }
    }

    std::string toString() const
    {
        std::stringstream ss;
        ss << HTTP << '/' << major_number << '.' << minor_number;
        return ss.str();
    }
};

struct HttpCommon
{
    using HeaderMap = std::map<std::string, std::string>;
    using HeaderPair = HeaderMap::value_type;

    HttpVersion version;
    HeaderMap   headers;
    std::string body;

    HttpCommon() { /* EMPTY. */ }
    HttpCommon(int maj, int min) : version(maj, min) { /* EMPTY. */ }

    inline std::size_t getHeaderSize() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(headers.size()))
    { return headers.size(); }
    inline bool isHeadersEmpty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(headers.empty()))
    { return headers.empty(); }

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

struct HttpRequest : public virtual HttpCommon
{
    std::string method;
    std::string url;
};

struct HttpResponse : public virtual HttpCommon
{
    int    status;
    std::string reason;

    inline std::string getStatus() const
    {
        return std::to_string(status);
    }

    inline void setStatus(std::string const & str)
    {
        try {
            status = std::stoi(str);
        } catch (...) {
            status = 0;
        }
    }
};

struct HttpProperty : public HttpRequest, public HttpResponse
{
    // EMPTY.
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

