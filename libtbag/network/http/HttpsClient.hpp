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
#include <libtbag/network/http/base/HttpReader.hpp>
#include <libtbag/network/http/tls/TlsReader.hpp>
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
class TBAG_API HttpsClient : public stream::StreamClient, public HttpReaderInterface
{
public:
    using StreamType = details::StreamType;
    using Parent     = stream::StreamClient;
    using Loop       = uvpp::Loop;
    using TlsReader  = tls::TlsReader;

public:
    enum class TlsState
    {
        TS_NOT_READY,
        TS_HELLO,
        TS_EXCHANGE_KEY,
        TS_FINISH,
    };

private:
    HttpReaderForCallback<HttpsClient> _reader;
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

