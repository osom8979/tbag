/**
 * @file   HttpRequest.hpp
 * @brief  HttpRequest class prototype.
 * @author zer0
 * @date   2017-09-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPREQUEST_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPREQUEST_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/http/common/HttpCommon.hpp>
#include <libtbag/network/http/common/HttpVersion.hpp>

#include <string>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

/**
 * Http request information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-5.1>
 */
struct HttpRequest
{
    std::string method;
    std::string uri;
    HttpVersion version;

    HttpRequest() : method(), uri(), version()
    { /* EMPTY. */ }
    ~HttpRequest()
    { /* EMPTY. */ }

    void setHttpMethod(HttpMethod m)
    {
        method = common::getHttpMethodName(m);
    }

    HttpMethod getHttpMethod() const
    {
        return common::getHttpMethod(method.c_str());
    }

    void swap(HttpRequest & obj)
    {
        method.swap(obj.method);
        uri.swap(obj.uri);
        version.swap(obj.version);
    }

    /**
     * @remarks
     *  @code
     *   Request-Line = Method SP Request-URI SP HTTP-Version CRLF
     *  @endcode
     */
    std::string toString() const
    {
        std::stringstream ss;
        ss << (method.empty() ? HttpMethodGET::getMethod() : method) << SP
           << (uri.empty() ? "/" : uri) << SP
           << version.toString();
        return ss.str();
    }
};

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPREQUEST_HPP__

