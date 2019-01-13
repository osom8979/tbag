/**
 * @file   HttpsClient.hpp
 * @brief  HttpsClient class prototype.
 * @author zer0
 * @date   2018-02-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/http/HttpReader.hpp>
#include <libtbag/crypto/TlsReader.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpsClient class prototype.
 *
 * @author zer0
 * @date   2018-02-04
 */
class TBAG_API HttpsClient : public libtbag::network::stream::StreamClient,
                             public libtbag::http::HttpReaderInterface
{
public:
    using StreamType  = libtbag::network::details::StreamType;
    using Parent      = libtbag::network::stream::StreamClient;
    using Loop        = libtbag::uvpp::Loop;
    using TlsReader   = libtbag::crypto::TlsReader;
    using TlsState    = libtbag::crypto::TlsState;
    using EventType   = libtbag::http::EventType;
    using HttpRequest = libtbag::http::HttpRequest;

private:
    libtbag::http::HttpReaderForCallback<HttpsClient> _reader;
    TlsReader _tls;
    TlsState  _state;

public:
    HttpsClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpsClient();

public:
    std::string getKey() const;

public:
    Err writeRequest();
    Err writeRequest(HttpRequest const & request);
    Err writeTls(void const * data, std::size_t size);

public:
    virtual void onConnect(Err code) override;
    virtual void onRead(Err code, ReadPacket const & packet) override;
    virtual void onParseError(Err code, void * arg) override;

private:
    void onHandshakeHello(ReadPacket const & packet);
    void onHandshakeExchangeKey(ReadPacket const & packet);
    void onApplication(ReadPacket const & packet);

// Extension callback methods.
public:
    virtual void onOpen();
    virtual void onEof();
    virtual void onError(EventType from, Err code);
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSCLIENT_HPP__

