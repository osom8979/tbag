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

#include <libtbag/network/SocketAddress.hpp>
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
    using Loop   = libtbag::uvpp::Loop;
    using Stream = libtbag::uvpp::Stream;
    using Async  = libtbag::uvpp::Async;
    using Tcp    = libtbag::uvpp::Tcp;
    using Pipe   = libtbag::uvpp::Pipe;
    using Timer  = libtbag::uvpp::Timer;

    using ConnectRequest  = libtbag::uvpp::ConnectRequest;
    using ShutdownRequest = libtbag::uvpp::ShutdownRequest;
    using WriteRequest    = libtbag::uvpp::WriteRequest;

    using Buffer = libtbag::util::Buffer;
    using binf   = libtbag::util::binf;
    using cbinf  = libtbag::util::cbinf;

    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqEventQueue   = libtbag::mq::details::MqEventQueue;
    using MqQueue        = libtbag::mq::details::MqQueue;
    using MqParams       = libtbag::mq::details::MqParams;
    using MqIsConsume    = libtbag::mq::details::MqIsConsume;

    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;
    using AsyncMsgQueue   = std::queue<AsyncMsgPointer>;

    using SocketAddress = libtbag::network::SocketAddress;
    using MsgPacket     = libtbag::proto::MsgPacket;

private:
    struct Initializer : public Async
    {
        MqStreamServer * parent = nullptr;

        Initializer(Loop & loop, MqStreamServer * p)
                : Async(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Initializer()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onInitializerAsync(this); }
        virtual void onClose() override
        { parent->onInitializerClose(this); }
    };

    struct Writer : public Async
    {
        MqStreamServer * parent = nullptr;

        MqRequestState state;
        AsyncMsgQueue  queue;
        std::size_t    write_count;

        Writer(Loop & loop, MqStreamServer * p)
                : Async(loop), parent(p),
                  state(MqRequestState::MRS_WAITING),
                  queue(), write_count(0)
        { assert(parent != nullptr); }
        virtual ~Writer()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onWriterAsync(this); }
        virtual void onClose() override
        { parent->onWriterClose(this); }
    };

    struct CloseTimer : public Timer
    {
        MqStreamServer * parent = nullptr;

        CloseTimer(Loop & loop, MqStreamServer * p) : Timer(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~CloseTimer()
        { /* EMPTY. */ }

        virtual void onTimer() override
        { parent->onCloseTimer(this); }
        virtual void onClose() override
        { parent->onCloseTimerClose(this); }
    };

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

public:
    MqStreamServer(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqStreamServer();

public:
    // @formatter:off
    inline SharedStream       & server()       TBAG_NOEXCEPT { return _server; }
    inline SharedStream const & server() const TBAG_NOEXCEPT { return _server; }
    // @formatter:on

public:
    virtual Err exit() override;

private:
    void onInitializerAsync(Initializer * init);
    void onInitializerClose(Initializer * init);

    virtual AfterAction onMsg(AsyncMsg * msg) override;
    virtual void onCloseMsgDone() override;

    void onWriterAsync(Writer * writer);
    void onWriterClose(Writer * writer);

    void onCloseTimer(CloseTimer * timer);
    void onCloseTimerClose(CloseTimer * timer);

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
     * Teardown process.
     *
     * @param[in] from_message_event
     *      Called from a message event.
     */
    void onTearDownStep1(bool from_message_event);
    void onTearDownStep2_SHUTDOWN();
    void onTearDownStep3_TIMEOUT();

    /**
     * Close process.
     */
    void onCloseStep1();
    void onCloseStep2_EVENT_QUEUE_CLOSED();
    void onCloseStep3_WRITER_CLOSED();
    void onCloseStep4_CLIENT_CLOSED();
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

