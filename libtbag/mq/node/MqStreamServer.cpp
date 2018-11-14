/**
 * @file   MqStreamServer.cpp
 * @brief  MqStreamServer class implementation.
 * @author zer0
 * @date   2018-11-13
 */

#include <libtbag/mq/node/MqStreamServer.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using binf = MqStreamServer::binf;

MqStreamServer::MqStreamServer(Loop & loop, Params const & params)
        : TYPE(params.type), THREAD_ID(std::this_thread::get_id()),
          _params(params), _events(loop, params.queue_size, params.msg_size, this),
          _server(), _nodes()
{
    if (TYPE == MqType::MT_PIPE) {
        _server = loop.newHandle<PipeServer>(loop, this);
    } else if (TYPE == MqType::MT_TCP) {
        _server = loop.newHandle<TcpServer>(loop, this);
    } else {
        throw ErrException(Err::E_ILLARGS);
    }

    assert(static_cast<bool>(_server));
}

MqStreamServer::~MqStreamServer()
{
    // EMPTY.
}

MqStreamServer::AfterAction MqStreamServer::onMsg(AsyncMsg * msg)
{
    return AfterAction::AA_DELAY;
}

void MqStreamServer::doWriteRequest(Stream * node, WriteRequest & request, AsyncMsgPointer const & value)
{
}

void MqStreamServer::onWriteDequeue(Stream * node, AsyncMsgPointer const & value)
{
}

void MqStreamServer::onWriteClose(Stream * node)
{
}

void MqStreamServer::onNodeShutdown(Stream * node, ShutdownRequest & request, Err code)
{
}

void MqStreamServer::onNodeWrite(Stream * node, WriteRequest & request, Err code)
{
}

binf MqStreamServer::onNodeAlloc(Stream * node, std::size_t suggested_size)
{
    if (TYPE == MqType::MT_PIPE) {
        return libtbag::uvpp::defaultOnAlloc(((PipeNode*)(node))->read_buffer, suggested_size);
    } else {
        assert(TYPE == MqType::MT_TCP);
        return libtbag::uvpp::defaultOnAlloc(((TcpNode*)(node))->read_buffer, suggested_size);
    }
}

void MqStreamServer::onNodeRead(Stream * node, Err code, char const * buffer, std::size_t size)
{
}

void MqStreamServer::onNodeClose(Stream * node)
{
    Loop * loop = node->getLoop();
    assert(loop != nullptr);

    auto shared = loop->findChildHandle(*node).lock();
    assert(static_cast<bool>(shared));

    auto const ERASE_RESULT = _nodes.erase(StreamPointer(node));
    assert(ERASE_RESULT == 1);
}

void MqStreamServer::onServerConnection(Stream * server, Err code)
{
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
}

Err MqStreamServer::send(char const * buffer, std::size_t size)
{
    return Err::E_UNSUPOP;
}

Err MqStreamServer::recv(std::vector<char> & buffer)
{
    return Err::E_UNSUPOP;
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

