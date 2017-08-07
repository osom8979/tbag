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
#include <libtbag/network/http/HttpCacheData.hpp>
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
 * WebSocketClient class prototype.
 *
 * @author zer0
 * @date   2017-06-30
 */
class TBAG_API WebSocketClient : public stream::StreamClient
{
public:
    using StreamType = details::StreamType;
    using Parent     = stream::StreamClient;

    using Loop     = uvpp::Loop;
    using WsBuffer = WebSocketFrame::WsBuffer;

public:
    TBAG_CONSTEXPR static uint64_t const DEFAULT_CLOSING_TIMEOUT_MILLISECOND = 5 * 1000;

private:
    HttpCacheData _cache;
    WsCloseResult _close;

public:
    WebSocketClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~WebSocketClient();

public:
    // @formatter:off
    inline HttpBuilder       & atBuilder()       TBAG_NOEXCEPT { return _cache.builder; }
    inline HttpBuilder const & atBuilder() const TBAG_NOEXCEPT { return _cache.builder; }
    inline HttpParser        & atParser ()       TBAG_NOEXCEPT { return _cache.parser;  }
    inline HttpParser  const & atParser () const TBAG_NOEXCEPT { return _cache.parser;  }
    // @formatter:on

public:
    void setup(HttpBuilder const & request);

public:
    Err writeText(std::string const & text, bool continuation = false, bool finish = true);
    Err writeBinary(WsBuffer const & binary, bool continuation = false, bool finish = true);
    Err writeClose();

private:
    bool runWebSocketChecker(HttpParser const & response);
    void runWebSocketRead(char const * buffer, std::size_t size);

// Stream event methods.
protected:
    virtual void onConnect (Err code) override;
    virtual void onShutdown(Err code) override;
    virtual void onWrite   (Err code) override;
    virtual void onRead    (Err code, ReadPacket const & packet) override;
    virtual void onClose   () override;
    virtual void onTimer   () override;

// Event methods.
public:
    /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
    virtual void onWsOpen(HttpResponse const & response);

    /** When a message has been received from WebSocket server. */
    virtual void onWsMessage(OpCode op, char const * buffer, std::size_t size);

    /** Triggered when error occurred. */
    virtual void onWsError(Err code);

    /** When a socket has been closed. */
    virtual void onWsClose(uint16_t code, std::string const & reason);
};

/**
 * FunctionalWebSocketClient class prototype.
 *
 * @author zer0
 * @date   2017-07-10
 */
struct FunctionalWebSocketClient : public WebSocketClient
{
    using StreamType = WebSocketClient::StreamType;

    using OnWsOpen    = std::function<void(HttpResponse const &)>;
    using OnWsMessage = std::function<void(OpCode, char const *, std::size_t)>;
    using OnWsError   = std::function<void(Err)>;
    using OnWsClose   = std::function<void(uint16_t, std::string const &)>;

    OnWsOpen     _ws_open_cb;
    OnWsMessage  _ws_message_cb;
    OnWsError    _ws_error_cb;
    OnWsClose    _ws_close_cb;

    FunctionalWebSocketClient(Loop & loop, StreamType type = StreamType::TCP) : WebSocketClient(loop, type)
    { /* EMPTY. */ }
    virtual ~FunctionalWebSocketClient()
    { /* EMPTY. */ }

    inline void setOnWsOpen   (OnWsOpen    const & cb) { _ws_open_cb    = cb; }
    inline void setOnWsMessage(OnWsMessage const & cb) { _ws_message_cb = cb; }
    inline void setOnWsError  (OnWsError   const & cb) { _ws_error_cb   = cb; }
    inline void setOnWsClose  (OnWsClose   const & cb) { _ws_close_cb   = cb; }

    virtual void onWsOpen(HttpResponse const & response) override
    { if (static_cast<bool>(_ws_open_cb)) { _ws_open_cb(response); } }
    virtual void onWsMessage(OpCode op, char const * buffer, std::size_t size) override
    { if (static_cast<bool>(_ws_message_cb)) { _ws_message_cb(op, buffer, size); } }
    virtual void onWsError(Err code) override
    { if (static_cast<bool>(_ws_error_cb)) { _ws_error_cb(code); } }
    virtual void onWsClose(uint16_t code, std::string const & reason) override
    { if (static_cast<bool>(_ws_close_cb)) { _ws_close_cb(code, reason); } }
};

using FuncWebSocketClient = FunctionalWebSocketClient;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WEBSOCKETCLIENT_HPP__

