/**
 * @file   MqLocalQueue.cpp
 * @brief  MqLocalQueue class implementation.
 * @author zer0
 * @date   2018-11-24
 */

#include <libtbag/mq/node/MqLocalQueue.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <chrono>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using AfterAction = MqLocalQueue::AfterAction;

MqLocalQueue::MqLocalQueue(Loop & loop, MqParams const & params)
        : MqBase(loop, params, MqMachineState::MMS_ACTIVE)
{
    // EMPTY.
}

MqLocalQueue::~MqLocalQueue()
{
    // EMPTY.
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
        PARAMS.recv_cb(*msg, this);
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

    using namespace libtbag::mq::details;
    if (isInactiveState(_state)) {
        assert(isCloseState(_state));
        tDLogIfW(PARAMS.verbose, "MqLocalQueue::onCloseEvent() It is already closing.");
        return;
    }

    assert(isActiveState(_state));
    _state = MqMachineState::MMS_CLOSING; // This prevents the send() method from receiving further input.
    std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.shutdown_wait_nanosec));
    while (_sending > 0) {
        // Busy waiting...
    }

    // [IMPORTANT]
    // From this moment on, there is no send-queue producer.
    assert(_sending == 0);

    closeAsyncMsgs();
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

