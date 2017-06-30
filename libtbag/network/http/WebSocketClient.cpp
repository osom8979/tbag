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

WebSocketClient::WebSocketClient(Loop & loop, StreamType type) : HttpClient(loop, type)
{
    // EMPTY.
}

WebSocketClient::~WebSocketClient()
{
    // EMPTY.
}

Err WebSocketClient::sendText(std::string const & text)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendBinary(Buffer const & binary)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendPing(Buffer const & binary)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendPong(Buffer const & binary)
{
    return Err::E_UNKNOWN;
}

Err WebSocketClient::sendClose()
{
    return Err::E_UNKNOWN;
}

void WebSocketClient::onConnect(Err code)
{
//    if (code != Err::E_SUCCESS) {
//        tDLogE("WebSocketClient::onConnect() {} error.", getErrName(code));
//        runCallback(EventStep::ET_CONNECT, code);
//        close();
//        return;
//    }
//
//    auto buffer = _builder.toDefaultRequestString();
//    Err const WRITE_CODE = write(buffer.data(), buffer.size());
//    if (WRITE_CODE != Err::E_SUCCESS) {
//        tDLogE("WebSocketClient::onConnect() write {} error.", getErrName(WRITE_CODE));
//        runCallback(EventStep::ET_CONNECT, WRITE_CODE);
//        close();
//        return;
//    }
//
//    using namespace std::chrono;
//    Millisec const LEFT_TIME = _timeout - duration_cast<Millisec>(SystemClock::now() - _start_time);
//
//    if (LEFT_TIME.count() <= 0) {
//        tDLogE("WebSocketClient::onConnect() timeout.");
//        runCallback(EventStep::ET_CONNECT, Err::E_TIMEOUT);
//        close();
//        return;
//    }
}

void WebSocketClient::onShutdown(Err code)
{
//    TBAG_INACCESSIBLE_BLOCK_ASSERT();
//    close();
}

void WebSocketClient::onWrite(Err code)
{
//    if (code != Err::E_SUCCESS) {
//        tDLogE("WebSocketClient::onWrite() {} error.", getErrName(code));
//        runCallback(EventStep::ET_WRITE, code);
//        close();
//        return;
//    }
//
//    Err const START_CODE = start();
//    if (START_CODE != Err::E_SUCCESS) {
//        tDLogE("WebSocketClient::onWrite() start {} error.", getErrName(START_CODE));
//        runCallback(EventStep::ET_WRITE, START_CODE);
//        close();
//        return;
//    }
}

void WebSocketClient::onRead(Err code, ReadPacket const & packet)
{
//    if (code == Err::E_EOF) {
//        tDLogI("WebSocketClient::onRead() EOF.");
//        if (_call_once == false) {
//            runCallback(EventStep::ET_READ, Err::E_NO_RESPONSE);
//        }
//        close();
//        return;
//    }
//
//    if (code != Err::E_SUCCESS) {
//        tDLogE("HttpServer::onRead() {} error", getErrName(code));
//        runCallback(EventStep::ET_READ, code);
//        close();
//        return;
//    }
//
//    assert(code == Err::E_SUCCESS);
//    _parser.execute(packet.buffer, packet.size);
//
//    if (_parser.isComplete()) {
//        tDLogD("WebSocketClient::onRead() Completed http parsing (HTTP STATUS: {}).", _parser.getStatusCode());
//        runCallback(EventStep::ET_READ, Err::E_SUCCESS);
//        close();
//        return;
//    }
//
//    tDLogD("WebSocketClient::onRead() Incomplete. Wait for response data.");
}

void WebSocketClient::onClose()
{
//    tDLogD("WebSocketClient::onClose()");
//    if (_call_once == false) {
//        runCallback(EventStep::ET_CLOSE, Err::E_TIMEOUT);
//    }
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

