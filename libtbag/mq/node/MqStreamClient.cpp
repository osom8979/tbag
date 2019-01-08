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
#include <thread>

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
        : MqBase(internal, params, MqMachineState::MMS_CLOSED),
          _loop(loop), _packer(params.packer_size), _read_error_count(0), _reconnect(0)
{
    assert(!MqEventQueue::exists());

    if (params.type != MqType::MT_PIPE && params.type != MqType::MT_TCP) {
        tDLogE("MqStreamClient::MqStreamClient() Unsupported type: {}({})",
               libtbag::mq::details::getTypeName(params.type), (int)(params.type));
        throw ErrException(Err::E_ILLARGS);
    }

    _terminator = _loop.newHandle<Terminator>(_loop, this);
    assert(static_cast<bool>(_terminator));
    assert(_terminator->isInit());

    _state = MqMachineState::MMS_INITIALIZING;
    onInitStep1_ASYNC();
}

MqStreamClient::~MqStreamClient()
{
    // EMPTY.
}

void MqStreamClient::onInitializerAsync(Initializer * init)
{
    assert(init != nullptr);
    init->close();
    onInitStep2_INIT();
}

void MqStreamClient::onInitializerClose(Initializer * init)
{
    assert(init != nullptr);
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onInitializerClose() Closed initializer.");
}

void MqStreamClient::onTerminatorAsync(Terminator * terminator)
{
}

void MqStreamClient::onTerminatorClose(Terminator * terminator)
{
    assert(terminator != nullptr);
    assert(terminator == _terminator.get());
    _terminator.reset();
    onCloseStep5_TERMINATOR_CLOSED();
}

