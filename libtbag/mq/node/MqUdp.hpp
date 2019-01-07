/**
 * @file   MqUdp.hpp
 * @brief  MqUdp class prototype.
 * @author zer0
 * @date   2018-12-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQUDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQUDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/mq/details/MqQueue.hpp>
#include <libtbag/mq/node/MqBase.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Udp.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <libtbag/network/SocketAddress.hpp>
#include <libtbag/proto/MsgPacket.hpp>
#include <libtbag/container/Pointer.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

/**
 * MqUdp class prototype.
 *
 * @author zer0
 * @date   2018-12-09
 */
class TBAG_API MqUdp : public libtbag::mq::node::MqBase
{
public:
    using Loop  = libtbag::uvpp::Loop;
    using Async = libtbag::uvpp::Async;
    using Udp   = libtbag::uvpp::Udp;
    using Timer = libtbag::uvpp::Timer;

    using UdpSendRequest = libtbag::uvpp::UdpSendRequest;

    using SharedHandle = Loop::SharedHandle;
    using WeakHandle   = Loop::WeakHandle;

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

public:
    struct Writer : public Async
    {
        MqUdp * parent = nullptr;

        MqRequestState state;
        AsyncMsgQueue  queue;

        Writer(Loop & loop, MqUdp * p)
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
        MqUdp * parent = nullptr;

        CloseTimer(Loop & loop, MqUdp * p) : Timer(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~CloseTimer()
        { /* EMPTY. */ }

        virtual void onTimer() override
        { parent->onCloseTimer(this); }
        virtual void onClose() override
        { parent->onCloseTimerClose(this); }
    };

    struct Node : public Udp
    {
        MqUdp * parent = nullptr;

        Node(Loop & loop, MqUdp * p) : Udp(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Node()
        { /* EMPTY. */ }

        virtual void onSend(UdpSendRequest & request, Err code) override
        { parent->onSend(request, code); }
        virtual binf onAlloc(std::size_t suggested_size) override
        { return parent->onAlloc(suggested_size); }
        virtual void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags) override
        { parent->onRecv(code, buffer, size, addr, flags); }
        virtual void onClose() override
        { parent->onClose(); }
    };

public:
    using SharedTimer  = std::shared_ptr<Timer>;
    using SharedNode   = std::shared_ptr<Node>;
    using SharedWriter = std::shared_ptr<Writer>;

private:
    SharedNode   _node;
    SharedWriter _writer;
    MsgPacket    _packer;

private:
    UdpSendRequest _send_req;

private:
    std::size_t _read_error_count;
    Buffer      _read_buffer;
    Buffer      _remaining_read;

public:
    MqUdp(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqUdp();

public:
    virtual Err exit() override;

private:
    void close();
    void shutdownAndClose();
    void tearDown(bool on_message);

private:
    virtual void onCloseMsgDone() override;
    virtual AfterAction onMsg(AsyncMsg * msg) override;

private:
    AfterAction onMsgEvent(AsyncMsg * msg);
    void onCloseEvent();

private:
    void afterProcessMessage(AsyncMsg * msg);

private:
    void onWriterAsync(Writer * writer);
    void onWriterClose(Writer * writer);

private:
    void onCloseTimer(CloseTimer * timer);
    void onCloseTimerClose(CloseTimer * timer);

private:
    void onSend(UdpSendRequest & request, Err code);
    binf onAlloc(std::size_t suggested_size);
    void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags);
    void onClose();
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQUDP_HPP__

