/**
 * @file   HttpProperty.hpp
 * @brief  HttpProperty class prototype.
 * @author zer0
 * @date   2017-09-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_BASE_HTTPPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_BASE_HTTPPROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/network/http/base/HttpCommon.hpp>
#include <libtbag/network/http/base/HttpMethod.hpp>
#include <libtbag/network/http/base/HttpStatus.hpp>
#include <libtbag/network/http/base/HttpHeader.hpp>
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
namespace base    {

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
    HttpVersion(HttpVersion const & obj) TBAG_NOEXCEPT : HttpVersion() { (*this) = obj; }
    HttpVersion(HttpVersion && obj) TBAG_NOEXCEPT : HttpVersion() { (*this) = std::move(obj); }
    ~HttpVersion() TBAG_NOEXCEPT { /* EMPTY. */ }

    inline void setVersion(int v1, int v2) TBAG_NOEXCEPT
    {
        http_major = v1;
        http_minor = v2;
    }

    inline void setMajor(int val) TBAG_NOEXCEPT { http_major = val; }
    inline void setMinor(int val) TBAG_NOEXCEPT { http_minor = val; }

    inline int getMajor() const TBAG_NOEXCEPT { return http_major; }
    inline int getMinor() const TBAG_NOEXCEPT { return http_minor; }

    inline friend bool operator ==(HttpVersion const & lh, HttpVersion const & rh) TBAG_NOEXCEPT
    {
        return lh.http_major == rh.http_major && lh.http_minor == rh.http_minor;
    }

    inline friend bool operator !=(HttpVersion const & lh, HttpVersion const & rh) TBAG_NOEXCEPT
    {
        return !operator==(lh, rh);
    }

    inline HttpVersion & operator =(HttpVersion const & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            http_major = obj.http_major;
            http_minor = obj.http_minor;
        }
        return *this;
    }

    inline HttpVersion & operator =(HttpVersion && obj) TBAG_NOEXCEPT
    {
        swap(obj);
        return *this;
    }

    inline void swap(HttpVersion & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            std::swap(http_major, obj.http_major);
            std::swap(http_minor, obj.http_minor);
        }
    }

    inline friend void swap(HttpVersion & lh, HttpVersion & rh) TBAG_NOEXCEPT { lh.swap(rh); }

    /**
     * @remarks
     *  @code
     *   HTTP-Version = "HTTP" "/" 1*DIGIT "." 1*DIGIT
     *  @endcode
     */
    std::string toVersionString() const;
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

    HttpCommonProperty() : body() { /* EMPTY. */ }
    HttpCommonProperty(HttpCommonProperty const & obj) : HttpCommonProperty() { (*this) = obj; }
    HttpCommonProperty(HttpCommonProperty && obj) : HttpCommonProperty() { (*this) = std::move(obj); }
    ~HttpCommonProperty() { /* EMPTY. */ }

    HttpCommonProperty & operator =(HttpCommonProperty const & obj);
    HttpCommonProperty & operator =(HttpCommonProperty && obj);

    void swap(HttpCommonProperty & obj);
    friend void swap(HttpCommonProperty & lh, HttpCommonProperty & rh) { lh.swap(rh); }

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

    HttpRequest() : method(), path() { /* EMPTY. */ }
    HttpRequest(HttpRequest const & obj) : HttpRequest() { (*this) = obj; }
    HttpRequest(HttpRequest && obj) : HttpRequest() { (*this) = std::move(obj); }
    ~HttpRequest() { /* EMPTY. */ }

    HttpRequest & operator =(HttpRequest const & obj);
    HttpRequest & operator =(HttpRequest && obj);

    void swap(HttpRequest & obj);
    friend void swap(HttpRequest & lh, HttpRequest & rh) { lh.swap(rh); }

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
    int code;
    std::string reason;

    HttpResponse() : code(0), reason() { /* EMPTY. */ }
    HttpResponse(HttpResponse const & obj) : HttpResponse() { (*this) = obj; }
    HttpResponse(HttpResponse && obj) : HttpResponse() { (*this) = std::move(obj); }
    ~HttpResponse() { /* EMPTY. */ }

    HttpResponse & operator =(HttpResponse const & obj);
    HttpResponse & operator =(HttpResponse && obj);

    void swap(HttpResponse & obj);
    friend void swap(HttpResponse & lh, HttpResponse & rh) { lh.swap(rh); }

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
    HttpProperty() { /* EMPTY. */ }
    HttpProperty(HttpProperty const & obj) : HttpProperty() { (*this) = obj; }
    HttpProperty(HttpProperty && obj) : HttpProperty() { (*this) = std::move(obj); }
    ~HttpProperty() { /* EMPTY. */ }

    HttpProperty & operator =(HttpProperty const & obj);
    HttpProperty & operator =(HttpProperty && obj);

    void swap(HttpProperty & obj);
    friend void swap(HttpProperty & lh, HttpProperty & rh) { lh.swap(rh); }

    void clear();
};

} // namespace base

using namespace libtbag::network::http::base;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_BASE_HTTPPROPERTY_HPP__

