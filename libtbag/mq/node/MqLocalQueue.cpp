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

    createTerminator(loop);

    _state = MqMachineState::MMS_ACTIVE;
    MqBase::enableWait();
}

MqLocalQueue::~MqLocalQueue()
{
    // EMPTY.
}

void MqLocalQueue::onTerminatorAsync(Terminator * terminator)
{
    assert(terminator != nullptr);
    assert(terminator == _terminator.get());
    onCloseStep1_TERMINATOR();
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
    auto const STATE = _state.load();

    using namespace libtbag::mq::details;
    if (STATE != MqMachineState::MMS_ACTIVE && STATE != MqMachineState::MMS_CLOSING) {
        tDLogIfW(PARAMS.verbose, "MqLocalQueue::onMsg() Illegal local-queue state({}), skip current message ({})",
                 getMachineStateName(_state), getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    assert(STATE == MqMachineState::MMS_ACTIVE);
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
    auto const STATE = _state.load();

    assert(STATE == MqMachineState::MMS_ACTIVE || STATE == MqMachineState::MMS_CLOSING);
    assert(msg != nullptr);
    assert(INTERNAL.recv_cb != nullptr);
    assert(INTERNAL.parent != nullptr);

    if (INTERNAL.recv_cb(*msg, INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
        tDLogIfD(PARAMS.verbose, "MqLocalQueue::onRead() Consumed this received message.");
        return;
    }

    enqueueReceiveForSingleProducer(*msg);
}

void MqLocalQueue::onCloseStep1_TERMINATOR()
{
}

void MqLocalQueue::onCloseStep1()
{
    auto const STATE = _state.load();

    if (STATE == MqMachineState::MMS_ACTIVE) {
        tDLogI("MqLocalQueue::onCloseStep1() Close message confirmed.");
    } else {
        tDLogIfW(PARAMS.verbose, "MqLocalQueue::onCloseStep1() It is already closing.");
        return;
    }

    changeClosingState();
    assert(STATE == MqMachineState::MMS_CLOSING);

    MqEventQueue::closeAsyncMessages();
}

void MqLocalQueue::onCloseStep2_EVENT_QUEUE_CLOSED()
{
    auto const STATE = _state.load();

    assert(!MqEventQueue::exists());
    assert(STATE == MqMachineState::MMS_CLOSING);

    tDLogI("MqLocalQueue::onCloseStep2_EVENT_QUEUE_CLOSED() Close local-queue!");

    changeClosedState();
    assert(STATE == MqMachineState::MMS_CLOSED);

    closeTerminator();
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

