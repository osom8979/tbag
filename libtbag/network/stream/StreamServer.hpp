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
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
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
    using Id     = ClientInterface::Id;

    using SharedClient = ServerInterface::SharedClient;
    using   WeakClient = ServerInterface::WeakClient;

    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync =   std::weak_ptr<SafetyAsync>;

    using SharedServerBackend = std::shared_ptr<Stream>;
    using   WeakServerBackend =   std::weak_ptr<Stream>;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    struct Internal;
    friend struct Internal;

public:
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    StreamType const STREAM_TYPE;

private:
    UniqueInternal _internal;
    mutable Mutex _mutex;

public:
    StreamServer(Loop & loop, StreamType type);
    virtual ~StreamServer();

public:
    bool isOnConnection() const;
    bool emptyClients() const;
    std::size_t sizeClients() const;

    WeakServerBackend getServer();
    WeakSafetyAsync getAsync();

public:
    virtual std::string dest() const override;
    virtual int         port() const override;

public:
    /** Initialize this class. */
    virtual Err init(char const * destination, int port = 0) override;

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
     * @param[in] id Node id.
     */
    virtual WeakClient get(Id id) override;

    /**
     * Remove client in buffer(map).
     *
     * @param[in] id Node id.
     */
    virtual Err remove(Id id) override;

public:
    virtual void backConnection(Err code) override;
    virtual void backClose() override;
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

