/**
 * @file   HttpServer.cpp
 * @brief  HttpServer class implementation.
 * @author zer0
 * @date   2017-05-22
 */

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>
#include "HttpServer.hpp"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// ------------------------
// HttpNode implementation.
// ------------------------

HttpServer::HttpNode::HttpNode(Loop & loop, StreamType type, SharedSafetyAsync async, HttpServer * parent)
        : StreamNode(loop, type, async, parent), _upgrade(false), _closing(false)
{
    // EMPTY.
}

HttpServer::HttpNode::~HttpNode()
{
    // EMPTY.
}

Err HttpServer::HttpNode::writeOrEnqueue(char const * buffer, std::size_t size)
{
    Guard const LOCK_GUARD(_queue_mutex);
    if (_queue.empty() && getWriteState() == WriteState::WS_READY) {
        return write(buffer, size);
    }
    _queue.push().assign(buffer, buffer + size);
    return Err::E_ENQASYNC;
}

Err HttpServer::HttpNode::writeOrEnqueue(WsFrame const & frame)
{
    WsBuffer buffer;
    std::size_t const SIZE = frame.copyTo(buffer);
    if (SIZE == 0) {
        tDLogE("HttpServer::HttpNode::writeOrEnqueue() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return writeOrEnqueue((char const *)buffer.data(), buffer.size());
}

Err HttpServer::HttpNode::writeFromQueue()
{
    Guard const LOCK_GUARD(_queue_mutex);
    if (_queue.empty() || getWriteState() != WriteState::WS_READY) {
        return Err::E_ILLSTATE;
    }

    auto & buffer = _queue.frontRef();
    Err const WRITE_CODE = write((char const *)buffer.data(), buffer.size());
    _queue.pop();

    if (TBAG_ERR_FAILURE(WRITE_CODE)) {
        tDLogE("HttpServer::HttpNode::writeFromQueue() write {} error", getErrName(WRITE_CODE));
    }
    return WRITE_CODE;
}

Err HttpServer::HttpNode::writeText(std::string const & text, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    WsFrame frame;
    Err const CODE = frame.text(text, continuation, finish);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("HttpServer::HttpNode::writeText() WsFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeOrEnqueue(frame);
}

Err HttpServer::HttpNode::writeBinary(WsBuffer const & binary, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    WsFrame frame;
    Err const CODE = frame.binary(binary, continuation, finish);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("HttpServer::HttpNode::writeBinary() WsFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeOrEnqueue(frame);
}

Err HttpServer::HttpNode::writeHttpResponse(HttpBuilder const & response)
{
    return writeString(response.buildDefaultResponseString());
}

Err HttpServer::HttpNode::writeString(std::string const & response)
{
    if (response.empty()) {
        return Err::E_ILLARGS;
    }
    return writeOrEnqueue(response.data(), response.size());
}

Err HttpServer::HttpNode::closeWebSocket(uint16_t status_code, std::string const & reason)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    Err const CLOSE_TIMER_CODE = startTimer(DEFAULT_CLOSING_TIMEOUT_MILLISECOND);
    _closing = true;

    if (TBAG_ERR_FAILURE(CLOSE_TIMER_CODE)) {
        tDLogE("HttpServer::HttpNode::closeWebSocket() Close timer error: {} -> Force closing!", getErrName(CLOSE_TIMER_CODE));
        close();
        return CLOSE_TIMER_CODE;
    }

    WsFrame frame;
    Err const CODE = frame.close(status_code, reason);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("HttpServer::HttpNode::closeWebSocket() WsFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeOrEnqueue(frame);
}

Err HttpServer::HttpNode::closeWebSocket(WsStatusCode code)
{
    return closeWebSocket(getWsStatusCodeNumber(code), getWsStatusCodeReason(code));
}

void HttpServer::HttpNode::backWsFrame(Err code, ReadPacket const & packet)
{
    __on_read_only__.receiver.exec(packet.buffer, packet.size, [&](OpCode opcode, bool finish, WsFrameBuffer::Buffer & buffer) -> bool {
        if (finish) {
            auto * server = getHttpServerPtr();
            assert(server != nullptr);

            if (opcode == OpCode::OC_TEXT_FRAME || opcode == OpCode::OC_BINARY_FRAME) {
                server->onWsMessage(getWeakClient(), opcode, &buffer[0], buffer.size());

            } else if (opcode == OpCode::OC_CONNECTION_CLOSE) {
                Err const WRITE_CLOSE_CODE = closeWebSocket(WsStatusCode::WSSC_NORMAL_CLOSURE);
                if (TBAG_ERR_FAILURE(WRITE_CLOSE_CODE)) {
                    tDLogE("HttpServer::HttpNode::backWsFrame() WebSocket close write {} error", getErrName(WRITE_CLOSE_CODE));
                    close();
                }

            } else {
                tDLogW("HttpServer::HttpNode::backWsFrame() Unsupported opcode: {}", getOpCodeName(opcode));
            }
        }
        return true;
    });
}

void HttpServer::HttpNode::onShutdown(Err code)
{
    writeFromQueue();

    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    server->onHttpShutdown(getWeakClient(), code);
}

void HttpServer::HttpNode::onWrite(Err code)
{
    writeFromQueue();

    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    server->onHttpWrite(getWeakClient(), code);
}

void HttpServer::HttpNode::onRead(Err code, ReadPacket const & packet)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);

    if (code == Err::E_EOF) {
        tDLogD("HttpServer::HttpNode::onRead() EOF.");
        close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::HttpNode::onRead() {} error", getErrName(code));
        close();
        return;
    }

    // ------------------
    // WebSocket process.
    // ------------------

    if (server->isEnableWebSocket() && isUpgrade()) {
        backWsFrame(code, packet);
        return;
    }

    // -------------
    // HTTP request.
    // -------------

    HttpParser  & request  = __on_read_only__.parser;
    HttpBuilder & response = __on_read_only__.builder;

    std::size_t const EXEC_SIZE = request.execute(packet.buffer, packet.size);
    if (EXEC_SIZE < packet.size) {
        tDLogW("HttpServer::HttpNode::onRead() exec_size({}) < packet_size({})", EXEC_SIZE, packet.size);
    } else if (EXEC_SIZE > packet.size) {
        tDLogW("HttpServer::HttpNode::onRead() exec_size({}) > packet_size({})", EXEC_SIZE, packet.size);
    } else {
        assert(EXEC_SIZE == packet.size);
    }

    if (request.isComplete() == false) {
        tDLogD("HttpServer::HttpNode::onRead() Not complete.");
        return;
    }

    // ------------------
    // WebSocket checker.
    // ------------------

    if (server->isEnableWebSocket() && request.isUpgrade()) {
        // WebSocket interrupt process (HTTP Request).
        if (updateResponseWebSocket(request, response) == Err::E_SUCCESS) {
            tDLogI("HttpServer::HttpNode::onClientRead() Request WebSocket header.");
            _upgrade = true;

            HttpPacket hp(request, response);
            server->onWsOpen(getWeakClient(), code, hp);

            Err const WRITE_CODE = writeHttpResponse(response);
            if (WRITE_CODE != Err::E_SUCCESS) {
                tDLogW("HttpServer::HttpNode::onRead() WebSocket response write {} error.", getErrName(WRITE_CODE));
            }

            request.clear();
            request.clearCache();
            response.clear();
            return;
        } else {
            tDLogW("HttpServer::HttpNode::onRead() Unknown WebSocket request. Switches to the regular HTTP protocol.");
        }
    }

    // ---------------------
    // Regular HTTP process.
    // ---------------------

    HttpPacket hp(request, response);
    server->findAndExecuteTheReadCallback(getWeakClient(), code, hp);
    Err const WRITE_CODE = writeHttpResponse(response);
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogW("HttpServer::HttpNode::onRead() Write {} error.", getErrName(WRITE_CODE));
    }

    request.clear();
    request.clearCache();
    response.clear();
}

void HttpServer::HttpNode::onTimer()
{
    if (isClosing()) {
        close();
        return;
    }

    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    server->onHttpTimer(getWeakClient());
}

// --------------------------
// HttpServer implementation.
// --------------------------

HttpServer::HttpServer(Loop & loop, StreamType type) : Parent(loop, type), _websocket(false)
{
    // EMPTY.
}

HttpServer::~HttpServer()
{
    // EMPTY.
}

void HttpServer::setRequest(std::string const & method, std::string const & regex_path, OnRequest const & cb, Order priority)
{
    setRequest(new (std::nothrow) HttpDefaultFilter(method, regex_path), cb);
}

void HttpServer::setRequest(std::string const & regex_path, OnRequest const & cb, Order priority)
{
    setRequest(new (std::nothrow) HttpDefaultFilter(regex_path), cb);
}

void HttpServer::setRequest(HttpFilterInterface * filter, OnRequest const & cb, Order priority)
{
    setRequest(SharedFilter(new (std::nothrow) Filter(filter, cb)), priority);
}

void HttpServer::setRequest(SharedFilter filter, Order priority)
{
    Guard LOCK_GUARD(_filters_mutex);
    _filters.insert(FilterPair(priority, filter));
}

Err HttpServer::writeText(WeakClient & node, std::string const & text, bool continuation, bool finish)
{
    if (auto shared = castSharedClient<HttpNode>(node)) {
        return shared->writeText(text, continuation, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeBinary(WeakClient & node, WsBuffer const & buffer, bool continuation, bool finish)
{
    if (auto shared = castSharedClient<HttpNode>(node)) {
        return shared->writeBinary(buffer, continuation, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::closeClient(WeakClient & node, uint16_t status_code, std::string const & reason)
{
    if (auto shared = castSharedClient<HttpNode>(node)) {
        return shared->closeWebSocket(status_code, reason);
    }
    return Err::E_EXPIRED;
}

bool HttpServer::isUpgradeWebSocket(SharedHttpNode const & node) const TBAG_NOEXCEPT
{
    if (static_cast<bool>(node)) {
        return _websocket && node->isUpgrade();
    }
    return false;
}

bool HttpServer::isUpgradeWebSocket(WeakClient const & node) const TBAG_NOEXCEPT
{
    return isUpgradeWebSocket(castSharedClient<HttpNode>(node));
}

HttpServer::SharedFilter HttpServer::findFilter(HttpParser const & request)
{
    Guard LOCK_GUARD(_filters_mutex);
    for (auto & cursor : _filters) {
        Order order = cursor.first;
        SharedFilter filter = cursor.second;

        if (static_cast<bool>(filter) == false || static_cast<bool>(filter->http_filter) == false) {
            continue;
        }
        if (filter->http_filter->filter(request) == false) {
            continue;
        }

        if (static_cast<bool>(filter)) {
            return filter;
        }
    }
    return SharedFilter();
}

void HttpServer::findAndExecuteTheReadCallback(WeakClient node, Err code, HttpPacket & packet)
{
    if (auto filter = findFilter(packet.request)) {
        filter->request_cb(node, code, packet);
    } else {
        onHttpRequest(node, code, packet); // Default request callback.
    }
}

// ---------------
// Event handling.
// ---------------

void HttpServer::onConnection(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onConnection() {} error.", getErrName(code));
        return;
    }

    auto node = accept();
    auto shared = node.lock();

    if (static_cast<bool>(shared) == false) {
        tDLogE("HttpServer::onConnection() Expired client.");
        return;
    }

    if (shared->start() != Err::E_SUCCESS) {
        tDLogE("HttpServer::onConnection() Start client error.");
        shared->close();
        return;
    }

    onHttpOpen(node);
}

void HttpServer::onClientShutdown(WeakClient node, Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void HttpServer::onClientWrite(WeakClient node, Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void HttpServer::onClientRead(WeakClient node, Err code, ReadPacket const & packet)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

void HttpServer::onClientClose(WeakClient node)
{
    onHttpClose(node);
}

void HttpServer::onClientTimer(WeakClient node)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
}

HttpServer::SharedStreamNode HttpServer::createClient(StreamType type,
                                                      Loop & loop,
                                                      SharedServerBackend & server,
                                                      SharedSafetyAsync & async)
{
    assert(static_cast<bool>(server));
    assert(static_cast<bool>(async));
    tDLogSD("HttpServer::createClient() Create HttpNode");
    return SharedStreamNode(new (std::nothrow) HttpNode(loop, type, async, this));
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

