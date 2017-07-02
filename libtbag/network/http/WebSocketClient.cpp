/**
 * @file   WebSocketClient.cpp
 * @brief  WebSocketClient class implementation.
 * @author zer0
 * @date   2017-06-30
 */

#include <libtbag/network/http/WebSocketClient.hpp>
#include <libtbag/log/Log.hpp>
#include "WebSocketClient.hpp"

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WebSocketClient::WebSocketClient(Loop & loop, StreamType type) : Parent(loop, type)
{
    setSkipTimeout();
}

WebSocketClient::~WebSocketClient()
{
    // EMPTY.
}

Err WebSocketClient::sendText(std::string const & text, uint64_t timeout)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendBinary(Buffer const & binary, uint64_t timeout)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendPing(Buffer const & binary, uint64_t timeout)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendPong(Buffer const & binary, uint64_t timeout)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendClose(uint64_t timeout)
{
    return Err::E_UNKNOWN;
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
    if (ACCEPT_KEY != getUpgradeWebSocketKey(_data.key)) {
        tDLogE("WebSocketClient::runWebSocketChecker() Accept key error: {}", ACCEPT_KEY);
        return false;
    }

    return true;
}

void WebSocketClient::runWebSocketRead(char const * buffer, std::size_t size)
{
    auto & recv_frame   = _data.websocket.recv_frame;
    auto & write_frame  = _data.websocket.write_frame;
    auto & frame_buffer = _data.websocket.frame_buffer;

    Err const EXECUTE_CODE = recv_frame.execute((uint8_t*)buffer, size);
    if (EXECUTE_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::runWebSocketRead() WebSocket frame {} error", getErrName(EXECUTE_CODE));
        return;
    }

    onWsMessage(recv_frame);
}

void WebSocketClient::onConnect(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onConnect() {} error.", getErrName(code));
        onWsError(code);
        close();
        return;
    }

    HttpBuilder & request = _data.builder;

    Err const UPDATE_WS_CODE = updateRequestWebSocket(request);
    if (UPDATE_WS_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onConnect() Upgrade WebSocket {} error.", getErrName(UPDATE_WS_CODE));
        onWsError(UPDATE_WS_CODE);
        close();
        return;
    }

    auto buffer = request.buildDefaultRequestString();
    Err const WRITE_CODE = write(buffer.data(), buffer.size());
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onConnect() write {} error.", getErrName(WRITE_CODE));
        onWsError(WRITE_CODE);
        close();
        return;
    }

    if (request.existsHeader(HEADER_ORIGIN) == false) {
        tDLogW("WebSocketClient::onConnect() Not found {} header.", HEADER_ORIGIN);
    }
    if (request.getMethod() == getHttpMethodName(HttpMethod::M_GET)) {
        tDLogW("WebSocketClient::onConnect() Not a GET method: {}", request.getMethod());
    }
    _data.key = request.getHeader(HEADER_SEC_WEBSOCKET_KEY);
    tDLogI("WebSocketClient::onConnect() Request WebSocket key: {}", _data.key);
}

void WebSocketClient::onShutdown(Err code)
{
    onWsError(Err::E_SHUTDOWN);
    // Don't use 'close' method.
}

void WebSocketClient::onWrite(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onWrite() {} error.", getErrName(code));
        onWsError(code);
        close();
        return;
    }

    Err const START_CODE = start();
    if (START_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::onWrite() start {} error.", getErrName(START_CODE));
        onWsError(START_CODE);
        close();
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
        close();
        return;
    }

    if (isUpgradeWebSocket()) {
        runWebSocketRead(packet.buffer, packet.size);
        return;
    }

    HttpBuilder & request  = _data.builder;
    HttpParser  & response = _data.parser;

    assert(code == Err::E_SUCCESS);
    response.execute(packet.buffer, packet.size);
    if (response.isComplete() == false) {
        tDLogD("WebSocketClient::onRead() Not complete.");
        return;
    }

    if (runWebSocketChecker(response)) {
        onWsError(Err::E_NOT_WS_RESPONSE);
        close();
        return;
    }

    tDLogI("WebSocketClient::onRead() Upgrade complete!");
    stopTimeout();
    _data.websocket.upgrade = true;
    onWsOpen(response.getResponse());
}

void WebSocketClient::onClose()
{
    tDLogD("WebSocketClient::onClose()");
    onWsClose();
}

// -----------------
// Callback methods.
// -----------------

void WebSocketClient::onWsOpen(HttpResponse const & response)
{
    // EMPTY.
}

void WebSocketClient::onWsMessage(Frame const & frame)
{
    // EMPTY.
}

void WebSocketClient::onWsError(Err code)
{
    // EMPTY.
}

void WebSocketClient::onWsClose()
{
    // EMPTY.
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

