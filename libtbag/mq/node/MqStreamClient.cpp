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
          TYPE(params.type), PARAMS(params), _client(), _packer(params.packer_size),
          _recv_queue(params.recv_queue_size, params.recv_msg_size)
{
    if (TYPE == MqType::MT_PIPE) {
        _client = loop.newHandle<PipeClient>(loop, this);
    } else if (TYPE == MqType::MT_TCP) {
        _client = loop.newHandle<TcpClient>(loop, this);
    } else {
        throw ErrException(Err::E_ILLARGS);
    }
    assert(static_cast<bool>(_client));
    assert(_client->isInit());

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    if (TYPE == MqType::MT_PIPE) {
        if (!libtbag::filesystem::Path(params.address).exists()) {
            throw ErrException(Err::E_EEXIST);
        }

        auto * pipe = (PipeClient*)(_client.get());
        assert(pipe != nullptr);
        pipe->connect(pipe->connect_req, params.address.c_str());
    } else {
        assert(TYPE == MqType::MT_TCP);
        auto * tcp = (TcpClient*)(_client.get());

        SocketAddress addr;
        auto const INIT_CODE = addr.init(params.address, params.port);
        if (isFailure(INIT_CODE)) {
            throw ErrException(INIT_CODE);
        }

        auto const BIND_CODE = tcp->connect(tcp->connect_req, addr.getCommon());
        if (isFailure(BIND_CODE)) {
            throw ErrException(BIND_CODE);
        }
    }
}

MqStreamClient::~MqStreamClient()
{
    assert(THREAD_ID == std::this_thread::get_id());
}

AfterAction MqStreamClient::onMsg(AsyncMsg * msg)
{
    assert(THREAD_ID == std::this_thread::get_id());
    if (msg->event == MqEvent::ME_MSG) {
        _writer->queue.push(AsyncMsgPointer(msg));

        if (_writer->state == RequestState::RS_WAITING) {
            auto const CODE = _writer->send();
            assert(isSuccess(CODE));
            _writer->state = RequestState::RS_ASYNC;
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

template <typename NodeT>
static Err __shutdown(Stream * stream)
{
    auto * node = (NodeT*)(stream);
    if (node->is_shutdown) {
        return Err::E_ILLSTATE;
    }

    auto const CODE = node->shutdown(node->shutdown_req);
    if (isSuccess(CODE)) {
        node->is_shutdown = true;
    }
    return CODE;
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
    tDLogI("MqStreamClient::onCloseEvent() Start closing ...");

    auto const CODE = __shutdown(_client.get(), TYPE);
    if (CODE != Err::E_SUCCESS && CODE != Err::E_ILLSTATE) {
        _client->close();
    }
}

template <typename NodeT>
static Err __write_client(Stream * stream, uint8_t const * data, std::size_t size)
{
    assert(stream != nullptr);
    auto * node = (NodeT*)(stream);
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
    assert(THREAD_ID == std::this_thread::get_id());
    assert(writer != nullptr);
    assert(writer->queue.empty());
    assert(writer->state == RequestState::RS_ASYNC);
    writer->state = RequestState::RS_REQUESTING;

    auto msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    auto const CODE = _packer.build(*(msg_pointer.get()));
    assert(isSuccess(CODE));

    auto const WRITE_CODE = __write_client(_client.get(), _packer.point(), _packer.size(), TYPE);
    if (isFailure(WRITE_CODE)) {
        tDLogE("MqStreamClient::onWriterAsync() Write error: {}", WRITE_CODE);

        writer->queue.pop();

        auto const CODE = restoreMessage(msg_pointer.get(), PARAMS.verify_restore_message);
        assert(isSuccess(CODE));

        if (writer->queue.empty()) {
            writer->state = RequestState::RS_WAITING;
        } else {
            auto const CODE = writer->send();
            assert(isSuccess(CODE));
            writer->state = RequestState::RS_ASYNC;
        }
    }
}

void MqStreamClient::onWriterClose(Writer * writer)
{
    // EMPTY.
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
    // EMPTY.
}

void MqStreamClient::onConnect(ConnectRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_client));

    if (isFailure(code)) {
        return;
    }

    Err const START_CODE = _client->startRead();
    if (isFailure(START_CODE)) {
        return;
    }
}

void MqStreamClient::onShutdown(ShutdownRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_client));

    tDLogI("MqStreamClient::onNodeShutdown() Shutdown node");

    if (PARAMS.wait_closing_millisec == 0) {
        _client->close();
        return;
    }

    auto * loop = _client->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this);
    if (!timer) {
        _client->close();
        return;
    }

    assert(PARAMS.wait_closing_millisec > 0);
    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    if (isFailure(START_CODE)) {
        _client->close();
        timer->close();
    }
}

