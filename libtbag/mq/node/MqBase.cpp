/**
 * @file   MqBase.cpp
 * @brief  MqBase class implementation.
 * @author zer0
 * @date   2018-11-25
 */

#include <libtbag/mq/node/MqBase.hpp>
#include <libtbag/log/Log.hpp>

#include <chrono>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using MqMachineState = MqBase::MqMachineState;
using MqParams       = MqBase::MqParams;

MqBase::MqBase(MqInternal const & internal, MqParams const & params)
        : MqEventQueue(), INTERNAL(internal), PARAMS(params),
          _receives(params.recv_queue_size, params.recv_msg_size),
          _state(MqMachineState::MMS_CLOSED), _sending(0), _exiting(0),
          _wait_enable(false)
{
    // EMPTY.
}

MqBase::MqBase(Loop & loop, MqInternal const & internal, MqParams const & params)
        : MqBase(internal, params)
{
    if (!MqEventQueue::initialize(loop, params.send_queue_size, params.send_msg_size)) {
        throw std::bad_alloc();
    }
}

MqBase::~MqBase()
{
    assert(_state == MqMachineState::MMS_CLOSED);
    disableWait();
}

std::size_t MqBase::getActiveSendSize() const
{
    return MqEventQueue::getInaccurateSizeOfActive();
}

std::size_t MqBase::getActiveRecvSize() const
{
    return _receives.getInaccurateSizeOfActive();
}

void MqBase::enableWait(bool enable)
{
    _wait_lock.lock();
    _wait_enable = enable;
    _wait_cond.broadcast();
    _wait_lock.unlock();
}

void MqBase::disableWait()
{
    enableWait(false);
}

void MqBase::createTerminator(Loop & loop)
{
    _terminator = loop.newHandle<Terminator>(loop, this);
    assert(static_cast<bool>(_terminator));
    assert(_terminator->isInit());
}

void MqBase::closeTerminator()
{
    assert(static_cast<bool>(_terminator));
    assert(!_terminator->isClosing());
    _terminator->close();
}

void MqBase::changeClosingState()
{
    _state = MqMachineState::MMS_CLOSING; // This prevents the send() method from receiving further input.
    std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.wait_next_opcode_nanosec));
    while (_sending > 0) {
        COMMENT("Busy waiting");
    }
    IMPORTANT("From this moment on, there is no send-queue producer.");
    assert(_sending == 0);
}

void MqBase::changeClosedState()
{
    _state = MqMachineState::MMS_CLOSED; // This prevents the exit() method from receiving further input.
    disableWait();
    std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.wait_next_opcode_nanosec));
    while (_exiting > 0) {
        COMMENT("Busy waiting");
    }
    IMPORTANT("From this moment on, there is no exit-request producer.");
    assert(_exiting == 0);
}

Err MqBase::enqueueReceiveForSingleProducer(MqMsg const & msg)
{
    while (!_wait_lock.tryLock()) {
        // Busy waiting...
    }
    Err const CODE = _receives.enqueue(msg);
    _wait_cond.signal();
    _wait_lock.unlock();

    if (isSuccess(CODE)) {
        tDLogIfD(PARAMS.verbose,
                 "MqBase::enqueueReceiveForSingleProducer() Enqueue success. "
                 "Perhaps the remaining queue size is {}.",
                 _receives.getInaccurateSizeOfActive());
    } else {
        tDLogE("MqBase::enqueueReceiveForSingleProducer() Enqueue error: {}", CODE);
    }

    return CODE;
}

Err MqBase::exit()
{
    if (_state == libtbag::mq::details::MqMachineState::MMS_CLOSED) {
        return Err::E_ILLSTATE;
    }

    assert(static_cast<bool>(_terminator));

    // [WARNING]
    // This point is a dangerous point
    // where the number of times to send(<code>_exiting</code>) out can be missed.
    // To avoid this, use sleep() on that thread.
    //
    // Note:
    // the moment when the <code>_state</code> information becomes
    // <code>MqMachineState::MMS_CLOSED</code>.

    ++_exiting;
    Err code;
    if (_terminator->isClosing()) {
        code = Err::E_CLOSING;
    } else {
        code = _terminator->send();
    }
    --_exiting;
    return code;
}

Err MqBase::send(MqMsg const & msg)
{
    if (!libtbag::mq::details::isActiveState(_state)) {
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
    auto const CODE = MqEventQueue::enqueue(msg);
    --_sending;
    return CODE;
}

Err MqBase::recv(MqMsg & msg)
{
    return _receives.dequeue(msg);
}

Err MqBase::waitEnable(uint64_t timeout_nano)
{
    using namespace std::chrono;
    auto const BEGIN = system_clock::now();
    nanoseconds const TIMEOUT(timeout_nano);

    Err code;

    _wait_lock.lock();
    while (true) {
        if (_wait_enable) {
            code = Err::E_SUCCESS;
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

Err MqBase::waitRecv(MqMsg & msg, uint64_t timeout_nano)
{
    using namespace std::chrono;
    auto const BEGIN = system_clock::now();
    nanoseconds const TIMEOUT(timeout_nano);

    Err code;

    _wait_lock.lock();
    while (true) {
        code = _receives.dequeue(msg);
        if (isSuccess(code)) {
            break;
        }
        if (!_wait_enable) {
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

