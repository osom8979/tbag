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

MqStreamServer::MqStreamServer(Loop & loop, MqInternal const & internal, MqParams const & params)
        : MqBase(loop, internal, params, MqMachineState::MMS_NONE),
          _packer(params.packer_size)
{
    if (params.type != MqType::MT_PIPE && params.type != MqType::MT_TCP) {
        tDLogE("MqStreamServer::MqStreamServer() Unsupported type: {}({})",
               libtbag::mq::details::getTypeName(params.type), (int)(params.type));
        throw ErrException(Err::E_ILLARGS);
    }

    onInitStep1_ASYNC(loop);
}

MqStreamServer::~MqStreamServer()
{
    // EMPTY.
}

void MqStreamServer::onInitializerAsync(Initializer * init)
{
    assert(init != nullptr);
    auto * loop = init->getLoop();
    assert(loop != nullptr);
    init->close();
    onInitStep2_INIT(*loop);
}

void MqStreamServer::onInitializerClose(Initializer * init)
{
    assert(init != nullptr);
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onInitializerClose() Close initializer.");
}

void MqStreamServer::onCloseMsgDone()
{
    onCloseStep2_EVENT_QUEUE_CLOSED();
}

AfterAction MqStreamServer::onMsg(AsyncMsg * msg)
{
    assert(msg != nullptr);

    using namespace libtbag::mq::details;
    if (!isActiveState(_state) && !isClosingState(_state)) {
        tDLogIfW(PARAMS.verbose, "MqStreamServer::onMsg() Illegal client state({}), skip current message ({})",
                 getMachineStateName(_state), getEventName(msg->event));
        return AfterAction::AA_OK;
    }

    if (msg->event == ME_CLOSE) {
        tDLogI("MqStreamServer::onMsg() Close message confirmed.");
        onTearDownStep1(true);
        return AfterAction::AA_OK;
    } else {
        onSendStep1_EVENT(msg);
        return AfterAction::AA_DELAY;
    }
}

void MqStreamServer::onWriterAsync(Writer * writer)
{
    assert(writer != nullptr);
    assert(writer == _writer.get());
    onSendStep2_ASYNC();
}

void MqStreamServer::onWriterClose(Writer * writer)
{
    assert(writer != nullptr);
    onCloseStep3_WRITER_CLOSED();
}

void MqStreamServer::onCloseTimer(CloseTimer * timer)
{
    assert(timer != nullptr);
    timer->close();

    onTearDownStep3_TIMEOUT();
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
        tDLogIfW(PARAMS.verbose, "MqStreamServer::onNodeShutdown() Shutdown error: {}", code);
    }
}

void MqStreamServer::onNodeWrite(Stream * node, WriteRequest & request, Err code)
{
    onSendStep4_WRITE_DONE(code);
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
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Read success. {}byte", size);

    // Give users the opportunity to use the original data.
    if (INTERNAL.default_read != nullptr) {
        assert(INTERNAL.parent != nullptr);
        INTERNAL.default_read(node, buffer, size, INTERNAL.parent);
        return;
    }

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

        assert(INTERNAL.recv_cb != nullptr);
        assert(INTERNAL.parent != nullptr);

        if (INTERNAL.recv_cb(_packer.msg(), INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
            tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Consumed this received message.");
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
                         "MqStreamServer::onNodeRead() Enqueue success. "
                         "Perhaps the remaining queue size is {}.",
                         _receives.getInaccurateSizeOfActive());
            } else {
                tDLogE("MqStreamServer::onNodeRead() Enqueue error: {}", enqueue_code);
                break;
            }
        }
    }

    // [DONE] All processes succeeded.
    if (computed_total >= 1) {
        auto begin = remaining_read->begin();
        remaining_read->erase(begin, begin + computed_total);
    }

    tDLogIfD(PARAMS.verbose, "MqStreamServer::onNodeRead() Remaining size: {}", remaining_read->size());
}

