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
        : MqEventQueue(loop, params.send_queue_size, params.send_msg_size),
          PARAMS(params), _client(), _packer(params.packer_size),
          _recv_queue(params.recv_queue_size, params.recv_msg_size),
          _read_error_count(0),
          _state(MqMachineState::MMS_OPENING), _now_sending(0),
          _closed_async_messages_count(0)
{
    if (PARAMS.type == MqType::MT_PIPE) {
        _client = loop.newHandle<PipeClient>(loop, this);
    } else if (PARAMS.type == MqType::MT_TCP) {
        _client = loop.newHandle<TcpClient>(loop, this);
    } else {
        tDLogE("MqStreamClient::MqStreamClient() Unknown type: {}", (int)(PARAMS.type));
        throw ErrException(Err::E_ILLARGS);
    }
    assert(static_cast<bool>(_client));
    assert(_client->isInit());

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    if (PARAMS.type == MqType::MT_PIPE) {
        if (!libtbag::filesystem::Path(params.address).exists()) {
            tDLogE("MqStreamClient::MqStreamClient() Exists pipe path: {}", params.address);
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
    assert(THREAD_ID == std::this_thread::get_id());
}

template <typename ClientT>
static Err __shutdown(Stream * stream)
{
    auto * client = (ClientT*)(stream);
    return client->shutdown(client->shutdown_req);
}

Err MqStreamClient::shutdown()
{
    Err code;
    if (PARAMS.type == MqType::MT_PIPE) {
        code = __shutdown<PipeClient>(_client.get());
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        code = __shutdown<TcpClient>(_client.get());
    }
    if (isSuccess(code)) {
        _state = MqMachineState::MMS_SHUTTING;
    }
    return code;
}

void MqStreamClient::close()
{
    if (_client->isClosing()) {
        // EOF can be called via onRead() after the Client has already closed.
        assert(_state == MqMachineState::MMS_CLOSING || _state == MqMachineState::MMS_CLOSED);
        return;
    }

    if (_state == MqMachineState::MMS_CONNECTED) {
        _state = MqMachineState::MMS_CLOSING; // This prevents the send() method from receiving further input.
        std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.shutdown_wait_nanosec));
        while (_now_sending > 0) {
            // Busy waiting...
        }

        // [IMPORTANT]
        // From this moment on, there is no send-queue producer.
        assert(_now_sending == 0);
        assert(static_cast<bool>(_writer));

        // Cancel all pending messages in send-queue and returns resources.
        while (!_writer->queue.empty()) {
            auto msg = _writer->queue.front();
            _writer->queue.pop();
            restoreMessage(msg.get(), PARAMS.verify_restore_message);
        }
    } else {
        _state = MqMachineState::MMS_CLOSING;
    }

    _client->close();
}

void MqStreamClient::onAsyncMsg(AsyncMsg * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);

    MqEventQueue::onAsyncMsg(async);
}

void MqStreamClient::onCloseMsg(AsyncMsg * async)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(async != nullptr);
    assert(_state == MqMachineState::MMS_CLOSING);

    MqEventQueue::onCloseMsg(async);

    ++_closed_async_messages_count;
    assert(_closed_async_messages_count <= QUEUE_SIZE);
    if (_closed_async_messages_count == QUEUE_SIZE) {
        assert(!_writer->isClosing());
        _writer->close();
        tDLogIfD(PARAMS.verbose,
                 "MqStreamClient::onCloseMsg() Close request of writer, Next, onWriterClose() event method.");
    }
}

