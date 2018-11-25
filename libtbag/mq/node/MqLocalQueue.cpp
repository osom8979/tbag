/**
 * @file   MqLocalQueue.cpp
 * @brief  MqLocalQueue class implementation.
 * @author zer0
 * @date   2018-11-24
 */

#include <libtbag/mq/node/MqLocalQueue.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using AfterAction = MqLocalQueue::AfterAction;

MqLocalQueue::MqLocalQueue(Loop & loop, MqParams const & params, MqRecvCallback * cb)
        : MqEventQueue(loop, params.send_queue_size, params.send_msg_size),
          PARAMS(params), _callback(cb),
          _receives(params.recv_queue_size, params.recv_msg_size),
          _state(MqMachineState::MMS_ACTIVE), _sending(0)
{
    // EMPTY.
}

MqLocalQueue::~MqLocalQueue()
{
    _wait_lock.lock();
    _state = MqMachineState::MMS_DESTROYING;
    _wait_cond.broadcast();
    _wait_lock.unlock();
}

void MqLocalQueue::onCloseMsgDone()
{
    assert(THREAD_ID == std::this_thread::get_id());

    using namespace libtbag::mq::details;
    assert(isClosingState(_state));

    tDLogI("MqLocalQueue::onClose() Close this client!");

    // Last closing point.
    _state = MqMachineState::MMS_CLOSED;
}

AfterAction MqLocalQueue::onMsg(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (!isActiveState(_state)) {
        tDLogIfW(PARAMS.verbose,
                 "MqLocalQueue::onMsg() Illegal client state, "
                 "skip current message ({})", getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    assert(_state == MqMachineState::MMS_ACTIVE);
    switch (msg->event) {
    case MqEvent::ME_MSG:
        return onMsgEvent(msg);

    case MqEvent::ME_CLOSE:
        onCloseEvent();
        break;

    case MqEvent::ME_NONE:
        break;

    default:
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        break;
    }
    return AfterAction::AA_OK;
}

AfterAction MqLocalQueue::onMsgEvent(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    if (PARAMS.recv_cb != nullptr) {
        assert(_callback != nullptr);
        _callback->onRecv(*msg);
    } else {
        Err enqueue_code;

        COMMENT("Single-Producer recv-queue") {
            while (!_wait_lock.tryLock()) {
                // Busy waiting...
            }
            enqueue_code = _receives.enqueue(*msg);
            _wait_cond.signal();
            _wait_lock.unlock();
        }

        if (isSuccess(enqueue_code)) {
            tDLogIfD(PARAMS.verbose, "MqLocalQueue::onMsgEvent() Enqueue success.");
        } else {
            tDLogE("MqLocalQueue::onMsgEvent() Enqueue error: {}", enqueue_code);
        }
    }

    return AfterAction::AA_OK;
}

void MqLocalQueue::onCloseEvent()
{
    assert(THREAD_ID == std::this_thread::get_id());
    tDLogI("MqLocalQueue::onCloseEvent() Close message confirmed.");

    closeAsyncMsgs();
}

Err MqLocalQueue::send(MqMsg const & msg)
{
    if (!details::isActiveState(_state)) {
        return Err::E_ILLSTATE;
    }

    // [WARNING]
    // This point is a dangerous point
    // where the number of times to send(<code>_sending</code>) out can be missed.
    // To avoid this, use sleep() on that thread.
    //
    // Note:
    // the moment when the <code>_state</code> information becomes
    // <code>MqMachineState::MMS_CLOSING</code>.

    ++_sending;
    auto const CODE = enqueue(msg);
    --_sending;
    return CODE;
}

Err MqLocalQueue::recv(MqMsg & msg)
{
    return _receives.dequeue(msg);
}

Err MqLocalQueue::recvWait(MqMsg & msg, uint64_t timeout_nano)
{
    using namespace std::chrono;
    auto const BEGIN = system_clock::now();
    nanoseconds const TIMEOUT(timeout_nano);
    nanoseconds remaining_timeout_nano;

    Err code;

    _wait_lock.lock();
    while (true) {
        code = _receives.dequeue(msg);
        if (isSuccess(code)) {
            break;
        }
        if (_state == MqMachineState::MMS_DESTROYING) {
            code = Err::E_ECANCELED;
            break;
        }

        if (timeout_nano == 0) {
            _wait_cond.wait(_wait_lock);
        } else {
            _wait_cond.wait(_wait_lock, (TIMEOUT - (system_clock::now() - BEGIN)).count());
            if ((system_clock::now() - BEGIN) >= TIMEOUT) {
                code = Err::E_TIMEOUT;
                break;
            }
        }
    }
    _wait_lock.unlock();
    return code;
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

