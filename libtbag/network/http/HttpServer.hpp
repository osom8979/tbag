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
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/http/HttpFilter.hpp>
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
    using StreamType = details::StreamType;
    using Parent     = stream::StreamServer;

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

    using OnOpen    = std::function<void(WeakClient)>;
    using OnRequest = std::function<void(Err, WeakClient, HttpParser const &, HttpBuilder &, uint64_t &)>;
    using OnClose   = std::function<void(WeakClient)>;
    using OnServerClose = std::function<void(void)>;

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
        Buffer      buffer;
        HttpBuilder builder;
        HttpParser  parser;
        Millisec    timeout;
        TimePoint   start_time;
    };

private:
    OnOpen        _open_cb;
    OnRequest     _request_cb;
    OnClose       _close_cb;
    OnServerClose _server_close_cb;

    FilterMap _filters;

public:
    HttpServer(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpServer();

public:
    // @formatter:off
    inline void setOnOpen       (OnOpen        const & cb) { _open_cb         = cb; }
    inline void setOnRequest    (OnRequest     const & cb) { _request_cb      = cb; }
    inline void setOnClose      (OnClose       const & cb) { _close_cb        = cb; }
    inline void setOnServerClose(OnServerClose const & cb) { _server_close_cb = cb; }
    // @formatter:on

public:
    void setOnRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setOnRequest(std::string const & regex_path, OnRequest const & cb, Order priority = 0);
    void setOnRequest(HttpFilterInterface * filter, OnRequest const & cb, Order priority = 0);
    void setOnRequest(SharedFilter filter, Order priority = 0);

public:
    virtual void onConnection(Err code) override;
    virtual void onClientRead(WeakClient node, Err code, char const * buffer, std::size_t size) override;
    virtual void onClientClose(WeakClient node) override;

public:
    virtual void onClientShutdown(WeakClient node, Err code) override;
    virtual void onClientWrite(WeakClient node, Err code) override;
    virtual void onServerClose() override;

public:
    virtual void * onClientUserDataAlloc(WeakClient node) override;
    virtual void onClientUserDataDealloc(WeakClient node, void * data) override;
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPSERVER_HPP__

