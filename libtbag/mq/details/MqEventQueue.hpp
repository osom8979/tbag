/**
 * @file   MqEventQueue.hpp
 * @brief  MqEventQueue class prototype.
 * @author zer0
 * @date   2018-11-12
 * @date   2018-11-13 (Rename: MqSendQueue -> MqEventQueue)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQEVENTQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQEVENTQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/container/Pointer.hpp>
#include <libtbag/lockfree/BoundedMpMcQueue.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/ex/AsyncRequestQueue.hpp>

#include <cassert>
#include <queue>
#include <vector>
#include <thread>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

/**
 * MqEventQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-12
 * @date   2018-11-13 (Rename: MqSendQueue -> MqEventQueue)
 *
 * @remarks
 *  @code
 *   +-----------------------------------------------------------------------------------------------------+
 *   |            :                                                                                        |
 *   |  [WORKING] : [EVENT THREAD]                                                                         |
 *   |  [THREAD ] :                                                                                        |
 *   |            :                                                                                        |
 *   |            : Lockfree                                                                               |
 *   |            :  Ready                                                                                 |
 *   |    User    :  Queue                                                                                 |
 *   |     |      :    |                                                                                   |
 *   |     *<~~~~~~~~~~*           MqMsg           . .[USER CUSTOM]. . . . . . . . . . . . . . . . . . . . |
 *   |  dequeue   :    |           Async           .                                                       |
 *   |     |      :    |             |             .                                                       |
 *   |     *~~~~~~~~~~~~~~~~~~~~~~~~>* (onAsync)   .                                                       |
 *   |  message   :    |             |             .  MqMsgQueue                                           |
 *   |   send     :    |             |             .      |                                                |
 *   |            :    |             *~~~~{if-msg}~~~~~~~>*                                                |
 *   |            :    |             |             .   enqueue          SendAsync {Sending-state}          |
 *   |            :    |             |             .      |                 |     {Waiting-state}          |
 *   |            :    *<~{if-event}~*             .      *~~{if-waiting}~~>*                              |
 *   |            : enqueue                        .      |              message                           |
 *   |            :    |                           .      |               send                             |
 *   |            :    |                           .      |                 |                              |
 *   |            :    |                           .      |                 * (onAsync)      Stream        |
 *   |            :    |                           .      |                 |                  |           |
 *   |            :    |                           .      |                 *~{send-message}~~>*           |
 *   |            :    |                           .      |                 |                  |           |
 *   |            :    |                           .      |                 *<~{write-message}~*(onWrite)  |
 *   |            :    |                           .      |{Loop}           |                              |
 *   |            :    |                           .      *<~~~~~~~~~~~~~~~~*                              |
 *   |            :    |                           .          {If there are messages left in the queue ...}|
 *   |            :    |                           .                        |                              |
 *   |            :    *<~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*                              |
 *   |            : enqueue                        .                                                       |
 *   |            :                                .                                                       |
 *   +-----------------------------------------------------------------------------------------------------+
 *  @endcode
 */
class TBAG_API MqEventQueue : private Noncopyable
{
public:
    using Loop  = libtbag::uvpp::Loop;
    using Async = libtbag::uvpp::Async;

public:
    using BoundedMpMcQueue = libtbag::lockfree::BoundedMpMcQueue;
    using MiscValidity     = BoundedMpMcQueue::MiscValidity;

public:
    enum class AfterAction : int
    {
        AA_DELAY,
        AA_READY,
    };

public:
    TBAG_CONSTEXPR static std::size_t DEFAULT_QUEUE_SIZE  = BoundedMpMcQueue::DEFAULT_QUEUE_SIZE;
    TBAG_CONSTEXPR static std::size_t DEFAULT_PACKET_SIZE = 1024;

public:
    struct AsyncMsg : public Async, public MqMsg
    {
        MqEventQueue * parent = nullptr;

        AsyncMsg(Loop & loop, std::size_t size, MqEventQueue * p)
                : Async(loop), MqMsg(size), parent(p)
        { assert(parent != nullptr); }
        virtual ~AsyncMsg()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onAsync(this); }

        virtual void onClose() override
        { parent->onClose(this); }
    };

public:
    using UniqueQueue     = std::unique_ptr<BoundedMpMcQueue>;
    using SharedAsyncMsg  = std::shared_ptr<AsyncMsg>;
    using SharedAsyncMsgs = std::vector<SharedAsyncMsg>;
    using ThreadId        = std::thread::id;

private:
    ThreadId const THREAD_ID;

private:
    /**
     * @warning
     *  It must be accessed only from the loop thread.
     */
    SharedAsyncMsgs __messages__;

private:
    UniqueQueue _ready;

public:
    MqEventQueue(Loop & loop,
                     std::size_t size = DEFAULT_QUEUE_SIZE,
                     std::size_t msg_size = DEFAULT_PACKET_SIZE);
    virtual ~MqEventQueue();

private:
    void onAsync(AsyncMsg * async);
    void onClose(AsyncMsg * async);

protected:
    void closeAsyncMsgs();

public:
    std::size_t getInaccurateSizeOfReady() const;
    MiscValidity validateOfReady(std::size_t min = 0, std::size_t max = 0) const;

public:
    template <typename Predicated>
    Err enqueue(Predicated predicated)
    {
        void * value = nullptr;
        if (!_ready->dequeue(&value)) {
            return Err::E_NREADY;
        }

        auto * msg = (AsyncMsg*)value;
        assert(msg != nullptr);

        if (!predicated(msg)) {
            auto const RESULT = _ready->enqueue(value);
            assert(RESULT);
            return Err::E_ECANCELED;
        }

        auto const CODE = msg->send();
        if (isFailure(CODE)) {
            auto const RESULT = _ready->enqueue(value);
            assert(RESULT);
        }
        return CODE;
    }

public:
    Err enqueueShutdown();
    Err enqueueClose();

public:
    Err enqueue(char const * data, std::size_t size);

protected:
    TBAG_CONSTEXPR static AfterAction const DELAY_ACTION = AfterAction::AA_DELAY;
    TBAG_CONSTEXPR static AfterAction const READY_ACTION = AfterAction::AA_READY;

protected:
    virtual AfterAction onMsg(MqMsg * msg);
};

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQEVENTQUEUE_HPP__

