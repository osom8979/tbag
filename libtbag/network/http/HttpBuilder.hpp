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
class TBAG_API HttpBuilder : public HttpProperty
{
public:
    HttpBuilder();
    HttpBuilder(int maj, int min);
    HttpBuilder(HttpBuilder const & obj);
    HttpBuilder(HttpBuilder && obj);
    ~HttpBuilder();

public:
    HttpBuilder & operator =(HttpBuilder const & obj);
    HttpBuilder & operator =(HttpBuilder && obj);

public:
    void swap(HttpBuilder & obj);
    void clear();

public:
    // @formatter:off
    inline HttpBuilder & setMajor(int val) { version.major = val; return *this; }
    inline HttpBuilder & setMinor(int val) { version.minor = val; return *this; }
    inline HttpBuilder & setVersion(int maj, int min) { version.set(maj, min); return *this; }

    inline HttpBuilder & setHeaders(HeaderMap const & val) { headers = val;  return *this; }
    inline HttpBuilder & setBody   (String    const & val) { body    = val;  return *this; }
    inline HttpBuilder & setMethod (String    const & val) { method  = val;  return *this; }
    inline HttpBuilder & setUrl    (String    const & val) { url     = val;  return *this; }
    inline HttpBuilder & setReason (String    const & val) { reason  = val;  return *this; }
    // @formatter:on

    inline HttpBuilder & setStatus (int val)
    {
        status = val;
        return *this;
    }

    inline HttpBuilder & setHeader(String const & key, String const & val)
    {
        insertHeader(key, val);
        return *this;
    }

public:
    String request() const;
    String response() const;

public:
    static String buildRequest(String const & method, String const & url,
                               HeaderMap const & headers, String const & body,
                               HttpVersion const & version);
    static String buildResponse(String const & status, String const & reason,
                                HeaderMap const & headers, String const & body,
                                HttpVersion const & version);
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPBUILDER_HPP__

