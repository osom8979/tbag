/**
 * @file   MqStreamServer.hpp
 * @brief  MqStreamServer class prototype.
 * @author zer0
 * @date   2018-11-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

/**
 * MqStreamServer class prototype.
 *
 * @author zer0
 * @date   2018-11-12
 */
class TBAG_API MqStreamServer : private Noncopyable
{
public:
    using Loop   = libtbag::uvpp::Loop;
    using Stream = libtbag::uvpp::Stream;
    using Tcp    = libtbag::uvpp::Tcp;
    using Pipe   = libtbag::uvpp::Pipe;

    using ConnectRequest  = libtbag::uvpp::ConnectRequest;
    using ShutdownRequest = libtbag::uvpp::ShutdownRequest;
    using WriteRequest    = libtbag::uvpp::WriteRequest;

    using binf   = libtbag::util::binf;
    using cbinf  = libtbag::util::cbinf;

public:
    struct tcp_t  { /* EMPTY. */ };
    struct pipe_t { /* EMPTY. */ };

public:
    struct TcpNode;
    struct PipeNode;
    struct TcpServer;
    struct PipeServer;

    friend struct TcpNode;
    friend struct PipeNode;
    friend struct TcpServer;
    friend struct PipeServer;

public:
    struct TcpNode : public Tcp
    {
        MqStreamServer * parent = nullptr;

        TcpNode(Loop & loop, MqStreamServer * p) : Tcp(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~TcpNode()
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

    struct PipeNode : public Pipe
    {
        MqStreamServer * parent = nullptr;

        PipeNode(Loop & loop, MqStreamServer * p) : Pipe(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~PipeNode()
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

    struct TcpServer : public Tcp
    {
        MqStreamServer * parent = nullptr;

        TcpServer(Loop & loop, MqStreamServer * p) : Tcp(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~TcpServer()
        { /* EMPTY. */ }

        virtual void onConnection(Err code) override
        { parent->onConnection(this, code); }
        virtual void onClose() override
        { parent->onClose(this); }
    };

    struct PipeServer : public Pipe
    {
        MqStreamServer * parent = nullptr;

        PipeServer(Loop & loop, MqStreamServer * p) : Pipe(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~PipeServer()
        { /* EMPTY. */ }

        virtual void onConnection(Err code) override
        { parent->onConnection(this, code); }
        virtual void onClose() override
        { parent->onClose(this); }
    };

public:
    MqStreamServer(Loop & loop, tcp_t const &);
    MqStreamServer(Loop & loop, pipe_t const &);
    virtual ~MqStreamServer();

protected:
    void onNodeShutdown(Stream * node, ShutdownRequest & request, Err code);
    void onNodeWrite   (Stream * node, WriteRequest & request, Err code);
    binf onNodeAlloc   (Stream * node, std::size_t suggested_size);
    void onNodeRead    (Stream * node, Err code, char const * buffer, std::size_t size);
    void onNodeClose   (Stream * node);

protected:
    void onConnection(Stream * server, Err code);
    void onClose     (Stream * server);
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