void MqStreamClient::onWrite(WriteRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_writer));
    assert(!_writer->queue.empty());
    assert(_writer->state == RequestState::RS_REQUESTING);

    auto value = _writer->queue.front();
    _writer->queue.pop();

    auto const CODE = restoreMessage(value.get(), PARAMS.verify_restore_message);
    assert(isSuccess(CODE));

    if (_writer->queue.empty()) {
        _writer->state = RequestState::RS_WAITING;
    } else {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = RequestState::RS_ASYNC;
    }
}

template <typename NodeT>
static Buffer & __get_read_buffer(Stream * stream)
{
    return ((NodeT*)(stream))->read_buffer;
}

static Buffer & __get_read_buffer(Stream * stream, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __get_read_buffer<PipeClient>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __get_read_buffer<TcpClient>(stream);
}

binf MqStreamClient::onAlloc(std::size_t suggested_size)
{
    assert(THREAD_ID == std::this_thread::get_id());
    return libtbag::uvpp::defaultOnAlloc(__get_read_buffer(_client.get(), TYPE), suggested_size);
}

template <typename NodeT>
static std::size_t & __get_read_error_count(Stream * stream)
{
    return ((NodeT*)(stream))->read_error_count;
}

static std::size_t & __get_read_error_count(Stream * stream, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __get_read_error_count<PipeClient>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __get_read_error_count<TcpClient>(stream);
}

template <typename NodeT>
static Buffer & __read_buffer(Stream * stream, char const * buffer, std::size_t size)
{
    auto * node = (NodeT*)(stream);
    node->remaining_read.insert(node->remaining_read.end(), buffer, buffer + size);
    return node->remaining_read;
}

static Buffer & __read_buffer(Stream * stream, char const * buffer, std::size_t size, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __read_buffer<PipeClient>(stream, buffer, size);
    }
    assert(type == MqType::MT_TCP);
    return __read_buffer<TcpClient>(stream, buffer, size);
}

void MqStreamClient::onRead(Err code, char const * buffer, std::size_t size)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(_client != nullptr);

    if (code == Err::E_EOF) {
        _client->close();
        return;
    }

    auto & error_count = __get_read_error_count(_client.get(), TYPE);
    if (code != Err::E_SUCCESS) {
        ++error_count;
        tDLogE("MqStreamClient::onNodeRead() Read error: {} ({}/{})",
               code, error_count, PARAMS.continuous_read_error_count);
        if (error_count >= PARAMS.continuous_read_error_count) {
            _client->close();
        }
        return;
    }

    assert(code == Err::E_SUCCESS);
    error_count = 0;
    auto & remaining_read = __read_buffer(_client.get(), buffer, size, TYPE);

    std::size_t computed_size = 0;
    auto const PARSE_CODE = _packer.parseAndUpdate(remaining_read, &computed_size);
    if (isFailure(PARSE_CODE)) {
        // Skip... (maybe no error)
        return;
    }

    assert(computed_size <= remaining_read.size());
    remaining_read.erase(remaining_read.begin(), remaining_read.begin() + computed_size);

    auto const ENQUEUE_CODE = _recv_queue.enqueue(_packer.msg());
    if (isFailure(ENQUEUE_CODE)) {
        tDLogE("MqStreamClient::onNodeRead() Enqueue error: {}", ENQUEUE_CODE);
        return;
    }

    // [DONE] All processes succeeded.
}

void MqStreamClient::onClose()
{
    assert(THREAD_ID == std::this_thread::get_id());
    tDLogI("MqStreamClient::onServerClose() CLOSE CLIENT!");

    if (!_writer->isClosing()) {
        _writer->close();
    }
    closeAsyncMsgs();
}

Err MqStreamClient::send(MqMsg const & msg)
{
    return enqueue(msg);
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

