/**
 * @file   SimpleHttpClient.hpp
 * @brief  SimpleHttpClient class prototype.
 * @author zer0
 * @date   2017-09-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_SIMPLEHTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_SIMPLEHTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/Uri.hpp>

#include <functional>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * SimpleHttpClient class prototype.
 *
 * @author zer0
 * @date   2017-09-29
 */
class TBAG_API SimpleHttpClient : public HttpClient
{
public:
    SimpleHttpClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~SimpleHttpClient();
};

// ----------
// Utilities.
// ----------

TBAG_API Err requestWithSync(HttpClient::StreamType type,
                             std::string const & host,
                             int port,
                             Uri const & uri,
                             HttpRequest const & request,
                             uint64_t timeout,
                             HttpResponse & result);

TBAG_API Err requestWithSync(Uri const & uri, HttpRequest const & request, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(std::string const & uri, HttpRequest const & request, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(Uri const & uri, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(std::string const & uri, uint64_t timeout, HttpResponse & result);


} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_SIMPLEHTTPCLIENT_HPP__

