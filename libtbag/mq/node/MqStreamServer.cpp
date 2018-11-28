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
#include <chrono>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

using MqType      = MqStreamServer::MqType;
using Stream      = MqStreamServer::Stream;
using PipeNode    = MqStreamServer::PipeNode;
using TcpNode     = MqStreamServer::TcpNode;
using NodeSet     = MqStreamServer::NodeSet;
using Buffer      = MqStreamServer::Buffer;
using binf        = MqStreamServer::binf;
using AfterAction = MqStreamServer::AfterAction;

MqStreamServer::MqStreamServer(Loop & loop, MqParams const & params)
        : MqBase(loop, params, MqMachineState::MMS_NONE),
          _server(), _nodes(), _packer(params.packer_size)
{
    if (PARAMS.type == MqType::MT_PIPE && libtbag::filesystem::Path(params.address).exists()) {
        tDLogE("MqStreamServer::MqStreamServer() Exists named pipe: {}", params.address);
        throw ErrException(Err::E_EEXIST);
    }

    if (PARAMS.type == MqType::MT_PIPE) {
        _server = loop.newHandle<PipeServer>(loop, this);
    } else if (PARAMS.type == MqType::MT_TCP) {
        _server = loop.newHandle<TcpServer>(loop, this);
    } else {
        tDLogE("MqStreamServer::MqStreamServer() Unsupported type: {}", (int)(PARAMS.type));
        throw ErrException(Err::E_ILLARGS);
    }
    assert(static_cast<bool>(_server));
    assert(_server->isInit());

    if (PARAMS.type == MqType::MT_PIPE) {
        auto * pipe = (PipeServer*)(_server.get());
        assert(pipe != nullptr);
        auto const BIND_CODE = pipe->bind(params.address.c_str());
        if (isFailure(BIND_CODE)) {
            tDLogE("MqStreamServer::MqStreamServer() Pipe bind error: {}", BIND_CODE);
            _server->close();
            throw ErrException(BIND_CODE);
        }

        auto const LISTEN_CODE = pipe->listen();
        if (isFailure(LISTEN_CODE)) {
            tDLogE("MqStreamServer::MqStreamServer() Pipe listen error: {}", LISTEN_CODE);
            _server->close();
            throw ErrException(LISTEN_CODE);
        }
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        auto * tcp = (TcpServer*)(_server.get());

        SocketAddress addr;
        auto const INIT_CODE = addr.init(params.address, params.port);
        if (isFailure(INIT_CODE)) {
            tDLogE("MqStreamServer::MqStreamServer() Socket init error: {}", INIT_CODE);
            _server->close();
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
            tDLogE("MqStreamServer::MqStreamServer() Socket bind error: {}", BIND_CODE);
            _server->close();
            throw ErrException(BIND_CODE);
        }

        auto const LISTEN_CODE = tcp->listen();
        if (isFailure(LISTEN_CODE)) {
            tDLogE("MqStreamServer::MqStreamServer() Socket listen error: {}", LISTEN_CODE);
            _server->close();
            throw ErrException(LISTEN_CODE);
        }
    }

    _writer = loop.newHandle<Writer>(loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    _state = MqMachineState::MMS_ACTIVE;
}

MqStreamServer::~MqStreamServer()
{
    // EMPTY.
}

template <typename NodeT>
static Err __shutdown(Stream * stream)
{
    auto * node = (NodeT*)(stream);
    return node->shutdown(node->shutdown_req);
}

Err MqStreamServer::shutdown(Stream * stream)
{
    assert(stream != nullptr);
    if (PARAMS.type == MqType::MT_PIPE) {
        return __shutdown<PipeNode>(stream);
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        return __shutdown<TcpNode>(stream);
    }
}

void MqStreamServer::close(Stream * stream)
{
    assert(stream != nullptr);
    if (!stream->isClosing()) {
        stream->close();
    }
}

std::size_t MqStreamServer::shutdownNodes()
{
    std::size_t success_count = 0;
    for (auto node : _nodes) {
        if (isSuccess(shutdown(node.get()))) {
            ++success_count;
        }
    }
    return success_count;
}

std::size_t MqStreamServer::closeNodes()
{
    std::size_t count = 0;
    for (auto node : _nodes) {
        close(node.get());
        ++count;
    }
    return count;
}

void MqStreamServer::shutdownAndClose()
{
    assert(_writer->queue.empty());
    std::size_t const SHUTDOWN_COUNT = shutdownNodes();

    if (SHUTDOWN_COUNT == _nodes.size()) {
        tDLogIfD(PARAMS.verbose,
                 "MqStreamServer::shutdownAndClose() Shutdown all nodes.");
    } else if (SHUTDOWN_COUNT >= 1) {
        tDLogE("MqStreamServer::shutdownAndClose() Some shutdown commands failed ({}/{}).",
               SHUTDOWN_COUNT, _nodes.size());
    } else {
        assert(SHUTDOWN_COUNT == 0);
        tDLogE("MqStreamServer::shutdownAndClose() All shutdowns failed.");
    }

    auto * loop = _server->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this);
    assert(static_cast<bool>(timer));

    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    assert(isSuccess(START_CODE));
}

