/**
 * @file   TcpServer.hpp
 * @brief  TcpServer class prototype.
 * @author zer0
 * @date   2017-05-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Server.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/details/TcpClient.hpp>

#include <mutex>
#include <atomic>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// Forward declaration.
class TcpClientNode;
class TcpRealServer;
class TcpServer;

/**
 * TcpClientNode class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpClientNode : public TcpClient
{
private:
    TcpServer & _parent;

private:
    Buffer _buffer;

public:
    TcpClientNode(Loop & loop, TcpServer & parent);
    virtual ~TcpClientNode();

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
 * TcpRealServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpRealServer : public details::NetCommon, public uvpp::Tcp
{
public:
    using AtomicBool = std::atomic_bool;

private:
    TcpServer & _parent;

private:
    AtomicBool _on_connection;

public:
    inline bool isOnConnection() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_on_connection.load()))
    { return _on_connection.load(); }

public:
    TcpRealServer(Loop & loop, TcpServer & parent);
    virtual ~TcpRealServer();

public:
    bool init(String const & ip, int port);

public:
    virtual void onConnection(uerr code) override;
    virtual void onClose() override;
};

/**
 * TcpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpServer : public Server
{
public:
    friend class TcpClientNode;
    friend class TcpRealServer;

public:
    using SharedServer = std::shared_ptr<TcpRealServer>;
    using   WeakServer =   std::weak_ptr<TcpRealServer>;

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
    SharedAsync  _async;
    ClientMap    _clients;

public:
    TcpServer(Loop & loop);
    virtual ~TcpServer();

public:
    // @formatter:off
    inline WeakServer getServer() { Guard g(_mutex); return WeakServer(_server); }
    inline WeakAsync  getAsync () { Guard g(_mutex); return WeakAsync (_async);  }
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
    static SharedClient createClient(Loop & loop, TcpServer & server);
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
    /** Obtain the TCP Network type. */
    virtual Type getType() const override
    { return Type::TCP; }

    /** Obtain the Tcp(server) handle id. */
    virtual Id getId() const override
    { return _server->id(); }

public:
    /**
     * Initialize this method.
     *
     * @param[in] ip
     *      IPv4 or IPv6 address.
     * @param[in] port
     *      Port number.
     *
     * @remarks
     *  init() -> bind() -> listen().
     */
    virtual bool init(String const & ip, int port) override;

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
 * FunctionalTcpServer typedef.
 *
 * @author zer0
 * @date   2017-05-08
 */
using FunctionalTcpServer = FunctionalServer<TcpServer>;

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__

