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
    MqLocalQueue(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqLocalQueue();

private:
    virtual void onTerminatorAsync(Terminator * terminator) override;
    virtual void onTerminatorClose(Terminator * terminator) override;

    virtual void onCloseMsgDone() override;
    virtual AfterAction onMsg(AsyncMsg * msg) override;

    void onRead(AsyncMsg * msg);

private:
    /**
     * Close process.
     */
    void onCloseStep1_TERMINATOR();
    void onCloseStep1();
    void onCloseStep2_EVENT_QUEUE_CLOSED();
    void onCloseStep3_TERMINATOR_CLOSED();
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQLOCALQUEUE_HPP__