AfterAction MqStreamClient::onMsg(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    if (_state != MqMachineState::MMS_CONNECTED) {
        using namespace libtbag::mq::details;
        tDLogIfW(PARAMS.verbose,
                 "MqStreamClient::onMsg() Illegal client state, skip current message ({})",
                 getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    assert(_state == MqMachineState::MMS_CONNECTED);
    switch (msg->event) {
    case MqEvent::ME_MSG:
        return onMsgEvent(msg);

    case MqEvent::ME_CLOSE:
        onCloseEvent(msg);
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
    assert(_state == MqMachineState::MMS_CONNECTED);

    _writer->queue.push(AsyncMsgPointer(msg));

    if (_writer->state != MqRequestState::MRS_WAITING) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onMsg() Writer is already working");
        return AfterAction::AA_DELAY;
    }

    auto const CODE = _writer->send();
    if (isFailure(CODE)) {
        tDLogE("MqStreamClient::onMsg() Async send error ({})", CODE);
        return AfterAction::AA_OK;
    }

    _writer->state = MqRequestState::MRS_ASYNC;
    return AfterAction::AA_DELAY;
}

void MqStreamClient::onCloseEvent(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    if (_state != MqMachineState::MMS_CONNECTED) {
        tDLogIfW(PARAMS.verbose, "MqStreamClient::onCloseEvent() Illegal state, This close message is skipped.");
        return;
    }

    assert(_state == MqMachineState::MMS_CONNECTED);
    _state = MqMachineState::MMS_ON_CLOSE_MSG; // This prevents the send() method from receiving further input.
    std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.shutdown_wait_nanosec));
    while (_now_sending > 0) {
        // Busy waiting...
    }

    // [IMPORTANT]
    // From this moment on, there is no send-queue producer.
    assert(_now_sending == 0);
    assert(static_cast<bool>(_writer));
    auto const ACTIVE_SEND_SIZE = getInaccurateSizeOfActive() - 1/*Subtract the current message*/;

    if (ACTIVE_SEND_SIZE > 0 || _writer->state != MqRequestState::MRS_WAITING) {
        // Clear any remaining transmission queues and continue with system shutdown.
        tDLogIfI(PARAMS.verbose, "MqStreamClient::onCloseEvent() Delay the shutdown request ...");
        _state = MqMachineState::MMS_DELAY_SHUTTING;
        return;
    }

    assert(_writer->queue.empty());
    auto const CODE = shutdown();
    if (isSuccess(CODE)) {
        tDLogIfD(PARAMS.verbose,
                 "MqStreamClient::onCloseEvent() Shutdown request ... Next, onShutdown() event method.");
    } else {
        tDLogE("MqStreamClient::onCloseEvent() Shutdown error: {}", CODE);
        close();
    }
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
        return;
    }

    _writer->state = MqRequestState::MRS_WAITING;

    // If the shutdown is delayed, proceed with it.
    if (_state == MqMachineState::MMS_DELAY_SHUTTING) {
        auto const CODE = shutdown();
        if (isSuccess(CODE)) {
            tDLogIfD(PARAMS.verbose,
                     "MqStreamClient::afterProcessMessage() Shutdown request ... Next, onShutdown() event method.");
        } else {
            tDLogE("MqStreamClient::afterProcessMessage() Shutdown error: {}", CODE);
            close();
            return;
        }
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
    assert(writer->queue.empty());
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
                 "MqStreamClient::onWriterAsync() Write process ... Next, onWrite() event method.");
    } else {
        tDLogW("MqStreamClient::onWriterAsync() Write error({}), skip this message.", WRITE_CODE);
        afterProcessMessage(msg_pointer.get());
    }
}

void MqStreamClient::onWriterClose(Writer * writer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(writer != nullptr);
    assert(_state == MqMachineState::MMS_CLOSING);

    // Last closing point.
    _state = MqMachineState::MMS_CLOSED;
}

void MqStreamClient::onCloseTimer(CloseTimer * timer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(timer != nullptr);
    assert(static_cast<bool>(_client));

    close();
    timer->close();
}

void MqStreamClient::onCloseTimerClose(CloseTimer * timer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(timer != nullptr);
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
    _state = MqMachineState::MMS_CONNECTED;
}

