/**
 * @file   WsClient.cpp
 * @brief  WsClient class implementation.
 * @author zer0
 * @date   2017-06-30
 * @date   2017-08-07 (Rename: WebSocketClient -> WsClient)
 */

#include <libtbag/network/http/WsClient.hpp>
#include <libtbag/log/Log.hpp>

//#define TBAG_WS_CLIENT_CHECK_BUFFER

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

static void checkWsBuffer(std::string const & prefix, uint8_t const * buffer, std::size_t size)
{
    tDLogD("checkWsBuffer({}) [CHECK] BUFFER SIZE: {}", prefix, size);

    using namespace libtbag::network::http;
    WsFrame frame;
    Err const EXECUTE_CODE = frame.execute(buffer, size);
    if (TBAG_ERR_FAILURE(EXECUTE_CODE)) {
        tDLogW("checkWsBuffer({}) [CHECK] PARSING {} ERROR", prefix, getErrName(EXECUTE_CODE));
        return;
    }

    if (isControlFrame(frame.opcode)) {
        tDLogW("checkWsBuffer({}) [CHECK] CONTROL FRAME", prefix);
    }
}

#if defined(TBAG_WS_CLIENT_CHECK_BUFFER)
#define TBAG_WS_CLIENT_CHECK_BUFFER_IMPL(prefix, buffer, size) \
    ::libtbag::network::http::checkWsBuffer(prefix, (uint8_t const *)buffer, (std::size_t)size)
#else
#define TBAG_WS_CLIENT_CHECK_BUFFER_IMPL(prefix, buffer, size)
#endif

WsClient::WsClient(Loop & loop, StreamType type)
        : Parent(loop, type), KEY(generateRandomWebSocketKey()),
          _upgrade(false), _closing(false), _close()
{
    // EMPTY.
}

WsClient::~WsClient()
{
    // EMPTY.
}

void WsClient::setup(HttpBuilder const & request)
{
    Guard const LOCK_GUARD(_request_mutex);
    _request = request;
}

HttpBuilder WsClient::getRequest() const
{
    Guard const LOCK_GUARD(_request_mutex);
    return _request;
}

Err WsClient::writeWsFrame(WsFrame const & frame)
{
    WsBuffer buffer;
    std::size_t const SIZE = frame.copyTo(buffer);
    if (SIZE == 0) {
        tDLogE("WsClient::writeOrEnqueue() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return write((char const *)buffer.data(), buffer.size());
}

Err WsClient::writeText(std::string const & text, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    WsFrame frame;
    Err const CODE = frame.text(text, _device.gen(), continuation, finish);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WsClient::writeText() WsFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeWsFrame(frame);
}

Err WsClient::writeBinary(WsBuffer const & binary, bool continuation, bool finish)
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    WsFrame frame;
    Err const CODE = frame.binary(binary, _device.gen(), continuation, finish);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WsClient::writeBinary() WsFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeWsFrame(frame);
}

Err WsClient::closeWebSocket()
{
    if (isUpgrade() == false) {
        return Err::E_ILLSTATE;
    }

    _closing.store(true);
    Err const CLOSE_TIMER_CODE = startTimer(DEFAULT_CLOSING_TIMEOUT_MILLISECOND);

    if (TBAG_ERR_FAILURE(CLOSE_TIMER_CODE)) {
        tDLogE("WsClient::closeWebSocket() Close timer error: {} -> Force closing!", getErrName(CLOSE_TIMER_CODE));
        _close.set(WsStatusCode::WSSC_CLIENT_TIMER_ERROR);
        close();
        return CLOSE_TIMER_CODE;
    }

    WsFrame frame;
    Err const CODE = frame.close(_device.gen());
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("WsClient::closeWebSocket() WsFrame build {} error.", getErrName(CODE));
        return CODE;
    }
    return writeWsFrame(frame);
}

bool WsClient::runWsChecker(HttpParser const & response)
{
    if (response.getStatusCode() != getHttpStatusNumber(HttpStatus::SC_SWITCHING_PROTOCOLS)) {
        tDLogE("WsClient::runWsChecker() Not 101 status code error: {}", response.getStatusCode());
        return false;
    }

    if (response.existsHeaderValue(HEADER_CONNECTION, VALUE_UPGRADE) == false) {
        tDLogE("WsClient::runWsChecker() Not found upgrade connection.");
        return false;
    }

    if (response.existsHeaderValue(HEADER_UPGRADE, VALUE_WEBSOCKET) == false) {
        tDLogE("WsClient::runWsChecker() Not found websocket upgrade.");
        return false;
    }

    if (response.existsHeader(HEADER_SEC_WEBSOCKET_ACCEPT) == false) {
        tDLogE("WsClient::runWsChecker() Not found Sec-WebSocket-Accept header.");
        return false;
    }

    std::string const ACCEPT_KEY = response.getHeader(HEADER_SEC_WEBSOCKET_ACCEPT);
    if (ACCEPT_KEY != getUpgradeWebSocketKey(KEY)) {
        tDLogE("WsClient::runWsChecker() Accept key error: {}", ACCEPT_KEY);
        return false;
    }

    return true;
}

