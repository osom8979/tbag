/**
 * @file   MqUdp.cpp
 * @brief  MqUdp class implementation.
 * @author zer0
 * @date   2018-12-09
 */

#include <libtbag/mq/node/MqUdp.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf        = MqUdp::binf;
using AfterAction = MqUdp::AfterAction;

using UdpSendRequest = MqUdp::UdpSendRequest;

MqUdp::MqUdp(Loop & loop, MqInternal const & internal, MqParams const & params)
        : MqBase(loop, internal, params),
          _packer(params.packer_size), _read_error_count(0)
{
    if (PARAMS.type == MqType::MT_UDP) {
        _node = loop.newHandle<Node>(loop, this);
    } else {
        tDLogE("MqUdp::MqUdp() Unsupported type: {}", (int)(PARAMS.type));
        throw ErrException(Err::E_ILLARGS);
    }

    assert(PARAMS.type == MqType::MT_UDP);
    assert(static_cast<bool>(_node));
    assert(_node->isInit());

    SocketAddress addr;
    auto const INIT_CODE = addr.init(params.address, params.port);
    if (isFailure(INIT_CODE)) {
        tDLogE("MqUdp::MqUdp() Socket init error: {}", INIT_CODE);
        _node->close();
        throw ErrException(INIT_CODE);
    }

    int flags = 0;
    if (params.tcp_ipv6_only) {
        flags = libtbag::uvpp::UDP_FLAG_IPV6ONLY;
    }

    auto const BIND_CODE = _node->bind(addr.getCommon(), flags);
    if (isFailure(BIND_CODE)) {
        tDLogE("MqUdp::MqUdp() Socket connect error: {}", BIND_CODE);
        _node->close();
        throw ErrException(BIND_CODE);
    }

    auto const RECV_CODE = _node->startRecv();
    if (isFailure(RECV_CODE)) {
        tDLogE("MqUdp::MqUdp() Start recv error: {}", RECV_CODE);
        _node->close();
        throw ErrException(RECV_CODE);
    }

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    _state = MqMachineState::MMS_INITIALIZED;
    //MqBase::enableWait();
}

MqUdp::~MqUdp()
{
    // EMPTY.
}

void MqUdp::onInitializerAsync(Initializer * init)
{
}

void MqUdp::onInitializerClose(Initializer * init)
{
}

void MqUdp::onTerminatorAsync(Terminator * terminator)
{
}

void MqUdp::onTerminatorClose(Terminator * terminator)
{
}

void MqUdp::close()
{
    assert(static_cast<bool>(_node));
    if (!_node->isClosing()) {
        _node->close();
    }
}

void MqUdp::shutdownAndClose()
{
    assert(_writer->queue.empty());

    // UDP does not support shutdown operation.

    auto * loop = _node->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this);
    assert(static_cast<bool>(timer));

    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    assert(isSuccess(START_CODE));
}

