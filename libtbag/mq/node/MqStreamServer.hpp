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
#include <libtbag/Err.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/mq/details/MqQueue.hpp>
#include <libtbag/mq/node/MqBase.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <libtbag/net/SocketAddress.hpp>
#include <libtbag/proto/MsgPacket.hpp>
#include <libtbag/container/Pointer.hpp>

#include <cassert>
#include <vector>
#include <unordered_set>
#include <atomic>
#include <thread>
#include <type_traits>

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
class TBAG_API MqStreamServer : public libtbag::mq::node::MqBase
{
public:
    using Stream = libtbag::uvpp::Stream;
    using Tcp    = libtbag::uvpp::Tcp;
    using Pipe   = libtbag::uvpp::Pipe;

    using ConnectRequest  = libtbag::uvpp::ConnectRequest;
    using ShutdownRequest = libtbag::uvpp::ShutdownRequest;
    using WriteRequest    = libtbag::uvpp::WriteRequest;

    using SocketAddress = libtbag::net::SocketAddress;
    using MsgPacket     = libtbag::proto::MsgPacket;

private:
    template <typename _BaseT>
    struct Node : public _BaseT
    {
        MqStreamServer * parent = nullptr;

        ShutdownRequest shutdown_req;
        WriteRequest    write_req;

        Buffer read_buffer;
        Buffer remaining_read;

        bool is_shutdown = false;
        std::size_t read_error_count = 0;

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
    using TcpNode    = Node<Tcp>;
    using PipeNode   = Node<Pipe>;
    using TcpServer  = Server<Tcp>;
    using PipeServer = Server<Pipe>;

public:
    using SharedStream     = std::shared_ptr<Stream>;
    using SharedWriter     = std::shared_ptr<Writer>;
    using SharedTcpNode    = std::shared_ptr<TcpNode>;
    using SharedPipeNode   = std::shared_ptr<PipeNode>;
    using SharedTcpServer  = std::shared_ptr<TcpServer>;
    using SharedPipeServer = std::shared_ptr<PipeServer>;

public:
    using StreamPointer = libtbag::container::Pointer<Stream>;
    using NodeSet       = std::unordered_set<StreamPointer, StreamPointer::Hash, StreamPointer::EqualTo>;

private:
    Loop & _loop;

private:
    SharedStream _server;
    SharedWriter _writer;

private:
    NodeSet _nodes;

private:
    MsgPacket _packer;

private:
    bool _exit;

public:
    MqStreamServer(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqStreamServer();

public:
    // clang-format off
    inline SharedStream       & server()       TBAG_NOEXCEPT { return _server; }
    inline SharedStream const & server() const TBAG_NOEXCEPT { return _server; }
    // clang-format on

private:
    virtual void onInitializerAsync(Initializer * init) override;
    virtual void onInitializerClose(Initializer * init) override;

    virtual void onTerminatorAsync(Terminator * terminator) override;
    virtual void onTerminatorClose(Terminator * terminator) override;

    virtual AfterAction onMsg(AsyncMsg * msg) override;
    virtual void onCloseMsgDone() override;

    virtual void onWriterAsync(Writer * writer) override;
    virtual void onWriterClose(Writer * writer) override;

    virtual void onCloseTimerTimer(CloseTimer * timer) override;
    virtual void onCloseTimerClose(CloseTimer * timer) override;

private:
    void onNodeShutdown(Stream * node, ShutdownRequest & request, Err code);
    void onNodeWrite   (Stream * node, WriteRequest & request, Err code);
    binf onNodeAlloc   (Stream * node, std::size_t suggested_size);
    void onNodeRead    (Stream * node, Err code, char const * buffer, std::size_t size);
    void onNodeClose   (Stream * node);

    void onServerConnection(Stream * server, Err code);
    void onServerClose     (Stream * server);

private:
    // ---------------------
    // Alias of event steps.
    // ---------------------

    /**
     * Initialize process.
     */
    void onInitStep1_ASYNC();
    void onInitStep2_INIT();
    void onInit_SUCCESS();
    void onInit_FAILURE(Err code);

    /**
     * Send message process.
     */
    void onSendStep1_EVENT(AsyncMsg * msg);
    void onSendStep2_ASYNC();
    void onSendStep3_ASYNC_ORIGINAL(AsyncMsg * msg);
    void onSendStep3_ASYNC_MQMSG(AsyncMsg * msg);
    void onSendStep4_WRITE_DONE(Err code);
    void onSendStep5_NEXT_MESSAGE(AsyncMsg * msg);

    /**
     * Termination request.
     */
    void onTerminationRequest();

    /**
     * Teardown process.
     */
    void onTearDownStep1();
    void onTearDownStep2_SHUTDOWN();
    void onTearDownStep3_TIMEOUT();

    /**
     * Close process.
     */
    void onCloseStep1();
    void onCloseStep2_EVENT_QUEUE_CLOSED();
    void onCloseStep3_WRITER_CLOSED();
    void onCloseStep4_CLIENT_CLOSED();
    void onCloseStep5_TERMINATOR_CLOSED();
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

