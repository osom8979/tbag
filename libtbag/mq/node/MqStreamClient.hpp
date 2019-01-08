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
    using Stream = libtbag::uvpp::Stream;
    using Tcp    = libtbag::uvpp::Tcp;
    using Pipe   = libtbag::uvpp::Pipe;

    using ConnectRequest  = libtbag::uvpp::ConnectRequest;
    using ShutdownRequest = libtbag::uvpp::ShutdownRequest;
    using WriteRequest    = libtbag::uvpp::WriteRequest;

    using SocketAddress = libtbag::network::SocketAddress;
    using MsgPacket     = libtbag::proto::MsgPacket;

private:
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
    using SharedStream     = std::shared_ptr<Stream>;
    using SharedWriter     = std::shared_ptr<Writer>;
    using SharedTcpClient  = std::shared_ptr<TcpClient>;
    using SharedPipeClient = std::shared_ptr<PipeClient>;

private:
    Loop & _loop;

private:
    SharedStream _client;
    SharedWriter _writer;
    SharedTimer  _connector;

private:
    MsgPacket _packer;

private:
    std::size_t _read_error_count;
    Buffer      _read_buffer;
    Buffer      _remaining_read;

private:
    std::size_t _reconnect;

public:
    MqStreamClient(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqStreamClient();

public:
    // @formatter:off
    inline SharedStream       & client()       TBAG_NOEXCEPT { return _client; }
    inline SharedStream const & client() const TBAG_NOEXCEPT { return _client; }
    // @formatter:on

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
    void onCloseStep5_TERMINATOR_CLOSED();
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQSTREAMCLIENT_HPP__

