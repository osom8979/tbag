/**
 * @file   MqEventQueue.cpp
 * @brief  MqEventQueue class implementation.
 * @author zer0
 * @date   2018-11-12
 * @date   2018-11-13 (Rename: MqSendQueue -> MqEventQueue)
 */

#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

MqEventQueue::MqEventQueue(Loop & loop, std::size_t size, std::size_t msg_size)
        : THREAD_ID(std::this_thread::get_id())
{
    std::size_t const POWER_OF_2 = BoundedMpMcQueue::calcMinimumQueueSize(size);
    _ready = std::make_unique<BoundedMpMcQueue>(POWER_OF_2);
    assert(static_cast<bool>(_ready));

    __messages__.resize(POWER_OF_2);
    for (std::size_t i = 0; i < POWER_OF_2; ++i) {
        auto async = loop.newHandle<AsyncMsg>(loop, msg_size, this);
        assert(static_cast<bool>(async));

        bool const ENQUEUE_RESULT = _ready->enqueue(async.get());
        assert(ENQUEUE_RESULT);

        __messages__.at(i) = async;
    }

    std::size_t const READY_QUEUE_SIZE = _ready->potentially_inaccurate_count();
    assert(POWER_OF_2 == READY_QUEUE_SIZE);
}

MqEventQueue::~MqEventQueue()
{
    assert(THREAD_ID == std::this_thread::get_id());
}

void MqEventQueue::onAsync(AsyncMsg * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);
    auto const AFTER_ACTION = onMsg(async);
    if (AFTER_ACTION == AfterAction::AA_OK) {
        auto const RESULT = _ready->enqueue(async);
        assert(RESULT);
    } else {
        assert(AFTER_ACTION == AfterAction::AA_DELAY);
    }
}

void MqEventQueue::onClose(AsyncMsg * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);
}

void MqEventQueue::closeAsyncMsgs()
{
    assert(THREAD_ID == std::this_thread::get_id());
    for (auto & msg : __messages__) {
        assert(static_cast<bool>(msg));
        msg->close();
    }
}

std::size_t MqEventQueue::getInaccurateSizeOfReady() const
{
    return _ready->potentially_inaccurate_count();
}

MqEventQueue::MiscValidity MqEventQueue::validateOfReady(std::size_t min, std::size_t max) const
{
    return _ready->singlethreaded_validate(min, max);
}

Err MqEventQueue::enqueueClose()
{
    return enqueue(MqMsg(MqEvent::ME_CLOSE));
}

Err MqEventQueue::enqueue(MqMsg const & msg)
{
    return enqueue(MqMsgCopyFrom(msg));
}

Err MqEventQueue::enqueue(MqEvent e, char const * data, std::size_t size)
{
    return enqueue(MqMsg(e, data, size));
}

Err MqEventQueue::enqueue(char const * data, std::size_t size)
{
    return enqueue(MqMsg(data, size));
}

MqEventQueue::AfterAction MqEventQueue::onMsg(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);
    return AfterAction::AA_OK;
}

Err MqEventQueue::restoreMessage(AsyncMsg * msg, bool verify)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    if (verify) {
        bool ok = false;
        for (auto & cursor : __messages__) {
            assert(static_cast<bool>(cursor));
            if (msg == cursor.get()) {
                ok = true;
                break;
            }
        }
        if (!ok) {
            return Err::E_ILLARGS;
        }
    }

    auto const RESULT = _ready->enqueue(msg);
    assert(RESULT);
    return RESULT ? Err::E_SUCCESS : Err::E_EPUSH;
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

