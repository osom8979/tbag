/**
 * @file   MqLocalQueue.hpp
 * @brief  MqLocalQueue class prototype.
 * @author zer0
 * @date   2018-11-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQLOCALQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQLOCALQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/mq/details/MqQueue.hpp>

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/proto/MsgPacket.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

/**
 * MqLocalQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-24
 */
class TBAG_API MqLocalQueue : protected libtbag::mq::details::MqEventQueue,
                              public    libtbag::mq::details::MqInterface
{
public:
    using UvLock      = libtbag::lock::UvLock;
    using UvCondition = libtbag::lock::UvCondition;

    using Loop = libtbag::uvpp::Loop;

    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqEventQueue   = libtbag::mq::details::MqEventQueue;
    using MqQueue        = libtbag::mq::details::MqQueue;
    using MqParams       = libtbag::mq::details::MqParams;
    using MqRecvCallback = libtbag::mq::details::MqRecvCallback;

    using Buffer = libtbag::util::Buffer;
    using binf   = libtbag::util::binf;
    using cbinf  = libtbag::util::cbinf;

    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;
    using AsyncMsgQueue   = std::queue<AsyncMsgPointer>;

    using MsgPacket = libtbag::proto::MsgPacket;

    using ThreadId    = std::thread::id;
    using AtomicState = std::atomic<MqMachineState>;
    using AtomicInt   = std::atomic_int;

public:
    MqParams const PARAMS;

private:
    // 'CALLBACK' is defined as a macro in Windows.
    // So use another variable name: '_callback'
    MqRecvCallback * const _callback;

private:
    MqQueue _receives; ///< Single-Producer, Many-consumer.

private:
    AtomicState _state;
    AtomicInt   _sending;

private:
    UvLock      _wait_lock;
    UvCondition _wait_cond;

public:
    MqLocalQueue(Loop & loop, MqParams const & params, MqRecvCallback * cb);
    virtual ~MqLocalQueue();

private:
    virtual void onCloseMsgDone() override;
    virtual AfterAction onMsg(AsyncMsg * msg) override;

private:
    AfterAction onMsgEvent(AsyncMsg * msg);
    void onCloseEvent();

public:
    virtual MqMachineState state() const TBAG_NOEXCEPT override
    { return _state; }

    virtual MqParams params() const override
    { return PARAMS; }

public:
    virtual Err send(MqMsg const & msg) override;
    virtual Err recv(MqMsg & msg) override;

public:
    virtual Err recvWait(MqMsg & msg, uint64_t timeout_nano) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQLOCALQUEUE_HPP__

