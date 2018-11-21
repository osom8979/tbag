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
        : THREAD_ID(std::this_thread::get_id()),
          QUEUE_SIZE(BoundedMpMcQueue::calcMinimumQueueSize(size)),
          __messages__(), __closed_messages__(0)
{
    _ready = std::make_unique<BoundedMpMcQueue>(QUEUE_SIZE);
    assert(static_cast<bool>(_ready));

    __messages__.resize(QUEUE_SIZE);
    for (std::size_t i = 0; i < QUEUE_SIZE; ++i) {
        auto async = loop.newHandle<AsyncMsg>(loop, msg_size, this);
        assert(static_cast<bool>(async));

        bool const ENQUEUE_RESULT = _ready->enqueue(async.get());
        assert(ENQUEUE_RESULT);

        __messages__.at(i) = async;
    }

    std::size_t const READY_QUEUE_SIZE = _ready->potentially_inaccurate_count();
    assert(QUEUE_SIZE == READY_QUEUE_SIZE);
}

MqEventQueue::~MqEventQueue()
{
    assert(THREAD_ID == std::this_thread::get_id());
}

void MqEventQueue::onAsyncMsg(AsyncMsg * async)
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

void MqEventQueue::onCloseMsg(AsyncMsg * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);

    ++__closed_messages__;
    assert(__closed_messages__ <= QUEUE_SIZE);
    if (__closed_messages__ == QUEUE_SIZE) {
        onCloseMsgDone();
    }
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

std::size_t MqEventQueue::getInaccurateSizeOfActive() const
{
    return QUEUE_SIZE - getInaccurateSizeOfReady();
}

MqEventQueue::MiscValidity MqEventQueue::validateOfReady(std::size_t min, std::size_t max) const
{
    return _ready->singlethreaded_validate(min, max);
}

using AsyncMsg = MqEventQueue::AsyncMsg;
using UniqueQueue = MqEventQueue::UniqueQueue;

template <typename Predicated>
static Err __enqueue(UniqueQueue & ready, Predicated predicated)
{
    void * value = nullptr;
    if (!ready->dequeue(&value)) {
        return Err::E_NREADY;
    }

    auto * msg = (AsyncMsg*)value;
    assert(msg != nullptr);

    if (!predicated(msg)) {
        auto const RESULT = ready->enqueue(value);
        assert(RESULT);
        return Err::E_ECANCELED;
    }

    auto const CODE = msg->send();
    if (isFailure(CODE)) {
        auto const RESULT = ready->enqueue(value);
        assert(RESULT);
    }
    return CODE;
}

Err MqEventQueue::enqueueClose()
{
    return enqueue(MqMsg(MqEvent::ME_CLOSE));
}

Err MqEventQueue::enqueue(MqMsg const & msg)
{
    return __enqueue(_ready, MqMsgCopyFrom(msg));
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

void MqEventQueue::onCloseMsgDone()
{
    assert(THREAD_ID == std::this_thread::get_id());
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

