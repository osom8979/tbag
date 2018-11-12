/**
 * @file   MqStreamClient.hpp
 * @brief  MqStreamClient class prototype.
 * @author zer0
 * @date   2018-11-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

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
 * MqStreamClient class prototype.
 *
 * @author zer0
 * @date   2018-11-12
 */
class TBAG_API MqStreamClient : private Noncopyable
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
    struct TcpClient;
    struct PipeClient;

    friend struct TcpClient;
    friend struct PipeClient;

public:
    struct TcpClient : public Tcp
    {
        MqStreamClient * parent = nullptr;

        TcpClient(Loop & loop, MqStreamClient * p) : Tcp(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~TcpClient()
        { /* EMPTY. */ }

        virtual void onConnect(ConnectRequest & request, Err code) override
        { parent->onConnect(this, request, code); }
        virtual void onShutdown(ShutdownRequest & request, Err code) override
        { parent->onShutdown(this, request, code); }
        virtual void onWrite(WriteRequest & request, Err code) override
        { parent->onWrite(this, request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onAlloc(this, suggested_size); }
        virtual void onRead(Err code, char const * buffer, std::size_t size) override
        { parent->onRead(this, code, buffer, size); }
        virtual void onClose() override
        { parent->onClose(this); }
    };

    struct PipeClient : public Pipe
    {
        MqStreamClient * parent = nullptr;

        PipeClient(Loop & loop, MqStreamClient * p) : Pipe(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~PipeClient()
        { /* EMPTY. */ }

        virtual void onShutdown(ShutdownRequest & request, Err code) override
        { parent->onShutdown(this, request, code); }
        virtual void onWrite(WriteRequest & request, Err code) override
        { parent->onWrite(this, request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onAlloc(this, suggested_size); }
        virtual void onRead(Err code, char const * buffer, std::size_t size) override
        { parent->onRead(this, code, buffer, size); }
        virtual void onClose() override
        { parent->onClose(this); }
    };

public:
    MqStreamClient(Loop & loop, tcp_t const &);
    MqStreamClient(Loop & loop, pipe_t const &);
    virtual ~MqStreamClient();

protected:
    void onConnect (Stream * stream, ConnectRequest & request, Err code);
    void onShutdown(Stream * stream, ShutdownRequest & request, Err code);
    void onWrite   (Stream * stream, WriteRequest & request, Err code);
    binf onAlloc   (Stream * stream, std::size_t suggested_size);
    void onRead    (Stream * stream, Err code, char const * buffer, std::size_t size);
    void onClose   (Stream * stream);
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