void MqStreamClient::onShutdown(ShutdownRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_client));
    assert(_state == MqMachineState::MMS_SHUTTING || _state == MqMachineState::MMS_DELAY_SHUTTING);

    if (isFailure(code)) {
        tDLogE("MqStreamClient::onShutdown() Shutdown error: {}", code);
        close();
        return;
    }

    if (PARAMS.wait_closing_millisec == 0) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onShutdown() Direct closing");
        close();
        return;
    }

    auto * loop = _client->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this);
    if (!timer) {
        tDLogE("MqStreamClient::onShutdown() Create timer error");
        close();
        return;
    }

    assert(PARAMS.wait_closing_millisec > 0);
    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    if (isFailure(START_CODE)) {
        tDLogE("MqStreamClient::onShutdown() Close timer start error: {}", START_CODE);
        close();
        timer->close();
        return;
    }

    tDLogI("MqStreamClient::onNodeShutdown() Shutdown event done");
    assert(_state == MqMachineState::MMS_SHUTTING || _state == MqMachineState::MMS_DELAY_SHUTTING);
    assert(_state != MqMachineState::MMS_SHUTDOWN);
    _state = MqMachineState::MMS_SHUTDOWN;
}

void MqStreamClient::onWrite(WriteRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_writer));
    assert(!_writer->queue.empty());
    assert(_writer->state == MqRequestState::MRS_REQUESTING);
    assert(_state == MqMachineState::MMS_CONNECTED || _state == MqMachineState::MMS_DELAY_SHUTTING);

    auto value = _writer->queue.front();
    afterProcessMessage(value.get());
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
        close();
        return;
    }

    if (isFailure(code)) {
        ++_read_error_count;
        tDLogE("MqStreamClient::onNodeRead() Read error: {} ({}/{})",
               code, _read_error_count, PARAMS.continuous_read_error_count);
        if (_read_error_count >= PARAMS.continuous_read_error_count) {
            close();
        }
        return;
    }

    assert(code == Err::E_SUCCESS);
    _remaining_read.insert(_remaining_read.end(), buffer, buffer + size);
    _read_error_count = 0;

    std::size_t computed_size = 0;
    auto const PARSE_CODE = _packer.parseAndUpdate(_remaining_read, &computed_size);
    if (isFailure(PARSE_CODE)) {
        // Skip... (maybe no error)
        return;
    }

    assert(computed_size <= _remaining_read.size());
    _remaining_read.erase(_remaining_read.begin(), _remaining_read.begin() + computed_size);

    auto const ENQUEUE_CODE = _recv_queue.enqueue(_packer.msg());
    if (ENQUEUE_CODE == Err::E_NREADY) {
        tDLogE("MqStreamClient::onNodeRead() The Recv-queue is not ready.");
        return;
    }

    // [DONE] All processes succeeded.
    assert(isSuccess(ENQUEUE_CODE));
}

void MqStreamClient::onClose()
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(_state == MqMachineState::MMS_CLOSING);

    if (PARAMS.verbose) {
        tDLogI("MqStreamClient::onClose() Close this client!");
    } else {
        tDLogI("MqStreamClient::onClose() Close this client! Next, close all async messages in send-queue.");
    }

    closeAsyncMsgs();
}

Err MqStreamClient::send(MqMsg const & msg)
{
    if (_state != MqMachineState::MMS_CONNECTED) {
        tDLogE("MqStreamClient::send() Illegal client state");
        return Err::E_ILLSTATE;
    }

    // [WARNING]
    // This point is a dangerous point where the number of times to send(<code>_now_sending</code>) out can be missed.
    // To avoid this, use sleep() on that thread.
    //
    // Note:
    // the moment when the <code>_state</code> information becomes <code>MqMachineState::MMS_SHUTTING</code>.

    ++_now_sending;
    auto const CODE = enqueue(msg);
    --_now_sending;
    return CODE;
}

Err MqStreamClient::recv(MqMsg & msg)
{
    return _recv_queue.dequeue(msg);
}

void MqStreamClient::recvWait(MqMsg & msg)
{
    _recv_queue.dequeueWait(msg);
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

