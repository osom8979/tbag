/**
 * @file   HttpBuilder.hpp
 * @brief  HttpBuilder class prototype.
 * @author zer0
 * @date   2017-05-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPBUILDER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPBUILDER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/http/HttpProperty.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpBuilder class prototype.
 *
 * @author zer0
 * @date   2017-05-19
 */
class TBAG_API HttpBuilder
{
private:
    HttpProperty _property;

public:
    HttpBuilder();
    HttpBuilder(int maj, int min);
    HttpBuilder(HttpBuilder const & obj);
    HttpBuilder(HttpBuilder && obj);
    ~HttpBuilder();

public:
    HttpBuilder(HttpVersionProperty  const & obj);
    HttpBuilder(HttpCommonProperty   const & obj);
    HttpBuilder(HttpRequestProperty  const & obj);
    HttpBuilder(HttpResponseProperty const & obj);
    HttpBuilder(HttpRequest          const & obj);
    HttpBuilder(HttpResponse         const & obj);
    HttpBuilder(HttpProperty         const & obj);

public:
    HttpBuilder & operator =(HttpBuilder const & obj);
    HttpBuilder & operator =(HttpBuilder && obj);

public:
    void swap(HttpBuilder & obj);
    void clear();

public:
    inline HttpProperty       & atProperty()       TBAG_NOEXCEPT { return _property; }
    inline HttpProperty const & atProperty() const TBAG_NOEXCEPT { return _property; }

public:
    HttpVersionProperty  getVersionProperty () const;
    HttpCommonProperty   getCommonProperty  () const;
    HttpRequestProperty  getRequestProperty () const;
    HttpResponseProperty getResponseProperty() const;
    HttpRequest          getRequest         () const;
    HttpResponse         getResponse        () const;
    HttpProperty         getProperty        () const;

    inline operator HttpVersionProperty () const { return getVersionProperty (); }
    inline operator HttpCommonProperty  () const { return getCommonProperty  (); }
    inline operator HttpRequestProperty () const { return getRequestProperty (); }
    inline operator HttpResponseProperty() const { return getResponseProperty(); }
    inline operator HttpRequest         () const { return getRequest         (); }
    inline operator HttpResponse        () const { return getResponse        (); }
    inline operator HttpProperty        () const { return getProperty        (); }

    HttpBuilder & operator =(HttpVersionProperty  const & obj);
    HttpBuilder & operator =(HttpCommonProperty   const & obj);
    HttpBuilder & operator =(HttpRequestProperty  const & obj);
    HttpBuilder & operator =(HttpResponseProperty const & obj);
    HttpBuilder & operator =(HttpRequest          const & obj);
    HttpBuilder & operator =(HttpResponse         const & obj);
    HttpBuilder & operator =(HttpProperty         const & obj);

public:
    inline int getMajor() const { return _property.getMajor(); }
    inline int getMinor() const { return _property.getMinor(); }

    inline bool existsHeader(std::string const & key) const
    { return _property.existsHeader(key); }
    inline bool existsHeaderValue(std::string const & key, std::string const & value, bool ignore_case = true) const
    { return _property.existsHeaderValue(key, value, ignore_case); }

    inline std::string getHeader(std::string const & key) const { return _property.getHeader(key); }
    inline std::string getBody() const { return std::string(_property.body.begin(), _property.body.end()); }

    inline std::string getMethod() const { return _property.method; }
    inline std::string getUrl() const { return _property.url; }

    inline int getStatus() const { return _property.status; }
    inline std::string getReason() const { return _property.reason; }

public:
    inline HttpBuilder & setVersion(int maj, int min) TBAG_NOEXCEPT
    { _property.http_major = maj; _property.http_minor = min; return *this; }
    inline HttpBuilder & setMajor(int val) TBAG_NOEXCEPT
    { _property.setMajor(val); return *this; }
    inline HttpBuilder & setMinor(int val) TBAG_NOEXCEPT
    { _property.setMinor(val); return *this; }

public:
    inline HttpBuilder & setBody(std::string const & val)
    { _property.body.assign(val.begin(), val.end()); return *this; }
    inline HttpBuilder & appendBody(char const * buffer, std::size_t size)
    { _property.appendBody(buffer, size); return *this; }
    inline HttpBuilder & appendBody(std::string const & val)
    { _property.appendBody(val); return *this; }

public:
    inline HttpBuilder & insertHeader(std::string const & key, std::string const & val)
    { _property.insertHeader(key, val); return *this; }
    inline HttpBuilder & insertIfNotExists(std::string const & key, std::string const & val)
    { _property.insertIfNotExists(key, val); return *this; }

public:
    inline HttpBuilder & setMethod(std::string const & val)
    { _property.method = val; return *this; }

    inline HttpBuilder & setMethod(HttpMethod val)
    { _property.setMethod(val); return *this; }

    inline HttpBuilder & setUrl(std::string const & val)
    { _property.url = val;  return *this; }

    inline HttpBuilder & setReason(std::string const & val)
    { _property.reason = val; return *this; }

    inline HttpBuilder & setStatus(int val)
    { _property.status = val; return *this; }

    inline HttpBuilder & setStatus(std::string const & val)
    { _property.setStatus(val); return *this; }

    inline HttpBuilder & setStatus(HttpStatus val)
    { _property.setStatus(val); return *this; }

public:
    std::string buildRequestString() const;
    std::string buildResponseString() const;

    std::string buildDefaultRequestString() const;
    std::string buildDefaultResponseString() const;

public:
    std::string toRequestDebugString() const;
    std::string toResponseDebugString() const;
};

// ----------------------
// Build default methods.
// ----------------------

TBAG_API std::string buildDefaultRequestString(HttpProperty const & req);
TBAG_API std::string buildDefaultResponseString(HttpProperty const & rsp);

TBAG_API std::string buildDefaultRequestString(HttpRequest const & req);
TBAG_API std::string buildDefaultResponseString(HttpResponse const & rsp);

TBAG_API std::string buildDefaultRequestString(
        std::string const & method, std::string const & url,
        HttpHeaderMap const & headers, HttpBodyBuffer const & body,
        int major = 1, int minor = 1, bool logging = false);

TBAG_API std::string buildDefaultResponseString(
        std::string const & status, std::string const & reason,
        HttpHeaderMap const & headers, HttpBodyBuffer const & body,
        int major = 1, int minor = 1, bool logging = false);

// --------------
// Build methods.
// --------------

TBAG_API std::string buildRequestString(HttpProperty const & req);
TBAG_API std::string buildResponseString(HttpProperty const & rsp);

TBAG_API std::string buildRequestString(HttpRequest const & req);
TBAG_API std::string buildResponseString(HttpResponse const & rsp);

TBAG_API std::string buildRequestString(
        std::string const & method, std::string const & url,
        HttpHeaderMap const & headers, HttpBodyBuffer const & body,
        int major = 1, int minor = 1);

TBAG_API std::string buildResponseString(
        std::string const & status, std::string const & reason,
        HttpHeaderMap const & headers, HttpBodyBuffer const & body,
        int major = 1, int minor = 1);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPBUILDER_HPP__

