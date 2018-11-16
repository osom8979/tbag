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

MqStreamServer::MqStreamServer(Loop & loop, MqParams const & params)
        : MqEventQueue(loop, params.send_queue_size, params.send_msg_size),
          TYPE(params.type), PARAMS(params), _server(), _nodes(), _packer(params.packer_size),
          _recv_queue(params.recv_queue_size, params.recv_msg_size),
          _closing_server(false)
{
    if (TYPE == MqType::MT_PIPE) {
        _server = loop.newHandle<PipeServer>(loop, this);
    } else if (TYPE == MqType::MT_TCP) {
        _server = loop.newHandle<TcpServer>(loop, this);
    } else {
        throw ErrException(Err::E_ILLARGS);
    }
    assert(static_cast<bool>(_server));
    assert(_server->isInit());

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    if (TYPE == MqType::MT_PIPE) {
        auto * pipe = (PipeServer*)(_server.get());
        assert(pipe != nullptr);
        auto const BIND_CODE = pipe->bind(params.address.c_str());
        if (isFailure(BIND_CODE)) {
            throw ErrException(BIND_CODE);
        }

        Err const LISTEN_CODE = pipe->listen();
        if (isFailure(LISTEN_CODE)) {
            throw ErrException(LISTEN_CODE);
        }
    } else {
        assert(TYPE == MqType::MT_TCP);
        auto * tcp = (TcpServer*)(_server.get());

        SocketAddress addr;
        auto const INIT_CODE = addr.init(params.address, params.port);
        if (isFailure(INIT_CODE)) {
            throw ErrException(INIT_CODE);
        }

        Tcp::BindFlag flags;
        if (params.tcp_ipv6_only) {
            flags = Tcp::BindFlag::BF_IPV6_ONLY;
        } else {
            flags = Tcp::BindFlag::BF_NONE;
        }

        auto const BIND_CODE = tcp->bind(addr.getCommon(), flags);
        if (isFailure(BIND_CODE)) {
            throw ErrException(BIND_CODE);
        }

        Err const LISTEN_CODE = tcp->listen();
        if (isFailure(LISTEN_CODE)) {
            throw ErrException(LISTEN_CODE);
        }
    }
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
        onCloseEvent();
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return AfterAction::AA_OK;
}

void MqStreamServer::onCloseEvent()
{
    assert(THREAD_ID == std::this_thread::get_id());
    tDLogI("MqStreamServer::onCloseEvent() Start closing ...");

    closeNodes();
    closeServer();
}

Err MqStreamServer::closeServer()
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(static_cast<bool>(_server));

    _closing_server = true;

    if (PARAMS.wait_closing_millisec == 0) {
        _server->close();
        return Err::E_SUCCESS;
    }

    auto * loop = _server->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this, _server.get());
    if (!timer) {
        _server->close();
        return Err::E_EXPIRED;
    }

    assert(PARAMS.wait_closing_millisec > 0);
    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    if (isFailure(START_CODE)) {
        _server->close();
        timer->close();
    }
    return START_CODE;
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
        return __shutdown<PipeNode>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __shutdown<TcpNode>(stream);
}

Err MqStreamServer::closeNode(Stream * node)
{
    assert(THREAD_ID == std::this_thread::get_id());
    auto const CODE = __shutdown(node, TYPE);
    if (CODE == Err::E_ILLSTATE) {
        return Err::E_ILLSTATE; // Skip...
    }
    if (isFailure(CODE)) {
        node->close();
    }
    return CODE;
}

std::size_t MqStreamServer::closeNodes()
{
    assert(THREAD_ID == std::this_thread::get_id());
    std::size_t skip_or_success_count = 0;
    for (auto /*&*/ node : _nodes) {
        auto const CODE = closeNode(node.get());
        if (CODE == Err::E_SUCCESS || CODE == Err::E_ILLSTATE) {
            ++skip_or_success_count;
        }
    }
    return skip_or_success_count;
}


template <typename NodeT>
static std::size_t __write_all_nodes(NodeSet & nodes, uint8_t const * data, std::size_t size)
{
    std::size_t success_count = 0;
    for (auto & cursor : nodes) {
        assert(static_cast<bool>(cursor));
        auto * node = (NodeT*)(cursor.get());
        auto const CODE = node->write(node->write_req, (char const *)data, size);
        if (isSuccess(CODE)) {
            ++success_count;
        }
    }
    return success_count;
}

static std::size_t __write_all_nodes(NodeSet & nodes, uint8_t const * data, std::size_t size, MqType type)
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

    auto msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    auto const CODE = _packer.build(*(msg_pointer.get()));
    assert(isSuccess(CODE));

    writer->write_count = __write_all_nodes(_nodes, _packer.point(), _packer.size(), TYPE);
    if (writer->write_count == 0) {
        tDLogE("MqStreamServer::onWriterAsync() Write error: {} nodes", _nodes.size());

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

void MqStreamServer::onWriterClose(Writer * writer)
{
    // EMPTY.
}

void MqStreamServer::onCloseTimer(CloseTimer * timer)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(timer != nullptr);
    assert(timer->stream != nullptr);

    if (!timer->stream->isClosing()) {
        timer->stream->close();
    }
    timer->close();
}

