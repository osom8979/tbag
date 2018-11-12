/**
 * @file   MqSendQueue.hpp
 * @brief  MqSendQueue class prototype.
 * @author zer0
 * @date   2018-11-12
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQSENDQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQSENDQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/lockfree/BoundedMpMcQueue.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <cassert>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

/**
 * MqSendQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-12
 */
class TBAG_API MqSendQueue : private Noncopyable
{
public:
    using Loop  = libtbag::uvpp::Loop;
    using Async = libtbag::uvpp::Async;

public:
    using BoundedMpMcQueue = libtbag::lockfree::BoundedMpMcQueue;
    using MiscValidity     = BoundedMpMcQueue::MiscValidity;

public:
    TBAG_CONSTEXPR static std::size_t DEFAULT_QUEUE_SIZE  = BoundedMpMcQueue::DEFAULT_QUEUE_SIZE;
    TBAG_CONSTEXPR static std::size_t DEFAULT_PACKET_SIZE = 1024;

public:
    struct AsyncMsg : public Async, public MqMsg
    {
        MqSendQueue * parent = nullptr;

        AsyncMsg(Loop & loop, std::size_t size, MqSendQueue * p)
                : Async(loop), MqMsg(size), parent(p)
        { /* EMPTY. */ }

        virtual ~AsyncMsg()
        { /* EMPTY. */ }

        virtual void onAsync() override
        {
            assert(parent != nullptr);
            parent->onAsync(this);
            auto const RESULT = parent->_ready->enqueue(this);
            assert(RESULT);
        }

        virtual void onClose() override
        {
            assert(parent != nullptr);
            parent->onClose(this);
        }
    };

    friend struct AsyncMsg;

public:
    using UniqueQueue = std::unique_ptr<BoundedMpMcQueue>;

private:
    UniqueQueue _ready;

public:
    MqSendQueue(Loop & loop, std::size_t size = DEFAULT_QUEUE_SIZE, std::size_t msg_size = DEFAULT_PACKET_SIZE);
    virtual ~MqSendQueue();

protected:
    virtual void onAsync(AsyncMsg * async);
    virtual void onClose(AsyncMsg * async);

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
    Err enqueue(char const * data, std::size_t size);
};

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQSENDQUEUE_HPP__