void MqStreamServer::onNodeClose(Stream * node)
{
    auto const ERASE_RESULT = _nodes.erase(StreamPointer(node));
    assert(ERASE_RESULT == 1);

    tDLogI("MqStreamServer::onNodeClose() Close node: ({}/{})", _nodes.size(), PARAMS.max_nodes);

    if (INTERNAL.close_node != nullptr) {
        assert(INTERNAL.parent != nullptr);
        INTERNAL.close_node(node, INTERNAL.parent);
    }
}

void MqStreamServer::onServerConnection(Stream * server, Err code)
{
    assert(server != nullptr);
    assert(server == _server.get());

    if (isFailure(code)) {
        tDLogE("MqStreamServer::onServerConnection() Connection error: {}", code);
        return;
    }

    if (libtbag::mq::details::isInactiveState(_state)) {
        tDLogW("MqStreamServer::onServerConnection() Inactive this server. Deny the current node.");
        return;
    }

    if (_nodes.size() + 1 >= PARAMS.max_nodes) {
        tDLogW("MqStreamServer::onServerConnection() The connection is full: {}", PARAMS.max_nodes);
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

    auto const INSERT_RESULT = _nodes.insert(StreamPointer(stream.get())).second;
    assert(INSERT_RESULT);

    auto const ACCEPT_CODE = server->accept(*stream);
    assert(isSuccess(ACCEPT_CODE));

    std::string peer;
    if (PARAMS.type == MqType::MT_PIPE) {
        auto * pipe = (PipeNode*)stream.get();
        peer = pipe->getPeerName();
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        auto * tcp = (TcpNode*)stream.get();
        peer = tcp->getPeerIp();
    }

    assert(INTERNAL.accept_cb != nullptr);
    assert(INTERNAL.parent != nullptr);

    if (!INTERNAL.accept_cb(stream.get(), peer, INTERNAL.parent)) {
        tDLogI("MqStreamServer::onServerConnection() Filter the current peer: {}", peer);
        stream->close();
        return;
    }

    auto const START_CODE = stream->startRead();
    assert(isSuccess(START_CODE));

    if (PARAMS.type == MqType::MT_PIPE) {
        tDLogI("MqStreamServer::onServerConnection() Connection success: PIPE ({}/{})",
               _nodes.size(), PARAMS.max_nodes);
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        tDLogI("MqStreamServer::onServerConnection() Connection success: TCP {} ({}/{})",
               peer, _nodes.size(), PARAMS.max_nodes);
    }
}

void MqStreamServer::onServerClose(Stream * server)
{
    onCloseStep4_CLIENT_CLOSED();
}

// ---------------------
// Alias of event steps.
// ---------------------

void MqStreamServer::onInitStep1_ASYNC(Loop & loop)
{
    assert(!static_cast<bool>(_server));
    assert(!static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_NONE || _state == MqMachineState::MMS_CLOSED);

    auto init = loop.newHandle<Initializer>(loop, this);
    assert(static_cast<bool>(init));
    assert(init->isInit());

    auto const CODE = init->send();
    assert(isSuccess(CODE));
    _state = MqMachineState::MMS_INITIALIZING;
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onInitStep1_ASYNC() Asynchronous initialization request.");
}

void MqStreamServer::onInitStep2_INIT(Loop & loop)
{
    assert(!static_cast<bool>(_server));
    assert(!static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_INITIALIZING);

    if (PARAMS.type == MqType::MT_PIPE) {
        _server = loop.newHandle<PipeServer>(loop, this);
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        _server = loop.newHandle<TcpServer>(loop, this);
    }

    assert(static_cast<bool>(_server));
    assert(_server->isInit());

    // Remove exists named pipe.
    if (PARAMS.type == MqType::MT_PIPE && libtbag::filesystem::Path(PARAMS.address).exists()) {
        if (PARAMS.remove_exists_pipe) {
            if (libtbag::filesystem::details::unlink(PARAMS.address)) {
                tDLogI("MqStreamServer::onInitStep2_INIT() Remove named pipe: {}", PARAMS.address);
            } else {
                tDLogE("MqStreamServer::onInitStep2_INIT() Failed to remove named pipe: {}", PARAMS.address);
                onInit_FAILURE(Err::E_UNKNOWN);
                return;
            }
        } else {
            tDLogE("MqStreamServer::onInitStep2_INIT() Exists named pipe: {}", PARAMS.address);
            onInit_FAILURE(Err::E_EEXIST);
            return;
        }
    }

    if (PARAMS.type == MqType::MT_PIPE) {
        auto * pipe = (PipeServer*)(_server.get());
        assert(pipe != nullptr);
        auto const BIND_CODE = pipe->bind(PARAMS.address.c_str());
        if (isFailure(BIND_CODE)) {
            tDLogE("MqStreamServer::onInitStep2_INIT() Pipe bind error: {}", BIND_CODE);
            onInit_FAILURE(BIND_CODE);
            return;
        }

        auto const LISTEN_CODE = pipe->listen();
        if (isFailure(LISTEN_CODE)) {
            tDLogE("MqStreamServer::onInitStep2_INIT() Pipe listen error: {}", LISTEN_CODE);
            onInit_FAILURE(LISTEN_CODE);
            return;
        }
    } else {
        assert(PARAMS.type == MqType::MT_TCP);
        auto * tcp = (TcpServer*)(_server.get());

        SocketAddress addr;
        auto const INIT_CODE = addr.init(PARAMS.address, PARAMS.port);
        if (isFailure(INIT_CODE)) {
            tDLogE("MqStreamServer::onInitStep2_INIT() Socket init error: {}", INIT_CODE);
            onInit_FAILURE(INIT_CODE);
            return;
        }

        Tcp::BindFlag flags;
        if (PARAMS.tcp_ipv6_only) {
            flags = Tcp::BindFlag::BF_IPV6_ONLY;
        } else {
            flags = Tcp::BindFlag::BF_NONE;
        }

        auto const BIND_CODE = tcp->bind(addr.getCommon(), flags);
        if (isFailure(BIND_CODE)) {
            tDLogE("MqStreamServer::onInitStep2_INIT() Socket bind error: {}", BIND_CODE);
            onInit_FAILURE(BIND_CODE);
            return;
        }

        auto const LISTEN_CODE = tcp->listen();
        if (isFailure(LISTEN_CODE)) {
            tDLogE("MqStreamServer::onInitStep2_INIT() Socket listen error: {}", LISTEN_CODE);
            onInit_FAILURE(LISTEN_CODE);
            return;
        }
    }

    _state = MqMachineState::MMS_INITIALIZED;
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onInitStep2_INIT() Initialization succeeded.");

    onInit_SUCCESS();
}

void MqStreamServer::onInit_SUCCESS()
{
    assert(static_cast<bool>(_server));
    assert(!static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_INITIALIZED);

    auto * loop = _server->getLoop();
    assert(loop != nullptr);

    _writer = loop->newHandle<Writer>(*loop, this);
    assert(static_cast<bool>(_writer));
    assert(_writer->isInit());

    tDLogI("MqStreamServer::onInit_SUCCESS() Active state!");
    _state = MqMachineState::MMS_ACTIVE;
    MqBase::enableWait();

    if (INTERNAL.connect_cb != nullptr) {
        assert(INTERNAL.parent);
        INTERNAL.connect_cb(INTERNAL.parent);
    }
}

void MqStreamServer::onInit_FAILURE(Err code)
{
    assert(static_cast<bool>(_server));
    assert(!static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_INITIALIZED);

    _state = MqMachineState::MMS_CLOSING;
    _server->close();
    _server.reset();
}

void MqStreamServer::onSendStep1_EVENT(AsyncMsg * msg)
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_ACTIVE || _state == MqMachineState::MMS_CLOSING);
    assert(msg != nullptr);

    _writer->queue.push(AsyncMsgPointer(msg));

    if (_writer->state == MqRequestState::MRS_WAITING) {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
    } else {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onSendStep1_EVENT() Writer is already working: {}",
                 _writer->queue.size());
    }
}

