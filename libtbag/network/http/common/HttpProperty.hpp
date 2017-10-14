/**
 * @file   HttpProperty.hpp
 * @brief  HttpProperty class prototype.
 * @author zer0
 * @date   2017-09-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPPROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/network/http/common/HttpCommon.hpp>
#include <libtbag/network/http/common/HttpMethod.hpp>
#include <libtbag/network/http/common/HttpStatus.hpp>
#include <libtbag/network/http/common/HttpVersion.hpp>
#include <libtbag/network/http/common/HttpRequest.hpp>
#include <libtbag/network/http/common/HttpResponse.hpp>
#include <libtbag/network/http/common/HttpHeader.hpp>
#include <libtbag/util/BufferInfo.hpp>

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
 * HttpProperty class prototype.
 *
 * @author zer0
 * @date   2017-09-30
 */
class TBAG_API HttpProperty : public HttpHeader
{
protected:
    HttpRequest  _request;
    HttpResponse _response;
    util::Buffer _body;

public:
    HttpProperty();
    HttpProperty(HttpProperty const & obj);
    HttpProperty(HttpProperty && obj);
    virtual ~HttpProperty();

public:
    HttpProperty & operator =(HttpProperty const & obj);
    HttpProperty & operator =(HttpProperty && obj);

public:
    void swap(HttpProperty & obj);

public:
    inline HttpRequest        & atRequest ()       TBAG_NOEXCEPT { return  _request; }
    inline HttpRequest  const & atRequest () const TBAG_NOEXCEPT { return  _request; }
    inline HttpResponse       & atResponse()       TBAG_NOEXCEPT { return _response; }
    inline HttpResponse const & atResponse() const TBAG_NOEXCEPT { return _response; }
    inline util::Buffer       & atBody    ()       TBAG_NOEXCEPT { return     _body; }
    inline util::Buffer const & atBody    () const TBAG_NOEXCEPT { return     _body; }

public:
    inline void setBody(std::string const & body) { _body.assign(body.begin(), body.end()); }
    inline std::string getBodyString() const { return std::string(_body.begin(), _body.end()); }

public:
    inline void  setRequestVersion(int major, int minor) TBAG_NOEXCEPT {  _request.version.set(major, minor); }
    inline void setResponseVersion(int major, int minor) TBAG_NOEXCEPT { _response.version.set(major, minor); }

    inline void setVersion(int major, int minor) TBAG_NOEXCEPT
    {
        setRequestVersion(major, minor);
        setResponseVersion(major, minor);
    }

    inline HttpVersion getRequestVersion () const TBAG_NOEXCEPT { return  _request.version; }
    inline HttpVersion getResponseVersion() const TBAG_NOEXCEPT { return _response.version; }

public:
    inline void setHttpMethod(HttpMethod m) { _request.setHttpMethod(m); }
    inline void     setMethod(std::string const & val) { _request.method = val; }
    inline void        setUri(std::string const & val) { _request.uri    = val; }

    inline HttpMethod  getHttpMethod() const { return _request.getHttpMethod(); }
    inline std::string     getMethod() const { return _request.method; }
    inline std::string        getUri() const { return _request.uri;    }

public:
    inline void setHttpStatus(HttpStatus s) { _response.setHttpStatus(s); }
    inline void setHttpStatus(std::string const & name) { _response.setHttpStatus(name); }
    inline void setStatusCode(int code) TBAG_NOEXCEPT { _response.code = code; }
    inline void setReason(std::string const & val) { _response.reason = val; }

    inline HttpStatus getHttpStatus() const { return _response.getHttpStatus(); }
    inline int getStatusCode() const TBAG_NOEXCEPT { return _response.code; }
    inline std::string getReason() const { return _response.reason; }

public:
    void clear();

public:
    std::string toRequestString() const;
    std::string toResponseString() const;
};

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPPROPERTY_HPP__

