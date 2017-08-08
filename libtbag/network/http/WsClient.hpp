/**
 * @file   WsClient.hpp
 * @brief  WsClient class prototype.
 * @author zer0
 * @date   2017-06-30
 * @date   2017-08-07 (Rename: WebSocketClient -> WsClient)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/http/WsFrameBuffer.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <libtbag/container/ReuseQueue.hpp>
#include <libtbag/random/MaskingDevice.hpp>

#include <functional>
#include <vector>
#include <string>
#include <atomic>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * WsClient class prototype.
 *
 * @author zer0
 * @date   2017-06-30
 * @date   2017-08-07 (Rename: WebSocketClient -> WsClient)
 */
class TBAG_API WsClient : public stream::StreamClient
{
public:
    using StreamType = details::StreamType;
    using Parent     = stream::StreamClient;

    using Loop     = uvpp::Loop;
    using WsBuffer = WsFrame::WsBuffer;
    using WsQueue  = container::ReuseQueue<WsBuffer>;
    using Masking  = random::MaskingDevice;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    TBAG_CONSTEXPR static uint64_t const DEFAULT_CLOSING_TIMEOUT_MILLISECOND = 5 * 1000;

private:
    std::string const KEY;

// Flags.
private:
    std::atomic_bool _upgrade;
    std::atomic_bool _closing;

private:
    Masking _device;

// HTTP Request.
private:
    mutable Mutex _request_mutex;
    HttpBuilder _request;

private:
    mutable Mutex _queue_mutex;
    WsQueue _queue;

private:
    WsCloseResult _close;

// Receive packet.
private:
    struct {
        HttpParser response; ///< Response packet parser.
        WsFrameBuffer receiver; ///< WebSocket frame buffer.
    } __on_read_only__; ///< @warning It should only be used with the onRead() method.

public:
    WsClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~WsClient();

public:
    inline bool isUpgrade() const TBAG_NOEXCEPT_SP_OP(_upgrade.load()) { return _upgrade.load(); }
    inline bool isClosing() const TBAG_NOEXCEPT_SP_OP(_closing.load()) { return _closing.load(); }

public:
    void setup(HttpBuilder const & request);
    HttpBuilder getRequest() const;

private:
    Err writeOrEnqueue(char const * buffer, std::size_t size);
    Err writeOrEnqueue(WsFrame const & frame);
    Err writeFromQueue();

public:
    Err writeText(std::string const & text, bool continuation = false, bool finish = true);
    Err writeBinary(WsBuffer const & binary, bool continuation = false, bool finish = true);

public:
    Err closeWebSocket();

private:
    bool runWsChecker(HttpParser const & response);
    void runWsRead(char const * buffer, std::size_t size);

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
 * Functional web-socket client class prototype.
 *
 * @author zer0
 * @date   2017-07-10
 * @date   2017-08-07 (Rename: FunctionalWebSocketClient -> FuncWsClient)
 */
struct FuncWsClient : public WsClient
{
    using StreamType = WsClient::StreamType;

    using OnWsOpen    = std::function<void(HttpResponse const &)>;
    using OnWsMessage = std::function<void(OpCode, char const *, std::size_t)>;
    using OnWsError   = std::function<void(Err)>;
    using OnWsClose   = std::function<void(uint16_t, std::string const &)>;

    OnWsOpen     _ws_open_cb;
    OnWsMessage  _ws_message_cb;
    OnWsError    _ws_error_cb;
    OnWsClose    _ws_close_cb;

    FuncWsClient(Loop & loop, StreamType type = StreamType::TCP) : WsClient(loop, type)
    { /* EMPTY. */ }
    virtual ~FuncWsClient()
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

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSCLIENT_HPP__

