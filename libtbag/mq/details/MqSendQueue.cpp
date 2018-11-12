/**
 * @file   MqSendQueue.cpp
 * @brief  MqSendQueue class implementation.
 * @author zer0
 * @date   2018-11-12
 */

#include <libtbag/mq/details/MqSendQueue.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

MqSendQueue::MqSendQueue(Loop & loop, std::size_t size, std::size_t msg_size)
{
    std::size_t const POWER_OF_2 = BoundedMpMcQueue::calcMinimumQueueSize(size);
    _ready = std::make_unique<BoundedMpMcQueue>(POWER_OF_2);
    assert(static_cast<bool>(_ready));
    for (std::size_t i = 0; i < POWER_OF_2; ++i) {
        auto async = loop.newHandle<AsyncMsg>(loop, msg_size, this);
        bool const ENQUEUE_RESULT = _ready->enqueue(async.get());
        assert(ENQUEUE_RESULT);
    }
    std::size_t const READY_QUEUE_SIZE = _ready->potentially_inaccurate_count();
    assert(POWER_OF_2 == READY_QUEUE_SIZE);
}

MqSendQueue::~MqSendQueue()
{
    // EMPTY.
}

void MqSendQueue::onAsync(AsyncMsg * async)
{
    // EMPTY.
}

void MqSendQueue::onClose(AsyncMsg * async)
{
    // EMPTY.
}

std::size_t MqSendQueue::getInaccurateSizeOfReady() const
{
    return _ready->potentially_inaccurate_count();
}

MqSendQueue::MiscValidity MqSendQueue::validateOfReady(std::size_t min, std::size_t max) const
{
    return _ready->singlethreaded_validate(min, max);
}

Err MqSendQueue::enqueue(char const * data, std::size_t size)
{
    return enqueue(MqMsgCopyFrom(data, size));
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

