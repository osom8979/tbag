/**
 * @file   HttpServer.hpp
 * @brief  HttpServer class prototype.
 * @author zer0
 * @date   2017-05-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/random/MaskingDevice.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/http/HttpFilter.hpp>
#include <libtbag/network/http/WebSocketFrame.hpp>
#include <libtbag/network/http/WebSocketParser.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <functional>
#include <memory>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-22
 */
class TBAG_API HttpServer : public stream::StreamServer
{
public:
    using Parent     = stream::StreamServer;
    using StreamType = details::StreamType;
    using ReadPacket = details::ReadPacket;

    using ClientInterface = details::ClientInterface;
    using ServerInterface = details::ServerInterface;

public:
    using Loop   = uvpp::Loop;
    using Buffer = std::vector<char>;

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    using SystemClock = std::chrono::system_clock;
    using TimePoint   = SystemClock::time_point;
    using Millisec    = std::chrono::milliseconds;
    using Seconds     = std::chrono::seconds;

    using MaskingDevice = random::MaskingDevice;

    using OnOpen            = std::function<void(WeakClient)>;
    using OnRequest         = std::function<void(Err, WeakClient, HttpParser const &, HttpBuilder &, uint64_t &)>;
    using OnClose           = std::function<void(WeakClient)>;
    using OnShutdown        = std::function<void(WeakClient, Err)>;
    using OnWrite           = std::function<void(WeakClient, Err)>;
    using OnUserDataAlloc   = std::function<void*(WeakClient)>;
    using OnUserDataDealloc = std::function<void(WeakClient, void*)>;
    using OnServerClose     = std::function<void(void)>;

    /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
    using OnWebSocketOpen = std::function<void(Err, WeakClient, HttpParser const &, HttpBuilder &, uint64_t &)>;

    /** When a message has been received from WebSocket server. */
    using OnWebSocketMessage = std::function<void(Err, WeakClient, WebSocketFrame const &, WebSocketFrame &, uint64_t &)>;

    using UniqueHttpFilterInterface = std::unique_ptr<HttpFilterInterface>;

public:
    struct Filter
    {
        UniqueHttpFilterInterface http_filter;
        OnRequest request_cb;

        Filter(HttpFilterInterface * f, OnRequest const & cb) : http_filter(f), request_cb(cb)
        { /* EMPTY. */ }
        virtual ~Filter()
        { /* EMPTY. */ }
    };

    using SharedFilter = std::shared_ptr<Filter>;
    using Order        = int;
    using FilterMap    = std::multimap<Order, SharedFilter>;
    using FilterPair   = FilterMap::value_type;

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
        Millisec    timeout;
        TimePoint   start_time;
    };

    using SharedClientData = std::shared_ptr<ClientData>;
    using   WeakClientData =   std::weak_ptr<ClientData>;

    using ClientDataMap    = std::unordered_map<Id, SharedClientData>;
    using ClientDataPair   = ClientDataMap::value_type;

private:
    OnOpen             _open_cb;
    OnRequest          _request_cb;
    OnClose            _close_cb;
    OnShutdown         _shutdown_cb;
    OnWrite            _write_cb;
    OnUserDataAlloc    _user_data_alloc_cb;
    OnUserDataDealloc  _user_data_dealloc_cb;
    OnServerClose      _server_close_cb;
    OnWebSocketOpen    _on_web_socket_open;
    OnWebSocketMessage _on_web_socket_message;

private:
    bool _use_websocket;
    FilterMap _filters;

private:
    ClientDataMap _dataset;

public:
    HttpServer(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpServer();

private:
    bool createClientData(Id id);
    bool removeClientData(Id id);
    WeakClientData getClientData(Id id);

public:
    // @formatter:off
    inline void setOnOpen            (OnOpen             const & cb) { _open_cb               = cb; }
    inline void setOnRequest         (OnRequest          const & cb) { _request_cb            = cb; }
    inline void setOnClose           (OnClose            const & cb) { _close_cb              = cb; }
    inline void setOnShutdown        (OnShutdown         const & cb) { _shutdown_cb           = cb; }
    inline void setOnWrite           (OnWrite            const & cb) { _write_cb              = cb; }
    inline void setOnUserDataAlloc   (OnUserDataAlloc    const & cb) { _user_data_alloc_cb    = cb; }
    inline void setOnUserDataDealloc (OnUserDataDealloc  const & cb) { _user_data_dealloc_cb  = cb; }
    inline void setOnServerClose     (OnServerClose      const & cb) { _server_close_cb       = cb; }
    inline void setOnWebSocketOpen   (OnWebSocketOpen    const & cb) { _on_web_socket_open    = cb; }
    inline void setOnWebSocketMessage(OnWebSocketMessage const & cb) { _on_web_socket_message = cb; }
    // @formatter:on

public:
    inline bool isUseWebSocket() const TBAG_NOEXCEPT { return _use_websocket; }
    inline void setUseWebSocket(bool flag = true) TBAG_NOEXCEPT { _use_websocket = flag; }

public:
    void setOnRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setOnRequest(std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setOnRequest(HttpFilterInterface * filter, OnRequest const & cb, Order priority = 0);
    void setOnRequest(SharedFilter filter, Order priority = 0);

public:
    virtual void onConnection(Err code) override;
    virtual void onClientRead(WeakClient node, Err code, ReadPacket const & packet) override;
    virtual void onClientClose(WeakClient node) override;

public:
    virtual void onClientShutdown(WeakClient node, Err code) override;
    virtual void onClientWrite(WeakClient node, Err code) override;

public:
    virtual void * onClientUdataAlloc(WeakClient node) override;
    virtual void   onClientUdataDealloc(WeakClient node, void * data) override;

public:
    virtual void onClose() override;
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

