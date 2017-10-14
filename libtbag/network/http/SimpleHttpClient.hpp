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
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

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
 *
 * @warning
 *  Unsupported websocket request.
 */
class TBAG_API SimpleHttpClient : public HttpClient
{
public:
    using HttpProperty = HttpClient::HttpProperty;

public:
    using OnResponse = std::function<void(HttpProperty const &)>;
    using OnError    = std::function<void(libtbag::Err)>;
    using OnClose    = std::function<void(void)>;

private:
    OnResponse _response_cb;
    OnError    _error_cb;

private:
    HttpProperty _request;

public:
    SimpleHttpClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~SimpleHttpClient();

public:
    inline void setOnResponse(OnResponse   const & cb) { _response_cb = cb; }
    inline void setOnError   (OnError      const & cb) {    _error_cb = cb; }
    inline void setRequest   (HttpProperty const & rq) {     _request = rq; }

private:
    void callOnErrorAndClose(Err code);

public:
    virtual void onShutdown(Err code) override;
    virtual void onWrite   (Err code) override;
    virtual void onClose   () override;
    virtual void onTimer   () override;

public:
    virtual void onContinue(void * arg) override;
    virtual bool onSwitchingProtocol(HttpProperty const & response, void * arg) override;
    virtual void onWsMessage(ws::WsOpCode opcode, util::Buffer const & payload, void * arg) override;
    virtual void onRegularHttp(HttpProperty const & response, void * arg) override;
    virtual void onParseError(Err code, void * arg) override;

public:
    virtual void onOpen() override;
    virtual void onEof() override;

public:
    virtual void onError(EventType from, Err code) override;
};

// ----------
// Utilities.
// ----------

TBAG_API Err requestWithSync(Uri const & uri,
                             common::HttpProperty const & request,
                             common::HttpProperty & response,
                             uint64_t timeout = common::DEFAULT_HTTP_TIMEOUT_MILLISEC,
                             HttpClient::StreamType type = HttpClient::StreamType::TCP);

TBAG_API Err requestWithSync(std::string const & uri, common::HttpProperty const & request, common::HttpProperty & response, uint64_t timeout = common::DEFAULT_HTTP_TIMEOUT_MILLISEC);
TBAG_API Err requestWithSync(std::string const & uri, common::HttpProperty & response, uint64_t timeout = common::DEFAULT_HTTP_TIMEOUT_MILLISEC);


} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_SIMPLEHTTPCLIENT_HPP__