void WsClient::runWsRead(char const * buffer, std::size_t size)
{
    __on_read_only__.receiver.exec(buffer, size, [&](OpCode opcode, bool finish, WsFrameBuffer::Buffer & buffer) -> bool {
        if (finish) {
            if (opcode == OpCode::OC_TEXT_FRAME || opcode == OpCode::OC_BINARY_FRAME) {
                onWsMessage(opcode, &buffer[0], buffer.size());

            } else if (opcode == OpCode::OC_CONNECTION_CLOSE) {
                _close = __on_read_only__.receiver.atCachedFrame().getCloseResult();
                Err const CLOSE_CODE = close();
                if (CLOSE_CODE != Err::E_SUCCESS) {
                    tDLogE("WsClient::runWsRead() Close {} error.", getErrName(CLOSE_CODE));
                }

            } else {
                tDLogW("WsClient::runWsRead() Unsupported opcode: {}", getOpCodeName(opcode));
            }
        }

        return true;
    });
}

// ---------------------
// Stream event methods.
// ---------------------

void WsClient::onConnect(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("WsClient::onConnect() {} error.", getErrName(code));
        onWsError(code);
        close();
        return;
    }

    HttpBuilder request = getRequest();

    Err const UPDATE_WS_CODE = updateRequestWebSocket(request, KEY);
    if (UPDATE_WS_CODE != Err::E_SUCCESS) {
        tDLogE("WsClient::onConnect() Upgrade WebSocket {} error.", getErrName(UPDATE_WS_CODE));
        onWsError(UPDATE_WS_CODE);
        close();
        return;
    }

    Err const START_CODE = start();
    if (START_CODE != Err::E_SUCCESS) {
        tDLogE("WsClient::onConnect() Start {} error.", getErrName(START_CODE));
        onWsError(START_CODE);
        close();
        return;
    }

    auto buffer = request.buildDefaultRequestString();
    Err const WRITE_CODE = write(buffer.data(), buffer.size());
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogE("WsClient::onConnect() Write {} error.", getErrName(WRITE_CODE));
        onWsError(WRITE_CODE);
        close();
        return;
    }

    if (request.existsHeader(HEADER_ORIGIN) == false) {
        tDLogW("WsClient::onConnect() Not found {} header.", HEADER_ORIGIN);
    }
    if (request.getMethod() != getHttpMethodName(HttpMethod::M_GET)) {
        tDLogW("WsClient::onConnect() Not a GET method: {}", request.getMethod());
    }
    tDLogI("WsClient::onConnect() Request WebSocket key: {}", KEY);
}

void WsClient::onShutdown(Err code)
{
    onWsError(code == Err::E_SUCCESS ? Err::E_SHUTDOWN : code);
    // [WARNING] Don't use 'close' method.
}

void WsClient::onWrite(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("WsClient::onWrite() {} error.", getErrName(code));
        onWsError(code);
        return;
    }
}

void WsClient::onRead(Err code, ReadPacket const & packet)
{
    if (code == Err::E_EOF) {
        tDLogI("WsClient::onRead() EOF.");
        close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("WsClient::onRead() {} error", getErrName(code));
        onWsError(code);
        return;
    }

    if (isUpgrade()) {
        // WebSocket packet processing...
        runWsRead(packet.buffer, packet.size);
        return;
    }

    HttpParser & res = __on_read_only__.response;

    assert(code == Err::E_SUCCESS);
    res.execute(packet.buffer, packet.size);
    if (res.isComplete() == false) {
        tDLogD("WsClient::onRead() Not complete.");
        return;
    }

    if (runWsChecker(res) == false) {
        onWsError(Err::E_NOT_WS_RESPONSE);
        close();
        return;
    }

    tDLogI("WsClient::onRead() Upgrade complete!");
    _upgrade.store(true);
    onWsOpen(res.getResponse());
}

void WsClient::onClose()
{
    tDLogD("WsClient::onClose({}, {})", _close.code, _close.reason);
    onWsClose(_close.code, _close.reason);
}

void WsClient::onTimer()
{
    if (isClosing()) {
        tDLogD("WsClient::onClientTimer() Closing timeout.");
        close();
    }
}

// -----------------
// Callback methods.
// -----------------

void WsClient::onWsOpen(HttpResponse const & response)
{
    // EMPTY.
}

void WsClient::onWsMessage(OpCode op, char const * buffer, std::size_t size)
{
    // EMPTY.
}

void WsClient::onWsError(Err code)
{
    // EMPTY.
}

void WsClient::onWsClose(uint16_t code, std::string const & reason)
{
    // EMPTY.
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

