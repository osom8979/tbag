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
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// Forward declaration.
class TcpRealServer;
class TcpServer;

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
    SharedServer _server;
    SharedAsync  _async;

private:
    mutable Mutex _client_mutex;
    ClientMap _clients;

public:
    TcpServer(Loop & loop);
    virtual ~TcpServer();

public:
    inline bool emptyClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.empty()))
    { Guard g(_client_mutex); return _clients.empty(); }
    inline Size sizeClients() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_clients.size()))
    { Guard g(_client_mutex); return _clients.size(); }

private:
    SharedClient createClient();
    bool insertClient(SharedClient node);
    bool removeClient(ClientKey key);

private:
    void closeAll();

public:
    template <typename Predicated>
    void foreach(Predicated predicated)
    {
        Guard guard(_client_mutex);
        for (auto & cursor : _clients) {
            predicated(cursor);
        }
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
 * FunctionalTcpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-08
 */
struct FunctionalTcpServer : public TcpServer
{
    // @formatter:off
    using uerr = NetCommon::uerr;
    using Size = NetCommon::Size;
    using WeakClient = Server::WeakClient;

    using OnConnection  = std::function<void(uerr)>;
    using OnClientWrite = std::function<void(WeakClient, uerr)>;
    using OnClientRead  = std::function<void(WeakClient, uerr, char const *, Size)>;
    using OnClientClose = std::function<void(WeakClient)>;
    using OnServerClose = std::function<void(void)>;

    OnConnection  connection_cb;
    OnClientWrite client_write_cb;
    OnClientRead  client_read_cb;
    OnClientClose client_close_cb;
    OnServerClose server_close_cb;

    FunctionalTcpServer(Loop & loop) : TcpServer(loop)
    { /* EMPTY */ }
    virtual ~FunctionalTcpServer()
    { /* EMPTY */ }

    inline void setOnConnection (OnConnection  const & cb) { connection_cb   = cb; }
    inline void setOnClientWrite(OnClientWrite const & cb) { client_write_cb = cb; }
    inline void setOnClientRead (OnClientRead  const & cb) { client_read_cb  = cb; }
    inline void setOnClientClose(OnClientClose const & cb) { client_close_cb = cb; }
    inline void setOnServerClose(OnServerClose const & cb) { server_close_cb = cb; }

    virtual void onConnection(uerr code) override
    { if (connection_cb) { connection_cb(code); } }
    virtual void onClientWrite(WeakClient node, uerr code) override
    { if (client_write_cb) { client_write_cb(node, code); } }
    virtual void onClientRead(WeakClient node, uerr code, char const * buffer, Size size) override
    { if (client_read_cb) { client_read_cb(node, code, buffer, size); } }
    virtual void onClientClose(WeakClient node) override
    { if (client_close_cb) { client_close_cb(node); } }
    virtual void onServerClose() override
    { if (server_close_cb) { server_close_cb(); } }
    // @formatter:on
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__