void MqStreamServer::onSendStep2_ASYNC()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_ACTIVE || _state == MqMachineState::MMS_CLOSING);
    assert(_writer->state == MqRequestState::MRS_ASYNC);
    assert(!_writer->queue.empty());

    _writer->state = MqRequestState::MRS_REQUESTING;
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onSendStep2_ASYNC() Async write: {}", _writer->queue.size());

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

void MqStreamServer::onSendStep3_ASYNC_ORIGINAL(AsyncMsg * msg)
{
    assert(msg != nullptr);
    assert(INTERNAL.default_write != nullptr);
    assert(INTERNAL.parent != nullptr);

    Stream * destination_node = nullptr;
    if (msg->stream != 0) {
        if (_nodes.find(StreamPointer(reinterpret_cast<Stream*>(msg->stream))) == _nodes.end()) {
            tDLogE("MqStreamServer::onSendStep3_ASYNC_ORIGINAL() Not found node, skip this message.");
            onSendStep5_NEXT_MESSAGE(msg);
            return;
        }
        destination_node = reinterpret_cast<Stream*>(msg->stream);
    }
    assert(destination_node != nullptr);

    _writer->write_count = INTERNAL.default_write(destination_node, msg->event, msg->data(), msg->size(), INTERNAL.parent);
    if (_writer->write_count >= 1) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onSendStep3_ASYNC_ORIGINAL() Default write process. Wait for the next step.");
    } else {
        assert(_writer->write_count == 0);
        tDLogIfW(PARAMS.verbose, "MqStreamServer::onSendStep3_ASYNC_ORIGINAL() Default write error, Skip this message.");
        onSendStep5_NEXT_MESSAGE(msg);
    }
}

