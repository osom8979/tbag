/**
 * @file   MqStreamClient.hpp
 * @brief  MqStreamClient class prototype.
 * @author zer0
 * @date   2018-11-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/details/MqEventQueue.hpp>

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
 * MqStreamClient class prototype.
 *
 * @author zer0
 * @date   2018-11-13
 */
class TBAG_API MqStreamClient : public libtbag::mq::details::MqEventQueue,
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
    TBAG_CONSTEXPR static std::size_t DEFAULT_QUEUE_SIZE       = MqEventQueue::DEFAULT_QUEUE_SIZE;
    TBAG_CONSTEXPR static std::size_t DEFAULT_PACKET_SIZE      = MqEventQueue::DEFAULT_PACKET_SIZE;
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
        MqStreamClient * parent = nullptr;

        RequestState  state;
        AsyncMsgQueue queue;

        Writer(Loop & loop, MqStreamClient * p)
                : Async(loop), parent(p), state(RequestState::RS_WAITING), queue()
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
        MqStreamClient * parent = nullptr;

        CloseTimer(Loop & loop, MqStreamClient * p) : Timer(loop), parent(p)
        { assert(parent != nullptr); }

        virtual ~CloseTimer()
        { /* EMPTY. */ }

        virtual void onTimer() override
        { parent->onCloseTimer(this); }

        virtual void onClose() override
        { parent->onCloseTimerClose(this); }
    };

    template <typename _BaseT>
    struct Client : public _BaseT
    {
        MqStreamClient * parent = nullptr;

        ConnectRequest  connect_req;
        ShutdownRequest shutdown_req;
        WriteRequest    write_req;

        Buffer read_buffer;
        Buffer remaining_read;

        bool is_shutdown = false;
        std::size_t read_error_count = 0;

        Client(Loop & loop, MqStreamClient * p) : _BaseT(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Client()
        { /* EMPTY. */ }

        virtual void onConnect(ConnectRequest & request, Err code) override
        { parent->onConnect(request, code); }
        virtual void onShutdown(ShutdownRequest & request, Err code) override
        { parent->onShutdown(request, code); }
        virtual void onWrite(WriteRequest & request, Err code) override
        { parent->onWrite(request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onAlloc(suggested_size); }
        virtual void onRead(Err code, char const * buffer, std::size_t size) override
        { parent->onRead(code, buffer, size); }
        virtual void onClose() override
        { parent->onClose(); }
    };

public:
    using TcpClient  = Client<Tcp>;
    using PipeClient = Client<Pipe>;

public:
    using SharedWriter = std::shared_ptr<Writer>;

    using SharedTcpClient  = std::shared_ptr<TcpClient>;
    using SharedPipeClient = std::shared_ptr<PipeClient>;

public:
    using SharedStream  = std::shared_ptr<Stream>;
    using ThreadId      = std::thread::id;

public:
    MqType const TYPE;
    MqParams const PARAMS;

private:
    SharedStream _client;
    SharedWriter _writer;
    MsgPacket    _packer;
    MqQueue      _recv_queue;

public:
    MqStreamClient(Loop & loop, MqParams const & params);
    virtual ~MqStreamClient();

private:
    virtual AfterAction onMsg(AsyncMsg * msg) override;

    void onCloseEvent();

    void onWriterAsync(Writer * writer);
    void onWriterClose(Writer * writer);

    void onCloseTimer     (CloseTimer * timer);
    void onCloseTimerClose(CloseTimer * timer);

    void onConnect (ConnectRequest & request, Err code);
    void onShutdown(ShutdownRequest & request, Err code);
    void onWrite   (WriteRequest & request, Err code);
    binf onAlloc   (std::size_t suggested_size);
    void onRead    (Err code, char const * buffer, std::size_t size);
    void onClose   ();

public:
    virtual Err send(MqMsg const & msg) override;
    virtual Err recv(MqMsg & msg) override;

public:
    virtual void recvWait(MqMsg & msg) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

