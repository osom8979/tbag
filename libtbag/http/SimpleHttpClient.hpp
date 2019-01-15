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

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/network/Uri.hpp>

#include <cassert>
#include <string>
#include <memory>
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * SimpleHttpClient class prototype.
 *
 * @author zer0
 * @date   2019-01-14
 * @date   2019-01-15 (Merge with SimpleHttpClient::Impl class)
 */
class TBAG_API SimpleHttpClient TBAG_FINAL : private Noncopyable
{
public:
    using MqParams     = libtbag::mq::details::MqParams;
    using MqType       = libtbag::mq::details::MqType;
    using Uri          = libtbag::network::Uri;
    using Loop         = libtbag::uvpp::Loop;
    using Timer        = libtbag::uvpp::Timer;
    using UniqueClient = std::unique_ptr<HttpClient>;

public:
    struct Timeout : public Timer
    {
        SimpleHttpClient * parent = nullptr;

        Timeout(Loop & loop, SimpleHttpClient * p) : Timer(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Timeout()
        { /* EMPTY. */ }

        virtual void onTimer() override
        { parent->onTimeout(this); }
    };

public:
    using SharedTimeout = std::shared_ptr<Timeout>;
    using TimePoint = std::chrono::system_clock::time_point;

public:
    int const TIMEOUT_MILLISEC;
    TimePoint const START_TIME;

private:
    UniqueClient  _client;
    SharedTimeout _timeout;
    HttpRequest   _request;
    HttpResponse  _response;

public:
    SimpleHttpClient(std::string const & uri, std::string const & method,
                     HttpCommon const & common, int timeout_millisec = DEFAULT_HTTP_TIMEOUT_MILLISEC);
    SimpleHttpClient(Uri const & uri, std::string const & method,
                     HttpCommon const & common, int timeout_millisec = DEFAULT_HTTP_TIMEOUT_MILLISEC);
    ~SimpleHttpClient();

private:
    void onBegin();
    void onEnd();
    void onTimeout(Timeout * timeout);
    void onRegularHttp(HttpResponse const & response);
    void onError(Err code);
    void stopTimerAndExit();

public:
    HttpResponse waitResponse() const;
};

// -----------------------
// Miscellaneous utilities
// -----------------------

TBAG_API HttpResponse request(std::string const & uri,
                              std::string const & method,
                              HttpCommon const & common,
                              int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);

// @formatter:off
TBAG_API HttpResponse requestGet   (std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestPost  (std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestPut   (std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestDelete(std::string const & uri, HttpCommon const & common, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
// @formatter:on

// @formatter:off
TBAG_API HttpResponse requestGet   (std::string const & uri, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestPost  (std::string const & uri, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestPut   (std::string const & uri, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API HttpResponse requestDelete(std::string const & uri, int timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
// @formatter:on

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_SIMPLEHTTPCLIENT_HPP__

