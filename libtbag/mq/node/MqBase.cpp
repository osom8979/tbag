/**
 * @file   MqBase.cpp
 * @brief  MqBase class implementation.
 * @author zer0
 * @date   2018-11-25
 */

#include <libtbag/mq/node/MqBase.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

MqBase::MqBase(Loop & loop, MqInternal const & internal, MqParams const & params, MqMachineState state)
        : MqEventQueue(loop, params.send_queue_size, params.send_msg_size),
          INTERNAL(internal), PARAMS(params),
          IS_SOCKET_SERVER(libtbag::mq::details::isSocketServerMode(internal)),
          IS_SOCKET_CLIENT(libtbag::mq::details::isSocketClientMode(internal)),
          IS_MQ_NODE(libtbag::mq::details::isMqNodeMode(internal)),
          _receives(params.recv_queue_size, params.recv_msg_size),
          _state(state), _sending(0), _wait_enable(false)
{
    // EMPTY.
}

MqBase::~MqBase()
{
    disableWait();
    assert(_state == MqMachineState::MMS_CLOSED);
}

bool MqBase::isWaitEnable() const
{
    bool result;
    _wait_lock.lock();
    result = _wait_enable;
    _wait_lock.unlock();
    return result;
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

MqBase::MqMachineState MqBase::state() const TBAG_NOEXCEPT
{
    return _state;
}

MqBase::MqParams MqBase::params() const
{
    return PARAMS;
}

Err MqBase::send(MqMsg const & msg)
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

