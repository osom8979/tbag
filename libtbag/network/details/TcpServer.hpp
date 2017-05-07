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

#include <mutex>
#include <unordered_map>

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
class TBAG_API TcpRealNode : public Server::NodeInterface, public uvpp::Tcp
{
private:
    TcpServer & _parent;
    SharedShutdown _shutdown;

private:
    Id _id;
    Buffer _buffer;

public:
    TcpRealNode(Loop & loop, TcpServer & parent);
    virtual ~TcpRealNode();

public:
    virtual Id getId() const override;

    virtual bool  start() override;
    virtual bool   stop() override;
    virtual bool  close() override;
    virtual bool cancel() override;

    virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) override;

public:
    virtual void onWrite(WriteRequest & request, uerr code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
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

public:
    SharedNode getNode(NodeKey key);
    SharedNode getNode(NodeKey key) const;

private:
    SharedNode insertNewNode();
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

public:
    /**
     * Initialize this method.
     *
     * @param[in] ip
     *      Ip address.
     * @param[in] port
     *      Port number.
     * @param[in] timeout
     *      Unused.
     *
     * @remarks
     *  init -> bind -> listen.
     */
    virtual bool init(String const & ip, int port, int unused = -1) override;

    /** This operation is async. */
    virtual bool close() override;
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPSERVER_HPP__