template <typename NodeT>
static Err __write_node(Stream * stream, uint8_t const * data, std::size_t size) TBAG_NOEXCEPT
{
    auto * node = (NodeT*)stream;
    return node->write(node->write_req, (char const *)data, size);
}

static Err __write_node(Stream * stream, uint8_t const * data, std::size_t size, MqType type) TBAG_NOEXCEPT
{
    if (type == MqType::MT_PIPE) {
        return __write_node<PipeNode>(stream, data, size);
    } else {
        assert(type == MqType::MT_TCP);
        return __write_node<TcpNode>(stream, data, size);
    }
}

template <typename NodeT>
static std::size_t __write_all_nodes(NodeSet & nodes, uint8_t const * data, std::size_t size) TBAG_NOEXCEPT
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

static std::size_t __write_all_nodes(NodeSet & nodes, uint8_t const * data, std::size_t size, MqType type) TBAG_NOEXCEPT
{
    if (type == MqType::MT_PIPE) {
        return __write_all_nodes<PipeNode>(nodes, data, size);
    } else {
        return __write_all_nodes<TcpNode>(nodes, data, size);
    }
}

void MqStreamServer::onSendStep3_ASYNC_MQMSG(AsyncMsg * msg)
{
    assert(msg != nullptr);
    assert(INTERNAL.write_cb != nullptr);
    assert(INTERNAL.parent != nullptr);

    if (msg->empty()) {
        tDLogE("MqStreamServer::onSendStep3_ASYNC_MQMSG() Empty message buffer, skip this message.");
        onSendStep5_NEXT_MESSAGE(msg);
        return;
    }

    auto const CODE = _packer.build(*msg);
    assert(isSuccess(CODE));

    // Give the user a chance to filter the message.
    if (INTERNAL.write_cb(*msg, INTERNAL.parent) == MqIsConsume::MIC_CONSUMED) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onSendStep3_ASYNC_MQMSG() The message to be sent has been filtered.");
        onSendStep5_NEXT_MESSAGE(msg);
        return;
    }

    if (_nodes.empty()) {
        tDLogIfW(PARAMS.verbose, "MqStreamServer::onSendStep3_ASYNC_MQMSG() The transport node does not exist.");
        onSendStep5_NEXT_MESSAGE(msg);
        return;
    }

    if (msg->stream) {
        // If a stream is specified, write it to the single node.
        auto itr = _nodes.find(StreamPointer(reinterpret_cast<Stream*>(msg->stream)));
        if (itr == _nodes.end()) {
            tDLogIfE(PARAMS.verbose, "MqStreamServer::onSendStep3_ASYNC_MQMSG() Not found node: {}", msg->stream);
            onSendStep5_NEXT_MESSAGE(msg);
            return;
        }

        auto node = *itr;
        auto const CODE = __write_node(node.get(), _packer.point(), _packer.size(), PARAMS.type);
        if (isSuccess(CODE)) {
            _writer->write_count = 1;
        } else {
            _writer->write_count = 0;
        }
    } else {
        assert(msg->stream == 0);
        _writer->write_count = __write_all_nodes(_nodes, _packer.point(), _packer.size(), PARAMS.type);
    }

    auto const NODE_SIZE = _nodes.size();
    assert(NODE_SIZE >= 1);

    if (_writer->write_count == NODE_SIZE) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onSendStep3_ASYNC_MQMSG() Write {} process {}byte ... "
                                 "Wait for the next step.", _writer->write_count, _packer.size());
    } else if (_writer->write_count >= 1) {
        tDLogW("MqStreamServer::onSendStep3_ASYNC_MQMSG() Some write commands failed ({}/{}) {}byte ... "
               "Wait for the next step.", _writer->write_count, NODE_SIZE, _packer.size());
    } else {
        assert(_writer->write_count == 0);
        tDLogE("MqStreamServer::onSendStep3_ASYNC_MQMSG() Write error: {} nodes", NODE_SIZE);
        onSendStep5_NEXT_MESSAGE(msg);
    }
}

