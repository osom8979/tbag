/**
 * @file   HttpProperty.hpp
 * @brief  HttpProperty class prototype.
 * @author zer0
 * @date   2017-09-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/network/http/HttpCommon.hpp>
#include <libtbag/network/http/HttpMethod.hpp>
#include <libtbag/network/http/HttpStatus.hpp>
#include <libtbag/network/http/HttpHeader.hpp>
#include <libtbag/network/http/ws/WsUtils.hpp>

#include <libtbag/string/StringUtils.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <algorithm>
#include <string>
#include <sstream>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API std::vector<int> getWsVersions(HttpHeader const & header);
TBAG_API bool testWsVersion(HttpHeader const & header, int test_version = WEBSOCKET_VERSION_HYBI13);

/**
 * Http version structure.
 *
 * @author zer0
 * @date   2017-05-20
 * @date   2017-09-29 (Rename: HttpVersionProperty -> HttpVersion)
 *
 * @remarks
 *  Use 'http_' as a prefix to avoid variable name conflicts.
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-3.1>
 */
struct TBAG_API HttpVersion
{
    int http_major;
    int http_minor;

    HttpVersion(int v1 = DEFAULT_HTTP_VERSION_MAJOR, int v2 = DEFAULT_HTTP_VERSION_MINOR) TBAG_NOEXCEPT
            : http_major(v1), http_minor(v2)
    { /* EMPTY. */ }
    ~HttpVersion()
    { /* EMPTY. */ }

    inline void setVersion(int v1, int v2) TBAG_NOEXCEPT
    {
        http_major = v1;
        http_minor = v2;
    }

    inline void setMajor(int val) TBAG_NOEXCEPT { http_major = val; }
    inline void setMinor(int val) TBAG_NOEXCEPT { http_minor = val; }

    inline int getMajor() const TBAG_NOEXCEPT { return http_major; }
    inline int getMinor() const TBAG_NOEXCEPT { return http_minor; }

    inline friend bool operator ==(HttpVersion & lh, HttpVersion & rh) TBAG_NOEXCEPT
    {
        return lh.http_major == rh.http_major && lh.http_minor == rh.http_minor;
    }

    std::string toVersionString() const;

    /**
     * @remarks
     *  @code
     *   HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
     *  @endcode
     */
    static std::string toVersionString(int major, int minor);
};

/**
 * Combine version, header and body.
 *
 * @author zer0
 * @date   2017-10-14
 */
struct TBAG_API HttpCommonProperty : public HttpVersion, public HttpHeader
{
    util::Buffer body;

    void setBody(std::string const & b);
    void setBody(char const * begin, std::size_t size);

    void appendBody(char const * begin, std::size_t size);
    void appendBody(util::Buffer const & buffer);
    void appendBody(std::string const & str);

    std::string getBodyString() const;
    std::string toBodyString() const;

    std::string toDebugHeaderString() const;
};

/**
 * Http request information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-5.1>
 */
struct TBAG_API HttpRequest : public virtual HttpCommonProperty
{
    std::string method;
    std::string path;

    void setHttpMethod(HttpMethod m);
    HttpMethod getHttpMethod() const;

    /**
     * Generally, Called from the server side.
     */
    bool checkWsRequest(int test_version = WEBSOCKET_VERSION_HYBI13) const;

    void updateDefaultRequest();
    void updateDefaultWsRequest(std::string const & ws_key);

    /**
     * @remarks
     *  @code
     *   Request-Line = Method SP Request-URI SP HTTP-Version CRLF
     *  @endcode
     */
    std::string toRequestLine() const;
    std::string toRequestString() const;
    std::string toDebugRequestString() const;
};

/**
 * Http response information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-6.1>
 */
struct TBAG_API HttpResponse : public virtual HttpCommonProperty
{
    int code = 0;
    std::string reason;

    void setHttpStatus(HttpStatus s);
    void setHttpStatus(std::string const & name);
    HttpStatus getHttpStatus() const;

    /**
     * Generally, Called from the client side.
     */
    bool checkWsResponse(std::string const & original_key) const;

    void updateDefaultResponse();
    void updateDefaultWsResponse(std::string const & key);
    void updateDefaultWsResponse(HttpRequest const & request);

    /**
     * @remarks
     *  @code
     *   Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
     *  @endcode
     */
    std::string toStatusLine() const;
    std::string toResponseString() const;
    std::string toDebugResponseString() const;
};

/**
 * HttpProperty class prototype.
 *
 * @author zer0
 * @date   2017-09-30
 */
struct TBAG_API HttpProperty : public HttpRequest, public HttpResponse
{
    void clear();
    void swap(HttpProperty & obj);
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

