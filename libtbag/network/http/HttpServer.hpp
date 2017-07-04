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
#include <libtbag/network/http/HttpCacheData.hpp>
#include <libtbag/network/http/WebSocketFrame.hpp>
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

public:
    TBAG_CONSTEXPR static uint64_t const DEFAULT_WRITE_TIMEOUT_MILLISECOND = 5 * 1000;

public:
    struct HttpPacket
    {
        HttpParser const & request;
        HttpBuilder & response;
        uint64_t timeout;

        HttpPacket(HttpParser const & req, HttpBuilder & rsp, uint64_t t) : request(req), response(rsp), timeout(t)
        { /* EMPTY. */ }
        ~HttpPacket()
        { /* EMPTY. */ }
    };

    struct WsPacket
    {
        OpCode op;
        char const * buffer;
        std::size_t size;

        WsPacket(OpCode o, char const * b, std::size_t s) : op(o), buffer(b), size(s)
        { /* EMPTY. */ }
        ~WsPacket()
        { /* EMPTY. */ }
    };

    using WC = WeakClient;
    using HP = HttpPacket;
    using WP = WsPacket;

    using OnRequest = std::function<void(WC, Err, HP&)>;

public:
    struct Callback
    {
        virtual void onOpen(WC client)
        { /* EMPTY. */ }

        virtual void onRequest(WC client, Err code, HP & packet)
        { /* EMPTY. */ }

        virtual void onClose(WC client)
        { /* EMPTY. */ }

        virtual void onShutdown(WC client, Err code)
        { /* EMPTY. */ }

        virtual void onWrite(WC client, Err code)
        { /* EMPTY. */ }

        /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
        virtual void onWsOpen(WC client, Err code, HP & packet)
        { /* EMPTY. */ }

        /** When a message has been received from WebSocket server. */
        virtual void onWsMessage(WC client, Err code, WP & packet)
        { /* EMPTY. */ }

        virtual void * onUdataAlloc(WC client)
        { return nullptr; }

        virtual void onUdataDealloc(WC client, void * udata)
        { /* EMPTY. */ }

        virtual void onServerClose()
        { /* EMPTY. */ }
    };

