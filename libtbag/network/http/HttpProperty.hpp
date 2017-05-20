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

struct HttpVersion
{
    using BaseChar     = char;
    using String       = std::basic_string<BaseChar>;
    using StringStream = std::basic_stringstream<BaseChar>;

    int major;
    int minor;

    HttpVersion() : major(1), minor(1)
    { /* EMPTY. */ }
    HttpVersion(int maj, int min) : major(maj), minor(min)
    { /* EMPTY. */ }

    inline HttpVersion & operator =(HttpVersion const & obj)
    {
        if (this != &obj) {
            major = obj.major;
            minor = obj.minor;
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

    inline void set(int maj, int min) TBAG_NOEXCEPT
    {
        major = maj;
        minor = min;
    }

    inline void clear() TBAG_NOEXCEPT
    {
        major = 0;
        minor = 0;
    }

    inline void swap(HttpVersion & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(major, obj.minor);
            std::swap(major, obj.minor);
        }
    }

    String toString() const
    {
        StringStream ss;
        ss << HTTP << '/' << major << '.' << minor;
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
    HttpCommon(int major, int minor) : version(major, minor) { /* EMPTY. */ }

    inline Size getHeadersSize() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(headers.size()))
    { return headers.size(); }
    inline bool isHeadersEmpty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(headers.empty()))
    { return headers.empty(); }

    inline bool insertHeader(String const & key, String const & val)
    { return headers.insert(HeaderPair(key, val)).second; }

    inline bool eraseHeader(String const & key)
    { return headers.erase(key) == 1U; }

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

