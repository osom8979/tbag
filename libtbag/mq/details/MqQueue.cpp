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

MqQueue::MqQueue(std::size_t size, std::size_t msg_size)
        : _active(), _ready()
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

using UniqueQueue = MqQueue::UniqueQueue;

template <typename Predicated>
static Err __enqueue(UniqueQueue & ready, UniqueQueue & active, Predicated predicated)
{
    void * value = nullptr;
    if (!ready->dequeue(&value)) {
        return E_NREADY;
    }

    auto * msg = (MqMsg*)value;
    assert(msg != nullptr);
    if (!predicated(msg)) {
        auto const RESULT = ready->enqueue(value);
        assert(RESULT);
        return E_ECANCELED;
    }

    auto const RESULT = active->enqueue(value);
    assert(RESULT);
    return E_SUCCESS;
}

template <typename Predicated>
static Err __dequeue(UniqueQueue & ready, UniqueQueue & active, Predicated predicated)
{
    void * value = nullptr;
    if (!active->dequeue(&value)) {
        return E_NREADY;
    }

    auto * msg = (MqMsg*)value;
    assert(msg != nullptr);
    if (!predicated(msg)) {
        auto const RESULT = active->enqueue(value);
        assert(RESULT);
        return E_ECANCELED;
    }

    auto const RESULT = ready->enqueue(value);
    assert(RESULT);
    return E_SUCCESS;
}

Err MqQueue::enqueue(MqMsg const & msg)
{
    return __enqueue(_ready, _active, MqMsgCopyFrom(msg));
}

Err MqQueue::enqueue(char const * data, std::size_t size)
{
    return enqueue(MqMsg(data, size));
}

Err MqQueue::dequeue(MqMsg & msg)
{
    return __dequeue(_ready, _active, MqMsgCopyTo(msg));
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

