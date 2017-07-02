/**
 * @file   WebSocketClient.hpp
 * @brief  WebSocketClient class prototype.
 * @author zer0
 * @date   2017-06-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/http/WebSocketFrame.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * WebSocketClient class prototype.
 *
 * @author zer0
 * @date   2017-06-30
 */
class TBAG_API WebSocketClient : protected stream::StreamClient
{
public:
    using StreamType = details::StreamType;
    using Parent     = stream::StreamClient;

public:
    using Loop   = uvpp::Loop;
    using Buffer = std::vector<char>;

    using Frame = WebSocketFrame;
    using FrameBuffer = WebSocketFrame::Buffer;

public:
    TBAG_CONSTEXPR static uint64_t const DEFAULT_WRITE_TIMEOUT_MILLISECOND = 5 * 1000;

public:
    struct ClientData
    {
        struct {
            bool upgrade = false;
            WebSocketFrame recv_frame;
            WebSocketFrame write_frame;
            WebSocketFrame::Buffer frame_buffer;
        } websocket;

        HttpBuilder builder;
        HttpParser  parser;

        std::string key;
        std::string protocols;
    };

    ClientData _data;

public:
    WebSocketClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~WebSocketClient();

public:
    // @formatter:off
    inline HttpBuilder       & atBuilder()       TBAG_NOEXCEPT { return _data.builder; }
    inline HttpBuilder const & atBuilder() const TBAG_NOEXCEPT { return _data.builder; }
    inline HttpParser        & atParser ()       TBAG_NOEXCEPT { return _data.parser;  }
    inline HttpParser  const & atParser () const TBAG_NOEXCEPT { return _data.parser;  }
    // @formatter:on

    inline bool isUpgradeWebSocket() const TBAG_NOEXCEPT
    { return _data.websocket.upgrade; }

public:
    Err sendText(std::string const & text, uint64_t timeout);
    Err sendBinary(Buffer const & binary, uint64_t timeout);

    Err sendPing(Buffer const & binary, uint64_t timeout);
    Err sendPong(Buffer const & binary, uint64_t timeout);
    Err sendClose(uint64_t timeout);

public:
    void setup(HttpBuilder const & request);

private:
    bool runWebSocketChecker(HttpParser const & response);
    void runWebSocketRead(char const * buffer, std::size_t size);

private:
    virtual void onConnect(Err code) override;
    virtual void onShutdown(Err code) override;
    virtual void onWrite(Err code) override;
    virtual void onRead(Err code, ReadPacket const & packet) override;
    virtual void onClose() override;

public:
    /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
    virtual void onWsOpen(HttpResponse const & response);

    /** When a message has been received from WebSocket server. */
    virtual void onWsMessage(Frame const & frame);

    /** Triggered when error occurred. */
    virtual void onWsError(Err code);

    /** When a socket has been closed. */
    virtual void onWsClose();
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETCLIENT_HPP__

