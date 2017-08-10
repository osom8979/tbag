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

#include <libtbag/container/ReuseQueue.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/http/HttpFilter.hpp>
#include <libtbag/network/http/WsFrameBuffer.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <functional>
#include <atomic>
#include <mutex>

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

    using           Id = ServerInterface::Id;
    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    using StreamNode       = stream::StreamNode;
    using SharedStreamNode = Parent::SharedStreamNode;

    using WsBuffer = WsFrame::WsBuffer;
    using WsQueue  = container::ReuseQueue<WsBuffer>;

    using Loop   = uvpp::Loop;
    using Buffer = std::vector<char>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    TBAG_CONSTEXPR static uint64_t const DEFAULT_CLOSING_TIMEOUT_MILLISECOND = 10 * 1000;

    /**
     * HttpNode class prototype.
     *
     * @author zer0
     * @date   2017-08-09
     */
    class TBAG_API HttpNode : public StreamNode
    {
    private:
        std::atomic_bool _upgrade;
        std::atomic_bool _closing;

    // Receive packet.
    private:
        struct {
            HttpParser      parser; ///< Request packet parser.
            HttpBuilder    builder; ///< Response packet parser.
            WsFrameBuffer receiver; ///< WebSocket frame buffer.
        } __on_read_only__; ///< @warning It should only be used with the onRead() method.

    private:
        WsFrameBuffer _buffer;

    private:
        mutable Mutex _queue_mutex;
        WsQueue _queue;

    public:
        HttpNode(Loop & loop, StreamType type, SharedSafetyAsync async, HttpServer * parent);
        virtual ~HttpNode();

    public:
        inline HttpServer       * getHttpServerPtr()       TBAG_NOEXCEPT { return static_cast<HttpServer*>(getParentPtr()); }
        inline HttpServer const * getHttpServerPtr() const TBAG_NOEXCEPT { return static_cast<HttpServer const *>(getParentPtr()); }

    public:
        inline bool isUpgrade() const TBAG_NOEXCEPT_SP_OP(_upgrade.load()) { return _upgrade.load(); }
        inline bool isClosing() const TBAG_NOEXCEPT_SP_OP(_closing.load()) { return _closing.load(); }

    private:
        Err writeOrEnqueue(char const * buffer, std::size_t size);
        Err writeOrEnqueue(WsFrame const & frame);
        Err writeFromQueue();

    public:
        Err writeText(std::string const & text, bool continuation = false, bool finish = true);
        Err writeBinary(WsBuffer const & binary, bool continuation = false, bool finish = true);
        Err writeHttpResponse(HttpBuilder const & response);
        Err writeString(std::string const & response);

    public:
        Err closeWebSocket(uint16_t status_code, std::string const & reason);
        Err closeWebSocket(WsStatusCode code);

    private:
        /** WebSocket interrupt process (WebSocket Frame). */
        void backWsFrame(Err code, ReadPacket const & packet);

    protected:
        virtual void onShutdown(Err code) final override;
        virtual void onWrite(Err code) final override;
        virtual void onRead(Err code, ReadPacket const & packet) final override;
        virtual void onTimer() final override;
    };

    using SharedHttpNode = std::shared_ptr<HttpNode>;
    using   WeakHttpNode =   std::weak_ptr<HttpNode>;

    /** HTTP packet reference. */
    struct HttpPacket
    {
        HttpParser const & request;
        HttpBuilder & response;

        HttpPacket(HttpParser const & req, HttpBuilder & rsp) : request(req), response(rsp) { /* EMPTY. */ }
        ~HttpPacket() { /* EMPTY. */ }
    };

    /** WebSocket packet information. */
    struct WsPacket
    {
        OpCode opcode;
        char const * buffer;
        std::size_t size;

        WsPacket(OpCode o, char const * b, std::size_t s) : opcode(o), buffer(b), size(s) { /* EMPTY. */ }
        ~WsPacket() { /* EMPTY. */ }
    };

    using OnRequest = std::function<void(WeakClient, Err, HttpPacket&)>;

    /** Request filter. */
    struct Filter
    {
        using UniqueInterface = std::unique_ptr<HttpFilterInterface>;

        UniqueInterface http_filter;
        OnRequest request_cb;

        Filter(HttpFilterInterface * f, OnRequest const & cb) : http_filter(f), request_cb(cb) { /* EMPTY. */ }
        virtual ~Filter() { /* EMPTY. */ }
    };

    using SharedFilter = std::shared_ptr<Filter>;
    using Order        = int;
    using FilterMap    = std::multimap<Order, SharedFilter>;
    using FilterPair   = FilterMap::value_type;

