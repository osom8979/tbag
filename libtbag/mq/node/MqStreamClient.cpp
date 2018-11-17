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
          _state(MqMachineState::MMS_OPENING), _now_sending(0)
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

AfterAction MqStreamClient::onMsg(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(msg != nullptr);

    if (_state != MqMachineState::MMS_CONNECTED) {
        using namespace libtbag::mq::details;
        tDLogW("MqStreamClient::onMsg() Illegal client state, skip current message ({})", getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    if (msg->event == MqEvent::ME_MSG) {
        _writer->queue.push(AsyncMsgPointer(msg));
        if (_writer->state == MqRequestState::MRS_WAITING) {
            auto const CODE = _writer->send();
            if (isSuccess(CODE)) {
                _writer->state = MqRequestState::MRS_ASYNC;
            } else {
                tDLogE("MqStreamClient::onMsg() Async send error ({})", CODE);
                return AfterAction::AA_OK;
            }
        }
        return AfterAction::AA_DELAY;
    }

    if (msg->event == MqEvent::ME_CLOSE) {
        onCloseEvent();
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return AfterAction::AA_OK;
}

template <typename ClientT>
static Err __shutdown(Stream * stream)
{
    auto * client = (ClientT*)(stream);
    return client->shutdown(client->shutdown_req);
}

static Err __shutdown(Stream * stream, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __shutdown<PipeClient>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __shutdown<TcpClient>(stream);
}

void MqStreamClient::onCloseEvent()
{
    assert(THREAD_ID == std::this_thread::get_id());

    _state = MqMachineState::MMS_SHUTTING;
    std::this_thread::sleep_for(std::chrono::nanoseconds(PARAMS.shutdown_wait_nanosec));
    while (_now_sending > 0) {
        // Busy waiting...
    }

    // [IMPORTANT]
    // From this moment on,
    // there is no send-queue producers.
    assert(_now_sending == 0);
    assert(static_cast<bool>(_writer));
    auto const ACTIVE_SEND_SIZE = getInaccurateSizeOfActive() - 1/*Subtract the current message*/;
    if (ACTIVE_SEND_SIZE == 0 && _writer->state == MqRequestState::MRS_WAITING) {
        assert(_writer->queue.size() == 0);

        auto const CODE = __shutdown(_client.get(), PARAMS.type);
        if (isSuccess(CODE)) {
            tDLogI("MqStreamClient::onCloseEvent() Shutdown request ...");
        } else {
            tDLogE("MqStreamClient::onCloseEvent() Shutdown error: {}", CODE);
            _client->close();
            return;
        }
    } else {
        // Clear all remaining send-queue and proceed with shutdown.
        tDLogI("MqStreamClient::onCloseEvent() Delay the shutdown request ...");
        _state = MqMachineState::MMS_DELAY_SHUTTING;
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
    assert(writer->queue.empty());
    assert(writer->state == MqRequestState::MRS_ASYNC);
    writer->state = MqRequestState::MRS_REQUESTING;

    auto msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    auto const CODE = _packer.build(*(msg_pointer.get()));
    assert(isSuccess(CODE));

    auto const WRITE_CODE = __write_client(_client.get(), _packer.point(), _packer.size(), PARAMS.type);
    if (isFailure(WRITE_CODE)) {
        tDLogE("MqStreamClient::onWriterAsync() Write error: {}", WRITE_CODE);

        writer->queue.pop();

        auto const CODE = restoreMessage(msg_pointer.get(), PARAMS.verify_restore_message);
        assert(isSuccess(CODE));

        if (writer->queue.empty()) {
            writer->state = MqRequestState::MRS_WAITING;
            if (_state == MqMachineState::MMS_DELAY_SHUTTING) {
                auto const CODE = __shutdown(_client.get(), PARAMS.type);
                if (isSuccess(CODE)) {
                    tDLogI("MqStreamClient::onWriterAsync() Shutdown request ...");
                } else {
                    tDLogE("MqStreamClient::onWriterAsync() Shutdown error: {}", CODE);
                    _client->close();
                    return;
                }
            }
        } else {
            auto const CODE = writer->send();
            assert(isSuccess(CODE));
            writer->state = MqRequestState::MRS_ASYNC;
        }
    }
}

void MqStreamClient::onWriterClose(Writer * writer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(writer != nullptr);
}

void MqStreamClient::onCloseTimer(CloseTimer * timer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(timer != nullptr);
    assert(static_cast<bool>(_client));

    if (!_client->isClosing()) {
        _client->close();
    }
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

    if (_state != MqMachineState::MMS_INITIALIZED) {
        tDLogE("MqStreamClient::onConnect() Illegal state error: not initialized.");
        _client->close();
        return;
    }

    if (isFailure(code)) {
        tDLogE("MqStreamClient::onConnect() Connect error: {}", code);
        _client->close();
        return;
    }

    Err const START_CODE = _client->startRead();
    if (isFailure(START_CODE)) {
        tDLogE("MqStreamClient::onConnect() Start read error: {}", code);
        _client->close();
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
        _client->close();
        return;
    }

    if (PARAMS.wait_closing_millisec == 0) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onShutdown() Direct closing");
        _client->close();
        return;
    }

    auto * loop = _client->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this);
    if (!timer) {
        tDLogE("MqStreamClient::onShutdown() Create timer error");
        _client->close();
        return;
    }

    assert(PARAMS.wait_closing_millisec > 0);
    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    if (isFailure(START_CODE)) {
        tDLogE("MqStreamClient::onShutdown() Close timer start error: {}", START_CODE);
        _client->close();
        timer->close();
        return;
    }

    tDLogI("MqStreamClient::onNodeShutdown() Shutdown event done");
    _state = MqMachineState::MMS_SHUTDOWN;
}

void MqStreamClient::onWrite(WriteRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_writer));
    assert(!_writer->queue.empty());
    assert(_writer->state == MqRequestState::MRS_REQUESTING);

    auto value = _writer->queue.front();
    _writer->queue.pop();

    auto const CODE = restoreMessage(value.get(), PARAMS.verify_restore_message);
    assert(isSuccess(CODE));

    if (_writer->queue.empty()) {
        _writer->state = MqRequestState::MRS_WAITING;
        if (_state == MqMachineState::MMS_DELAY_SHUTTING) {
            auto const CODE = __shutdown(_client.get(), PARAMS.type);
            if (isSuccess(CODE)) {
                tDLogI("MqStreamClient::onWrite() Shutdown request ...");
            } else {
                tDLogE("MqStreamClient::onWrite() Shutdown error: {}", CODE);
                _client->close();
                return;
            }
        }
    } else {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
    }
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
        _client->close();
        return;
    }

    if (isFailure(code)) {
        ++_read_error_count;
        tDLogE("MqStreamClient::onNodeRead() Read error: {} ({}/{})",
               code, _read_error_count, PARAMS.continuous_read_error_count);
        if (_read_error_count >= PARAMS.continuous_read_error_count) {
            _client->close();
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

    tDLogI("MqStreamClient::onServerClose() Close this client!");
    _state = MqMachineState::MMS_CLOSED;

    if (!_writer->isClosing()) {
        _writer->close();
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

