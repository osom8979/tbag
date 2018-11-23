/**
 * @file   MqQueue.hpp
 * @brief  MqQueue class prototype.
 * @author zer0
 * @date   2018-11-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQQUEUE_HPP__

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

#include <cassert>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

/**
 * MqQueue class prototype.
 *
 * @author zer0
 * @date   2018-11-10
 */
class TBAG_API MqQueue : private Noncopyable
{
public:
    using BoundedMpMcQueue = libtbag::lockfree::BoundedMpMcQueue;
    using MiscValidity     = BoundedMpMcQueue::MiscValidity;

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_QUEUE_SIZE  = BoundedMpMcQueue::DEFAULT_QUEUE_SIZE;
    TBAG_CONSTEXPR static std::size_t const DEFAULT_PACKET_SIZE = 1024;

public:
    struct Queue : public BoundedMpMcQueue
    {
        MqQueue * parent = nullptr;

        Queue(MqQueue * p, std::size_t size) : parent(p), BoundedMpMcQueue(size)
        { /* EMPTY. */ }

        virtual ~Queue()
        { /* EMPTY. */ }

        virtual void onCleanup(void * key, void * value) override
        {
            assert(parent != nullptr);
            assert(value != nullptr);
            parent->removeMsg((MqMsg*)value);
        }
    };

public:
    friend struct Queue;
    using UniqueQueue = std::unique_ptr<Queue>;

private:
    UniqueQueue _active;
    UniqueQueue _ready;

public:
    MqQueue(std::size_t size = DEFAULT_QUEUE_SIZE,
            std::size_t msg_size = DEFAULT_PACKET_SIZE);
    ~MqQueue();

private:
    MqMsg * createMsg(std::size_t size);
    void removeMsg(MqMsg * value);

public:
    std::size_t getInaccurateSizeOfActive() const;
    std::size_t getInaccurateSizeOfReady() const;

public:
    MiscValidity validateOfActive(std::size_t min = 0, std::size_t max = 0) const;
    MiscValidity validateOfReady(std::size_t min = 0, std::size_t max = 0) const;

public:
    Err enqueue(MqMsg const & msg);
    Err enqueue(char const * data, std::size_t size);

public:
    Err dequeue(MqMsg & msg);
};

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_DETAILS_MQQUEUE_HPP__

