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
#include <unordered_map>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

// Forward declaration.
class TcpRealNode;
class TcpRealServer;
class TcpServer;

/**
 * TcpRealNode class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpRealNode : public TcpClient
{
private:
    TcpServer & _parent;

private:
    Buffer _buffer;

public:
    TcpRealNode(Loop & loop, TcpServer & parent);
    virtual ~TcpRealNode();

public:
    virtual void onWrite(uerr code) override;
    virtual void onRead (uerr code, char const * buffer, Size size) override;
    virtual void onClose() override;
};

/**
 * TcpRealServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpRealServer : public details::NetCommon, public uvpp::Tcp
{
private:
    TcpServer & _parent;

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

    using SharedNode = std::shared_ptr<TcpRealNode>;
    using   WeakNode =   std::weak_ptr<TcpRealNode>;

public:
    using NodeKey  = Id;
    using NodeMap  = std::unordered_map<NodeKey, SharedNode>;
    using NodePair = NodeMap::value_type;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    SharedServer _server;
    SharedAsync  _async;

private:
    mutable Mutex _node_mutex;
    NodeMap _nodes;

public:
    TcpServer(Loop & loop);
    virtual ~TcpServer();

public:
    inline bool emptyNode() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_nodes.empty()))
    { Guard g(_node_mutex); return _nodes.empty(); }
    inline Size sizeNode() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_nodes.size()))
    { Guard g(_node_mutex); return _nodes.size(); }

private:
    SharedNode createNode();
    bool insertNode(SharedNode node);
    bool removeNode(NodeKey key);

public:
    template <typename Predicated>
    void foreach(Predicated predicated)
    {
        Guard guard(_node_mutex);
        for (auto & cursor : _nodes) {
            predicated(cursor);
        }
    }

public:
    virtual Type getType() const override
    { return Type::TCP; }
    virtual Id getId() const override
    { return _server->id(); }

public:
    /**
     * Initialize this method.
     *
     * @param[in] ip
     *      Ip address.
     * @param[in] port
     *      Port number.
     *
     * @remarks
     *  init -> bind -> listen.
     */
    virtual bool init(String const & ip, int port) override;

    virtual NodeInterface * accept() override;

    /** This operation is async. */
    virtual bool close() override;
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
    using NodeInterface = Server::NodeInterface;

    using OnConnection  = std::function<void(uerr)>;
    using OnClientWrite = std::function<void(NodeInterface *, uerr)>;
    using OnClientRead  = std::function<void(NodeInterface *, uerr, char const *, Size)>;
    using OnClientClose = std::function<void(NodeInterface *)>;
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
    virtual void onClientWrite(NodeInterface * node, uerr code) override
    { if (client_write_cb) { client_write_cb(node, code); } }
    virtual void onClientRead(NodeInterface * node, uerr code, char const * buffer, Size size) override
    { if (client_read_cb) { client_read_cb(node, code, buffer, size); } }
    virtual void onClientClose(NodeInterface * node) override
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

