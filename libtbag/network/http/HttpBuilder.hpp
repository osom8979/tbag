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

/**
 * HttpBuilder class prototype.
 *
 * @author zer0
 * @date   2017-05-19
 */
struct TBAG_API HttpBuilder
{
    using String = std::string;
    using Size   = std::size_t;

    using HeaderMap = std::map<String, String>;
    using HeaderPair = HeaderMap::value_type;

    int major;
    int minor;

    HeaderMap headers;
    String    body;

    String method; ///< Request only.
    String    uri; ///< Request only.

    String status; ///< Response only.
    String reason; ///< Response only.

    HttpBuilder();
    HttpBuilder(int http_major, int http_minor);
    ~HttpBuilder();

    HttpBuilder & clear();

    // @formatter:off
    inline HttpBuilder & setMajor  (int val) { major = val; return *this; }
    inline HttpBuilder & setMinor  (int val) { minor = val; return *this; }
    inline HttpBuilder & setHeaders(HeaderMap const & val) { headers = val; return *this; }
    inline HttpBuilder & setBody   (String    const & val) { body    = val; return *this; }
    inline HttpBuilder & setMethod (String    const & val) { method  = val; return *this; }
    inline HttpBuilder & setUri    (String    const & val) { uri     = val; return *this; }
    inline HttpBuilder & setStatus (String    const & val) { status  = val; return *this; }
    inline HttpBuilder & setReason (String    const & val) { reason  = val; return *this; }
    // @formatter:on

    inline HttpBuilder & insertHeader(String const & key, String const & val)
    {
        headers.insert(HeaderPair(key, val));
        return *this;
    }

    inline HttpBuilder & eraseHeader(String const & key)
    {
        headers.erase(key);
        return *this;
    }

// Alias methods.
public:
    inline HttpBuilder & setVersion(int maj, int min)
    { major = maj; minor = min; return *this; }

    inline HttpBuilder & setStatus(int val)
    { return setStatus(std::to_string(val)); }

    inline HttpBuilder & setHeader(String const & key, String const & val)
    { return insertHeader(key, val); }

public:
    String buildRequest();
    String buildResponse();

public:
    static String buildVersionString(int major = 1, int minor = 1);
    static String buildRequest(String const & method,
                               String const & uri,
                               HeaderMap const & headers,
                               String const & body,
                               int major = 1, int minor = 1);
    static String buildResponse(String const & status,
                                String const & reason,
                                HeaderMap const & headers,
                                String const & body,
                                int major = 1, int minor = 1);
};


} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPBUILDER_HPP__

