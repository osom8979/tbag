/**
 * @file   WebSocketClient.cpp
 * @brief  WebSocketClient class implementation.
 * @author zer0
 * @date   2017-06-30
 */

#include <libtbag/network/http/WebSocketClient.hpp>
#include <libtbag/log/Log.hpp>
#include "HttpCacheData.hpp"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WebSocketClient::WebSocketClient(Loop & loop, StreamType type) : Parent(loop, type), _cache(this), _close()
{
    _cache.generateWebSocketKey();
}

WebSocketClient::~WebSocketClient()
{
    // EMPTY.
}

void WebSocketClient::setup(HttpBuilder const & request)
{
    _cache.builder = request;
}

Err WebSocketClient::writeText(std::string const & text, bool continuation, bool finish)
{
    if (_cache.isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }
    return _cache.writeTextRequest(text, continuation, finish);
}

Err WebSocketClient::writeBinary(WsBuffer const & binary, bool continuation, bool finish)
{
    if (_cache.isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }
    return _cache.writeBinaryRequest(binary, continuation, finish);
}

Err WebSocketClient::writeClose()
{
    if (_cache.isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    Err const CLOSE_TIMER_CODE = startTimer(DEFAULT_CLOSING_TIMEOUT_MILLISECOND);
    _cache.ws.closing = true;
    if (TBAG_ERR_FAILURE(CLOSE_TIMER_CODE)) {
        tDLogE("WebSocketClient::writeClose() close timer error: {} -> Force closing!", getErrName(CLOSE_TIMER_CODE));
        _close.set(WebSocketStatusCode::WSSC_CLIENT_TIMER_ERROR);
        close();
    }
    return _cache.writeCloseRequest();
}

bool WebSocketClient::runWebSocketChecker(HttpParser const & response)
{
    if (response.getStatusCode() != getHttpStatusNumber(HttpStatus::SC_SWITCHING_PROTOCOLS)) {
        tDLogE("WebSocketClient::runWebSocketChecker() Not 101 status code error: {}", response.getStatusCode());
        return false;
    }

    if (response.existsHeaderValue(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        tDLogE("WebSocketClient::runWebSocketChecker() Not found upgrade connection.");
        return false;
    }

    if (response.existsHeaderValue(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        tDLogE("WebSocketClient::runWebSocketChecker() Not found websocket upgrade.");
        return false;
    }

    if (response.existsHeader(HEADER_SEC_WEBSOCKET_ACCEPT) == false) {
        tDLogE("WebSocketClient::runWebSocketChecker() Not found Sec-WebSocket-Accept header.");
        return false;
    }

    std::string const ACCEPT_KEY = response.getHeader(HEADER_SEC_WEBSOCKET_ACCEPT);
    if (ACCEPT_KEY != getUpgradeWebSocketKey(_cache.getKey())) {
        tDLogE("WebSocketClient::runWebSocketChecker() Accept key error: {}", ACCEPT_KEY);
        return false;
    }

    return true;
}

void WebSocketClient::runWebSocketRead(char const * buffer, std::size_t size)
{
    Err const EXECUTE_CODE = _cache.ws.receiver.execute((uint8_t*)buffer, size);
    if (EXECUTE_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::runWebSocketRead() WebSocket frame {} error", getErrName(EXECUTE_CODE));
        return;
    }

    if (_cache.ws.receiver.fin == false) {
        tDLogD("WebSocketClient::runWebSocketRead() Waiting next frame ...");
        return;
    }

    assert(_cache.ws.receiver.fin);
    WebSocketFrame & frame = _cache.ws.receiver;

    if (frame.opcode == OpCode::OC_TEXT_FRAME || frame.opcode == OpCode::OC_BINARY_FRAME) {
        onWsMessage(frame.opcode, (char const *)frame.getPayloadDataPtr(), frame.getPayloadSize());

    } else if (frame.opcode == OpCode::OC_CONNECTION_CLOSE) {
        _close = frame.getCloseResult();
        _cache.ws.closing = true;

        Err const CLOSE_CODE = close();
        if (CLOSE_CODE != Err::E_SUCCESS) {
            tDLogE("WebSocketClient::runWebSocketRead() Close {} error.", getErrName(CLOSE_CODE));
        }

    } else {
        tDLogW("WebSocketClient::runWebSocketRead() Unsupported opcode: {}", getOpCodeName(frame.opcode));
    }
}

// ---------------------
// Stream event methods.
// ---------------------

void WebSocketClient::onConnect(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onConnect() {} error.", getErrName(code));
        onWsError(code);
        close();
        return;
    }

    HttpBuilder & request = _cache.builder;

    Err const UPDATE_WS_CODE = updateRequestWebSocket(request, _cache.getKey());
    if (UPDATE_WS_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onConnect() Upgrade WebSocket {} error.", getErrName(UPDATE_WS_CODE));
        onWsError(UPDATE_WS_CODE);
        close();
        return;
    }

    Err const START_CODE = start();
    if (START_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onConnect() Start {} error.", getErrName(START_CODE));
        onWsError(START_CODE);
        close();
        return;
    }

    auto buffer = request.buildDefaultRequestString();
    Err const WRITE_CODE = write(buffer.data(), buffer.size());
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onConnect() Write {} error.", getErrName(WRITE_CODE));
        onWsError(WRITE_CODE);
        close();
        return;
    }

    if (request.existsHeader(HEADER_ORIGIN) == false) {
        tDLogW("WebSocketClient::onConnect() Not found {} header.", HEADER_ORIGIN);
    }
    if (request.getMethod() != getHttpMethodName(HttpMethod::M_GET)) {
        tDLogW("WebSocketClient::onConnect() Not a GET method: {}", request.getMethod());
    }
    tDLogI("WebSocketClient::onConnect() Request WebSocket key: {}", _cache.getKey());
}

void WebSocketClient::onShutdown(Err code)
{
    // [WARNING] Don't use 'close' method.
    onWsError(code == Err::E_SUCCESS ? Err::E_SHUTDOWN : code);
}

void WebSocketClient::onWrite(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onWrite() {} error.", getErrName(code));
        onWsError(code);
        return;
    }
}

