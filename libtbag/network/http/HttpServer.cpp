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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

// ------------------------
// HttpNode implementation.
// ------------------------

HttpServer::HttpNode::HttpNode(Loop & loop, StreamType type, HttpServer * parent)
        : StreamNode(loop, type, parent), _upgrade(false), _closing(false)
{
    // EMPTY.
}

HttpServer::HttpNode::~HttpNode()
{
    // EMPTY.
}

Err HttpServer::HttpNode::writeWsFrame(ws::WsFrame const & frame)
{
    WsBuffer buffer;
    std::size_t const SIZE = frame.copyTo(buffer);
    if (SIZE == 0) {
        tDLogE("HttpServer::HttpNode::writeOrEnqueue() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return write((char const *)buffer.data(), buffer.size());
}

Err HttpServer::HttpNode::writeText(char const * buffer, std::size_t size, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    ws::WsFrame frame;
    frame.text(buffer, size, finish);
    return writeWsFrame(frame);
}

Err HttpServer::HttpNode::writeText(std::string const & text, bool continuation, bool finish)
{
    return writeText(text.c_str(), text.size(), continuation, finish);
}

Err HttpServer::HttpNode::writeBinary(char const * buffer, std::size_t size, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    ws::WsFrame frame;
    frame.binary(buffer, size, finish);
    return writeWsFrame(frame);
}

Err HttpServer::HttpNode::writeBinary(WsBuffer const & binary, bool continuation, bool finish)
{
    return writeBinary(binary.data(), binary.size(), continuation, finish);
}

Err HttpServer::HttpNode::writeHttpResponse(HttpResponse const & response)
{
    HttpResponse temp = response;
    temp.updateDefaultResponse();
    return writeString(temp.toResponseString());
}

Err HttpServer::HttpNode::writeString(std::string const & response)
{
    if (response.empty()) {
        return Err::E_ILLARGS;
    }
    return write(response.data(), response.size());
}

Err HttpServer::HttpNode::closeWebSocket(uint16_t status_code, std::string const & reason)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    Err const CLOSE_TIMER_CODE = startTimer(DEFAULT_CLOSING_TIMEOUT_MILLISECOND);
    _closing = true;

    if (isFailure(CLOSE_TIMER_CODE)) {
        tDLogE("HttpServer::HttpNode::closeWebSocket() Close timer error: {} -> Force closing!", getErrName(CLOSE_TIMER_CODE));
        close();
        return CLOSE_TIMER_CODE;
    }

    ws::WsFrame frame;
    frame.close(status_code, reason);
    return writeWsFrame(frame);
}

Err HttpServer::HttpNode::closeWebSocket(ws::WsStatusCode code)
{
    return closeWebSocket(getWsStatusCodeNumber(code), getWsStatusCodeReason(code));
}

void HttpServer::HttpNode::backWsFrame(Err code, ReadPacket const & packet)
{
    __on_read_only__.receiver.push(packet.buffer, packet.size);
    while (__on_read_only__.receiver.next()) {
        auto opcode = __on_read_only__.receiver.getOpCode();
        auto & payload = __on_read_only__.receiver.atPayload();

        auto * server = getHttpServerPtr();
        assert(server != nullptr);

        if (opcode == ws::WsOpCode::WSOC_TEXT_FRAME || opcode == ws::WsOpCode::WSOC_BINARY_FRAME) {
            server->onWsMessage(getWeakClient(), opcode, &payload[0], payload.size());

        } else if (opcode == ws::WsOpCode::WSOC_CONNECTION_CLOSE) {
            Err const WRITE_CLOSE_CODE = closeWebSocket(ws::WsStatusCode::WSSC_NORMAL_CLOSURE);
            if (isFailure(WRITE_CLOSE_CODE)) {
                tDLogE("HttpServer::HttpNode::backWsFrame() WebSocket close write {} error", getErrName(WRITE_CLOSE_CODE));
                close();
            }

        } else {
            tDLogW("HttpServer::HttpNode::backWsFrame() Unsupported opcode: {}", getWsOpCodeName(opcode));
        }
    }
}

void HttpServer::HttpNode::onShutdown(Err code)
{
    auto * server = getHttpServerPtr();
    assert(server != nullptr);
    server->onHttpShutdown(getWeakClient(), code);
}

void HttpServer::HttpNode::onWrite(Err code)
{
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

    HttpParser   & request  = __on_read_only__.parser;
    HttpResponse & response = __on_read_only__.builder;

    std::size_t exec_size = 0;
    request.execute(packet.buffer, packet.size, &exec_size);
    if (exec_size < packet.size) {
        tDLogW("HttpServer::HttpNode::onRead() exec_size({}) < packet_size({})", exec_size, packet.size);
    } else if (exec_size > packet.size) {
        tDLogW("HttpServer::HttpNode::onRead() exec_size({}) > packet_size({})", exec_size, packet.size);
    } else {
        assert(exec_size == packet.size);
    }

    if (request.isFinish() == false) {
        tDLogD("HttpServer::HttpNode::onRead() Not complete.");
        return;
    }

    // ------------------
    // WebSocket checker.
    // ------------------

    if (server->isEnableWebSocket() && request.isUpgrade()) {
        // WebSocket interrupt process (HTTP Request).
        if (request.checkWsRequest()) {
            response.updateDefaultWsResponse(request);
            //updateResponseWebSocket(request, response)
            tDLogI("HttpServer::HttpNode::onClientRead() Request WebSocket header.");
            _upgrade = true;

            HttpPacket hp(request, response);
            server->onWsOpen(getWeakClient(), code, hp);

            Err const WRITE_CODE = writeHttpResponse(response);
            if (WRITE_CODE != Err::E_SUCCESS) {
                tDLogW("HttpServer::HttpNode::onRead() WebSocket response write {} error.", getErrName(WRITE_CODE));
            }

            request.clear();
            response.code = 0;
            response.reason.clear();
            response.body.clear();
            response.clearHeaders();
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
    response.code = 0;
    response.reason.clear();
    response.body.clear();
    response.clearHeaders();
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

Err HttpServer::writeText(WeakClient & node, char const * buffer, std::size_t size, bool continuation, bool finish)
{
    if (auto shared = castSharedClient<HttpNode>(node)) {
        return shared->writeText(buffer, size, continuation, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeText(WeakClient & node, std::string const & text, bool continuation, bool finish)
{
    return writeText(node, text.c_str(), text.size(), continuation, finish);
}

Err HttpServer::writeBinary(WeakClient & node, char const * buffer, std::size_t size, bool continuation, bool finish)
{
    if (auto shared = castSharedClient<HttpNode>(node)) {
        return shared->writeBinary(buffer, size, continuation, finish);
    }
    return Err::E_EXPIRED;
}

Err HttpServer::writeBinary(WeakClient & node, WsBuffer const & buffer, bool continuation, bool finish)
{
    return writeBinary(node, buffer.data(), buffer.size(), continuation, finish);
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
                                                      SharedStream & server)
{
    assert(static_cast<bool>(server));
    tDLogSD("HttpServer::createClient() Create HttpNode");
    return SharedStreamNode(new (std::nothrow) HttpNode(loop, type, this));
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

