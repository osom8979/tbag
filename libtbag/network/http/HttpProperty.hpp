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
struct HttpVersion
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

    std::string toVersionString() const
    {
        return toVersionString(http_major, http_minor);
    }

    /**
     * @remarks
     *  @code
     *   HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
     *  @endcode
     */
    static std::string toVersionString(int major, int minor)
    {
        return string::fformat("HTTP/{}.{}", major, minor);
    }
};

/**
 * Combine version, header and body.
 *
 * @author zer0
 * @date   2017-10-14
 */
struct HttpCommonProperty : public HttpVersion, public HttpHeader
{
    util::Buffer body;

    void setBody(std::string const & b)
    {
        body.assign(b.begin(), b.end());
    }

    void setBody(char const * begin, std::size_t size)
    {
        body.assign(begin, begin + size);
    }

    void appendBody(char const * begin, std::size_t size)
    {
        body.insert(body.end(), begin, begin + size);
    }

    std::string getBodyString() const
    {
        return std::string(body.begin(), body.end());
    }

    std::string toBodyString() const
    {
        return std::string(body.begin(), body.end());
    }

    std::string toDebugHeaderString() const
    {
        std::stringstream ss;
        std::size_t const SIZE = sizeOfHeaders();
        auto itr = begin();
        for (std::size_t i = 0; i < SIZE; ++i, ++itr) {
            ss << "[H] " << itr->first << ": " << itr->second;
            if (i + 1 < SIZE) {
                ss << std::endl;
            }
        }
        return ss.str();
    }
};

/**
 * Http request information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-5.1>
 */
struct HttpRequest : public virtual HttpCommonProperty
{
    std::string method;
    std::string path;

    void setHttpMethod(HttpMethod m)
    {
        method = http::getHttpMethodName(m);
    }

    HttpMethod getHttpMethod() const
    {
        return http::getHttpMethod(method.c_str());
    }

