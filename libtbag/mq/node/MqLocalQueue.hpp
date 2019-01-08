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
#include <libtbag/mq/node/MqBase.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/Loop.hpp>

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
class TBAG_API MqLocalQueue : public libtbag::mq::node::MqBase
{
public:
    using Loop = libtbag::uvpp::Loop;

    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqEventQueue   = libtbag::mq::details::MqEventQueue;
    using MqQueue        = libtbag::mq::details::MqQueue;

    using Buffer = libtbag::util::Buffer;
    using binf   = libtbag::util::binf;
    using cbinf  = libtbag::util::cbinf;

    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;
    using AsyncMsgQueue   = std::queue<AsyncMsgPointer>;

private:
    AtomicInt _exiting;

public:
    MqLocalQueue(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqLocalQueue();

public:
    virtual Err exit() override;

private:
    virtual void onCloseMsgDone() override;
    virtual AfterAction onMsg(AsyncMsg * msg) override;

private:
    void onMsgEvent(AsyncMsg * msg);
    void onCloseEvent();
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQLOCALQUEUE_HPP__

