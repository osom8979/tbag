/**
 * @file   MqStreamServer.cpp
 * @brief  MqStreamServer class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf = MqStreamServer::binf;

MqStreamServer::MqStreamServer(Loop & loop, Params const & params)
        : MqEventQueue(loop, params.queue_size, params.msg_size),
          TYPE(params.type), _params(params), _server(), _nodes()
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
    // EMPTY.
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
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return AfterAction::AA_OK;
}

// ---------------
namespace __impl {
// ---------------

template <typename NodeT>
std::size_t writeAll(MqStreamServer::NodeSet & nodes, char const * data, std::size_t size)
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

// ------------------
} // namespace __impl
// ------------------

void MqStreamServer::onWriterAsync(Writer * writer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(writer != nullptr);
    assert(writer->queue.empty());
    assert(writer->state == RequestState::RS_ASYNC);
    writer->state = RequestState::RS_REQUESTING;

    auto const msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    auto const * data = msg_pointer->data.data();
    auto const   size = msg_pointer->data.size();

    if (TYPE == MqType::MT_PIPE) {
        writer->write_count = __impl::writeAll<PipeNode>(_nodes, data, size);
    } else {
        writer->write_count = __impl::writeAll<TcpNode>(_nodes, data, size);
    }
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

binf MqStreamServer::onNodeAlloc(Stream * node, std::size_t suggested_size)
{
    assert(THREAD_ID == std::this_thread::get_id());
    if (TYPE == MqType::MT_PIPE) {
        return libtbag::uvpp::defaultOnAlloc(((PipeNode*)(node))->read_buffer, suggested_size);
    } else {
        assert(TYPE == MqType::MT_TCP);
        return libtbag::uvpp::defaultOnAlloc(((TcpNode*)(node))->read_buffer, suggested_size);
    }
}

void MqStreamServer::onNodeRead(Stream * node, Err code, char const * buffer, std::size_t size)
{
    assert(THREAD_ID == std::this_thread::get_id());

    if (code == Err::E_EOF) {
        // Close.
    } else if (code != Err::E_SUCCESS) {
        // Error.
    } else {
        // Success.
    }
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

