/**
 * @file   SimpleHttpClient.hpp
 * @brief  SimpleHttpClient class prototype.
 * @author zer0
 * @date   2019-01-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_SIMPLEHTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_SIMPLEHTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/http/HttpCommon.hpp>
#include <libtbag/http/HttpClient.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * SimpleHttpClient class prototype.
 *
 * @author zer0
 * @date   2019-01-14
 */
class TBAG_API SimpleHttpClient : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    SimpleHttpClient(std::string const & uri,
                     std::string const & method,
                     HttpCommon const & common,
                     int timeout_millisec = DEFAULT_HTTP_TIMEOUT_MILLISEC);
    virtual ~SimpleHttpClient();

public:
    HttpResponse waitResponse() const;
};

// -----------------------
// Miscellaneous utilities
// -----------------------

TBAG_API HttpResponse request(std::string const & uri,
                              std::string const & method,
                              HttpCommon const & common,
                              int timeout_millisec = DEFAULT_HTTP_TIMEOUT_MILLISEC);

// @formatter:off
TBAG_API HttpResponse requestGet   (std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestPost  (std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestPut   (std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestDelete(std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
// @formatter:on

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_SIMPLEHTTPCLIENT_HPP__

