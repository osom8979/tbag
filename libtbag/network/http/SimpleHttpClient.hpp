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
#include <libtbag/Err.hpp>

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/http/HttpsClient.hpp>
#include <libtbag/network/http/base/HttpProperty.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <functional>
#include <vector>
#include <type_traits>

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
 * @date   2018-02-04 (Rename: SimpleHttpClient -> SimpleClient)
 *
 * @warning
 *  Unsupported websocket request.
 */
template <typename ParentClient>
class SimpleClient : public ParentClient
{
public:
    TBAG_CONSTEXPR static bool const IS_HTTP  = std::is_same<ParentClient, HttpClient>::value;
    TBAG_CONSTEXPR static bool const IS_HTTPS = std::is_same<ParentClient, HttpsClient>::value;
    static_assert(IS_HTTP || IS_HTTPS, "ParentClient must be HttpClient or HttpsClient.");

public:
    using OnResponse = std::function<void(HttpResponse const &)>;
    using OnError    = std::function<void(libtbag::Err)>;
    using OnClose    = std::function<void(void)>;

    using Loop       = typename ParentClient::Loop;
    using StreamType = typename ParentClient::StreamType;

private:
    OnResponse _response_cb;
    OnError    _error_cb;

private:
    HttpRequest _request;

public:
    SimpleClient(Loop & loop, StreamType type = StreamType::TCP) : ParentClient(loop, type)
    { /* EMPTY. */ }
    virtual ~SimpleClient()
    { /* EMPTY. */ }

public:
    inline void setOnResponse(OnResponse  const & cb) { _response_cb = cb; }
    inline void setOnError   (OnError     const & cb) {    _error_cb = cb; }
    inline void setRequest   (HttpRequest const & rq) {     _request = rq; }

private:
    void callOnErrorAndClose(Err code)
    {
        if (static_cast<bool>(_error_cb)) {
            _error_cb(code);
        }
        ParentClient::close();
    }

public:
    virtual void onShutdown(Err code) override
    {
        callOnErrorAndClose(Err::E_INACCES);
    }

    virtual void onWrite(Err code) override
    {
        if (isFailure(code)) {
            callOnErrorAndClose(code);
        }
    }

    virtual void onClose() override
    {
        // EMPTY.
    }

    virtual void onTimer() override
    {
        callOnErrorAndClose(Err::E_TIMEOUT);
    }

public:
    virtual void onContinue(void * arg) override
    {
        // EMPTY.
    }

    virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
    {
        callOnErrorAndClose(Err::E_INACCES);
        return false;
    }

    virtual void onWsMessage(WsOpCode opcode, util::Buffer const & payload, void * arg) override
    {
        callOnErrorAndClose(Err::E_INACCES);
    }

    virtual void onRegularHttp(HttpProperty const & property, void * arg) override
    {
        if (static_cast<bool>(_response_cb)) {
            _response_cb(property);
        }
        ParentClient::stopTimer();
        ParentClient::close();
    }

    virtual void onParseError(Err code, void * arg) override
    {
        callOnErrorAndClose(code);
    }

public:
    virtual void onOpen() override
    {
        Err const WRITE_CODE = ParentClient::writeRequest(_request);
        if (isFailure(WRITE_CODE)) {
            callOnErrorAndClose(WRITE_CODE);
        }
    }

    virtual void onEof() override
    {
        ParentClient::close();
    }

public:
    virtual void onError(EventType from, Err code) override
    {
        callOnErrorAndClose(code);
    }
};

using SimpleHttpClient  = SimpleClient<HttpClient>;
using SimpleHttpsClient = SimpleClient<HttpsClient>;

// -------------
// HTTP Request.
// -------------

TBAG_API Err requestWithSync(Uri const & uri, HttpRequest const & request, HttpResponse & response,
                             uint64_t timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC,
                             HttpClient::StreamType type = HttpClient::StreamType::TCP);

TBAG_API Err requestWithSync(std::string const & uri, HttpRequest const & request, HttpResponse & response, uint64_t timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API Err requestWithSync(std::string const & uri, HttpResponse & response, uint64_t timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);

// --------------
// HTTPS Request.
// --------------

TBAG_API Err requestWithTlsSync(Uri const & uri, HttpRequest const & request, HttpResponse & response,
                                uint64_t timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC,
                                HttpsClient::StreamType type = HttpsClient::StreamType::TCP);

TBAG_API Err requestWithTlsSync(std::string const & uri, HttpRequest const & request, HttpResponse & response, uint64_t timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API Err requestWithTlsSync(std::string const & uri, HttpResponse & response, uint64_t timeout = DEFAULT_HTTP_TIMEOUT_MILLISEC);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_SIMPLEHTTPCLIENT_HPP__

