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
    using BaseChar     = char;
    using String       = std::basic_string<BaseChar>;
    using StringStream = std::basic_stringstream<BaseChar>;

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

    String toString() const
    {
        StringStream ss;
        ss << HTTP << '/' << major_number << '.' << minor_number;
        return ss.str();
    }
};

struct HttpCommon
{
    using Size         = std::size_t;
    using BaseChar     = char;
    using String       = std::basic_string<BaseChar>;
    using StringStream = std::basic_stringstream<BaseChar>;

    using HeaderMap = std::map<String, String>;
    using HeaderPair = HeaderMap::value_type;

    HttpVersion version;
    HeaderMap   headers;
    String      body;

    HttpCommon() { /* EMPTY. */ }
    HttpCommon(int maj, int min) : version(maj, min) { /* EMPTY. */ }

    inline Size getHeadersSize() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(headers.size()))
    { return headers.size(); }
    inline bool isHeadersEmpty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(headers.empty()))
    { return headers.empty(); }

    inline bool insertHeader(String const & key, String const & val)
    { return headers.insert(HeaderPair(key, val)).second; }

    inline bool eraseHeader(String const & key)
    { return headers.erase(key) == 1U; }

    inline bool existsHeader(String const & key) const
    { return headers.find(key) != headers.end(); }

    String getHeader(String const & key) const
    {
        auto itr = headers.find(key);
        if (itr != headers.end()) {
            return itr->second;
        }
        return String();
    }

    inline void appendBody(String const & content)
    { body.append(content); }
};

struct HttpRequest : public virtual HttpCommon
{
    String method;
    String url;
};

struct HttpResponse : public virtual HttpCommon
{
    int    status;
    String reason;

    inline String getStatus() const
    {
        return std::to_string(status);
    }

    inline void setStatus(String const & str)
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
