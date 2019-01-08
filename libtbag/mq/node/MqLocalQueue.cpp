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

MqLocalQueue::MqLocalQueue(Loop & loop, MqInternal const & internal, MqParams const & params)
        : MqBase(loop, internal, params, MqMachineState::MMS_ACTIVE)
{
    MqBase::enableWait();
}

MqLocalQueue::~MqLocalQueue()
{
    // EMPTY.
}

void MqLocalQueue::onCloseMsgDone()
{
    using namespace libtbag::mq::details;
    assert(_state == MqMachineState::MMS_CLOSING);

    tDLogI("MqLocalQueue::onClose() Close this client!");

    // Last closing point.
    _state = MqMachineState::MMS_CLOSED;
    MqBase::disableWait();
}

AfterAction MqLocalQueue::onMsg(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (!isActiveState(_state)) {
        tDLogIfW(PARAMS.verbose, "MqLocalQueue::onMsg() Illegal client state, skip current message ({})",
                 getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    assert(_state == MqMachineState::MMS_ACTIVE);
    if (msg->event == ME_CLOSE) {
        onCloseEvent();
        return AfterAction::AA_OK;
    } else {
        onMsgEvent(msg);
        return AfterAction::AA_OK;
    }
}

void MqLocalQueue::onMsgEvent(AsyncMsg * msg)
{
    assert(msg != nullptr);
    assert(INTERNAL.recv_cb != nullptr);
    assert(INTERNAL.parent != nullptr);

    if (INTERNAL.recv_cb(*msg, INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
        tDLogIfD(PARAMS.verbose, "MqLocalQueue::onMsgEvent() Consumed this received message.");
        return;
    }

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
        tDLogIfD(PARAMS.verbose, "MqLocalQueue::onMsgEvent() Enqueue success. "
                 "Perhaps the remaining queue size is {}.", _receives.getInaccurateSizeOfActive());
    } else {
        tDLogE("MqLocalQueue::onMsgEvent() Enqueue error: {}", enqueue_code);
    }
}

void MqLocalQueue::onCloseEvent()
{
    tDLogI("MqLocalQueue::onCloseEvent() Close message confirmed.");

    using namespace libtbag::mq::details;
    if (_state != MqMachineState::MMS_ACTIVE) {
        tDLogIfW(PARAMS.verbose, "MqLocalQueue::onCloseEvent() It is already closing.");
        return;
    }

    assert(isActiveState(_state));
    _state = MqMachineState::MMS_CLOSING; // This prevents the send() method from receiving further input.
    std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.wait_next_opcode_nanosec));
    while (_sending > 0) {
        // Busy waiting...
    }

    // [IMPORTANT]
    // From this moment on, there is no send-queue producer.
    assert(_sending == 0);
    MqEventQueue::closeAsyncMessages();
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