void MqUdp::tearDown(bool on_message)
{
    using namespace libtbag::mq::details;
    if (_state != MqMachineState::MMS_ACTIVE) {
        tDLogIfD(PARAMS.verbose, "MqUdp::tearDown() It is already closing.");
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
    assert(static_cast<bool>(_writer));

    std::size_t active_send_size = getInaccurateSizeOfActive();
    if (on_message) {
        assert(active_send_size >= 1);
        // Subtract the current message.
        --active_send_size;
    }

    if (active_send_size > 0 || _writer->state != MqRequestState::MRS_WAITING) {
        // Clear any remaining transmission queues and continue with system shutdown.
        tDLogI("MqUdp::tearDown() Delay the shutdown request ...");
        return;
    }

    assert(_writer->queue.empty());
    shutdownAndClose();
}

void MqUdp::onCloseMsgDone()
{
    assert(!_writer->isClosing());
    _writer->close();

    if (PARAMS.verbose) {
        tDLogD("MqUdp::onCloseMsgDone() Close request of writer. "
               "Next, onWriterClose() event method.");
    } else {
        tDLogD("MqUdp::onCloseMsgDone() Close request of writer.");
    }
}

AfterAction MqUdp::onMsg(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (!isActiveState(_state) && !isClosingState(_state)) {
        tDLogIfW(PARAMS.verbose,
                 "MqUdp::onMsg() Illegal client state({}), skip current message ({})",
                 getMachineStateName(_state), getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    if (msg->event == ME_CLOSE) {
        onCloseEvent();
        return AfterAction::AA_OK;
    } else {
        return onMsgEvent(msg);
    }
}

AfterAction MqUdp::onMsgEvent(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    _writer->queue.push(AsyncMsgPointer(msg));

    if (_writer->state != MqRequestState::MRS_WAITING) {
        tDLogIfD(PARAMS.verbose, "MqUdp::onMsgEvent() Writer is already working");
        return AfterAction::AA_DELAY;
    }

    auto const CODE = _writer->send();
    assert(isSuccess(CODE));

    _writer->state = MqRequestState::MRS_ASYNC;
    return AfterAction::AA_DELAY;
}

void MqUdp::onCloseEvent()
{
    tDLogI("MqUdp::onCloseEvent() Close message confirmed.");
    tearDown(true);
}

void MqUdp::afterProcessMessage(AsyncMsg * msg)
{
    assert(msg != nullptr);

    _writer->queue.pop();
    auto const CODE = restoreMessage(msg, PARAMS.verify_restore_message);
    assert(isSuccess(CODE));

    if (!_writer->queue.empty()) {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
        tDLogIfD(PARAMS.verbose,
                 "MqUdp::afterProcessMessage() Async next message (queue:{}) ...",
                 _writer->queue.size());
        return;
    }

    assert(_writer->queue.empty());
    _writer->state = MqRequestState::MRS_WAITING;

    // If the shutdown is delayed, proceed with it.
    if (_state == MqMachineState::MMS_CLOSING) {
        tDLogIfN(PARAMS.verbose, "MqUdp::afterProcessMessage() Delayed shutdown progresses.");
        shutdownAndClose();
    } else {
        tDLogIfI(PARAMS.verbose, "MqUdp::afterProcessMessage() Waiting for messages.");
    }
}

void MqUdp::onWriterAsync(Writer * writer)
{
    assert(writer != nullptr);
    assert(writer == _writer.get());
    assert(writer->state == MqRequestState::MRS_ASYNC);
    writer->state = MqRequestState::MRS_REQUESTING;

    if (writer->queue.empty()) {
        assert(_state == MqMachineState::MMS_CLOSING);
        writer->state = MqRequestState::MRS_WAITING;
        tDLogIfW(PARAMS.verbose, "MqUdp::onWriterAsync() All messages are canceled.");
        return;
    }

    auto msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    // Give users the opportunity to use the original data.
    if (INTERNAL.default_write != nullptr) {
        assert(INTERNAL.parent != nullptr);
        if (INTERNAL.default_write(nullptr, msg_pointer->event, msg_pointer->data(), msg_pointer->size(), INTERNAL.parent) >= 1) {
            tDLogIfD(PARAMS.verbose,
                     "MqUdp::onWriterAsync() Default write process... "
                     "Next, onWrite() event method.");
        } else {
            tDLogIfW(PARAMS.verbose,
                     "MqUdp::onWriterAsync() Default write error, "
                     "skip this message.");
            afterProcessMessage(msg_pointer.get());
        }
        return;
    }

    auto const CODE = _packer.build(*(msg_pointer.get()));
    assert(isSuccess(CODE));

    assert(INTERNAL.write_cb != nullptr);
    assert(INTERNAL.parent != nullptr);

    // Give the user a chance to filter the message.
    if (INTERNAL.write_cb(*(msg_pointer.get()), INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
        tDLogIfD(PARAMS.verbose, "MqUdp::onWriterAsync() The message to be sent has been filtered.");
        afterProcessMessage(msg_pointer.get());
        return;
    }

    sockaddr addr;
    // TODO: msg_pointer->stream -> addr
    auto const WRITE_CODE = _node->send(_send_req, (char const *)_packer.point(), _packer.size(), &addr);
    if (isSuccess(WRITE_CODE)) {
        tDLogIfD(PARAMS.verbose,
                 "MqUdp::onWriterAsync() Write process {}byte ... "
                 "Next, onWrite() event method.",
                 _packer.size());
    } else {
        tDLogW("MqUdp::onWriterAsync() Write error({}), "
               "skip this message.", WRITE_CODE);
        afterProcessMessage(msg_pointer.get());
    }
}

void MqUdp::onWriterClose(Writer * writer)
{
    using namespace libtbag::mq::details;
    assert(writer != nullptr);
    assert(isClosingState(_state));
    assert(!_node->isClosing());
    _node->close();
}

void MqUdp::onCloseTimerTimer(CloseTimer * timer)
{
    assert(timer != nullptr);

    using namespace libtbag::mq::details;
    assert(isClosingState(_state));

    timer->close();
    closeAsyncMessages();
}

void MqUdp::onCloseTimerClose(CloseTimer * timer)
{
    assert(timer != nullptr);
    tDLogIfD(PARAMS.verbose, "MqUdp::onCloseTimerClose() Close timer.");
}

void MqUdp::onSend(UdpSendRequest & request, Err code)
{
    assert(static_cast<bool>(_writer));
    assert(!_writer->queue.empty());
    assert(_writer->state == MqRequestState::MRS_REQUESTING);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqUdp::onSend() Write success.");
    } else {
        tDLogE("MqUdp::onSend() Write error: {}", code);
    }

    afterProcessMessage(_writer->queue.front().get());
}

binf MqUdp::onAlloc(std::size_t suggested_size)
{
    return libtbag::uvpp::defaultOnAlloc(_read_buffer, suggested_size);
}

void MqUdp::onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
{
    assert(_node != nullptr);

    if (code == Err::E_EOF) {
        tDLogI("MqUdp::onRecv() End of file.");
        using namespace libtbag::mq::details;
        if (isActiveState(_state)) {
            tearDown(false);
        }
        return;
    }

    if (isFailure(code)) {
        ++_read_error_count;
        if (_read_error_count >= PARAMS.continuous_read_error_count) {
            tDLogE("MqUdp::onRecv() Read error({}) and close.", code);
            using namespace libtbag::mq::details;
            if (isActiveState(_state)) {
                tearDown(false);
            }
        } else {
            tDLogE("MqUdp::onRecv() Read error: {} ({}/{})",
                   code, _read_error_count, PARAMS.continuous_read_error_count);
        }
        return;
    }

    assert(isSuccess(code));
    tDLogIfD(PARAMS.verbose, "MqUdp::onRecv() Read success. {}byte", size);

    // Give users the opportunity to use the original data.
    if (INTERNAL.default_read != nullptr) {
        assert(INTERNAL.parent != nullptr);
        INTERNAL.default_read(nullptr, buffer, size, INTERNAL.parent);
        return;
    }

    _read_error_count = 0;
    _remaining_read.insert(_remaining_read.end(), buffer, buffer + size);

    std::size_t const REMAINING_SIZE = _remaining_read.size();
    std::size_t computed_total = 0;
    std::size_t computed_size  = 0;

    Err parse_code;
    Err enqueue_code;

    while (true) {
        parse_code = _packer.parseAndUpdate(_remaining_read.data() + computed_total,
                                            REMAINING_SIZE - computed_total,
                                            &computed_size);
        if (isSuccess(parse_code)) {
            assert(0 < COMPARE_AND(computed_size) <= _remaining_read.size());
            computed_total += computed_size;
            tDLogIfD(PARAMS.verbose, "MqUdp::onRecv() Remaining size: {}",
                     REMAINING_SIZE - computed_total);
        } else {
            assert(parse_code == Err::E_VERIFIER);
            if (computed_size) {
                tDLogIfD(PARAMS.verbose, "MqUdp::onRecv() Verify error "
                                         "(remaining size: {}, Required size: {})",
                         REMAINING_SIZE - computed_total, computed_size);
            } else {
                tDLogIfD(PARAMS.verbose, "MqUdp::onRecv() Verify error "
                                         "(remaining size: {})", REMAINING_SIZE - computed_total);
            }
            break;
        }

        assert(INTERNAL.recv_cb != nullptr);
        assert(INTERNAL.parent != nullptr);

        if (INTERNAL.recv_cb(_packer.msg(), INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
            tDLogIfD(PARAMS.verbose, "MqUdp::onRecv() Consumed this received message.");
        } else {
            COMMENT("Single-Producer recv-queue") {
                while (!_wait_lock.tryLock()) {
                    // Busy waiting...
                }
                enqueue_code = _receives.enqueue(_packer.msg());
                _wait_cond.signal();
                _wait_lock.unlock();
            }

            if (isSuccess(enqueue_code)) {
                tDLogIfD(PARAMS.verbose,
                         "MqUdp::onRecv() Enqueue success. "
                         "Perhaps the remaining queue size is {}.",
                         _receives.getInaccurateSizeOfActive());
            } else {
                tDLogE("MqUdp::onRecv() Enqueue error: {}", enqueue_code);
                break;
            }
        }
    }

    // [DONE] All processes succeeded.
    if (computed_total >= 1) {
        _remaining_read.erase(_remaining_read.begin(),
                              _remaining_read.begin() + computed_total);
    }

    tDLogIfD(PARAMS.verbose, "MqUdp::onRecv() Remaining size: {}",
             _remaining_read.size());
}

void MqUdp::onClose()
{
    tDLogI("MqUdp::onClose() Close this client!");
    _state = MqMachineState::MMS_CLOSED;
    MqBase::disableWait();
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

