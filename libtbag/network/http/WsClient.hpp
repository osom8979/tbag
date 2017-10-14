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
    using WsBuffer = http::WsFrame::Buffer;
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
    WsStatus _close;

// Receive packet.
private:
    struct {
        http::HttpParser    response; ///< Response packet parser.
        http::WsFrameBuffer receiver; ///< WebSocket frame buffer.
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
    Err writeWsFrame(http::WsFrame const & frame);

public:
    Err writeText(char const * buffer, std::size_t size, bool continuation = false, bool finish = true);
    Err writeText(std::string const & text, bool continuation = false, bool finish = true);

    Err writeBinary(char const * buffer, std::size_t size, bool continuation = false, bool finish = true);
    Err writeBinary(WsBuffer const & binary, bool continuation = false, bool finish = true);

public:
    Err closeWebSocket();

private:
    bool runWsChecker(http::HttpParser const & response);
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
    virtual void onWsOpen(http::HttpResponse const & response) { /* EMPTY. */ }

    /** When a message has been received from WebSocket server. */
    virtual void onWsMessage(WsOpCode op, char const * buffer, std::size_t size) { /* EMPTY. */ }

    /** Triggered when error occurred. */
    virtual void onWsError(Err code) { /* EMPTY. */ }

    /** When a socket has been closed. */
    virtual void onWsClose(uint16_t code, std::string const & reason) { /* EMPTY. */ }
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSCLIENT_HPP__