void MqStreamServer::tearDown(bool on_message)
{
    using namespace libtbag::mq::details;
    if (isInactiveState(_state)) {
        assert(isCloseState(_state));
        tDLogIfW(PARAMS.verbose, "MqStreamServer::tearDown() It is already closing.");
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
        tDLogI("MqStreamServer::tearDown() Delay the shutdown request ...");
        _state = MqMachineState::MMS_DELAY_CLOSING;
        return;
    }

    assert(_writer->queue.empty());
    shutdownAndClose();
}

void MqStreamServer::onCloseMsgDone()
{
    assert(!_writer->isClosing());
    _writer->close();

    if (PARAMS.verbose) {
        tDLogD("MqStreamServer::onCloseMsgDone() Close request of writer. "
               "Next, onWriterClose() event method.");
    } else {
        tDLogD("MqStreamServer::onCloseMsgDone() Close request of writer.");
    }
}

AfterAction MqStreamServer::onMsg(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (!isActiveState(_state)) {
        tDLogIfW(PARAMS.verbose,
                 "MqStreamServer::onMsg() Illegal client state, "
                 "skip current message ({})", getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    assert(_state == MqMachineState::MMS_ACTIVE);
    if (msg->event == ME_CLOSE) {
        onCloseEvent();
        return AfterAction::AA_OK;
    } else {
        return onMsgEvent(msg);
    }
}

AfterAction MqStreamServer::onMsgEvent(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    _writer->queue.push(AsyncMsgPointer(msg));

    if (_writer->state != MqRequestState::MRS_WAITING) {
        tDLogIfD(PARAMS.verbose,
                 "MqStreamServer::onMsgEvent() Writer is already working: queueing({})",
                 _writer->queue.size());
        return AfterAction::AA_DELAY;
    }

    auto const CODE = _writer->send();
    assert(isSuccess(CODE));

    _writer->state = MqRequestState::MRS_ASYNC;
    return AfterAction::AA_DELAY;
}

void MqStreamServer::onCloseEvent()
{
    tDLogI("MqStreamServer::onCloseEvent() Close message confirmed.");
    tearDown(true);
}

void MqStreamServer::afterProcessMessage(AsyncMsg * msg)
{
    assert(msg != nullptr);

    _writer->queue.pop();
    auto const CODE = restoreMessage(msg, PARAMS.verify_restore_message);
    assert(isSuccess(CODE));

    if (!_writer->queue.empty()) {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
        tDLogIfD(PARAMS.verbose, "MqStreamServer::afterProcessMessage() Async next message ...");
        return;
    }

    _writer->state = MqRequestState::MRS_WAITING;

    // If the shutdown is delayed, proceed with it.
    if (_state == MqMachineState::MMS_DELAY_CLOSING) {
        tDLogIfI(PARAMS.verbose, "MqStreamServer::afterProcessMessage() Async next message ...");
        shutdownAndClose();
    } else {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::afterProcessMessage() Waiting for messages ...");
    }
}

template <typename NodeT>
static Err __write_node(Stream * stream, uint8_t const * data, std::size_t size)
{
    assert(stream != nullptr);
    auto * node = (NodeT*)stream;
    return node->write(node->write_req, (char const *)data, size);
}

static Err __write_node(Stream * stream, uint8_t const * data, std::size_t size, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __write_node<PipeNode>(stream, data, size);
    } else {
        assert(type == MqType::MT_TCP);
        return __write_node<TcpNode>(stream, data, size);
    }
}

template <typename NodeT>
static std::size_t __write_all_nodes(NodeSet & nodes, uint8_t const * data, std::size_t size)
{
    std::size_t success_count = 0;
    for (auto cursor : nodes) {
        auto const CODE = __write_node<NodeT>(cursor.get(), data, size);
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
    } else {
        assert(type == MqType::MT_TCP);
        return __write_all_nodes<TcpNode>(nodes, data, size);
    }
}

void MqStreamServer::onWriterAsync(Writer * writer)
{
    assert(writer != nullptr);
    assert(writer == _writer.get());
    assert(writer->state == MqRequestState::MRS_ASYNC);
    writer->state = MqRequestState::MRS_REQUESTING;

    tDLogIfD(PARAMS.verbose,
             "MqStreamServer::onWriterAsync() Async write: queueing({})",
             _writer->queue.size());

    if (writer->queue.empty()) {
        assert(_state == MqMachineState::MMS_CLOSING);
        writer->state = MqRequestState::MRS_WAITING;
        tDLogIfW(PARAMS.verbose, "MqStreamServer::onWriterAsync() All messages are canceled.");
        return;
    }

    auto msg_pointer = writer->queue.front();
    assert(static_cast<bool>(msg_pointer));

    auto const CODE = _packer.build(*(msg_pointer.get()));
    assert(isSuccess(CODE));

    if (PARAMS.write_cb != nullptr) {
        // Give the user a chance to filter the message.
        if (PARAMS.write_cb(*(msg_pointer.get()), this) == MqIsConsume::MIC_CONSUMED) {
            tDLogIfI(PARAMS.verbose, "MqStreamServer::onWriterAsync() Consumed this message.");
            afterProcessMessage(msg_pointer.get());
            return;
        }
    }

    if (_nodes.empty()) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onWriterAsync() The transport node does not exist.");
        afterProcessMessage(msg_pointer.get());
        return;
    }

    if (msg_pointer.get()->stream) {
        auto itr = _nodes.find(StreamPointer(reinterpret_cast<Stream*>(msg_pointer.get()->stream)));
        if (itr == _nodes.end()) {
            tDLogIfE(PARAMS.verbose, "MqStreamServer::onWriterAsync() Not found node: {}",
                     msg_pointer.get()->stream);
            afterProcessMessage(msg_pointer.get());
            return;
        }

        auto node = *itr;
        auto const CODE = __write_node(node.get(), _packer.point(), _packer.size(), PARAMS.type);
        if (isSuccess(CODE)) {
            writer->write_count = 1;
        } else {
            writer->write_count = 0;
        }
    } else {
        assert(msg_pointer.get()->stream == 0);
        writer->write_count = __write_all_nodes(_nodes, _packer.point(), _packer.size(), PARAMS.type);
    }

    assert(!_nodes.empty());
    if (writer->write_count == _nodes.size()) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onWriterAsync() Write process ({}) ... "
                 "Next, onNodeWrite() event method.", writer->write_count);
    } else if (writer->write_count >= 1) {
        tDLogW("MqStreamServer::onWriterAsync() Some write commands failed ({}/{})."
               "Next, onNodeWrite() event method.", writer->write_count, _nodes.size());
    } else {
        assert(writer->write_count == 0);
        tDLogE("MqStreamServer::onWriterAsync() Write error: {} nodes", _nodes.size());
        afterProcessMessage(msg_pointer.get());
    }
}