void MqStreamServer::onSendStep4_WRITE_DONE(Err code)
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_ACTIVE || _state == MqMachineState::MMS_CLOSING);
    assert(!_writer->queue.empty());
    assert(_writer->state == MqRequestState::MRS_REQUESTING);

    if (_writer->write_count >= 1) {
        _writer->write_count--;
    }

    if (isSuccess(code)) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onSendStep4_WRITE_DONE() Write success. Remaining write size: {}",
                 _writer->write_count);
    } else {
        tDLogE("MqStreamServer::onSendStep4_WRITE_DONE() Write error({}). Remaining write size: {}",
               code, _writer->write_count);
    }

    if (_writer->write_count == 0) {
        onSendStep5_NEXT_MESSAGE(_writer->queue.front().get());
    }
}

void MqStreamServer::onSendStep5_NEXT_MESSAGE(AsyncMsg * msg)
{
    assert(msg != nullptr);

    _writer->queue.pop();
    auto const CODE = MqEventQueue::restoreMessage(msg, PARAMS.verify_restore_message);
    assert(isSuccess(CODE));

    if (!_writer->queue.empty()) {
        auto const CODE = _writer->send();
        assert(isSuccess(CODE));
        _writer->state = MqRequestState::MRS_ASYNC;
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onSendStep5_NEXT_MESSAGE() Async next message (queue:{}) ...",
                 _writer->queue.size());
        return;
    }

    assert(_writer->queue.empty());
    _writer->state = MqRequestState::MRS_WAITING;

    // If the shutdown is delayed, proceed with it.
    if (_state == MqMachineState::MMS_CLOSING) {
        tDLogIfN(PARAMS.verbose, "MqStreamServer::onSendStep5_NEXT_MESSAGE() Delayed shutdown progresses.");
        onTearDownStep2_SHUTDOWN();
    } else {
        tDLogIfI(PARAMS.verbose, "MqStreamServer::onSendStep5_NEXT_MESSAGE() Waiting for messages.");
    }
}

void MqStreamServer::onTearDownStep1(bool from_message_event)
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_writer));

    using namespace libtbag::mq::details;
    if (isInactiveState(_state)) {
        // After acquiring the 'Close Message', the second call can be made to the 'Read Event'.
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onTearDownStep1() It is already closing.");
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
    if (from_message_event) {
        assert(active_send_size >= 1);
        // Subtract the current message.
        --active_send_size;
    }

    if (active_send_size > 0 || _writer->state != MqRequestState::MRS_WAITING) {
        // Clear any remaining transmission queues and continue with system shutdown.
        tDLogI("MqStreamServer::onTearDownStep1() Delay the shutdown request ...");
        return;
    }

    assert(_writer->queue.empty());
    onTearDownStep2_SHUTDOWN();
}

