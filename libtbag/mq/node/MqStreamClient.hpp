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
 * MqStreamClient class prototype.
 *
 * @author zer0
 * @date   2018-11-13
 */
class TBAG_API MqStreamClient : public libtbag::mq::node::MqBase
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

    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;
    using AsyncMsgQueue   = std::queue<AsyncMsgPointer>;

    using SocketAddress = libtbag::network::SocketAddress;
    using MsgPacket     = libtbag::proto::MsgPacket;

private:
    struct Initializer : public Async
    {
        MqStreamClient * parent = nullptr;

        Initializer(Loop & loop, MqStreamClient * p)
                : Async(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Initializer()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onInitializerAsync(this); }
        virtual void onClose() override
        { parent->onInitializerClose(this); }
    };

    struct Terminator : public Async
    {
        MqStreamClient * parent = nullptr;

        Terminator(Loop & loop, MqStreamClient * p)
                : Async(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Terminator()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onTerminatorAsync(this); }
        virtual void onClose() override
        { parent->onTerminatorClose(this); }
    };

    struct Writer : public Async
    {
        MqStreamClient * parent = nullptr;

        MqRequestState state;
        AsyncMsgQueue  queue;

        Writer(Loop & loop, MqStreamClient * p)
                : Async(loop), parent(p), state(MqRequestState::MRS_WAITING), queue()
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

    struct ConnectTimer : public Timer
    {
        MqStreamClient * parent = nullptr;

        ConnectTimer(Loop & loop, MqStreamClient * p) : Timer(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~ConnectTimer()
        { /* EMPTY. */ }

        virtual void onTimer() override
        { parent->onConnectTimer(this); }
        virtual void onClose() override
        { parent->onConnectTimerClose(this); }
    };

    template <typename _BaseT>
    struct Client : public _BaseT
    {
        MqStreamClient * parent = nullptr;

        ConnectRequest  connect_req;
        ShutdownRequest shutdown_req;
        WriteRequest    write_req;

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
    using SharedTimer      = std::shared_ptr<Timer>;
    using SharedTerminator = std::shared_ptr<Terminator>;
    using SharedStream     = std::shared_ptr<Stream>;
    using SharedWriter     = std::shared_ptr<Writer>;
    using SharedTcpClient  = std::shared_ptr<TcpClient>;
    using SharedPipeClient = std::shared_ptr<PipeClient>;

private:
    Loop & _loop;

private:
    SharedStream     _client;
    SharedWriter     _writer;
    SharedTimer      _connector;
    SharedTerminator _terminator;

private:
    MsgPacket _packer;

private:
    std::size_t _read_error_count;
    Buffer      _read_buffer;
    Buffer      _remaining_read;

private:
    std::size_t _reconnect;

private:
    AtomicInt _exiting;

public:
    MqStreamClient(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqStreamClient();

public:
    // @formatter:off
    inline SharedStream       & client()       TBAG_NOEXCEPT { return _client; }
    inline SharedStream const & client() const TBAG_NOEXCEPT { return _client; }
    // @formatter:on

public:
    virtual Err exit() override;

private:
    void onInitializerAsync(Initializer * init);
    void onInitializerClose(Initializer * init);

    void onTerminatorAsync(Terminator * terminator);
    void onTerminatorClose(Terminator * terminator);

    virtual AfterAction onMsg(AsyncMsg * msg) override;
    virtual void onCloseMsgDone() override;

    void onWriterAsync(Writer * writer);
    void onWriterClose(Writer * writer);

    void onCloseTimer(CloseTimer * timer);
    void onCloseTimerClose(CloseTimer * timer);

    void onConnectTimer(ConnectTimer * timer);
    void onConnectTimerClose(ConnectTimer * timer);

    void onConnect(ConnectRequest & request, Err code);
    void onShutdown(ShutdownRequest & request, Err code);
    void onWrite(WriteRequest & request, Err code);
    binf onAlloc(std::size_t suggested_size);
    void onRead(Err code, char const * buffer, std::size_t size);
    void onClose();

private:
    // ---------------------
    // Alias of event steps.
    // ---------------------

    /**
     * Initialize process.
     */
    void onInitStep1_ASYNC();
    void onInitStep2_INIT();
    void onInitStep3_TIMEOUT();
    void onInitStep3_CONNECT(Err code);
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
    void onCloseStep4_TERMINATOR_CLOSED();
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

