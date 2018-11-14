/**
 * @file   MqQueue.cpp
 * @brief  MqQueue class implementation.
 * @author zer0
 * @date   2018-11-10
 */

#include <libtbag/mq/details/MqQueue.hpp>
#include <libtbag/bitwise/BitHacks.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <cmath>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

MqQueue::MqQueue(std::size_t size, std::size_t msg_size) : _active(), _ready()
{
    std::size_t const POWER_OF_2 = BoundedMpMcQueue::calcMinimumQueueSize(size);
    _active = std::make_unique<Queue>(this, POWER_OF_2);
    _ready = std::make_unique<Queue>(this, POWER_OF_2);
    assert(static_cast<bool>(_active));
    assert(static_cast<bool>(_ready));
    for (std::size_t i = 0; i < POWER_OF_2; ++i) {
        bool const ENQUEUE_RESULT = _ready->enqueue(createMsg(msg_size));
        assert(ENQUEUE_RESULT);
    }
    std::size_t const READY_QUEUE_SIZE = _ready->potentially_inaccurate_count();
    assert(POWER_OF_2 == READY_QUEUE_SIZE);
    std::size_t const ACTIVE_QUEUE_SIZE = _active->potentially_inaccurate_count();
    assert(0 == ACTIVE_QUEUE_SIZE);
}

MqQueue::~MqQueue()
{
    _active.reset();
    _ready.reset();
}

MqMsg * MqQueue::createMsg(std::size_t size)
{
    return new MqMsg(size);
}

void MqQueue::removeMsg(MqMsg * value)
{
    delete value;
}

std::size_t MqQueue::getInaccurateSizeOfActive() const
{
    return _active->potentially_inaccurate_count();
}

std::size_t MqQueue::getInaccurateSizeOfReady() const
{
    return _ready->potentially_inaccurate_count();
}

MqQueue::MiscValidity MqQueue::validateOfActive(std::size_t min, std::size_t max) const
{
    return _active->singlethreaded_validate(min, max);
}

MqQueue::MiscValidity MqQueue::validateOfReady(std::size_t min, std::size_t max) const
{
    return _ready->singlethreaded_validate(min, max);
}

Err MqQueue::enqueue(MqMsg const & msg)
{
    return enqueue(msg.event, msg.data(), msg.size());
}

Err MqQueue::enqueue(char const * data, std::size_t size)
{
    return enqueue(MqEvent::ME_MSG, data, size);
}

Err MqQueue::enqueue(MqEvent event, char const * data, std::size_t size)
{
    return enqueue(MqMsgCopyFrom(event, data, size));
}

Err MqQueue::dequeue(char * data, std::size_t max_buffer, MqEvent * event, std::size_t * size)
{
    return dequeue(MqMsgCopyTo(event, data, max_buffer, size));
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