void WebSocketClient::onRead(Err code, ReadPacket const & packet)
{
    if (code == Err::E_EOF) {
        tDLogI("WebSocketClient::onRead() EOF.");
        close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onRead() {} error", getErrName(code));
        onWsError(code);
        return;
    }

    if (_cache.isUpgrade()) {
        // WebSocket packet processing.
        runWebSocketRead(packet.buffer, packet.size);
        return;
    }

    HttpParser & response = _cache.parser;

    assert(code == Err::E_SUCCESS);
    response.execute(packet.buffer, packet.size);
    if (response.isComplete() == false) {
        tDLogD("WebSocketClient::onRead() Not complete.");
        return;
    }

    if (runWebSocketChecker(response) == false) {
        onWsError(Err::E_NOT_WS_RESPONSE);
        close();
        return;
    }

    tDLogI("WebSocketClient::onRead() Upgrade complete!");

    _cache.ws.upgrade = true;
    onWsOpen(response.getResponse());
}

void WebSocketClient::onClose()
{
    tDLogD("WebSocketClient::onClose({}, {})", _close.code, _close.reason);
    onWsClose(_close.code, _close.reason);
}

void WebSocketClient::onTimer()
{
    if (_cache.isClosing()) {
        tDLogD("WebSocketClient::onClientTimer() Closing timeout.");
        close();
    }
}

// -----------------
// Callback methods.
// -----------------

void WebSocketClient::onWsOpen(HttpResponse const & response)
{
    // EMPTY.
}

void WebSocketClient::onWsMessage(OpCode op, char const * buffer, std::size_t size)
{
    // EMPTY.
}

void WebSocketClient::onWsError(Err code)
{
    // EMPTY.
}

void WebSocketClient::onWsClose(uint16_t code, std::string const & reason)
{
    // EMPTY.
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

