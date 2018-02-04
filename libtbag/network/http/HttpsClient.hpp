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
        TS_HANDSHAKING,
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
    // README:
    // The reason for writing the callback in the header is to verify the logic when redefining the virtual function.

    virtual void onConnect(Err code) override
    {
        if (isFailure(code)) {
            onError(EventType::ET_CONNECT, code);
            return;
        }

        _tls.connect();
        _state = TlsState::TS_HANDSHAKING;

        Err const START_CODE = start();
        if (isFailure(START_CODE)) {
            onError(EventType::ET_START, START_CODE);
            return;
        }

        onOpen();
    }

    virtual void onRead(Err code, ReadPacket const & packet) override
    {
        if (code == Err::E_EOF) {
            onEof();
        } else if (code != Err::E_SUCCESS) {
            onError(EventType::ET_READ, code);
        } else {
            Err decode_code = Err::E_UNKNOWN;
            auto decode_result = _tls.decode(packet.buffer, packet.size, &decode_code);
            if (isFailure(decode_code)) {
                onError(EventType::ET_READ, decode_code);
            } else {
                if (_tls.isFinished() == false) {
                    if (isSuccess(_tls.handshake())) {
                        // Recheck if handshake is complete now.
                        _state = TlsState::TS_FINISH;
                        return;
                    }
                }

                if (_state == TlsState::TS_HANDSHAKING) {
                    Err const WRITE_CODE = writeTls(decode_result.data(), decode_result.size());
                    if (isFailure(WRITE_CODE)) {
                        // Handshaking error.
                    }
                } else if (_state == TlsState::TS_FINISH) {
                    _reader.parse(decode_result.data(), decode_result.size());
                }
            }
        }
    }

    virtual void onParseError(Err code, void * arg) override
    {
        close();
    }

// Extension callback methods.
public:
    virtual void onOpen() { /* EMPTY. */ }
    virtual void onEof() { close(); }
    virtual void onError(EventType from, Err code) { close(); }
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSCLIENT_HPP__

