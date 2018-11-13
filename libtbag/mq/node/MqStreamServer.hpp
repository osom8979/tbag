/**
 * @file   MqStreamServer.hpp
 * @brief  MqStreamServer class prototype.
 * @author zer0
 * @date   2018-11-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

/**
 * MqStreamServer class prototype.
 *
 * @author zer0
 * @date   2018-11-13
 */
class TBAG_API MqStreamServer : public libtbag::mq::details::MqInterface
{
public:
    using MqType = libtbag::mq::details::MqType;

private:
    template <typename _BaseT>
    struct Node : public _BaseT
    {
        MqStreamServer * parent = nullptr;

        ShutdownRequest shutdown_req;
        WriteRequest       write_req;

        Node(Loop & loop, MqStreamServer * p) : _BaseT(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Node()
        { /* EMPTY. */ }

        virtual void onShutdown(ShutdownRequest & request, Err code) override
        { parent->onNodeShutdown(this, request, code); }
        virtual void onWrite(WriteRequest & request, Err code) override
        { parent->onNodeWrite(this, request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onNodeAlloc(this, suggested_size); }
        virtual void onRead(Err code, char const * buffer, std::size_t size) override
        { parent->onNodeRead(this, code, buffer, size); }
        virtual void onClose() override
        { parent->onNodeClose(this); }
    };

    template <typename _BaseT>
    struct Server : public _BaseT
    {
        MqStreamServer * parent = nullptr;

        Server(Loop & loop, MqStreamServer * p) : _BaseT(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Server()
        { /* EMPTY. */ }

        virtual void onConnection(Err code) override
        { parent->onServerConnection(this, code); }
        virtual void onClose() override
        { parent->onServerClose(this); }
    };

public:
    using TcpNode  = Node<Tcp>;
    using PipeNode = Node<Pipe>;

    using TcpServer  = Server<Tcp>;
    using PipeServer = Server<Pipe>;

public:
    using SharedTcpNode  = std::shared_ptr<TcpNode>;
    using SharedPipeNode = std::shared_ptr<PipeNode>;

    using SharedTcpServer  = std::shared_ptr<TcpServer>;
    using SharedPipeServer = std::shared_ptr<PipeServer>;

public:
    using SharedStream = std::shared_ptr<Stream>;

public:
    using pipe_t = libtbag::mq::details::pipe_t;
    using tcp_t  = libtbag::mq::details::tcp_t;

public:
    MqType const TYPE;

public:
    MqStreamServer(Loop & loop, pipe_t const & UNUSED_PARAM(x));
    MqStreamServer(Loop & loop, tcp_t const & UNUSED_PARAM(x));
    virtual ~MqStreamServer();

protected:
    void onNodeShutdown(Stream * node, ShutdownRequest & request, Err code);
    void onNodeWrite   (Stream * node, WriteRequest & request, Err code);
    binf onNodeAlloc   (Stream * node, std::size_t suggested_size);
    void onNodeRead    (Stream * node, Err code, char const * buffer, std::size_t size);
    void onNodeClose   (Stream * node);

protected:
    void onServerConnection(Stream * server, Err code);
    void onServerClose(Stream * server);

public:
    virtual Err open(std::string const & uri) override;
    virtual Err close() override;

public:
    virtual Err send(char const * buffer, std::size_t size) override;
    virtual Err recv(std::vector<char> & buffer) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

