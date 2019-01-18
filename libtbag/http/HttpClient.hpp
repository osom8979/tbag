/**
 * @file   HttpClient.hpp
 * @brief  HttpClient class prototype.
 * @author zer0
 * @date   2019-01-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/http/HttpCommon.hpp>
#include <libtbag/http/HttpReader.hpp>
#include <libtbag/http/WsFrame.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/net/Uri.hpp>
#include <libtbag/net/socket/NetStreamClient.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <libtbag/crypto/TlsReader.hpp>
#include <libtbag/random/MaskingDevice.hpp>
#include <libtbag/Type.hpp>

#include <string>
#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpClient class prototype.
 *
 * @author zer0
 * @date   2019-01-11
 * @date   2019-01-15 (Merge with HttpClient::Impl class)
 */
class TBAG_API HttpClient TBAG_FINAL : public libtbag::http::HttpReaderInterface
{
public:
    using Loop         = libtbag::uvpp::Loop;
    using Uri          = libtbag::net::Uri;
    using MqParams     = libtbag::mq::details::MqParams;
    using HttpRequest  = libtbag::http::HttpRequest;
    using HttpResponse = libtbag::http::HttpResponse;
    using Buffer       = libtbag::util::Buffer;
    using WsFrame      = libtbag::http::WsFrame;

    using HttpReaderInterface = libtbag::http::HttpReaderInterface;
    using Reader              = libtbag::http::HttpReaderForCallback<HttpClient>;
    using Masking             = libtbag::random::MaskingDevice;
    using TlsReader           = libtbag::crypto::TlsReader;
    using TlsState            = libtbag::crypto::TlsState;
    using NetStreamClient     = libtbag::net::socket::NetStreamClient;
    using UniqueClient        = std::unique_ptr<NetStreamClient>;

    using OnBegin   = std::function<void(void)>;
    using OnEnd     = std::function<void(void)>;
    using OnHttp    = std::function<void(HttpResponse const &)>;
    using OnSwitch  = std::function<bool(HttpResponse const &)>;
    using OnMessage = std::function<void(WsOpCode, Buffer const &)>;
    using OnError   = std::function<void(Err)>;

public:
    friend struct libtbag::http::HttpReaderForCallback<HttpClient>;

public:
    STATIC_ASSERT_CHECK_IS_SAME(Buffer, HttpReaderInterface::Buffer);

public:
    struct Callbacks
    {
        OnBegin   begin_cb;
        OnEnd     end_cb;
        OnHttp    http_cb;
        OnSwitch  switch_cb;
        OnMessage message_cb;
        OnError   error_cb;
    };

private:
    bool const ENABLE_TLS;

private:
    UniqueClient _client;
    Reader       _reader;
    Masking      _masking;
    TlsReader    _tls;
    TlsState     _state;

private:
    Callbacks _callbacks;

public:
    HttpClient(MqParams const & params, Callbacks const & cbs,
               bool use_websocket = false, bool enable_tls = false);
    HttpClient(MqParams const & params, Callbacks const & cbs, std::string const & key,
               bool use_websocket = false, bool enable_tls = false);
    virtual ~HttpClient();

public:
    // @formatter:off
    Loop       & loop();
    Loop const & loop() const;
    // @formatter:on

private:
    Err sendTls(void const * data, std::size_t size);
    Err sendTls(std::string const & text);

private:
    void onBegin();
    void onBeginTls();
    void onEnd();
    void onRecv(char const * buffer, std::size_t size);
    void onRecvTls(char const * buffer, std::size_t size);
    void onHandshakeHello(char const * buffer, std::size_t size);
    void onHandshakeExchangeKey(char const * buffer, std::size_t size);
    void onApplication(char const * buffer, std::size_t size);
    void onError(Err code);

protected:
    virtual void onContinue(void * arg) override;
    virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override;
    virtual void onWsMessage(WsOpCode opcode, Buffer const & payload, void * arg) override;
    virtual void onRegularHttp(HttpProperty const & property, void * arg) override;
    virtual void onParseError(Err code, void * arg) override;

public:
    void join();

public:
    Err exit();

public:
    Err writeRequest();
    Err writeRequest(HttpRequest const & request);

public:
    Err writeWsRequest();
    Err writeWsRequest(HttpRequest const & request);

public:
    Err writeWsFrame(WsFrame const & frame);
    Err writeText   (char const * buffer, std::size_t size, bool finish = true);
    Err writeText   (std::string const & text, bool finish = true);
    Err writeBinary (char const * buffer, std::size_t size, bool finish = true);
    Err writeBinary (Buffer const & buffer, bool finish = true);
    Err writeClose  ();

public:
    static MqParams getDefaultParams(std::string const & uri, int timeout_millisec);
    static MqParams getDefaultParams(Uri const & uri, int timeout_millisec);
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

