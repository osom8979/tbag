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
class TBAG_API HttpProperty
{
private:
    HttpRequest  _request;
    HttpResponse _response;
    HttpHeader   _headers;
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
    inline HttpRequest        & atRequest ()       TBAG_NOEXCEPT { return _request;  }
    inline HttpRequest  const & atRequest () const TBAG_NOEXCEPT { return _request;  }
    inline HttpResponse       & atResponse()       TBAG_NOEXCEPT { return _response; }
    inline HttpResponse const & atResponse() const TBAG_NOEXCEPT { return _response; }
    inline HttpHeader         & atHeaders ()       TBAG_NOEXCEPT { return _headers;  }
    inline HttpHeader   const & atHeaders () const TBAG_NOEXCEPT { return _headers;  }
    inline util::Buffer       & atBody    ()       TBAG_NOEXCEPT { return _body;     }
    inline util::Buffer const & atBody    () const TBAG_NOEXCEPT { return _body;     }

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