AfterAction MqStreamClient::onMsg(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (_state != MqMachineState::MMS_ACTIVE && _state != MqMachineState::MMS_CLOSING) {
        tDLogIfW(PARAMS.verbose, "MqStreamClient::onMsg() Illegal client state({}), skip current message ({})",
                 getMachineStateName(_state), getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    if (msg->event == ME_CLOSE) {
        tDLogI("MqStreamClient::onMsg() Close message confirmed.");
        _reconnect = libtbag::mq::details::RECONNECT_DONE;
        onTearDownStep1(true);
        return AfterAction::AA_OK;
    } else {
        onSendStep1_EVENT(msg);
        return AfterAction::AA_DELAY;
    }
}

void MqStreamClient::onCloseMsgDone()
{
    MqEventQueue::clear();
    onCloseStep2_EVENT_QUEUE_CLOSED();
}

void MqStreamClient::onWriterAsync(Writer * writer)
{
    assert(writer != nullptr);
    assert(writer == _writer.get());
    onSendStep2_ASYNC();
}

void MqStreamClient::onWriterClose(Writer * writer)
{
    assert(writer != nullptr);
    assert(writer == _writer.get());
    _writer.reset();
    onCloseStep3_WRITER_CLOSED();
}

void MqStreamClient::onCloseTimerTimer(CloseTimer * timer)
{
    assert(timer != nullptr);
    timer->close();
    onTearDownStep3_TIMEOUT();
}

void MqStreamClient::onCloseTimerClose(CloseTimer * timer)
{
    assert(timer != nullptr);
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onCloseTimerClose() Close timer.");
}

void MqStreamClient::onConnectTimer(ConnectTimer * timer)
{
    assert(timer != nullptr);
    assert(timer == _connector.get());
    onInitStep3_TIMEOUT();
}

void MqStreamClient::onConnectTimerClose(ConnectTimer * timer)
{
    assert(timer != nullptr);
    assert(timer == _connector.get());
    _connector.reset();
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onConnectTimerClose() Close connect timer.");
}

void MqStreamClient::onConnect(ConnectRequest & request, Err code)
{
    onInitStep3_CONNECT(code);
}

void MqStreamClient::onShutdown(ShutdownRequest & request, Err code)
{
    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onShutdown() Shutdown success.");
    } else {
        tDLogIfW(PARAMS.verbose, "MqStreamClient::onShutdown() Shutdown error: {}", code);
    }
}

void MqStreamClient::onWrite(WriteRequest & request, Err code)
{
    onSendStep4_WRITE_DONE(code);
}

binf MqStreamClient::onAlloc(std::size_t suggested_size)
{
    return libtbag::uvpp::defaultOnAlloc(_read_buffer, suggested_size);
}

void MqStreamClient::onRead(Err code, char const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_client));

    if (code == Err::E_EOF) {
        tDLogI("MqStreamClient::onRead() End of file.");
        if (libtbag::mq::details::isActiveState(_state)) {
            onTearDownStep1(false);
        }
        return;
    }

    if (isFailure(code)) {
        ++_read_error_count;
        if (_read_error_count >= PARAMS.continuous_read_error_count) {
            tDLogE("MqStreamClient::onRead() Read error({}) and close.", code);
            if (libtbag::mq::details::isActiveState(_state)) {
                onTearDownStep1(false);
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
        auto begin = _remaining_read.begin();
        _remaining_read.erase(begin, begin + computed_total);
    }

    tDLogIfD(PARAMS.verbose, "MqStreamClient::onRead() Remaining size: {}", _remaining_read.size());
}

void MqStreamClient::onClose()
{
    _client.reset();
    onCloseStep4_CLIENT_CLOSED();
}

// ---------------------
// Alias of event steps.
// ---------------------

void MqStreamClient::onInitStep1_ASYNC()
{
    assert(!static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_INITIALIZING);

    auto init = _loop.newHandle<Initializer>(_loop, this);
    assert(static_cast<bool>(init));
    assert(init->isInit());

    auto const CODE = init->send();
    assert(isSuccess(CODE));

    ++_reconnect;
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onInitStep1_ASYNC() Asynchronous initialization request.");
}

void MqStreamClient::onInitStep2_INIT()
{
    assert(!static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_INITIALIZING);

    if (PARAMS.type == MqType::MT_PIPE) {
        _client = _loop.newHandle<PipeClient>(_loop, this);
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        _client = _loop.newHandle<TcpClient>(_loop, this);
    }

    assert(static_cast<bool>(_client));
    assert(_client->isInit());

    if (PARAMS.type == MqType::MT_PIPE) {
        if (!libtbag::filesystem::Path(PARAMS.address).exists()) {
            tDLogE("MqStreamClient::onInitStep2_INIT() Not found named pipe: {}", PARAMS.address);
            onInit_FAILURE(Err::E_ENFOUND);
            return;
        }

        auto * pipe = (PipeClient*)(_client.get());
        assert(pipe != nullptr);
        pipe->connect(pipe->connect_req, PARAMS.address.c_str());
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        auto * tcp = (TcpClient*)(_client.get());

        SocketAddress addr;
        auto const INIT_CODE = addr.init(PARAMS.address, PARAMS.port);
        if (isFailure(INIT_CODE)) {
            tDLogE("MqStreamClient::onInitStep2_INIT() Socket init error: {} (Address:{}, Port:{})",
                   INIT_CODE, PARAMS.address, PARAMS.port);
            onInit_FAILURE(INIT_CODE);
            return;
        }

        auto const BIND_CODE = tcp->connect(tcp->connect_req, addr.getCommon());
        if (isFailure(BIND_CODE)) {
            tDLogE("MqStreamClient::onInitStep2_INIT() Socket connect error: {}", BIND_CODE);
            onInit_FAILURE(BIND_CODE);
            return;
        }
    }

    _connector = _loop.newHandle<ConnectTimer>(_loop, this);
    assert(static_cast<bool>(_connector));
    assert(_connector->isInit());

    auto const START_CODE = _connector->start(PARAMS.connect_timeout_millisec);
    assert(isSuccess(START_CODE));

    _state = MqMachineState::MMS_INITIALIZED;
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onInitStep2_INIT() Initialization succeeded. Wait for the connection.");
}

void MqStreamClient::onInitStep3_TIMEOUT()
{
    assert(static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_INITIALIZED);

    auto const STOP_CODE = _connector->stop();
    assert(isSuccess(STOP_CODE));
    _connector->close();

    Err code;
    if (PARAMS.type == MqType::MT_PIPE) {
        auto * pipe = (PipeClient*)(_client.get());
        code = pipe->connect_req.cancel();
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        auto * tcp = (TcpClient*)(_client.get());
        code = tcp->connect_req.cancel();
    }

    assert(isSuccess(code));
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onInitStep3_TIMEOUT() Cancel the connect request.");

    onInit_FAILURE(Err::E_TIMEOUT);
}

void MqStreamClient::onInitStep3_CONNECT(Err code)
{
    assert(static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_INITIALIZED);

    auto const STOP_CODE = _connector->stop();
    assert(isSuccess(STOP_CODE));
    _connector->close();

    if (isFailure(code)) {
        tDLogE("MqStreamClient::onInitStep3_CONNECT() Connect error: {}", code);
        onInit_FAILURE(code);
        return;
    }

    auto const START_CODE = _client->startRead();
    if (isFailure(START_CODE)) {
        tDLogE("MqStreamClient::onInitStep3_CONNECT() Start read error: {}", START_CODE);
        onInit_FAILURE(START_CODE);
        return;
    }

    onInit_SUCCESS();
}

void MqStreamClient::onInit_SUCCESS()
{
    assert(static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_INITIALIZED);

    _writer = _loop.newHandle<Writer>(_loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    bool const EVENT_QUEUE_INIT = MqEventQueue::initialize(_loop, PARAMS.send_queue_size, PARAMS.send_msg_size);
    assert(EVENT_QUEUE_INIT);
    assert(MqEventQueue::exists());

    tDLogI("MqStreamClient::onInit_SUCCESS() Active state!");
    _state = MqMachineState::MMS_ACTIVE;
    MqBase::enableWait();

    if (INTERNAL.connect_cb != nullptr) {
        assert(INTERNAL.parent);
        INTERNAL.connect_cb(INTERNAL.parent);
    }
}

void MqStreamClient::onInit_FAILURE(Err code)
{
    assert(static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(libtbag::mq::details::isInitializeState(_state));

    _state = MqMachineState::MMS_CLOSING;
    assert(!_client->isClosing());
    _client->close();
}

void MqStreamClient::onSendStep1_EVENT(AsyncMsg * msg)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_ACTIVE || _state == MqMachineState::MMS_CLOSING);
    assert(msg != nullptr);

    _writer->queue.push(AsyncMsgPointer(msg));

    if (_writer->state == MqRequestState::MRS_WAITING) {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
    } else {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onSendStep1_EVENT() Writer is already working: {}",
                 _writer->queue.size());
    }
}