public:
    struct Filter
    {
        using UniqueHttpFilterInterface = std::unique_ptr<HttpFilterInterface>;

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
    using       CacheData = HttpCacheData;
    using SharedCacheData = std::shared_ptr<CacheData>;
    using   WeakCacheData =   std::weak_ptr<CacheData>;

    using CacheDataMap  = std::unordered_map<Id, SharedCacheData>;
    using CacheDataPair = CacheDataMap::value_type;

private:
    Callback * _callback;
    bool _use_websocket;

private:
    MaskingDevice _masking;
    FilterMap     _filters;
    CacheDataMap  _cache_map;

public:
    HttpServer(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpServer();

private:
    bool createCacheData(Id id);
    bool removeCacheData(Id id);
    WeakCacheData getCacheData(Id id);

public:
    inline void setCallback(Callback * cb) TBAG_NOEXCEPT { _callback = cb; }
    inline bool isUseWebSocket() const TBAG_NOEXCEPT { return _use_websocket; }
    inline void setUseWebSocket(bool flag = true) TBAG_NOEXCEPT { _use_websocket = flag; }

public:
    void setOnRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setOnRequest(std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setOnRequest(HttpFilterInterface * filter, OnRequest const & cb, Order priority = 0);
    void setOnRequest(SharedFilter filter, Order priority = 0);

private:
    bool isUpgradeWebSocket(CacheData const & client_data) const TBAG_NOEXCEPT;

    /** WebSocket interrupt process (HTTP Request). */
    void runWebSocketOpen(SharedClient node, Err code, ReadPacket const & packet, CacheData & client_data);

    /** WebSocket interrupt process (WebSocket Frame). */
    void runWebSocketRead(SharedClient node, Err code, ReadPacket const & packet, CacheData & client_data);

    /** Regular HTTP process. */
    void runHttpRead(SharedClient node, Err code, ReadPacket const & packet, CacheData & client_data);

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

/**
 * FunctionalHttpServer class prototype.
 *
 * @author zer0
 * @date   2017-06-30
 */
struct FunctionalHttpServer : public HttpServer, public HttpServer::Callback
{
    using StreamType = HttpServer::StreamType;

    using WC = HttpServer::WC;
    using HP = HttpServer::HP;
    using WP = HttpServer::WP;

    using OnOpen         = std::function<void(WC)>;
    using OnRequest      = HttpServer::OnRequest;
    using OnClose        = std::function<void(WC)>;
    using OnShutdown     = std::function<void(WC, Err)>;
    using OnWrite        = std::function<void(WC client, Err)>;
    using OnWsOpen       = std::function<void(WC, Err, HP&)>;
    using OnWsMessage    = std::function<void(WC, Err, WP&)>;
    using OnUdataAlloc   = std::function<void*(WC)>;
    using OnUdataDealloc = std::function<void(WC, void*)>;
    using OnServerClose  = std::function<void(void)>;

    OnOpen                  open_cb;
    OnRequest            request_cb;
    OnClose                close_cb;
    OnShutdown          shutdown_cb;
    OnWrite                write_cb;
    OnWsOpen             ws_open_cb;
    OnWsMessage       ws_message_cb;
    OnUdataAlloc     udata_alloc_cb;
    OnUdataDealloc udata_dealloc_cb;
    OnServerClose   server_close_cb;

    FunctionalHttpServer(Loop & loop, StreamType type = StreamType::TCP) : HttpServer(loop, type)
    { setCallback(this); }
    virtual ~FunctionalHttpServer()
    { /* EMPTY. */ }

    void setOnOpen          (OnOpen         const & cb) {          open_cb = cb; }
    void setOnDefaultRequest(OnRequest      const & cb) {       request_cb = cb; }
    void setOnClose         (OnClose        const & cb) {         close_cb = cb; }
    void setOnShutdown      (OnShutdown     const & cb) {      shutdown_cb = cb; }
    void setOnWrite         (OnWrite        const & cb) {         write_cb = cb; }
    void setOnWsOpen        (OnWsOpen       const & cb) {       ws_open_cb = cb; }
    void setOnWsMessage     (OnWsMessage    const & cb) {    ws_message_cb = cb; }
    void setOnUdataAlloc    (OnUdataAlloc   const & cb) {   udata_alloc_cb = cb; }
    void setOnUdataDealloc  (OnUdataDealloc const & cb) { udata_dealloc_cb = cb; }
    void setOnServerClose   (OnServerClose  const & cb) {  server_close_cb = cb; }

    virtual void onOpen(WC client) override
    { if (static_cast<bool>(open_cb)) { open_cb(client); } }
    virtual void onRequest(WC client, Err code, HP & packet) override
    { if (static_cast<bool>(request_cb)) { request_cb(client, code, packet); } }
    virtual void onClose(WC client) override
    { if (static_cast<bool>(close_cb)) { close_cb(client); } }
    virtual void onShutdown(WC client, Err code) override
    { if (static_cast<bool>(shutdown_cb)) { shutdown_cb(client, code); } }
    virtual void onWrite(WC client, Err code) override
    { if (static_cast<bool>(write_cb)) { write_cb(client, code); } }
    virtual void onWsOpen(WC client, Err code, HP & packet) override
    { if (static_cast<bool>(ws_open_cb)) { ws_open_cb(client, code, packet); } }
    virtual void onWsMessage(WC client, Err code, WP & packet) override
    { if (static_cast<bool>(ws_message_cb)) { ws_message_cb(client, code, packet); } }
    virtual void * onUdataAlloc(WC client) override
    { if (static_cast<bool>(udata_alloc_cb)) { return udata_alloc_cb(client); } return nullptr; }
    virtual void onUdataDealloc(WC client, void * udata) override
    { if (static_cast<bool>(udata_dealloc_cb)) { udata_dealloc_cb(client, udata); } }
    virtual void onServerClose() override
    { if (static_cast<bool>(server_close_cb)) { server_close_cb(); } }
};

using FuncHttpServer = FunctionalHttpServer;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

