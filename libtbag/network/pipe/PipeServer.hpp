/**
 * @file   PipeServer.hpp
 * @brief  PipeServer class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPESERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPESERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Server.hpp>
#include <libtbag/network/pipe/PipeClient.hpp>

#include <mutex>
#include <atomic>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace pipe    {

// Forward declaration.
class PipeClientNode;
class PipeRealServer;
class PipeServer;

/**
 * PipeClientNode class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
class TBAG_API PipeClientNode : public PipeClient
{
private:
    PipeServer & _parent;

private:
    Buffer _buffer;

public:
    PipeClientNode(Loop & loop, PipeServer & parent);
    virtual ~PipeClientNode();

private:
    Server::WeakClient getWeakClient();

public:
    virtual void onShutdown(uerr code) override;
    virtual void onWrite   (uerr code) override;
    virtual void onRead    (uerr code, char const * buffer, Size size) override;
    virtual void onClose   () override;

public:
    virtual void * onUserDataAlloc() override;
    virtual void onUserDataDealloc(void * data) override;
};

/**
 * PipeRealServer class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
class TBAG_API PipeRealServer : public details::NetCommon, public uvpp::Pipe
{
public:
    using AtomicBool = std::atomic_bool;

private:
    PipeServer & _parent;

private:
    AtomicBool _on_connection;

public:
    inline bool isOnConnection() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_on_connection.load()))
    { return _on_connection.load(); }

public:
    PipeRealServer(Loop & loop, PipeServer & parent);
    virtual ~PipeRealServer();

public:
    bool init(String const & path, int unused = 0);

public:
    virtual void onConnection(uerr code) override;
    virtual void onClose() override;
};

/**
 * PipeServer class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
class TBAG_API PipeServer : public Server
{
public:
    friend class PipeClientNode;
    friend class PipeRealServer;

public:
    using SharedServer = std::shared_ptr<PipeRealServer>;
    using   WeakServer =   std::weak_ptr<PipeRealServer>;

public:
    using ClientKey  = Id;
    using ClientMap  = std::unordered_map<ClientKey, SharedClient>;
    using ClientPair = ClientMap::value_type;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    mutable Mutex _mutex;

    SharedServer _server;
    SharedSafetyWriteAsync  _async;
    ClientMap    _clients;

public:
    PipeServer(Loop & loop);
    virtual ~PipeServer();

public:
    // @formatter:off
    inline WeakServer getServer() { Guard g(_mutex); return WeakServer(_server); }
    inline SharedSafetyWriteAsync  getAsync () { Guard g(_mutex); return SharedSafetyWriteAsync (_async);  }
    // @formatter:on

public:
    // @formatter:off
    inline bool emptyClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.empty()))
    { Guard g(_mutex); return _clients.empty(); }
    inline Size sizeClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.size()))
    { Guard g(_mutex); return _clients.size(); }
    // @formatter:on

public:
    // @formatter:off
    inline void lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.lock()))
    { _mutex.lock(); }
    inline void unlock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.unlock()))
    { _mutex.unlock(); }
    inline bool try_lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.try_lock()))
    { return _mutex.try_lock(); }
    // @formatter:on

private:
    static SharedClient createClient(Loop & loop, PipeServer & server);
    SharedClient createClient();

private:
    SharedClient getSharedClient(ClientKey key);
    WeakClient getWeakClient(ClientKey key);

    bool insertClient(SharedClient node);
    bool removeClient(ClientKey key);

private:
    void closeAll();

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

public:
    /** Obtain the PIPE Network type. */
    virtual Type getType() const override
    { return Type::PIPE; }

    /** Obtain the Pipe(server) handle id. */
    virtual Id getId() const override
    { return _server->id(); }

public:
    /**
     * Initialize this method.
     *
     * @param[in] path
     *      Pipe file path.
     * @param[in] unused
     *      Unused parameter.
     *
     * @remarks
     *  init() -> bind() -> listen().
     */
    virtual bool init(String const & path, int unused = 0) override;

    /**
     * Accept client.
     *
     * @warning
     *  This operation can only used with the onConnection() method.
     */
    virtual WeakClient accept() override;

    /**
     * Safety close() operation.
     *
     * @remarks
     *  Select sync/async operations automatically according to Thread ID.
     */
    virtual void close() override;
};

/**
 * FunctionalPipeServer typedef.
 *
 * @author zer0
 * @date   2017-05-09
 */
using FunctionalPipeServer = FunctionalServer<PipeServer>;

} // namespace pipe
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_PIPE_PIPESERVER_HPP__

