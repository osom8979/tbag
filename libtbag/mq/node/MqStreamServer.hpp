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
#include <libtbag/container/Pointer.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/mq/details/MqQueue.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Pipe.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/network/SocketAddress.hpp>
#include <libtbag/proto/MsgPacket.hpp>

#include <vector>
#include <unordered_set>
#include <regex>
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
class TBAG_API MqStreamServer : public libtbag::mq::details::MqEventQueue,
                                public libtbag::mq::details::MqInterface
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

    using SharedHandle = Loop::SharedHandle;
    using WeakHandle   = Loop::WeakHandle;

    using Buffer = libtbag::util::Buffer;
    using binf   = libtbag::util::binf;
    using cbinf  = libtbag::util::cbinf;

    using MqEvent      = libtbag::mq::details::MqEvent;
    using MqType       = libtbag::mq::details::MqType;
    using MqMsg        = libtbag::mq::details::MqMsg;
    using MqEventQueue = libtbag::mq::details::MqEventQueue;
    using MqQueue      = libtbag::mq::details::MqQueue;
    using MqParams     = libtbag::mq::details::MqParams;

    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;
    using AsyncMsgQueue   = std::queue<AsyncMsgPointer>;

    using SocketAddress = libtbag::network::SocketAddress;
    using Uri           = libtbag::network::Uri;
    using MsgPacket     = libtbag::proto::MsgPacket;

public:
    enum class RequestState
    {
        RS_WAITING,
        RS_ASYNC,
        RS_REQUESTING,
    };

private:
    struct Writer : public Async
    {
        MqStreamServer * parent = nullptr;

        RequestState  state;
        std::size_t   write_count;
        AsyncMsgQueue queue;

        Writer(Loop & loop, MqStreamServer * p)
                : Async(loop), parent(p),
                  state(RequestState::RS_WAITING),
                  write_count(0), queue()
        { assert(parent != nullptr); }

        virtual ~Writer()
        { /* EMPTY. */ }

        inline bool isWaiting() const TBAG_NOEXCEPT
        { return state == RequestState::RS_WAITING; }

        virtual void onAsync() override
        { parent->onWriterAsync(this); }
        virtual void onClose() override
        { parent->onWriterClose(this); }
    };

    struct CloseTimer : public Timer
    {
        MqStreamServer * parent = nullptr;
        Stream * stream = nullptr;

        CloseTimer(Loop & loop, MqStreamServer * p, Stream * s) : Timer(loop), parent(p), stream(s)
        {
            assert(parent != nullptr);
            assert(stream != nullptr);
        }

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
    using TcpNode  = Node<Tcp>;
    using PipeNode = Node<Pipe>;

    using TcpServer  = Server<Tcp>;
    using PipeServer = Server<Pipe>;

public:
    using SharedWriter = std::shared_ptr<Writer>;

    using SharedTcpNode  = std::shared_ptr<TcpNode>;
    using SharedPipeNode = std::shared_ptr<PipeNode>;

    using SharedTcpServer  = std::shared_ptr<TcpServer>;
    using SharedPipeServer = std::shared_ptr<PipeServer>;

public:
    using StreamPointer = libtbag::container::Pointer<Stream>;
    using NodeSet       = std::unordered_set<StreamPointer, StreamPointer::Hash, StreamPointer::EqualTo>;
    using SharedStream  = std::shared_ptr<Stream>;
    using ThreadId      = std::thread::id;

public:
    MqType const TYPE;
    MqParams const PARAMS;

private:
    SharedStream _server;
    SharedWriter _writer;
    NodeSet      _nodes;
    MsgPacket    _packer;
    MqQueue      _recv_queue;

private:
    std::atomic_bool _closing_server;

public:
    MqStreamServer(Loop & loop, MqParams const & params);
    virtual ~MqStreamServer();

private:
    virtual AfterAction onMsg(AsyncMsg * msg) override;

    void onCloseEvent();

    void onWriterAsync(Writer * writer);
    void onWriterClose(Writer * writer);

    void onCloseTimer(CloseTimer * timer);
    void onCloseTimerClose(CloseTimer * timer);

    void onNodeShutdown(Stream * node, ShutdownRequest & request, Err code);
    void onNodeWrite   (Stream * node, WriteRequest & request, Err code);
    binf onNodeAlloc   (Stream * node, std::size_t suggested_size);
    void onNodeRead    (Stream * node, Err code, char const * buffer, std::size_t size);
    void onNodeClose   (Stream * node);

    void closeServer();
    Err closeNode(Stream * node);
    std::size_t closeAllNode();

    void onServerConnection(Stream * server, Err code);
    void onServerClose(Stream * server);

public:
    virtual Err send(MqMsg const & msg) override;
    virtual Err recv(MqMsg & msg) override;

    virtual void recvWait(MqMsg & msg) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