void MqStreamServer::onWriterClose(Writer * writer)
{
    using namespace libtbag::mq::details;
    assert(writer != nullptr);
    assert(isClosingState(_state));
    assert(!_server->isClosing());
    _server->close();
}

void MqStreamServer::onCloseTimer(CloseTimer * timer)
{
    assert(timer != nullptr);

    using namespace libtbag::mq::details;
    assert(isClosingState(_state));

    timer->close();
    closeNodes();
    closeAsyncMsgs();
}

void MqStreamServer::onCloseTimerClose(CloseTimer * timer)
{
    assert(timer != nullptr);
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onCloseTimerClose() Close timer.");
}

void MqStreamServer::onNodeShutdown(Stream * node, ShutdownRequest & request, Err code)
{
    assert(node != nullptr);
    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeShutdown() Shutdown success.");
    } else {
        tDLogE("MqStreamServer::onNodeShutdown() Shutdown error: {}", code);
    }
}

void MqStreamServer::onNodeWrite(Stream * node, WriteRequest & request, Err code)
{
    assert(static_cast<bool>(_writer));
    assert(!_writer->queue.empty());
    assert(_writer->state == MqRequestState::MRS_REQUESTING);

    using namespace libtbag::mq::details;
    assert(isActiveState(_state) || isClosingState(_state));

    if (_writer->write_count >= 1) {
        _writer->write_count--;
    }

    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeWrite() Write success. "
                "Remaining write size: {}", _writer->write_count);
    } else {
        tDLogE("MqStreamServer::onNodeWrite() Write error({}). "
               "Remaining write size: {}", code, _writer->write_count);
    }

    if (_writer->write_count == 0) {
        afterProcessMessage(_writer->queue.front().get());
    }
}