void MqStreamClient::onSendStep2_ASYNC()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_ACTIVE || _state == MqMachineState::MMS_CLOSING);
    assert(_writer->state == MqRequestState::MRS_ASYNC);
    assert(!_writer->queue.empty());

    _writer->state = MqRequestState::MRS_REQUESTING;
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onSendStep2_ASYNC() Async write: {}", _writer->queue.size());

    auto msg_pointer = _writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    auto * msg = msg_pointer.get();
    assert(msg != nullptr);

    if (INTERNAL.default_write != nullptr) {
        // Give users the opportunity to use the original data.
        onSendStep3_ASYNC_ORIGINAL(msg);
    } else {
        onSendStep3_ASYNC_MQMSG(msg);
    }
}

void MqStreamClient::onSendStep3_ASYNC_ORIGINAL(AsyncMsg * msg)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(msg != nullptr);
    assert(INTERNAL.default_write != nullptr);
    assert(INTERNAL.parent != nullptr);

    auto const WRITE_COUNT = INTERNAL.default_write(_client.get(), msg->event, msg->data(), msg->size(), INTERNAL.parent);
    if (WRITE_COUNT >= 1) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onSendStep3_ASYNC_ORIGINAL() Default write process. Wait for the next step.");
    } else {
        assert(WRITE_COUNT == 0);
        tDLogIfW(PARAMS.verbose, "MqStreamClient::onSendStep3_ASYNC_ORIGINAL() Default write error, Skip this message.");
        onSendStep5_NEXT_MESSAGE(msg);
    }
}

