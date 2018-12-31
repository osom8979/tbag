/**
 * @file   MqEventQueue.cpp
 * @brief  MqEventQueue class implementation.
 * @author zer0
 * @date   2018-11-12
 * @date   2018-11-13 (Rename: MqSendQueue -> MqEventQueue)
 */

#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

MqEventQueue::MqEventQueue() : _queue_size(0), _close_counter(0)
{
    // EMPTY.
}

MqEventQueue::MqEventQueue(Loop & loop, std::size_t queue_size, std::size_t packet_size) : MqEventQueue()
{
    if (!initialize(loop, queue_size, packet_size)) {
        throw std::bad_alloc();
    }
}

MqEventQueue::~MqEventQueue()
{
    clear();
}

void MqEventQueue::onAsyncMsg(AsyncMsg * async)
{
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
    ++_close_counter;
    assert(_close_counter <= _queue_size);
    if (_close_counter == _queue_size) {
        onCloseMsgDone();
    }
}

bool MqEventQueue::initialize(Loop & loop, std::size_t queue_size, std::size_t packet_size)
{
    if (exists()) {
        return false;
    }

    _close_counter = 0;
    _queue_size = BoundedMpMcQueue::calcMinimumQueueSize(queue_size);
    assert(_queue_size >= 1);

    _ready = std::make_unique<BoundedMpMcQueue>(_queue_size);
    assert(static_cast<bool>(_ready));

    _active.resize(_queue_size);
    for (std::size_t i = 0; i < _queue_size; ++i) {
        auto async = loop.newHandle<AsyncMsg>(loop, packet_size, this);
        assert(static_cast<bool>(async));

        bool const ENQUEUE_RESULT = _ready->enqueue(async.get());
        assert(ENQUEUE_RESULT);

        _active.at(i) = async;
    }

    assert(_ready->potentially_inaccurate_count() == _queue_size);
    return true;
}

void MqEventQueue::closeAsyncMessages()
{
    for (auto & msg : _active) {
        assert(static_cast<bool>(msg));
        msg->close();
    }
}

void MqEventQueue::clear()
{
    if (_queue_size >= 1 && _queue_size != _close_counter) {
        tDLogW("MqEventQueue::clear() An unclosed handle exists: {}/{}",
               _close_counter, _queue_size);
    }

    _queue_size = 0;
    _close_counter = 0;
    _ready.reset();
    _active.clear();
}

std::size_t MqEventQueue::getInaccurateSizeOfReady() const
{
    return _ready->potentially_inaccurate_count();
}

std::size_t MqEventQueue::getInaccurateSizeOfActive() const
{
    return _queue_size - getInaccurateSizeOfReady();
}

MqEventQueue::MiscValidity MqEventQueue::validateOfReady(std::size_t min, std::size_t max) const
{
    return _ready->singlethreaded_validate(min, max);
}

using AsyncMsg    = MqEventQueue::AsyncMsg;
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
    return enqueue(MqMsg(ME_CLOSE));
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
    return AfterAction::AA_OK;
}

void MqEventQueue::onCloseMsgDone()
{
    // EMPTY.
}

Err MqEventQueue::restoreMessage(AsyncMsg * msg, bool verify)
{
    assert(msg != nullptr);

    if (verify) {
        bool ok = false;
        for (auto & cursor : _active) {
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

