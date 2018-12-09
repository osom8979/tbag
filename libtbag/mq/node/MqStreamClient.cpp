/**
 * @file   MqStreamClient.cpp
 * @brief  MqStreamClient class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf        = MqStreamClient::binf;
using Stream      = MqStreamClient::Stream;
using MqType      = MqStreamClient::MqType;
using PipeClient  = MqStreamClient::PipeClient;
using TcpClient   = MqStreamClient::TcpClient;
using Buffer      = MqStreamClient::Buffer;
using AfterAction = MqStreamClient::AfterAction;

using ConnectRequest  = MqStreamClient::ConnectRequest;
using ShutdownRequest = MqStreamClient::ShutdownRequest;
using WriteRequest    = MqStreamClient::WriteRequest;

MqStreamClient::MqStreamClient(Loop & loop, MqInternal const & internal, MqParams const & params)
        : MqBase(loop, internal, params, MqMachineState::MMS_NONE),
          _packer(params.packer_size), _read_error_count(0)
{
    if (PARAMS.type == MqType::MT_PIPE && !libtbag::filesystem::Path(params.address).exists()) {
        tDLogE("MqStreamClient::MqStreamClient() Not found named pipe: {}", params.address);
        throw ErrException(Err::E_EEXIST);
    }

    if (PARAMS.type == MqType::MT_PIPE) {
        _client = loop.newHandle<PipeClient>(loop, this);
    } else if (PARAMS.type == MqType::MT_TCP) {
        _client = loop.newHandle<TcpClient>(loop, this);
    } else {
        tDLogE("MqStreamClient::MqStreamClient() "
               "Unsupported type: {}", (int)(PARAMS.type));
        throw ErrException(Err::E_ILLARGS);
    }
    assert(static_cast<bool>(_client));
    assert(_client->isInit());

    if (PARAMS.type == MqType::MT_PIPE) {
        auto * pipe = (PipeClient*)(_client.get());
        assert(pipe != nullptr);
        pipe->connect(pipe->connect_req, params.address.c_str());
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        auto * tcp = (TcpClient*)(_client.get());

        SocketAddress addr;
        auto const INIT_CODE = addr.init(params.address, params.port);
        if (isFailure(INIT_CODE)) {
            tDLogE("MqStreamClient::MqStreamClient() Socket init error: {}", INIT_CODE);
            _client->close();
            throw ErrException(INIT_CODE);
        }

        auto const BIND_CODE = tcp->connect(tcp->connect_req, addr.getCommon());
        if (isFailure(BIND_CODE)) {
            tDLogE("MqStreamClient::MqStreamClient() Socket connect error: {}", BIND_CODE);
            _client->close();
            throw ErrException(BIND_CODE);
        }
    }

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    if (params.connect_timeout_millisec >= 1) {
        _connect_timer = loop.newHandle<ConnectTimer>(loop, this);
        assert(static_cast<bool>(_connect_timer));
        assert(_connect_timer->isInit());
        _connect_timer->start(params.connect_timeout_millisec);
    }

    _state = MqMachineState::MMS_INITIALIZED;
}

MqStreamClient::~MqStreamClient()
{
    // EMPTY.
}

template <typename ClientT>
static Err __shutdown(Stream * stream)
{
    auto * node = ((ClientT*)(stream));
    return node->shutdown(node->shutdown_req);
}

Err MqStreamClient::shutdown()
{
    assert(static_cast<bool>(_client));
    if (PARAMS.type == MqType::MT_PIPE) {
        return __shutdown<PipeClient>(_client.get());
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        return __shutdown<TcpClient>(_client.get());
    }
}

void MqStreamClient::close()
{
    assert(static_cast<bool>(_client));
    if (!_client->isClosing()) {
        _client->close();
    }
}

void MqStreamClient::shutdownAndClose()
{
    assert(_writer->queue.empty());
    auto const SHUTDOWN_CODE = shutdown();

    if (isSuccess(SHUTDOWN_CODE)) {
        tDLogIfD(PARAMS.verbose,
                 "MqStreamClient::shutdownAndClose() "
                 "Shutdown success.");
    } else {
        tDLogW("MqStreamClient::shutdownAndClose() "
               "Shutdown error: {}", SHUTDOWN_CODE);
    }

    auto * loop = _client->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this);
    assert(static_cast<bool>(timer));

    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    assert(isSuccess(START_CODE));
}

void MqStreamClient::tearDown(bool on_message)
{
    using namespace libtbag::mq::details;
    if (isInactiveState(_state)) {
        assert(isCloseState(_state));
        tDLogIfD(PARAMS.verbose, "MqStreamClient::tearDown() It is already closing.");
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
    assert(static_cast<bool>(_writer));

    std::size_t active_send_size = getInaccurateSizeOfActive();
    if (on_message) {
        assert(active_send_size >= 1);
        // Subtract the current message.
        --active_send_size;
    }

    if (active_send_size > 0 || _writer->state != MqRequestState::MRS_WAITING) {
        // Clear any remaining transmission queues and continue with system shutdown.
        tDLogI("MqStreamClient::tearDown() Delay the shutdown request ...");
        _state = MqMachineState::MMS_DELAY_CLOSING;
        return;
    }

    assert(_writer->queue.empty());
    shutdownAndClose();
}

void MqStreamClient::onCloseMsgDone()
{
    assert(!_writer->isClosing());
    _writer->close();

    if (PARAMS.verbose) {
        tDLogD("MqStreamClient::onCloseMsgDone() Close request of writer. "
               "Next, onWriterClose() event method.");
    } else {
        tDLogD("MqStreamClient::onCloseMsgDone() Close request of writer.");
    }
}

AfterAction MqStreamClient::onMsg(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (!isActiveState(_state) && !isClosingState(_state)) {
        tDLogIfW(PARAMS.verbose,
                 "MqStreamClient::onMsg() Illegal client state({}), skip current message ({})",
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

AfterAction MqStreamClient::onMsgEvent(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    _writer->queue.push(AsyncMsgPointer(msg));

    if (_writer->state != MqRequestState::MRS_WAITING) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onMsgEvent() Writer is already working");
        return AfterAction::AA_DELAY;
    }

    auto const CODE = _writer->send();
    assert(isSuccess(CODE));

    _writer->state = MqRequestState::MRS_ASYNC;
    return AfterAction::AA_DELAY;
}

void MqStreamClient::onCloseEvent()
{
    tDLogI("MqStreamClient::onCloseEvent() Close message confirmed.");
    tearDown(true);
}

void MqStreamClient::afterProcessMessage(AsyncMsg * msg)
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
                 "MqStreamClient::afterProcessMessage() Async next message (queue:{}) ...",
                 _writer->queue.size());
        return;
    }

    assert(_writer->queue.empty());
    _writer->state = MqRequestState::MRS_WAITING;

    // If the shutdown is delayed, proceed with it.
    if (_state == MqMachineState::MMS_DELAY_CLOSING) {
        tDLogIfN(PARAMS.verbose, "MqStreamClient::afterProcessMessage() Delayed shutdown progresses.");
        shutdownAndClose();
    } else {
        tDLogIfI(PARAMS.verbose, "MqStreamClient::afterProcessMessage() Waiting for messages.");
    }
}

template <typename ClientT>
static Err __write_client(Stream * stream, uint8_t const * data, std::size_t size)
{
    assert(stream != nullptr);
    auto * node = ((ClientT*)(stream));
    return node->write(node->write_req, (char const *)data, size);
}

static Err __write_client(Stream * stream, uint8_t const * data, std::size_t size, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __write_client<PipeClient>(stream, data, size);
    }
    assert(type == MqType::MT_TCP);
    return __write_client<TcpClient>(stream, data, size);
}

void MqStreamClient::onWriterAsync(Writer * writer)
{
    assert(writer != nullptr);
    assert(writer == _writer.get());
    assert(writer->state == MqRequestState::MRS_ASYNC);
    writer->state = MqRequestState::MRS_REQUESTING;

    if (writer->queue.empty()) {
        assert(_state == MqMachineState::MMS_CLOSING);
        writer->state = MqRequestState::MRS_WAITING;
        tDLogIfW(PARAMS.verbose, "MqStreamClient::onWriterAsync() All messages are canceled.");
        return;
    }

    auto msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    // Give users the opportunity to use the original data.
    if (INTERNAL.default_write != nullptr) {
        assert(INTERNAL.parent != nullptr);
        if (INTERNAL.default_write(_client.get(), msg_pointer->data(), msg_pointer->size(), INTERNAL.parent) >= 1) {
            tDLogIfD(PARAMS.verbose,
                     "MqStreamClient::onWriterAsync() Default write process... "
                     "Next, onWrite() event method.");
        } else {
            tDLogIfW(PARAMS.verbose,
                     "MqStreamClient::onWriterAsync() Default write error, "
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
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onWriterAsync() The message to be sent has been filtered.");
        afterProcessMessage(msg_pointer.get());
        return;
    }

    auto const WRITE_CODE = __write_client(_client.get(), _packer.point(), _packer.size(), PARAMS.type);
    if (isSuccess(WRITE_CODE)) {
        tDLogIfD(PARAMS.verbose,
                 "MqStreamClient::onWriterAsync() Write process {}byte ... "
                 "Next, onWrite() event method.",
                 _packer.size());
    } else {
        tDLogW("MqStreamClient::onWriterAsync() Write error({}), "
               "skip this message.", WRITE_CODE);
        afterProcessMessage(msg_pointer.get());
    }
}

void MqStreamClient::onWriterClose(Writer * writer)
{
    using namespace libtbag::mq::details;
    assert(writer != nullptr);
    assert(isClosingState(_state));
    assert(!_client->isClosing());
    _client->close();
}

void MqStreamClient::onCloseTimer(CloseTimer * timer)
{
    assert(timer != nullptr);

    using namespace libtbag::mq::details;
    assert(isClosingState(_state));

    timer->close();
    closeAsyncMsgs();
}

void MqStreamClient::onCloseTimerClose(CloseTimer * timer)
{
    assert(timer != nullptr);
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onCloseTimerClose() Close timer.");
}

void MqStreamClient::onConnectTimer(ConnectTimer * timer)
{
    Err code;
    if (PARAMS.type == MqType::MT_PIPE) {
        auto * pipe = (PipeClient*)(_client.get());
        code = pipe->connect_req.cancel();
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        auto * tcp = (TcpClient*)(_client.get());
        code = tcp->connect_req.cancel();
    }

    if (isSuccess(code)) {
        tDLogN("MqStreamClient::onConnectTimer() Connect cancel success.");
    } else {
        tDLogW("MqStreamClient::onConnectTimer() Connect cancel failure: {}", code);
    }

    _connect_timer->close();
    _state = MqMachineState::MMS_CLOSING;
    close();
}

void MqStreamClient::onConnectTimerClose(ConnectTimer * timer)
{
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onConnectTimerClose() Close timer.");
}

void MqStreamClient::onConnect(ConnectRequest & request, Err code)
{
    assert(static_cast<bool>(_client));
    assert(_state == MqMachineState::MMS_INITIALIZED);

    auto const STOP_CODE = _connect_timer->stop();
    assert(isSuccess(STOP_CODE));
    _connect_timer->close();

    if (isFailure(code)) {
        tDLogE("MqStreamClient::onConnect() Connect error: {}", code);
        _state = MqMachineState::MMS_CLOSING;
        close();
        return;
    }

    auto const START_CODE = _client->startRead();
    if (isFailure(START_CODE)) {
        tDLogE("MqStreamClient::onConnect() Start read error: {}", code);
        _state = MqMachineState::MMS_CLOSING;
        close();
        return;
    }

    tDLogI("MqStreamClient::onConnect() Connection & Read start.");
    _state = MqMachineState::MMS_ACTIVE;
    if (INTERNAL.connect_cb != nullptr) {
        assert(INTERNAL.parent);
        INTERNAL.connect_cb(INTERNAL.parent);
    }
}

void MqStreamClient::onShutdown(ShutdownRequest & request, Err code)
{
    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onNodeShutdown() Shutdown success.");
    } else {
        tDLogE("MqStreamClient::onNodeShutdown() Shutdown error: {}", code);
    }
}

void MqStreamClient::onWrite(WriteRequest & request, Err code)
{
    assert(static_cast<bool>(_writer));
    assert(!_writer->queue.empty());
    assert(_writer->state == MqRequestState::MRS_REQUESTING);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onWrite() Write success.");
    } else {
        tDLogE("MqStreamClient::onWrite() Write error: {}", code);
    }

    afterProcessMessage(_writer->queue.front().get());
}

binf MqStreamClient::onAlloc(std::size_t suggested_size)
{
    return libtbag::uvpp::defaultOnAlloc(_read_buffer, suggested_size);
}

void MqStreamClient::onRead(Err code, char const * buffer, std::size_t size)
{
    assert(_client != nullptr);

    if (code == Err::E_EOF) {
        tDLogI("MqStreamClient::onRead() End of file.");
        using namespace libtbag::mq::details;
        if (isActiveState(_state)) {
            tearDown(false);
        }
        return;
    }

    if (isFailure(code)) {
        ++_read_error_count;
        if (_read_error_count >= PARAMS.continuous_read_error_count) {
            tDLogE("MqStreamClient::onRead() Read error({}) and close.", code);
            using namespace libtbag::mq::details;
            if (isActiveState(_state)) {
                tearDown(false);
            }
        } else {
            tDLogE("MqStreamClient::onRead() Read error: {} ({}/{})",
                   code, _read_error_count, PARAMS.continuous_read_error_count);
        }
        return;
    }

    assert(isSuccess(code));
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Read success. {}byte", size);

    // Give users the opportunity to use the original data.
    if (INTERNAL.default_read != nullptr) {
        assert(INTERNAL.parent != nullptr);
        INTERNAL.default_read(_client.get(), buffer, size, INTERNAL.parent);
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
            tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Remaining size: {}",
                     REMAINING_SIZE - computed_total);
        } else {
            assert(parse_code == Err::E_VERIFIER);
            if (computed_size) {
                tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Verify error "
                         "(remaining size: {}, Required size: {})",
                         REMAINING_SIZE - computed_total, computed_size);
            } else {
                tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Verify error "
                         "(remaining size: {})", REMAINING_SIZE - computed_total);
            }
            break;
        }

        assert(INTERNAL.recv_cb != nullptr);
        assert(INTERNAL.parent != nullptr);

        if (INTERNAL.recv_cb(_packer.msg(), INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
            tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Consumed this received message.");
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
                         "MqStreamClient::onRead() Enqueue success. "
                         "Perhaps the remaining queue size is {}.",
                         _receives.getInaccurateSizeOfActive());
            } else {
                tDLogE("MqStreamClient::onRead() Enqueue error: {}", enqueue_code);
                break;
            }
        }
    }

    // [DONE] All processes succeeded.
    if (computed_total >= 1) {
        _remaining_read.erase(_remaining_read.begin(),
                              _remaining_read.begin() + computed_total);
    }

    tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Remaining size: {}",
             _remaining_read.size());
}

void MqStreamClient::onClose()
{
    tDLogI("MqStreamClient::onClose() Close this client!");
    updateAndBroadcast(MqMachineState::MMS_CLOSED);

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

