/**
 * @file   MqStreamClient.cpp
 * @brief  MqStreamClient class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqStreamClient.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
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

MqStreamClient::MqStreamClient(Loop & loop, MqParams const & params)
        : MqBase(loop, params, MqMachineState::MMS_NONE),
          _client(), _packer(params.packer_size), _read_error_count(0)
{
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

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    if (PARAMS.type == MqType::MT_PIPE) {
        if (!libtbag::filesystem::Path(params.address).exists()) {
            tDLogE("MqStreamClient::MqStreamClient() Not found named pipe: {}", params.address);
            throw ErrException(Err::E_EEXIST);
        }

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
            throw ErrException(INIT_CODE);
        }

        auto const BIND_CODE = tcp->connect(tcp->connect_req, addr.getCommon());
        if (isFailure(BIND_CODE)) {
            tDLogE("MqStreamClient::MqStreamClient() Socket connect error: {}", BIND_CODE);
            throw ErrException(BIND_CODE);
        }
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
    auto * client = (ClientT*)(stream);
    return client->shutdown(client->shutdown_req);
}

Err MqStreamClient::shutdown()
{
    assert(THREAD_ID == std::this_thread::get_id());
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
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_client));
    if (!_client->isClosing()) {
        _client->close();
    }
}

void MqStreamClient::shutdownAndClose()
{
    assert(THREAD_ID == std::this_thread::get_id());
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
    assert(THREAD_ID == std::this_thread::get_id());

    using namespace libtbag::mq::details;
    if (isInactiveState(_state)) {
        assert(isCloseState(_state));
        tDLogIfW(PARAMS.verbose, "MqStreamClient::tearDown() It is already closing.");
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
    assert(THREAD_ID == std::this_thread::get_id());
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
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (!isActiveState(_state)) {
        tDLogIfW(PARAMS.verbose,
                 "MqStreamClient::onMsg() Illegal client state, "
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

AfterAction MqStreamClient::onMsgEvent(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    _writer->queue.push(AsyncMsgPointer(msg));

    if (_writer->state != MqRequestState::MRS_WAITING) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onMsg() Writer is already working");
        return AfterAction::AA_DELAY;
    }

    auto const CODE = _writer->send();
    assert(isSuccess(CODE));

    _writer->state = MqRequestState::MRS_ASYNC;
    return AfterAction::AA_DELAY;
}

void MqStreamClient::onCloseEvent()
{
    assert(THREAD_ID == std::this_thread::get_id());
    tDLogI("MqStreamClient::onCloseEvent() Close message confirmed.");
    tearDown(true);
}

void MqStreamClient::afterProcessMessage(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    _writer->queue.pop();
    auto const CODE = restoreMessage(msg, PARAMS.verify_restore_message);
    assert(isSuccess(CODE));

    if (!_writer->queue.empty()) {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
        tDLogIfD(PARAMS.verbose, "MqStreamClient::afterProcessMessage() Async next message ...");
        return;
    }

    _writer->state = MqRequestState::MRS_WAITING;

    // If the shutdown is delayed, proceed with it.
    if (_state == MqMachineState::MMS_DELAY_CLOSING) {
        tDLogIfI(PARAMS.verbose, "MqStreamClient::afterProcessMessage() Async next message ...");
        shutdownAndClose();
    } else {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::afterProcessMessage() Waiting for messages ...");
    }
}

template <typename ClientT>
static Err __write_client(Stream * stream, uint8_t const * data, std::size_t size)
{
    assert(stream != nullptr);
    auto * client = (ClientT*)(stream);
    return client->write(client->write_req, (char const *)data, size);
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
    assert(THREAD_ID == std::this_thread::get_id());
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

    auto const CODE = _packer.build(*(msg_pointer.get()));
    assert(isSuccess(CODE));

    auto const WRITE_CODE = __write_client(_client.get(), _packer.point(), _packer.size(), PARAMS.type);
    if (isSuccess(WRITE_CODE)) {
        tDLogIfD(PARAMS.verbose,
                 "MqStreamClient::onWriterAsync() Write process ... "
                 "Next, onWrite() event method.");
    } else {
        tDLogW("MqStreamClient::onWriterAsync() Write error({}), "
               "skip this message.", WRITE_CODE);
        afterProcessMessage(msg_pointer.get());
    }
}

void MqStreamClient::onWriterClose(Writer * writer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(writer != nullptr);

    using namespace libtbag::mq::details;
    assert(isClosingState(_state));
    assert(!_client->isClosing());
    _client->close();
}

void MqStreamClient::onCloseTimer(CloseTimer * timer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(timer != nullptr);

    using namespace libtbag::mq::details;
    assert(isClosingState(_state));

    timer->close();
    closeAsyncMsgs();
}

void MqStreamClient::onCloseTimerClose(CloseTimer * timer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(timer != nullptr);
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onCloseTimerClose() Close timer.");
}

void MqStreamClient::onConnect(ConnectRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_client));
    assert(_state == MqMachineState::MMS_INITIALIZED);

    if (isFailure(code)) {
        tDLogE("MqStreamClient::onConnect() Connect error: {}", code);
        close();
        return;
    }

    auto const START_CODE = _client->startRead();
    if (isFailure(START_CODE)) {
        tDLogE("MqStreamClient::onConnect() Start read error: {}", code);
        close();
        return;
    }

    tDLogI("MqStreamClient::onConnect() Connection & Read start.");
    _state = MqMachineState::MMS_ACTIVE;
}

void MqStreamClient::onShutdown(ShutdownRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onNodeShutdown() Shutdown success.");
    } else {
        tDLogE("MqStreamClient::onNodeShutdown() Shutdown error: {}", code);
    }
}

void MqStreamClient::onWrite(WriteRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
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
    assert(THREAD_ID == std::this_thread::get_id());
    return libtbag::uvpp::defaultOnAlloc(_read_buffer, suggested_size);
}

void MqStreamClient::onRead(Err code, char const * buffer, std::size_t size)
{
    assert(THREAD_ID == std::this_thread::get_id());
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

        if (PARAMS.recv_cb != nullptr) {
            PARAMS.recv_cb(_packer.msg(), this);
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
                tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Enqueue success.");
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
    assert(THREAD_ID == std::this_thread::get_id());

    using namespace libtbag::mq::details;
    assert(isClosingState(_state));

    tDLogI("MqStreamClient::onClose() Close this client!");

    // Last closing point.
    _state = MqMachineState::MMS_CLOSED;
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

