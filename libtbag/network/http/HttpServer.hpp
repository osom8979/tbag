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

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/stream/StreamServer.hpp>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpReader.hpp>
#include <libtbag/network/http/HttpFilter.hpp>
#include <libtbag/network/http/ws/WsFrameBuffer.hpp>
#include <libtbag/network/Uri.hpp>

#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/container/ReuseQueue.hpp>
#include <libtbag/lock/RwLock.hpp>
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
 * @date   2017-10-16 (Refactoring)
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

    using SharedStreamNode = Parent::SharedStreamNode;

    using Loop         = uvpp::Loop;
    using Buffer       = util::Buffer;
    using WsFrame      = ws::WsFrame;
    using WsStatus     = ws::WsStatus;
    using WsStatusCode = ws::WsStatusCode;

    using RwLock     = lock::RwLock;
    using ReadGuard = lock::ReadLockGuard;
    using WriteGuard = lock::WriteLockGuard;

    /**
     * HttpNode class prototype.
     *
     * @author zer0
     * @date   2017-08-09
     * @date   2017-10-16 (Refactoring)
     */
    class TBAG_API HttpNode : public stream::StreamNode, public HttpReaderInterface
    {
    private:
        HttpReaderForCallback<HttpNode> _reader;

    public:
        HttpNode(Loop & loop, StreamType type, HttpServer * parent);
        virtual ~HttpNode();

    public:
        inline HttpServer       * getHttpServerPtr()       TBAG_NOEXCEPT { return static_cast<HttpServer       *>(getParentPtr()); }
        inline HttpServer const * getHttpServerPtr() const TBAG_NOEXCEPT { return static_cast<HttpServer const *>(getParentPtr()); }

    public:
        Err writeResponse(HttpResponse const & response);
        Err writeWsResponse(HttpRequest const & request, HttpResponse const & response);
        Err writeWsFrame(WsFrame const & frame);

    public:
        Err writeText(char const * buffer, std::size_t size, bool finish = true);
        Err writeText(std::string const & text, bool finish = true);

        Err writeBinary(char const * buffer, std::size_t size, bool finish = true);
        Err writeBinary(util::Buffer const & binary, bool finish = true);

        Err writeClose(WsStatus const & status);
        Err writeClose(WsStatusCode code);
        Err writeClose();

    public:
        virtual void onRead(Err code, ReadPacket const & packet) override;

    public:
        virtual void onContinue(void * arg) override;
        virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override;
        virtual void onWsMessage(ws::WsOpCode opcode, util::Buffer const & payload, void * arg) override;
        virtual void onRegularHttp(HttpProperty const & property, void * arg) override;
        virtual void onParseError(Err code, void * arg) override;
    };

    using SharedHttpNode = std::shared_ptr<HttpNode>;
    using   WeakHttpNode =   std::weak_ptr<HttpNode>;

    using OnRequest = std::function<void(WeakClient, HttpRequest const &)>;

    /**
     * HttpFilter class prototype.
     *
     * @author zer0
     * @date   2017-10-16
     */
    struct HttpFilter
    {
        using WeakClient   = details::ServerInterface::WeakClient;
        using SharedFilter = std::shared_ptr<HttpFilterInterface>;

        SharedFilter filter;
        OnRequest    request_cb;

        HttpFilter() : filter(), request_cb()
        { /* EMPTY. */ }
        HttpFilter(SharedFilter const & f, OnRequest const & cb) : filter(f), request_cb(cb)
        { /* EMPTY. */ }
        ~HttpFilter()
        { /* EMPTY. */ }
    };

    using HttpFilterMap = std::multimap<int, HttpFilter>;

private:
    RwLock _filters_mutex;
    HttpFilterMap _filters;

public:
    HttpServer(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpServer();

public:
    void setRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, int priority = 0);
    void setRequest(std::regex const & regex_path, OnRequest const & cb, int priority = 0);
    void setRequest(HttpFilter::SharedFilter const & filter, OnRequest const & cb, int priority = 0);
    void setRequest(HttpFilter const & filter, int priority = 0);

public:
    static std::shared_ptr<HttpNode> toSharedNode(WeakClient & node) TBAG_NOEXCEPT_SP_OP(castSharedClient<HttpNode>(node))
    { return castSharedClient<HttpNode>(node); }

public:
    Err writeResponse  (WeakClient & node, HttpResponse const & response);
    Err writeWsResponse(WeakClient & node, HttpRequest const & request, HttpResponse const & response);
    Err writeWsFrame   (WeakClient & node, WsFrame const & frame);

public:
    Err writeText(WeakClient & node, char const * buffer, std::size_t size, bool finish = true);
    Err writeText(WeakClient & node, std::string const & text, bool finish = true);

    Err writeBinary(WeakClient & node, char const * buffer, std::size_t size, bool finish = true);
    Err writeBinary(WeakClient & node, util::Buffer const & buffer, bool finish = true);

    Err writeClose(WeakClient & node, WsStatus const & status);
    Err writeClose(WeakClient & node, WsStatusCode code);
    Err writeClose(WeakClient & node);

public:
    HttpFilter findFilter(HttpRequest const & request);
    void findAndExecute(WeakClient node, HttpRequest const & request);

private:
    virtual void onClientRead(WeakClient node, Err code, ReadPacket const & packet) override;

protected:
    virtual void onConnection(Err code) override;

protected:
    virtual SharedStreamNode createClient(StreamType type, Loop & loop, SharedStream & server) override;

public:
    virtual void onClientEof(WeakClient node);
    virtual void onClientReadError(WeakClient node, Err code);
    virtual void onClientParseError(WeakClient node, Err code);

public:
    virtual void onClientOpen(WeakClient node) { /* EMPTY. */ }
    virtual void onClientContinue(WeakClient node) { /* EMPTY. */ }
    virtual bool onClientSwitchingProtocol(WeakClient node, HttpRequest const & property) { return true; }
    virtual void onClientWsMessage(WeakClient node, ws::WsOpCode opcode, util::Buffer const & payload) { /* EMPTY. */ }
    virtual void onClientRequest(WeakClient node, HttpRequest const & request) { /* EMPTY. */ }
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

