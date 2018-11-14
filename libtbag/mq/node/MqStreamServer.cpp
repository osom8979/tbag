/**
 * @file   MqStreamServer.cpp
 * @brief  MqStreamServer class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using MqType   = MqStreamServer::MqType;
using Stream   = MqStreamServer::Stream;
using PipeNode = MqStreamServer::PipeNode;
using TcpNode  = MqStreamServer::TcpNode;
using NodeSet  = MqStreamServer::NodeSet;
using Buffer   = MqStreamServer::Buffer;
using binf     = MqStreamServer::binf;

MqStreamServer::MqStreamServer(Loop & loop, Params const & params)
        : MqEventQueue(loop, params.send_queue_size, params.send_msg_size),
          TYPE(params.type), _params(params), _server(), _nodes(),
          _packer(params.packer_size)
{
    if (TYPE == MqType::MT_PIPE) {
        _server = loop.newHandle<PipeServer>(loop, this);
    } else if (TYPE == MqType::MT_TCP) {
        _server = loop.newHandle<TcpServer>(loop, this);
    } else {
        throw ErrException(Err::E_ILLARGS);
    }
    assert(static_cast<bool>(_server));

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
}

MqStreamServer::~MqStreamServer()
{
    assert(THREAD_ID == std::this_thread::get_id());
}

MqStreamServer::AfterAction MqStreamServer::onMsg(AsyncMsg * msg)
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
        closeAllNode(_params.wait_closing_millisec);
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return AfterAction::AA_OK;
}

template <typename NodeT>
static std::size_t __write_all_nodes(NodeSet & nodes, char const * data, std::size_t size)
{
    std::size_t success_counter = 0;
    for (auto & cursor : nodes) {
        assert(static_cast<bool>(cursor));
        auto * node = (NodeT*)(cursor.get());
        auto const CODE = node->write(node->write_req, data, size);
        if (isSuccess(CODE)) {
            ++success_counter;
        }
    }
    return success_counter;
}

template <typename NodeT>
static std::size_t __write_all_nodes(NodeSet & nodes, char const * data, std::size_t size, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __write_all_nodes<PipeNode>(nodes, data, size);
    }
    assert(type == MqType::MT_TCP);
    return __write_all_nodes<TcpNode>(nodes, data, size);
}

void MqStreamServer::onWriterAsync(Writer * writer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(writer != nullptr);
    assert(writer->queue.empty());
    assert(writer->state == RequestState::RS_ASYNC);
    writer->state = RequestState::RS_REQUESTING;

    auto const msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    auto const * data = msg_pointer->data();
    auto const   size = msg_pointer->size();

    writer->write_count = __write_all_nodes<PipeNode>(_nodes, data, size, TYPE);
}

void MqStreamServer::onWriterClose(Writer * writer)
{
    assert(THREAD_ID == std::this_thread::get_id());
}

void MqStreamServer::onNodeShutdown(Stream * node, ShutdownRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
}

void MqStreamServer::onNodeWrite(Stream * node, WriteRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_writer));
    assert(!_writer->queue.empty());
    assert(_writer->state == RequestState::RS_REQUESTING);

    if (_writer->write_count > 0) {
        (_writer->write_count)--;
        return;
    }

    auto value = _writer->queue.front();
    _writer->queue.pop();

    auto const CODE = restoreMessage(value.get());
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
        return __get_read_buffer<PipeNode>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __get_read_buffer<TcpNode>(stream);
}

binf MqStreamServer::onNodeAlloc(Stream * node, std::size_t suggested_size)
{
    assert(THREAD_ID == std::this_thread::get_id());
    return libtbag::uvpp::defaultOnAlloc(__get_read_buffer(node, TYPE), suggested_size);
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
        return __read_buffer<PipeNode>(stream, buffer, size);
    }
    assert(type == MqType::MT_TCP);
    return __read_buffer<TcpNode>(stream, buffer, size);
}

void MqStreamServer::onNodeRead(Stream * node, Err code, char const * buffer, std::size_t size)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(node != nullptr);

    if (code == Err::E_EOF) {
        node->close();
        return;
    }
    if (code != Err::E_SUCCESS) {
        // Error.
        return;
    }

    assert(code == Err::E_SUCCESS);
    auto & remaining_read = __read_buffer(node, buffer, size, TYPE);

    MqMsg msg;
    std::size_t computed_size = 0;
    auto const PARSE_CODE = _packer.parse(remaining_read.data(), remaining_read.size(), &msg, &computed_size);
    if (isFailure(PARSE_CODE)) {
        // Skip...
        return;
    }

    auto const ENQUEUE_CODE = _recv_queue.enqueue(msg);
    if (isFailure(ENQUEUE_CODE)) {
        // Skip...
        return;
    }

    // All processes succeeded.
}

void MqStreamServer::onNodeClose(Stream * node)
{
    assert(THREAD_ID == std::this_thread::get_id());
    Loop * loop = node->getLoop();
    assert(loop != nullptr);

    auto shared = loop->findChildHandle(*node).lock();
    assert(static_cast<bool>(shared));

    auto const ERASE_RESULT = _nodes.erase(StreamPointer(node));
    assert(ERASE_RESULT == 1);
}

template <typename NodeT>
static Err __shutdown(MqStreamServer::Stream * stream)
{
    auto * node = (NodeT*)(stream);
    return node->shutdown(node->shutdown_req);
}

static Err __shutdown(Stream * stream, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __shutdown<PipeNode>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __shutdown<TcpNode>(stream);
}

Err MqStreamServer::closeNode(Stream * node, std::size_t wait_closing_millisec)
{
    if (wait_closing_millisec == 0) {
        node->close();
        return Err::E_SUCCESS;
    }

    auto * loop = node->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, node);
    if (!timer) {
        node->close();
        return Err::E_EXPIRED;
    }

    auto const SHUTDOWN_CODE = __shutdown(node, TYPE);
    if (isFailure(SHUTDOWN_CODE)) {
        node->close();
        timer->close();
        return SHUTDOWN_CODE;
    }

    auto const START_CODE = timer->start(wait_closing_millisec);
    if (isFailure(START_CODE)) {
        node->close();
        timer->close();
    }
    return START_CODE;
}

void MqStreamServer::closeAllNode(std::size_t wait_closing_millisec)
{
    for (auto /*&*/ node : _nodes) {
        closeNode(node.get(), wait_closing_millisec);
    }
}