    /**
     * Generally, Called from the server side.
     */
    bool checkWsRequest(int test_version = WEBSOCKET_VERSION_HYBI13) const
    {
        if (getHttpMethod() != HttpMethod::M_GET) {
            return false;
        }
        if (exists(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
            return false;
        }
        if (exists(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
            return false;
        }
        if (testWsVersion(*this, test_version) == false) {
            return false;
        }
        return true;
    }

    void updateDefaultRequest()
    {
        insertIfNotExists(HEADER_USER_AGENT, DEFAULT_VALUE_OF_USER_AGENT);
        insertIfNotExists(HEADER_ACCEPT, DEFAULT_VALUE_OF_ACCEPT);
        if (body.empty() == false) {
            insertIfNotExists(HEADER_CONTENT_LENGTH, std::to_string(body.size()));
        }

        // @formatter:off
        if (method.empty()) { method = http::getHttpMethodName(HttpMethod::M_GET); }
        if (path.empty()) { path = "/"; }
        if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
        if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
        // @formatter:on
    }

    void updateDefaultWsRequest(std::string const & ws_key)
    {
        insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
        insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
        insertIfNotExists(HEADER_SEC_WEBSOCKET_VERSION, std::to_string(WEBSOCKET_VERSION_HYBI13));
        insertIfNotExists(HEADER_SEC_WEBSOCKET_KEY, ws_key);

        // @formatter:off
        if (method.empty()) { method = http::getHttpMethodName(HttpMethod::M_GET); }
        if (path.empty()) { path = "/"; }
        if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
        if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
        // @formatter:on
    }

    /**
     * @remarks
     *  @code
     *   Request-Line = Method SP Request-URI SP HTTP-Version CRLF
     *  @endcode
     */
    std::string toRequestLine() const
    {
        std::stringstream ss;
        ss << (method.empty() ? HttpMethodGET::getMethod() : method) << SP
           << (path.empty() ? "/" : path) << SP
           << toVersionString();
        return ss.str();
    }

    std::string toRequestString() const
    {
        std::stringstream ss;
        ss << toRequestLine() << CRLF
           << toMessageHeader() /* Include CRLF */ << CRLF
           << toBodyString();
        return ss.str();
    }

    std::string toDebugRequestString() const
    {
        std::stringstream ss;
        ss << "[REQ] " << toRequestLine() << std::endl
           << toDebugHeaderString();
        return ss.str();
    }
};

/**
 * Http response information.
 *
 * @author zer0
 * @date   2017-09-30
 *
 * @see <https://tools.ietf.org/html/rfc2616#section-6.1>
 */
struct HttpResponse : public virtual HttpCommonProperty
{
    int code = 0;
    std::string reason;

    void setHttpStatus(HttpStatus s)
    {
        code   = http::getHttpStatusNumber(s);
        reason = http::getHttpStatusReason(s);
    }

    void setHttpStatus(std::string const & name)
    {
        setHttpStatus(http::getHttpStatus(name));
    }

    HttpStatus getHttpStatus() const
    {
        return http::getHttpStatus(code);
    }

    /**
     * Generally, Called from the client side.
     */
    bool checkWsResponse(std::string const & original_key) const
    {
        if (code != getHttpStatusNumber(HttpStatus::SC_SWITCHING_PROTOCOLS)) {
            return false;
        }
        if (exists(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
            return false;
        }
        if (exists(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
            return false;
        }
        if (exists(HEADER_SEC_WEBSOCKET_ACCEPT) == false) {
            return false;
        }
        std::string const ACCEPT_KEY = get(HEADER_SEC_WEBSOCKET_ACCEPT);
        if (ACCEPT_KEY != ws::getUpgradeWebSocketKey(original_key)) {
            return false;
        }
        return true;
    }

    void updateDefaultResponse()
    {
        insertIfNotExists(HEADER_SERVER, DEFAULT_VALUE_OF_SERVER);
        insertIfNotExists(HEADER_CONTENT_TYPE, DEFAULT_VALUE_OF_CONTENT_TYPE);
        if (body.empty() == false) {
            insertIfNotExists(HEADER_CONTENT_LENGTH, std::to_string(body.size()));
        }

        // @formatter:off
        if (code == 0) { setHttpStatus(HttpStatus::SC_OK); }
        if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
        if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
        // @formatter:on
    }

    void updateDefaultWsResponse(std::string const & key)
    {
        insertIfNotExists(HEADER_UPGRADE, VALUE_WEBSOCKET);
        insertIfNotExists(HEADER_CONNECTION, VALUE_UPGRADE);
        insertIfNotExists(HEADER_SEC_WEBSOCKET_ACCEPT, ws::getUpgradeWebSocketKey(key));
        setHttpStatus(HttpStatus::SC_SWITCHING_PROTOCOLS); // Force update!

        // @formatter:off
        if (http_major == 0) { http_major = DEFAULT_HTTP_VERSION_MAJOR; }
        if (http_minor == 0) { http_minor = DEFAULT_HTTP_VERSION_MINOR; }
        // @formatter:on
    }

    void updateDefaultWsResponse(HttpRequest const & request)
    {
        updateDefaultWsResponse(request.get(HEADER_SEC_WEBSOCKET_KEY));
    }

    /**
     * @remarks
     *  @code
     *   Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
     *  @endcode
     */
    std::string toStatusLine() const
    {
        std::stringstream ss;
        ss << toVersionString() << SP
           << code << SP
           << (reason.empty() ? getErrName(Err::E_EINIT) : reason);
        return ss.str();
    }

    std::string toResponseString() const
    {
        std::stringstream ss;
        ss << toStatusLine() << CRLF
           << toMessageHeader() /* Include CRLF */ << CRLF
           << toBodyString();
        return ss.str();
    }

    std::string toDebugResponseString() const
    {
        std::stringstream ss;
        ss << "[RES] " << toStatusLine() << std::endl
           << toDebugHeaderString();
        return ss.str();
    }
};

/**
 * HttpProperty class prototype.
 *
 * @author zer0
 * @date   2017-09-30
 */
struct HttpProperty : public HttpRequest, public HttpResponse
{
    void clear()
    {
        http_major = 0;
        http_minor = 0;

        clearHeaders();
        body.clear();

        method.clear();
        path.clear();

        code = 0;
        reason.clear();
    }

    void swap(HttpProperty & obj)
    {
        std::swap(http_major, obj.http_major);
        std::swap(http_minor, obj.http_minor);

        swapHeaders(obj._headers);
        body.swap(obj.body);

        method.swap(obj.method);
        path.swap(obj.path);

        std::swap(code, obj.code);
        reason.swap(obj.reason);
    }
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPPROPERTY_HPP__