template <typename NodeT>
static Buffer & __at_read_buffer(Stream * stream)
{
    return ((NodeT*)(stream))->read_buffer;
}

static Buffer & __at_read_buffer(Stream * stream, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __at_read_buffer<PipeNode>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __at_read_buffer<TcpNode>(stream);
}

binf MqStreamServer::onNodeAlloc(Stream * node, std::size_t suggested_size)
{
    return libtbag::uvpp::defaultOnAlloc(__at_read_buffer(node, PARAMS.type), suggested_size);
}

template <typename NodeT>
static std::size_t & __at_read_error_count(Stream * stream)
{
    return ((NodeT*)(stream))->read_error_count;
}

static std::size_t & __at_read_error_count(Stream * stream, MqType type)
{
    if (type == MqType::MT_PIPE) {
        return __at_read_error_count<PipeNode>(stream);
    }
    assert(type == MqType::MT_TCP);
    return __at_read_error_count<TcpNode>(stream);
}

template <typename NodeT>
static void __insert_buffer(Stream * stream, char const * buffer, std::size_t size)
{
    auto * node = (NodeT*)(stream);
    node->remaining_read.insert(node->remaining_read.end(), buffer, buffer + size);
}

static void __insert_buffer(Stream * stream, char const * buffer, std::size_t size, MqType type)
{
    if (type == MqType::MT_PIPE) {
        __insert_buffer<PipeNode>(stream, buffer, size);
    } else {
        assert(type == MqType::MT_TCP);
        __insert_buffer<TcpNode>(stream, buffer, size);
    }
}

void MqStreamServer::onNodeRead(Stream * node, Err code, char const * buffer, std::size_t size)
{
    assert(node != nullptr);
    tDLogD("MqStreamServer::onNodeRead() code: {}", code);

    if (code == Err::E_EOF) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() End of file.");
        if (!node->isClosing()) {
            node->close();
        }
        return;
    }

    auto & read_error_count = __at_read_error_count(node, PARAMS.type);
    if (isFailure(code)) {
        ++read_error_count;
        if (read_error_count >= PARAMS.continuous_read_error_count) {
            tDLogE("MqStreamServer::onNodeRead() Read error({}) and close.", code);
            node->close();
        } else {
            tDLogE("MqStreamServer::onNodeRead() Read error: {} ({}/{})",
                   code, read_error_count, PARAMS.continuous_read_error_count);
        }
        return;
    }

    assert(isSuccess(code));
    read_error_count = 0;
    __insert_buffer(node, buffer, size, PARAMS.type);

    Buffer * remaining_read = nullptr;
    if (PARAMS.type == MqType::MT_PIPE) {
        remaining_read = &(((PipeNode*)(node))->remaining_read);
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        remaining_read = &(((TcpNode*)(node))->remaining_read);
    }
    assert(remaining_read != nullptr);
    assert(remaining_read->size() >= size);

    std::size_t const REMAINING_SIZE = remaining_read->size();
    std::size_t computed_total = 0;
    std::size_t computed_size  = 0;

    Err parse_code;
    Err enqueue_code;

    while (true) {
        parse_code = _packer.parseAndUpdate(remaining_read->data() + computed_total,
                                            REMAINING_SIZE - computed_total,
                                            &computed_size);
        if (isSuccess(parse_code)) {
            assert(0 < COMPARE_AND(computed_size) <= remaining_read->size());
            computed_total += computed_size;
            tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Remaining size: {}",
                     REMAINING_SIZE - computed_total);
        } else {
            assert(parse_code == Err::E_VERIFIER);
            if (computed_size) {
                tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Verify error "
                         "(remaining size: {}, Required size: {})",
                         REMAINING_SIZE - computed_total, computed_size);
            } else {
                tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Verify error "
                         "(remaining size: {})", REMAINING_SIZE - computed_total);
            }
            break;
        }

        // Update current node key(id);
        _packer.msg().stream = reinterpret_cast<std::intptr_t>(node);

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
                tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Enqueue success.");
            } else {
                tDLogE("MqStreamServer::onNodeRead() Enqueue error: {}", enqueue_code);
                break;
            }
        }
    }

    // [DONE] All processes succeeded.
    if (computed_total >= 1) {
        remaining_read->erase(remaining_read->begin(),
                              remaining_read->begin() + computed_total);
    }

    tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Remaining size: {}",
             remaining_read->size());
}