template <typename ClientT>
static Err __write_client(Stream * stream, uint8_t const * data, std::size_t size) TBAG_NOEXCEPT
{
    auto * node = ((ClientT*)(stream));
    return node->write(node->write_req, (char const *)data, size);
}

static Err __write_client(MqType type, Stream * stream, uint8_t const * data, std::size_t size) TBAG_NOEXCEPT
{
    if (type == MqType::MT_PIPE) {
        return __write_client<PipeClient>(stream, data, size);
    } else {
        return __write_client<TcpClient>(stream, data, size);
    }
}

void MqStreamClient::onSendStep3_ASYNC_MQMSG(AsyncMsg * msg)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(msg != nullptr);
    assert(INTERNAL.write_cb != nullptr);
    assert(INTERNAL.parent != nullptr);

    if (msg->empty()) {
        tDLogE("MqStreamClient::onSendStep3_ASYNC_MQMSG() Empty message buffer, skip this message.");
        onSendStep5_NEXT_MESSAGE(msg);
        return;
    }

    auto const CODE = _packer.build(*msg);
    assert(isSuccess(CODE));

    // Give the user a chance to filter the message.
    if (INTERNAL.write_cb(*msg, INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onSendStep3_ASYNC_MQMSG() The message to be sent has been filtered.");
        onSendStep5_NEXT_MESSAGE(msg);
        return;
    }

    auto const WRITE_CODE = __write_client(PARAMS.type, _client.get(), _packer.point(), _packer.size());
    if (isSuccess(WRITE_CODE)) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onSendStep3_ASYNC_MQMSG() Write process {}byte ... "
                 "Wait for the next step.", _packer.size());
    } else {
        tDLogE("MqStreamClient::onSendStep3_ASYNC_MQMSG() Write error({}), Skip this message.", WRITE_CODE);
        onSendStep5_NEXT_MESSAGE(msg);
    }
}

void MqStreamClient::onSendStep4_WRITE_DONE(Err code)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_ACTIVE || _state == MqMachineState::MMS_CLOSING);
    assert(!_writer->queue.empty());
    assert(_writer->state == MqRequestState::MRS_REQUESTING);

    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onSendStep4_WRITE_DONE() Write success.");
    } else {
        tDLogE("MqStreamClient::onSendStep4_WRITE_DONE() Write error: {}", code);
    }
    onSendStep5_NEXT_MESSAGE(_writer->queue.front().get());
}

void MqStreamClient::onSendStep5_NEXT_MESSAGE(AsyncMsg * msg)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(msg != nullptr);

    _writer->queue.pop();
    auto const CODE = MqEventQueue::restoreMessage(msg, PARAMS.verify_restore_message);
    assert(isSuccess(CODE));

    if (!_writer->queue.empty()) {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onSendStep5_NEXT_MESSAGE() Async next message (queue:{}) ...",
                 _writer->queue.size());
        return;
    }

    assert(_writer->queue.empty());
    _writer->state = MqRequestState::MRS_WAITING;

    // If the shutdown is delayed, proceed with it.
    if (_state == MqMachineState::MMS_CLOSING) {
        tDLogIfN(PARAMS.verbose, "MqStreamClient::onSendStep5_NEXT_MESSAGE() Delayed shutdown progresses.");
        onTearDownStep2_SHUTDOWN();
    } else {
        tDLogIfI(PARAMS.verbose, "MqStreamClient::onSendStep5_NEXT_MESSAGE() Waiting for messages.");
    }
}

