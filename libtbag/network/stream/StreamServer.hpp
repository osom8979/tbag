/**
 * @file   StreamServer.hpp
 * @brief  StreamServer class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/id/Id.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/ex/SafetyWriteAsync.hpp>
#include <libtbag/network/details/FunctionalNet.hpp>

#include <unordered_map>
#include <string>
#include <atomic>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamServer class prototype.
 *
 * @author zer0
 * @date   2017-05-10
 */
class TBAG_API StreamServer : public details::ServerInterface
{
public:
    using StreamType      = details::StreamType;
    using ClientInterface = details::ClientInterface;
    using ServerInterface = details::ServerInterface;

    using Loop   = uvpp::Loop;
    using Stream = uvpp::Stream;

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

public:
    using SafetyAsync      = uvpp::ex::SafetyAsync;
    using SafetyWriteAsync = uvpp::ex::SafetyWriteAsync;

    using SharedServerBackend = std::shared_ptr<Stream>;
    using   WeakServerBackend =   std::weak_ptr<Stream>;

    using SharedSafetyWriteAsync = std::shared_ptr<SafetyWriteAsync>;
    using   WeakSafetyWriteAsync =   std::weak_ptr<SafetyWriteAsync>;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using ClientMap  = std::unordered_map<Id, SharedClient>;
    using ClientPair = ClientMap::value_type;

    using AtomicBool = std::atomic_bool;

    using Id = id::Id;

private:
    StreamType const STREAM_TYPE;

private:
    SharedServerBackend     _server;
    SharedSafetyWriteAsync  _async;
    ClientMap               _clients;

private:
    std::string _destination;
    int _port;

private:
    AtomicBool _on_connection;
    mutable Mutex _mutex;

public:
    StreamServer(Loop & loop, StreamType type);
    virtual ~StreamServer();

public:
    // @formatter:off
    inline bool isOnConnection() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_on_connection.load()))
    { return _on_connection.load(); }
    inline bool emptyClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.empty()))
    { Guard g(_mutex); return _clients.empty(); }
    inline std::size_t sizeClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.size()))
    { Guard g(_mutex); return _clients.size(); }
    // @formatter:on

public:
    WeakServerBackend getServer();
    WeakSafetyWriteAsync getAsync();

private:
    SharedClient createClient();
    SharedClient getSharedClient(Id id);

    bool insertClient(SharedClient client);
    bool removeClient(Id id);

private:
    void closeAll();

public:
    /** Initialize this class. */
    virtual bool init(char const * destination, int port = 0) override;

    /**
     * Safety close() operation.
     *
     * @remarks
     *  Select sync/async operations automatically according to Thread ID.
     */
    virtual void close() override;

    /**
     * Accept client.
     *
     * @warning
     *  This operation can only used with the onConnection() method.
     */
    virtual WeakClient accept() override;

    /**
     * Obtain client.
     *
     * @param[in] id  Node id.
     */
    virtual WeakClient getClient(Id id) override;

public:
    virtual char const * getDestination() const override;
    virtual int getPort() const override;

public:
    virtual void runBackendConnection(Err code) override;
    virtual void runBackendClose() override;

public:
    template <typename Predicated>
    void foreach(Predicated predicated)
    {
        Guard guard(_mutex);
        for (auto & cursor : _clients) {
            predicated(cursor);
        }
    }

    template <typename Predicated>
    void updateClients(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_clients);
    }
};

/**
 * TcpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
struct TcpServer : public stream::StreamServer
{
    TcpServer(Loop & loop) : stream::StreamServer(loop, StreamType::TCP)
    { /* EMPTY. */ }
};

/**
 * PipeServer class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
struct PipeServer : public stream::StreamServer
{
    PipeServer(Loop & loop) : stream::StreamServer(loop, StreamType::PIPE)
    { /* EMPTY. */ }
};

using FunctionalTcpServer  = details::FunctionalServer<TcpServer>;
using FunctionalPipeServer = details::FunctionalServer<PipeServer>;

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__