void MqStreamServer::onCloseTimerClose(CloseTimer * timer)
{
    // EMPTY.
}

void MqStreamServer::onNodeShutdown(Stream * node, ShutdownRequest & request, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    assert(node != nullptr);

    tDLogI("MqStreamServer::onNodeShutdown() Shutdown node: @{}", (void*)node);

    if (PARAMS.wait_closing_millisec == 0) {
        node->close();
        return;
    }

    auto * loop = node->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this, node);
    if (!timer) {
        node->close();
        return;
    }

    assert(PARAMS.wait_closing_millisec > 0);
    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    if (isFailure(START_CODE)) {
        node->close();
        timer->close();
    }
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
static std::size_t & __get_read_error_count(Stream * stream)
{
    return ((NodeT*)(stream))->read_error_count;
}

static std::size_t & __get_read_error_count(Stream * stream, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __get_read_error_count<PipeNode>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __get_read_error_count<TcpNode>(stream);
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

    auto & error_count = __get_read_error_count(node, TYPE);
    if (code != Err::E_SUCCESS) {
        ++error_count;
        tDLogE("MqStreamServer::onNodeRead() Read error: {} ({}/{})",
               code, error_count, PARAMS.continuous_read_error_count);
        if (error_count >= PARAMS.continuous_read_error_count) {
            node->close();
        }
        return;
    }

    assert(code == Err::E_SUCCESS);
    error_count = 0;
    auto & remaining_read = __read_buffer(node, buffer, size, TYPE);

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
        tDLogE("MqStreamServer::onNodeRead() Enqueue error: {}", ENQUEUE_CODE);
        return;
    }

    // [DONE] All processes succeeded.
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

    tDLogI("MqStreamServer::onNodeClose() Close node: @{}", (void*)node);
}

void MqStreamServer::onServerConnection(Stream * server, Err code)
{
    assert(THREAD_ID == std::this_thread::get_id());
    if (code != Err::E_SUCCESS) {
        tDLogE("MqStreamServer::onServerConnection() Connection {} error.", code);
        return;
    }

    if (_closing_server) {
        tDLogE("MqStreamServer::onServerConnection() Closing this server.");
        return;
    }

    if (_nodes.size() + 1 >= PARAMS.max_nodes) {
        tDLogE("MqStreamServer::onServerConnection() The connection is full ({})", PARAMS.max_nodes);
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
    assert(static_cast<bool>(stream));
    assert(stream->isInit());

    auto const ACCEPT_CODE = server->accept(*stream);
    assert(ACCEPT_CODE == Err::E_SUCCESS);

    if (TYPE == MqType::MT_TCP && !PARAMS.accept_ip_regex.empty()) {
        auto * tcp = (TcpNode*)stream.get();
        auto const PEER_IP = tcp->getPeerIp();
        if (!std::regex_match(PEER_IP, std::regex(PARAMS.accept_ip_regex))) {
            tDLogD("MqStreamServer::onServerConnection() Filtered ip: {}", PEER_IP);
            stream->close();
            return;
        }
    }

    auto const START_CODE = stream->startRead();
    assert(START_CODE == Err::E_SUCCESS);

    auto const INSERT_RESULT = _nodes.insert(StreamPointer(stream.get())).second;
    assert(INSERT_RESULT);

    std::string peer_info;
    if (TYPE == MqType::MT_PIPE) {
        //peer_info = ((PipeNode*)stream.get())->getPeerName();
        peer_info = ((PipeNode*)stream.get())->getSockName();
    } else if (TYPE == MqType::MT_TCP) {
        peer_info = ((TcpNode*)stream.get())->getPeerIp();
        peer_info += ':';
        peer_info += libtbag::string::toString(((TcpNode*)stream.get())->getPeerPort());
    }
    tDLogI("MqStreamServer::onServerConnection() Connecton success: {}", peer_info);
}

void MqStreamServer::onServerClose(Stream * server)
{
    assert(THREAD_ID == std::this_thread::get_id());
    tDLogI("MqStreamServer::onServerClose() CLOSE SERVER!");

    if (!_writer->isClosing()) {
        _writer->close();
    }
    closeAsyncMsgs();
}

Err MqStreamServer::send(MqMsg const & msg)
{
    if (_closing_server) {
        return Err::E_CLOSING;
    }
    return enqueue(msg);
}

Err MqStreamServer::recv(MqMsg & msg)
{
    if (_closing_server) {
        return Err::E_CLOSING;
    }
    return _recv_queue.dequeue(msg);
}

void MqStreamServer::recvWait(MqMsg & msg)
{
    if (_closing_server) {
        return;
    }
    _recv_queue.dequeueWait(msg);
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

