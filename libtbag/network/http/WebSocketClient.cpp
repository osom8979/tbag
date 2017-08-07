/**
 * @file   WebSocketClient.cpp
 * @brief  WebSocketClient class implementation.
 * @author zer0
 * @date   2017-06-30
 */

#include <libtbag/network/http/WebSocketClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WebSocketClient::WebSocketClient(Loop & loop, StreamType type)
        : Parent(loop, type), KEY(generateRandomWebSocketKey()),
          _upgrade(false), _closing(false), _close()
{
    // EMPTY.
}

WebSocketClient::~WebSocketClient()
{
    // EMPTY.
}

void WebSocketClient::setup(HttpBuilder const & request)
{
    Guard const LOCK_GUARD(_request_mutex);
    _request = request;
}

HttpBuilder WebSocketClient::getRequest() const
{
    Guard const LOCK_GUARD(_request_mutex);
    return _request;
}

Err WebSocketClient::writeOrEnqueue(char const * buffer, std::size_t size)
{
    Guard const LOCK_GUARD(_queue_mutex);
    if (_queue.empty() && getWriteState() == WriteState::WS_READY) {
        return write(buffer, size);
    }
    _queue.push().assign(buffer, buffer + size);
    return Err::E_ENQASYNC;
}

Err WebSocketClient::writeOrEnqueue(WebSocketFrame const & frame)
{
    WsBuffer buffer;
    std::size_t const SIZE = frame.copyTo(buffer);
    if (SIZE == 0) {
        tDLogE("WebSocketClient::writeOrEnqueue() WebSocketFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return writeOrEnqueue((char const *)buffer.data(), buffer.size());
}

Err WebSocketClient::writeFromQueue()
{
    Guard const LOCK_GUARD(_queue_mutex);
    if (_queue.empty() || getWriteState() != WriteState::WS_READY) {
        return Err::E_ILLSTATE;
    }

    auto & buffer = _queue.frontRef();
    Err const WRITE_CODE = write((char const *)buffer.data(), buffer.size());
    _queue.pop();

    if (TBAG_ERR_FAILURE(WRITE_CODE)) {
        tDLogE("WebSocketClient::writeFromQueue() write {} error", getErrName(WRITE_CODE));
    }
    return WRITE_CODE;
}

Err WebSocketClient::writeText(std::string const & text, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    WebSocketFrame frame;
    Err const CODE = frame.text(text, _device.gen(), continuation, finish);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WebSocketClient::writeText() WebSocketFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeOrEnqueue(frame);
}

Err WebSocketClient::writeBinary(WsBuffer const & binary, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    WebSocketFrame frame;
    Err const CODE = frame.binary(binary, _device.gen(), continuation, finish);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WebSocketClient::writeBinary() WebSocketFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeOrEnqueue(frame);
}

Err WebSocketClient::closeWebSocket()
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    Err const CLOSE_TIMER_CODE = startTimer(DEFAULT_CLOSING_TIMEOUT_MILLISECOND);
    _closing.store(true);

    if (TBAG_ERR_FAILURE(CLOSE_TIMER_CODE)) {
        tDLogE("WebSocketClient::closeWebSocket() Close timer error: {} -> Force closing!", getErrName(CLOSE_TIMER_CODE));
        _close.set(WebSocketStatusCode::WSSC_CLIENT_TIMER_ERROR);
        close();
        return CLOSE_TIMER_CODE;
    }

    WebSocketFrame frame;
    Err const CODE = frame.close(_device.gen());
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WebSocketClient::closeWebSocket() WebSocketFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeOrEnqueue(frame);
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
    if (ACCEPT_KEY != getUpgradeWebSocketKey(KEY)) {
        tDLogE("WebSocketClient::runWebSocketChecker() Accept key error: {}", ACCEPT_KEY);
        return false;
    }

    return true;
}

void WebSocketClient::runWebSocketRead(char const * buffer, std::size_t size)
{
    WebSocketFrame & frame = __on_read_only__.receiver;
    Err const EXECUTE_CODE = frame.execute((uint8_t*)buffer, size);
    if (EXECUTE_CODE != Err::E_SUCCESS) {
        tDLogE("WebSocketClient::runWebSocketRead() WebSocket frame {} error", getErrName(EXECUTE_CODE));
        return;
    }

    if (frame.fin == false) {
        tDLogD("WebSocketClient::runWebSocketRead() Waiting next frame ...");
        return;
    }

    assert(frame.fin);
    if (frame.opcode == OpCode::OC_TEXT_FRAME || frame.opcode == OpCode::OC_BINARY_FRAME) {
        onWsMessage(frame.opcode, (char const *)frame.getPayloadDataPtr(), frame.getPayloadSize());

    } else if (frame.opcode == OpCode::OC_CONNECTION_CLOSE) {
        _close = frame.getCloseResult();
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

    HttpBuilder request = getRequest();

    Err const UPDATE_WS_CODE = updateRequestWebSocket(request, KEY);
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
    tDLogI("WebSocketClient::onConnect() Request WebSocket key: {}", KEY);
}

void WebSocketClient::onShutdown(Err code)
{
    writeFromQueue();

    onWsError(code == Err::E_SUCCESS ? Err::E_SHUTDOWN : code);
    // [WARNING] Don't use 'close' method.
}

void WebSocketClient::onWrite(Err code)
{
    writeFromQueue();

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

    if (isUpgrade()) {
        // WebSocket packet processing...
        runWebSocketRead(packet.buffer, packet.size);
        return;
    }

    HttpParser & res = __on_read_only__.response;

    assert(code == Err::E_SUCCESS);
    res.execute(packet.buffer, packet.size);
    if (res.isComplete() == false) {
        tDLogD("WebSocketClient::onRead() Not complete.");
        return;
    }

    if (runWebSocketChecker(res) == false) {
        onWsError(Err::E_NOT_WS_RESPONSE);
        close();
        return;
    }

    tDLogI("WebSocketClient::onRead() Upgrade complete!");
    _upgrade.store(true);
    onWsOpen(res.getResponse());
}

void WebSocketClient::onClose()
{
    tDLogD("WebSocketClient::onClose({}, {})", _close.code, _close.reason);
    onWsClose(_close.code, _close.reason);
}

void WebSocketClient::onTimer()
{
    if (isClosing()) {
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

