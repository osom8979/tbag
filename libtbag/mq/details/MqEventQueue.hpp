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

#include <cassert>
#include <queue>
#include <vector>
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
        AA_OK,
        AA_DELAY,
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
        { parent->onAsyncMsg(this); }
        virtual void onClose() override
        { parent->onCloseMsg(this); }
    };

public:
    using UniqueQueue    = std::unique_ptr<BoundedMpMcQueue>;
    using SharedAsyncMsg = std::shared_ptr<AsyncMsg>;
    using AsyncMessages  = std::vector<SharedAsyncMsg>;

private:
    std::size_t _queue_size = 0;
    std::size_t _close_counter = 0;

private:
    UniqueQueue   _ready;
    AsyncMessages _active;

public:
    MqEventQueue();
    MqEventQueue(Loop & loop,
                 std::size_t queue_size = DEFAULT_QUEUE_SIZE,
                 std::size_t packet_size = DEFAULT_PACKET_SIZE);
    virtual ~MqEventQueue();

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_ready); }

protected:
    virtual void onAsyncMsg(AsyncMsg * async);
    virtual void onCloseMsg(AsyncMsg * async);

public:
    bool initialize(Loop & loop,
                    std::size_t queue_size = DEFAULT_QUEUE_SIZE,
                    std::size_t packet_size = DEFAULT_PACKET_SIZE);
    void closeAsyncMessages();
    void clear();

public:
    std::size_t getInaccurateSizeOfReady() const;
    std::size_t getInaccurateSizeOfActive() const;
    MiscValidity validateOfReady(std::size_t min = 0, std::size_t max = 0) const;

public:
    Err enqueue(MqMsg const & msg);
    Err enqueue(MqEvent e, char const * data, std::size_t size);
    Err enqueue(char const * data, std::size_t size);

protected:
    virtual AfterAction onMsg(AsyncMsg * msg);

protected:
    /**
     * All messages have been closed.
     */
    virtual void onCloseMsgDone();

protected:
    /**
     * @warning
     *  Only messages extracted via onMsg() can be enqueue.
     */
    Err restoreMessage(AsyncMsg * msg, bool verify = false);
};

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQEVENTQUEUE_HPP__

