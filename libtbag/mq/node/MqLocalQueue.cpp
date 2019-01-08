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
        : MqBase(internal, params)
{
    assert(!MqEventQueue::exists());

    if (params.type != MqType::MT_LOCAL) {
        tDLogE("MqLocalQueue::MqLocalQueue() Unsupported type: {}({})", getTypeName(), getTypeInteger());
        throw ErrException(Err::E_ILLARGS);
    }

    bool const EVENT_QUEUE_INIT = MqEventQueue::initialize(loop, PARAMS.send_queue_size, PARAMS.send_msg_size);
    assert(EVENT_QUEUE_INIT);
    assert(MqEventQueue::exists());

    _terminator = loop.newHandle<Terminator>(loop, this);
    assert(static_cast<bool>(_terminator));
    assert(_terminator->isInit());

    _state = MqMachineState::MMS_ACTIVE;
    MqBase::enableWait();
}

MqLocalQueue::~MqLocalQueue()
{
    // EMPTY.
}

void MqLocalQueue::onTerminatorAsync(Terminator * terminator)
{
}

void MqLocalQueue::onTerminatorClose(Terminator * terminator)
{
    assert(terminator != nullptr);
    assert(terminator == _terminator.get());
    _terminator.reset();
    onCloseStep3_TERMINATOR_CLOSED();
}

void MqLocalQueue::onCloseMsgDone()
{
    MqEventQueue::clear();
    onCloseStep2_EVENT_QUEUE_CLOSED();
}

AfterAction MqLocalQueue::onMsg(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (_state != MqMachineState::MMS_ACTIVE && _state != MqMachineState::MMS_CLOSING) {
        tDLogIfW(PARAMS.verbose, "MqLocalQueue::onMsg() Illegal local-queue state({}), skip current message ({})",
                 getMachineStateName(_state), getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    assert(_state == MqMachineState::MMS_ACTIVE);
    if (msg->event == ME_CLOSE) {
        onCloseStep1();
        return AfterAction::AA_OK;
    } else {
        onRead(msg);
        return AfterAction::AA_OK;
    }
}

void MqLocalQueue::onRead(AsyncMsg * msg)
{
    assert(MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_ACTIVE || _state == MqMachineState::MMS_CLOSING);
    assert(msg != nullptr);
    assert(INTERNAL.recv_cb != nullptr);
    assert(INTERNAL.parent != nullptr);

    if (INTERNAL.recv_cb(*msg, INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
        tDLogIfD(PARAMS.verbose, "MqLocalQueue::onRead() Consumed this received message.");
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
        tDLogIfD(PARAMS.verbose, "MqLocalQueue::onRead() Enqueue success. "
                 "Perhaps the remaining queue size is {}.", _receives.getInaccurateSizeOfActive());
    } else {
        tDLogE("MqLocalQueue::onRead() Enqueue error: {}", enqueue_code);
    }
}

void MqLocalQueue::onCloseStep1()
{
    tDLogI("MqLocalQueue::onCloseStep1() Close message confirmed.");

    using namespace libtbag::mq::details;
    if (_state != MqMachineState::MMS_ACTIVE) {
        tDLogIfW(PARAMS.verbose, "MqLocalQueue::onCloseStep1() It is already closing.");
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

void MqLocalQueue::onCloseStep2_EVENT_QUEUE_CLOSED()
{
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSING);

    tDLogI("MqLocalQueue::onCloseStep2_EVENT_QUEUE_CLOSED() Close local-queue!");
    _state = MqMachineState::MMS_CLOSED;
    MqBase::disableWait();

    std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.wait_next_opcode_nanosec));
    while (_exiting > 0) {
        // Busy waiting...
    }

    assert(_exiting == 0);
    assert(static_cast<bool>(_terminator));
    assert(!_terminator->isClosing());
    _terminator->close();
}

void MqLocalQueue::onCloseStep3_TERMINATOR_CLOSED()
{
    assert(!static_cast<bool>(_terminator));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSED);

    if (INTERNAL.close_cb != nullptr) {
        assert(INTERNAL.parent != nullptr);
        INTERNAL.close_cb(INTERNAL.parent);
    }
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

