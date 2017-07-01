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

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/http/WebSocketFrame.hpp>

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
class TBAG_API WebSocketClient : protected HttpClient
{
public:
    using StreamType = HttpClient::StreamType;
    using Buffer = HttpClient::Buffer;
    using Loop = uvpp::Loop;

    using Frame = WebSocketFrame;
    using FrameBuffer = WebSocketFrame::Buffer;

public:
    struct WebSocketCallback
    {
        /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
        virtual void onOpen() = 0;

        /** When a message has been received from WebSocket server. */
        virtual void onMessage(Err code, Frame const & frame) = 0;

        /** Triggered when error occurred. */
        virtual void onError(Err code) = 0;

        /** When a socket has been closed. */
        virtual void onClose() = 0;
    };

private:
    WebSocketCallback * _callback;

private:
    Frame _recv_frame;
    Frame _write_frame;
    FrameBuffer _frame_buffer;

public:
    WebSocketClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~WebSocketClient();

public:
    Err sendText(std::string const & text);
    Err sendBinary(Buffer const & binary);
    Err sendPing(Buffer const & binary);
    Err sendPong(Buffer const & binary);
    Err sendClose();

private:
    virtual void onConnect(Err code) override;
    virtual void onShutdown(Err code) override;
    virtual void onWrite(Err code) override;
    virtual void onRead(Err code, ReadPacket const & packet) override;
    virtual void onClose() override;
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETCLIENT_HPP__