template <typename NodeT>
static Err __shutdown(Stream * stream) TBAG_NOEXCEPT
{
    auto * node = (NodeT*)(stream);
    return node->shutdown(node->shutdown_req);
}

static Err __shutdown(MqType type, Stream * stream) TBAG_NOEXCEPT
{
    if (type == MqType::MT_PIPE) {
        return __shutdown<PipeNode>(stream);
    } else {
        return __shutdown<TcpNode>(stream);
    }
}

static std::size_t __shutdown(MqType type, NodeSet & nodes) TBAG_NOEXCEPT
{
    std::size_t success_count = 0;
    for (auto node : nodes) {
        if (isSuccess(__shutdown(type, node.get()))) {
            ++success_count;
        }
    }
    return success_count;
}

void MqStreamServer::onTearDownStep2_SHUTDOWN()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(_writer->queue.empty());

    if (_nodes.empty()) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onTearDownStep2_SHUTDOWN() Not exists nodes. Skip the shutdown step.");
        onCloseStep1();
        return;
    }

    auto const NODE_SIZE = _nodes.size();
    assert(NODE_SIZE >= 1);

    std::size_t const SHUTDOWN_COUNT = __shutdown(PARAMS.type, _nodes);
    if (SHUTDOWN_COUNT == NODE_SIZE) {
        tDLogIfD(PARAMS.verbose, "MqStreamServer::onTearDownStep2_SHUTDOWN() Shutdown all nodes: {}", NODE_SIZE);
    } else if (SHUTDOWN_COUNT >= 1) {
        tDLogW("MqStreamServer::onTearDownStep2_SHUTDOWN() Some shutdown commands failed ({}/{}).",
               SHUTDOWN_COUNT, NODE_SIZE);
    } else {
        assert(SHUTDOWN_COUNT == 0);
        tDLogW("MqStreamServer::onTearDownStep2_SHUTDOWN() All shutdowns failed.");
    }

    auto * loop = _server->getLoop();
    assert(loop != nullptr);

    auto timer = loop->newHandle<CloseTimer>(*loop, this);
    assert(static_cast<bool>(timer));

    auto const START_CODE = timer->start(PARAMS.wait_closing_millisec);
    assert(isSuccess(START_CODE));
}

void MqStreamServer::onTearDownStep3_TIMEOUT()
{
    onCloseStep1();
}

void MqStreamServer::onCloseStep1()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(_writer->queue.empty());

    if (!_nodes.empty()) {
        for (auto node : _nodes) {
            if (!node->isClosing()) {
                node->close();
            }
        }
    }

    MqEventQueue::closeAsyncMsgs();
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onCloseStep1() Close request of all messages.");
}

void MqStreamServer::onCloseStep2_EVENT_QUEUE_CLOSED()
{
    assert(static_cast<bool>(_server));
    assert(static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(_writer->queue.empty());
    assert(!_writer->isClosing());

    _writer->close();
    _writer.reset();
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onCloseStep2_EVENT_QUEUE_CLOSED() Close request of writer.");
}

void MqStreamServer::onCloseStep3_WRITER_CLOSED()
{
    assert(static_cast<bool>(_server));
    assert(!static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_CLOSING);
    assert(!_server->isClosing());

    _server->close();
    _server.reset();
    tDLogIfD(PARAMS.verbose, "MqStreamServer::onCloseStep3_WRITER_CLOSED() Close request of server.");
}

void MqStreamServer::onCloseStep4_CLIENT_CLOSED()
{
    assert(!static_cast<bool>(_server));
    assert(!static_cast<bool>(_writer));
    assert(_state == MqMachineState::MMS_CLOSING);

    tDLogI("MqStreamServer::onCloseStep4_CLIENT_CLOSED() Close server!");
    _state = MqMachineState::MMS_CLOSED;
    MqBase::disableWait();

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