private:
    Mutex _filters_mutex;
    FilterMap _filters;

private:
    std::atomic_bool _websocket;

public:
    HttpServer(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpServer();

public:
    inline bool isEnableWebSocket() const TBAG_NOEXCEPT_SP_OP(_websocket.load())
    { return _websocket.load(); }
    inline void enableWebSocket(bool flag = true) TBAG_NOEXCEPT_SP_OP(_websocket.store(flag))
    { _websocket.store(flag); }

public:
    void setRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setRequest(std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setRequest(HttpFilterInterface * filter, OnRequest const & cb, Order priority = 0);
    void setRequest(SharedFilter filter, Order priority = 0);

public:
    Err writeText(WeakClient & node, std::string const & text, bool continuation = false, bool finish = true);
    Err writeBinary(WeakClient & node, WsBuffer const & buffer, bool continuation = false, bool finish = true);
    Err closeClient(WeakClient & node, uint16_t status_code, std::string const & reason);

public:
    bool isUpgradeWebSocket(SharedHttpNode const & node) const TBAG_NOEXCEPT;
    bool isUpgradeWebSocket(WeakClient const & node) const TBAG_NOEXCEPT;

public:
    SharedFilter findFilter(HttpParser const & request);
    void findAndExecuteTheReadCallback(WeakClient node, Err code, HttpPacket & packet);

protected:
    virtual void onConnection    (Err code) final override;
    virtual void onClientShutdown(WeakClient node, Err code) final override;
    virtual void onClientWrite   (WeakClient node, Err code) final override;
    virtual void onClientRead    (WeakClient node, Err code, ReadPacket const & packet) final override;
    virtual void onClientClose   (WeakClient node) final override;
    virtual void onClientTimer   (WeakClient node) final override;

protected:
    virtual SharedStreamNode createClient(StreamType type,
                                          Loop & loop,
                                          SharedServerBackend & server,
                                          SharedSafetyAsync & async) final override;

// ------------------------
// User's callback methods.
// ------------------------

public:
    virtual void onServerClose() override
    { /* EMPTY. */ }

    virtual void * onClientUdataAlloc(WeakClient node) override
    { return nullptr; }

    virtual void onClientUdataDealloc(WeakClient node, void * data) override
    { /* EMPTY. */ }

public:
    virtual void onHttpOpen    (WeakClient node) { /* EMPTY. */ }
    virtual void onHttpRequest (WeakClient node, Err code, HttpPacket & packet) { /* EMPTY. */ }
    virtual void onHttpClose   (WeakClient node) { /* EMPTY. */ }
    virtual void onHttpTimer   (WeakClient node) { /* EMPTY. */ }
    virtual void onHttpShutdown(WeakClient node, Err code) { /* EMPTY. */ }
    virtual void onHttpWrite   (WeakClient node, Err code) { /* EMPTY. */ }

public:
    /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
    virtual void onWsOpen(WeakClient node, Err code, HttpPacket & packet) { /* EMPTY. */ }

    /** When a message has been received from WebSocket server. */
    virtual void onWsMessage(WeakClient node, OpCode op, char const * buffer, std::size_t size) { /* EMPTY. */ }
};

/**
 * FunctionalHttpServer class prototype.
 *
 * @author zer0
 * @date   2017-06-30
 * @date   2017-08-09 (Rename: FunctionalHttpServer -> FuncHttpServer)
 */
struct FuncHttpServer : public HttpServer
{
    using StreamType = HttpServer::StreamType;

    using WeakClient = HttpServer::WeakClient;
    using HttpPacket = HttpServer::HttpPacket;
    using WsPacket   = HttpServer::WsPacket;

    using OnServerClose        = std::function<void ()>;
    using OnClientUdataAlloc   = std::function<void*(WeakClient)>;
    using OnClientUdataDealloc = std::function<void (WeakClient, void*)>;
    using OnHttpOpen           = std::function<void (WeakClient)>;
    using OnHttpRequest        = std::function<void (WeakClient, Err, HttpPacket&)>;
    using OnHttpClose          = std::function<void (WeakClient)>;
    using OnHttpTimer          = std::function<void (WeakClient)>;
    using OnHttpShutdown       = std::function<void (WeakClient, Err)>;
    using OnHttpWrite          = std::function<void (WeakClient, Err)>;
    using OnWsOpen             = std::function<void (WeakClient, Err, HttpPacket&)>;
    using OnWsMessage          = std::function<void (WeakClient, OpCode, char const*, std::size_t)>;

    OnServerClose server_close_cb;

    OnClientUdataAlloc   client_udata_alloc_cb;
    OnClientUdataDealloc client_udata_dealloc_cb;

    OnHttpOpen     http_open_cb;
    OnHttpRequest  http_request_cb;
    OnHttpClose    http_close_cb;
    OnHttpTimer    http_timer_cb;
    OnHttpShutdown http_shutdown_cb;
    OnHttpWrite    http_write_cb;

    OnWsOpen    ws_open_cb;
    OnWsMessage ws_message_cb;

    FuncHttpServer(Loop & loop, StreamType type = StreamType::TCP) : HttpServer(loop, type)
    { /* EMPTY. */ }
    virtual ~FuncHttpServer()
    { /* EMPTY. */ }

    void setOnServerClose(OnServerClose const & cb) { server_close_cb = cb; }

    void setOnClientUdataAlloc  (OnClientUdataAlloc   const & cb) {   client_udata_alloc_cb = cb; }
    void setOnClientUdataDealloc(OnClientUdataDealloc const & cb) { client_udata_dealloc_cb = cb; }

    void setOnHttpOpen    (OnHttpOpen     const & cb) {     http_open_cb = cb; }
    void setOnHttpRequest (OnHttpRequest  const & cb) {  http_request_cb = cb; }
    void setOnHttpClose   (OnHttpClose    const & cb) {    http_close_cb = cb; }
    void setOnHttpTimer   (OnHttpTimer    const & cb) {    http_timer_cb = cb; }
    void setOnHttpShutdown(OnHttpShutdown const & cb) { http_shutdown_cb = cb; }
    void setOnHttpWrite   (OnHttpWrite    const & cb) {    http_write_cb = cb; }

    void setOnWsOpen   (OnWsOpen    const & cb) {    ws_open_cb = cb; }
    void setOnWsMessage(OnWsMessage const & cb) { ws_message_cb = cb; }

    virtual void onServerClose() override
    { if (static_cast<bool>(server_close_cb)) { server_close_cb(); } }

    virtual void * onClientUdataAlloc(WeakClient node) override
    { if (static_cast<bool>(client_udata_alloc_cb)) { return client_udata_alloc_cb(node); } return nullptr; }
    virtual void onClientUdataDealloc(WeakClient node, void * data) override
    { if (static_cast<bool>(client_udata_dealloc_cb)) { client_udata_dealloc_cb(node, data); } }

    virtual void onHttpOpen(WeakClient node) override
    { if (static_cast<bool>(http_open_cb)) { http_open_cb(node); } }
    virtual void onHttpRequest(WeakClient node, Err code, HttpPacket & packet) override
    { if (static_cast<bool>(http_request_cb)) { http_request_cb(node, code, packet); } }
    virtual void onHttpClose(WeakClient node) override
    { if (static_cast<bool>(http_close_cb)) { http_close_cb(node); } }
    virtual void onHttpTimer(WeakClient node) override
    { if (static_cast<bool>(http_timer_cb)) { http_timer_cb(node); } }
    virtual void onHttpShutdown(WeakClient node, Err code) override
    { if (static_cast<bool>(http_shutdown_cb)) { http_shutdown_cb(node, code); } }
    virtual void onHttpWrite(WeakClient node, Err code) override
    { if (static_cast<bool>(http_write_cb)) { http_write_cb(node, code); } }

    virtual void onWsOpen(WeakClient node, Err code, HttpPacket & packet) override
    { if (static_cast<bool>(ws_open_cb)) { ws_open_cb(node, code, packet); } }
    virtual void onWsMessage(WeakClient node, OpCode op, char const * buffer, std::size_t size) override
    { if (static_cast<bool>(ws_message_cb)) { ws_message_cb(node, op, buffer, size); } }
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

