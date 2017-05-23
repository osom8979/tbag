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
    inline HttpBuilder & setBody   (std::string const & val) { body    = val;  return *this; }
    inline HttpBuilder & setMethod (std::string const & val) { method  = val;  return *this; }
    inline HttpBuilder & setUrl    (std::string const & val) { url     = val;  return *this; }
    inline HttpBuilder & setReason (std::string const & val) { reason  = val;  return *this; }
    // @formatter:on

    inline HttpBuilder & setStatus(int val)
    {
        status = val;
        return *this;
    }

    inline HttpBuilder & setHeader(std::string const & key, std::string const & val)
    {
        insertHeader(key, val);
        return *this;
    }

public:
    std::string toRequestString() const;
    std::string toResponseString() const;

    std::string toRequestDefaultString() const;
    std::string toResponseDefaultString() const;

public:
    static void existsOrInsert(HeaderMap & headers, std::string const & key, std::string const & val);
    static std::string getVersionString(int major = 1, int minor = 1);
    static std::string getRequestString(std::string const & method, std::string const & url,
                                        HeaderMap const & headers, std::string const & body,
                                        int major = 1, int minor = 1);
    static std::string getResponseString(std::string const & status, std::string const & reason,
                                         HeaderMap const & headers, std::string const & body,
                                         int major = 1, int minor = 1);
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPBUILDER_HPP__

