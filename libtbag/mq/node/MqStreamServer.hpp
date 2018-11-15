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

    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;
    using AsyncMsgQueue   = std::queue<AsyncMsgPointer>;

    using SocketAddress = libtbag::network::SocketAddress;
    using Uri           = libtbag::network::Uri;
    using MsgPacket     = libtbag::proto::MsgPacket;

public:
    TBAG_CONSTEXPR static std::size_t DEFAULT_QUEUE_SIZE       = MqEventQueue::DEFAULT_QUEUE_SIZE;
    TBAG_CONSTEXPR static std::size_t DEFAULT_PACKET_SIZE      = MqEventQueue::DEFAULT_PACKET_SIZE;
    TBAG_CONSTEXPR static std::size_t DEFAULT_MAX_NODE_SIZE    = 100000; // C10K
    TBAG_CONSTEXPR static std::size_t DEFAULT_BUILDER_SIZE     = MsgPacket::DEFAULT_BUILDER_CAPACITY;
    TBAG_CONSTEXPR static std::size_t DEFAULT_CLOSE_MILLISEC   = 1 * 1000;
    TBAG_CONSTEXPR static std::size_t DEFAULT_READ_ERROR_COUNT = 4;

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
    struct Params
    {
        /**
         * Type of stream. must be TCP or PIPE.
         */
        MqType type = MqType::MT_TCP;

        /**
         * Bind address.
         *
         * @remarks
         *  - tcp: bind socket address.
         *  - pipe: pipe file path.
         */
        std::string bind;

        /**
         * Bind port number.
         *
         * @remarks
         *  - tcp: port number.
         *  - pipe: unused.
         */
        int port = 0;

        /*
         * Used with uv_tcp_bind, when an IPv6 address is used.
         *
         * @remarks
         *  - tcp: use this flag.
         *  - pipe: unused.
         */
        bool tcp_ipv6_only = false;

        /**
         * The maximum size of the queue for transmission.
         */
        std::size_t send_queue_size = DEFAULT_QUEUE_SIZE;

        /**
         * The default size of the transmission message packet.
         *
         * @remarks
         *  If memory is insufficient, it will be more expanded.
         */
        std::size_t send_msg_size = DEFAULT_PACKET_SIZE;

        /**
         * The maximum size of the queue for receive
         */
        std::size_t recv_queue_size = DEFAULT_QUEUE_SIZE;

        /**
         * The default size of the receive message packet.
         *
         * @remarks
         *  If memory is insufficient, it will be more expanded.
         */
        std::size_t recv_msg_size = DEFAULT_PACKET_SIZE;

        /**
         * The number of clients that can be accepted.
         */
        std::size_t max_nodes = DEFAULT_MAX_NODE_SIZE;

        /**
         * Temporary buffer size for serialization.
         */
        std::size_t packer_size = DEFAULT_BUILDER_SIZE;

        /**
         * Wait time to closing. If this value is 0, close immediately.
         *
         * @remarks
         *  If you request a shutdown directly, You need time to wait for an idle recv request.
         */
        std::size_t wait_closing_millisec = DEFAULT_CLOSE_MILLISEC;

        /**
         * Verify the restore message.
         */
        bool verify_restore_message = false;

        /**
         * If the consecutive read error is maximum,
         * the connection is forced to close.
         */
        std::size_t continuous_read_error_count = DEFAULT_READ_ERROR_COUNT;

        /**
         * You are given the opportunity to filter IP addresses for acceptance.
         *
         * @remarks
         *  - tcp: use this value.
         *  - pipe: unused.
         */
        std::string accept_ip_regex;

        Params() { /* EMPTY. */ }
        ~Params() { /* EMPTY. */ }
    };

public:
    MqType const TYPE;

private:
    Params _params;

private:
    SharedStream _server;
    SharedWriter _writer;
    NodeSet      _nodes;
    MsgPacket    _packer;
    MqQueue      _recv_queue;

public:
    MqStreamServer(Loop & loop, Params const & params);
    virtual ~MqStreamServer();

protected:
    virtual AfterAction onMsg(AsyncMsg * msg) override;

protected:
    void onWriterAsync(Writer * writer);
    void onWriterClose(Writer * writer);

protected:
    void onCloseTimer(CloseTimer * timer);
    void onCloseTimerClose(CloseTimer * timer);

protected:
    void onNodeShutdown(Stream * node, ShutdownRequest & request, Err code);
    void onNodeWrite   (Stream * node, WriteRequest & request, Err code);
    binf onNodeAlloc   (Stream * node, std::size_t suggested_size);
    void onNodeRead    (Stream * node, Err code, char const * buffer, std::size_t size);
    void onNodeClose   (Stream * node);

private:
    Err closeNode(Stream * node);
    std::size_t closeAllNode();

protected:
    void onServerConnection(Stream * server, Err code);
    void onServerClose(Stream * server);

public:
    virtual Err send(MqMsg const & msg) override;
    virtual Err recv(MqMsg & msg) override;

public:
    virtual Err recvWait(MqMsg & msg) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMSERVER_HPP__