void MqStreamServer::onNodeClose(Stream * node)
{
    auto const ERASE_RESULT = _nodes.erase(StreamPointer(node));
    assert(ERASE_RESULT == 1);

    tDLogI("MqStreamServer::onNodeClose() Close node: ({}/{})",
           _nodes.size(), PARAMS.max_nodes);
}

void MqStreamServer::onServerConnection(Stream * server, Err code)
{
    if (isFailure(code)) {
        tDLogE("MqStreamServer::onServerConnection() Connection error: {}", code);
        return;
    }

    using namespace libtbag::mq::details;
    if (isInactiveState(_state)) {
        assert(isClosingState(_state));
        tDLogW("MqStreamServer::onServerConnection() Closing this server ... "
               "Deny the current node.");
        return;
    }

    if (_nodes.size() + 1 >= PARAMS.max_nodes) {
        tDLogW("MqStreamServer::onServerConnection() "
               "The connection is full ({})", PARAMS.max_nodes);
        return;
    }

    assert(server != nullptr);
    assert(isSuccess(code));

    Loop * loop = server->getLoop();
    assert(loop != nullptr);

    SharedStream stream;
    if (PARAMS.type == MqType::MT_PIPE) {
        stream = loop->newHandle<PipeNode>(*loop, this);
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        stream = loop->newHandle<TcpNode>(*loop, this);
    }
    assert(static_cast<bool>(stream));
    assert(stream->isInit());

    auto const ACCEPT_CODE = server->accept(*stream);
    assert(isSuccess(ACCEPT_CODE));

    if (PARAMS.type == MqType::MT_TCP && !PARAMS.accept_ip_regex.empty()) {
        auto * tcp = (TcpNode*)stream.get();
        auto const PEER_IP = tcp->getPeerIp();
        if (!std::regex_match(PEER_IP, std::regex(PARAMS.accept_ip_regex))) {
            tDLogI("MqStreamServer::onServerConnection() "
                   "Filter the current peer IP: {}", PEER_IP);
            stream->close();
            return;
        }
    }

    auto const START_CODE = stream->startRead();
    assert(isSuccess(START_CODE));

    auto const INSERT_RESULT = _nodes.insert(StreamPointer(stream.get())).second;
    assert(INSERT_RESULT);

    if (PARAMS.type == MqType::MT_PIPE) {
        tDLogI("MqStreamServer::onServerConnection() Connection success: PIPE ({}/{})",
               _nodes.size(), PARAMS.max_nodes);
    } else if (PARAMS.type == MqType::MT_TCP) {
        tDLogI("MqStreamServer::onServerConnection() Connection success: TCP {} ({}/{})",
               ((TcpNode*)stream.get())->getPeerIp(),
               _nodes.size(), PARAMS.max_nodes);
    }
}

void MqStreamServer::onServerClose(Stream * server)
{
    _state = MqMachineState::MMS_CLOSED;
    tDLogI("MqStreamServer::onServerClose() Close this server!");
}

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

