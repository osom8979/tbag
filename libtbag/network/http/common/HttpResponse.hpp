/**
 * @file   HttpResponse.hpp
 * @brief  HttpResponse class prototype.
 * @author zer0
 * @date   2017-09-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPRESPONSE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPRESPONSE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/http/common/HttpCommon.hpp>
#include <libtbag/network/http/common/HttpVersion.hpp>

#include <algorithm>
#include <string>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

/**
 * Http response information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-6.1>
 */
struct HttpResponse
{
    HttpVersion version;
    int code;
    std::string reason;

    HttpResponse() : version(), code(0), reason()
    { /* EMPTY. */ }
    ~HttpResponse()
    { /* EMPTY. */ }

    void setHttpStatus(HttpStatus s)
    {
        code   = common::getHttpStatusNumber(s);
        reason = common::getHttpStatusReason(s);
    }

    void setHttpStatus(std::string const & name)
    {
        setHttpStatus(common::getHttpStatus(name));
    }

    HttpStatus getHttpStatus() const
    {
        return common::getHttpStatus(code);
    }

    void swap(HttpResponse & obj)
    {
        version.swap(obj.version);
        std::swap(code, obj.code);
        reason.swap(obj.reason);
    }

    void clear()
    {
        version.clear();
        code = 0;
        reason.clear();
    }

    /**
     * @remarks
     *  @code
     *   Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
     *  @endcode
     */
    std::string toString() const
    {
        std::stringstream ss;
        ss << version.toString() << SP
           << code << SP
           << (reason.empty() ? getErrName(Err::E_EINIT) : reason);
        return ss.str();
    }
};

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPRESPONSE_HPP__