void MqStreamClient::onTearDownStep1(bool from_message_event)
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());

    using namespace libtbag::mq::details;
    if (_state != MqMachineState::MMS_ACTIVE) {
        // After acquiring the 'Close Message', the second call can be made to the 'Read Event'.
        tDLogIfD(PARAMS.verbose, "MqStreamClient::onTearDownStep1() It is already closing.");
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
    if (from_message_event) {
        assert(active_send_size >= 1);
        // Subtract the current message.
        --active_send_size;
    }

    if (active_send_size > 0 || _writer->state != MqRequestState::MRS_WAITING) {
        // Clear any remaining transmission queues and continue with system shutdown.
        tDLogI("MqStreamClient::onTearDownStep1() Delay the shutdown request ...");
        return;
    }

    assert(_writer->queue.empty());
    onTearDownStep2_SHUTDOWN();
}

template <typename ClientT>
static Err __shutdown(Stream * stream) TBAG_NOEXCEPT
{
    auto * node = ((ClientT*)(stream));
    return node->shutdown(node->shutdown_req);
}

static Err __shutdown(MqType type, Stream * stream) TBAG_NOEXCEPT
{
    if (type == MqType::MT_PIPE) {
        return __shutdown<PipeClient>(stream);
    } else {
        return __shutdown<TcpClient>(stream);
    }
}

void MqStreamClient::onTearDownStep2_SHUTDOWN()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(_writer->queue.empty());

    auto const SHUTDOWN_CODE = __shutdown(PARAMS.type, _client.get());
    if (isSuccess(SHUTDOWN_CODE)) {
        tDLogIfI(PARAMS.verbose, "MqStreamClient::onTearDownStep2_SHUTDOWN() Shutdown success.");
    } else {
        tDLogW("MqStreamClient::onTearDownStep2_SHUTDOWN() Shutdown error: {}", SHUTDOWN_CODE);
    }

    auto timer = _loop.newHandle<CloseTimer>(_loop, this);
    assert(static_cast<bool>(timer));

    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    assert(isSuccess(START_CODE));
}

void MqStreamClient::onTearDownStep3_TIMEOUT()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSING);

    onCloseStep1();
}

void MqStreamClient::onCloseStep1()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(_writer->queue.empty());

    MqEventQueue::closeAsyncMessages();
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onCloseStep1() Close request of all messages.");
}

void MqStreamClient::onCloseStep2_EVENT_QUEUE_CLOSED()
{
    assert(static_cast<bool>(_client));
    assert(static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(_writer->queue.empty());
    assert(!_writer->isClosing());

    _writer->close();
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onCloseStep2_EVENT_QUEUE_CLOSED() Close request of writer.");
}

void MqStreamClient::onCloseStep3_WRITER_CLOSED()
{
    assert(static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(!_client->isClosing());

    _client->close();
    tDLogIfD(PARAMS.verbose, "MqStreamClient::onCloseStep3_WRITER_CLOSED() Close request of client.");
}

void MqStreamClient::onCloseStep4_CLIENT_CLOSED()
{
    assert(!static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
    assert(!MqEventQueue::exists());
    assert(_state == MqMachineState::MMS_CLOSING);

    if (_reconnect != libtbag::mq::details::RECONNECT_DONE) {
        if (PARAMS.reconnect_count == 0 || _reconnect < PARAMS.reconnect_count) {
            if (PARAMS.reconnect_count == 0) {
                tDLogI("MqStreamClient::onCloseStep4_CLIENT_CLOSED() Try reconnect: INFINITY");
            } else {
                tDLogI("MqStreamClient::onCloseStep4_CLIENT_CLOSED() Try reconnect: {}/{}",
                       _reconnect, PARAMS.reconnect_count);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(PARAMS.reconnect_delay_millisec));
            _state = MqMachineState::MMS_INITIALIZING;
            onInitStep1_ASYNC();
            return;
        }

        assert(_reconnect == PARAMS.reconnect_count);
        tDLogIfW(PARAMS.verbose,
                 "MqStreamClient::onCloseStep4_CLIENT_CLOSED() "
                 "The maximum number of reconnection count: {}",
                 PARAMS.reconnect_count);
    }

    tDLogI("MqStreamClient::onCloseStep4_TERMINATOR_CLOSED() Close client!");
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

void MqStreamClient::onCloseStep5_TERMINATOR_CLOSED()
{
    assert(!static_cast<bool>(_client));
    assert(!static_cast<bool>(_writer));
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