void MqStreamServer::onServerConnection(Stream * server, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    if (code != Err::E_SUCCESS) {
        tDLogE("MqStreamServer::onServerConnection() Connection {} error.", code);
        return;
    }

    if (_nodes.size() + 1 >= _params.max_nodes) {
        tDLogE("MqStreamServer::onServerConnection() The connection is full ({})", _params.max_nodes);
        return;
    }

    assert(server != nullptr);
    assert(code == Err::E_SUCCESS);

    Loop * loop = server->getLoop();
    assert(loop != nullptr);

    SharedStream stream;
    if (TYPE == MqType::MT_PIPE) {
        stream = loop->newHandle<PipeNode>(*loop, this);
    } else {
        assert(TYPE == MqType::MT_TCP);
        stream = loop->newHandle<TcpNode>(*loop, this);
    }

    auto const ACCEPT_CODE = server->accept(*stream);
    assert(ACCEPT_CODE == Err::E_SUCCESS);

    auto const START_CODE = stream->startRead();
    assert(START_CODE == Err::E_SUCCESS);

    auto const INSERT_RESULT = _nodes.insert(StreamPointer(stream.get())).second;
    assert(INSERT_RESULT);
}

void MqStreamServer::onServerClose(Stream * server)
{
    assert(THREAD_ID == std::this_thread::get_id());
}

Err MqStreamServer::send(char const * buffer, std::size_t size)
{
    return Err::E_UNSUPOP;
}

Err MqStreamServer::recv(std::vector<char> & buffer)
{
    return Err::E_UNSUPOP;
}

Err MqStreamServer::recvWait(std::vector<char> & buffer)
{
    return Err::E_UNSUPOP;
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

