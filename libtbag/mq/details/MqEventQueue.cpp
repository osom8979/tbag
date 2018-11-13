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

    __local__.messages.resize(POWER_OF_2);
    for (std::size_t i = 0; i < POWER_OF_2; ++i) {
        auto async = loop.newHandle<AsyncMsg>(loop, msg_size, this);
        assert(static_cast<bool>(async));

        bool const ENQUEUE_RESULT = _ready->enqueue(async.get());
        assert(ENQUEUE_RESULT);

        __local__.messages.at(i) = async;
    }

    std::size_t const READY_QUEUE_SIZE = _ready->potentially_inaccurate_count();
    assert(POWER_OF_2 == READY_QUEUE_SIZE);

    __local__.sender = loop.newHandle<SendAsync>(loop, this);
    assert(static_cast<bool>(__local__.sender));
}

MqEventQueue::~MqEventQueue()
{
    assert(THREAD_ID == std::this_thread::get_id());
}

void MqEventQueue::onAsync(AsyncMsg * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);

    if (async->event == MqEvent::ME_MSG) {
        __local__.queue.push(AsyncMsgPointer(async));
        assert(static_cast<bool>(__local__.sender));
        if (__local__.state == SendState::SS_WAITING) {
            auto const CODE = __local__.sender->send();
            assert(isSuccess(CODE));
            __local__.state = SendState::SS_ASYNC;
        }
    } else {
        onEvent(async->event, async->box.cast<char>(), async->box.size());
        auto const RESULT = _ready->enqueue(async);
        assert(RESULT);
    }
}

void MqEventQueue::onAsync(SendAsync * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);
    assert(!__local__.queue.empty());
    assert(__local__.state == SendState::SS_ASYNC);

    AsyncMsgPointer pointer = __local__.queue.front();
    assert(static_cast<bool>(pointer));

    AsyncMsg * msg = pointer.get();
    assert(msg != nullptr);

    __local__.state = SendState::SS_SENDING;
    onSend(msg->box.cast<char>(), msg->box.size());
}

void MqEventQueue::onClose(AsyncMsg * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);
}

void MqEventQueue::onClose(SendAsync * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);
}

void MqEventQueue::closeAsyncMsgs()
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(__local__.sender));
    __local__.sender->close();
}

void MqEventQueue::closeSendAsync()
{
    assert(THREAD_ID == std::this_thread::get_id());
    for (auto & msg : __local__.messages) {
        assert(static_cast<bool>(msg));
        msg->close();
    }
}

void MqEventQueue::closeAll()
{
    closeAsyncMsgs();
    closeSendAsync();
}

bool MqEventQueue::isWatingSender() const
{
    return __local__.queue.empty() && __local__.state == SendState::SS_WAITING;
}

void MqEventQueue::doneWrite(Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(!__local__.queue.empty());
    assert(__local__.state == SendState::SS_SENDING);

    auto ptr = __local__.queue.front();
    assert(static_cast<bool>(ptr));
    __local__.queue.pop();

    auto const RESULT = _ready->enqueue(ptr.get());
    assert(RESULT);

    if (__local__.queue.empty()) {
        __local__.state = SendState::SS_WAITING;
    } else {
        AsyncMsgPointer pointer = __local__.queue.front();
        assert(static_cast<bool>(pointer));

        AsyncMsg * msg = pointer.get();
        assert(msg != nullptr);

        auto const CODE = __local__.sender->send();
        assert(isSuccess(CODE));
        __local__.state = SendState::SS_ASYNC;
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

Err MqEventQueue::enqueueShutdown()
{
    return enqueue(MqMsgCopyFrom(MqEvent::ME_SHUTDOWN));
}

Err MqEventQueue::enqueueClose()
{
    return enqueue(MqMsgCopyFrom(MqEvent::ME_CLOSE));
}

Err MqEventQueue::enqueue(char const * data, std::size_t size)
{
    return enqueue(MqMsgCopyFrom(MqEvent::ME_MSG, data, size));
}

void MqEventQueue::onEvent(MqEvent event, char const * data, std::size_t size)
{
    assert(THREAD_ID == std::this_thread::get_id());
}

void MqEventQueue::onSend(char const * data, std::size_t size)
{
    assert(THREAD_ID == std::this_thread::get_id());
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

