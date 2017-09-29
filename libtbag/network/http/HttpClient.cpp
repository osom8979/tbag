/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2017-05-20
 */

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpClient::HttpClient(Loop & loop, StreamType type)
        : Parent(loop, type), _builder(), _parser(HttpParser::Type::RESPONSE), _call_once(false)
{
    // EMPTY.
}

HttpClient::~HttpClient()
{
    // EMPTY.
}

void HttpClient::setup(HttpBuilder const & request, Callback const & cb)
{
    _builder  = request;
    _callback = cb;
}

void HttpClient::runCallback(EventStep step, Err code)
{
    if (static_cast<bool>(_callback)) {
        _callback(Event(step, code, _parser));
    }
    _call_once = true;
}

void HttpClient::onConnect(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpClient::onConnect() {} error.", getErrName(code));
        runCallback(EventStep::ET_CONNECT, code);
        close();
        return;
    }

    auto buffer = _builder.buildDefaultRequestString();
    Err const WRITE_CODE = write(buffer.data(), buffer.size());
    if (WRITE_CODE != Err::E_SUCCESS) {
        tDLogE("HttpClient::onConnect() write {} error.", getErrName(WRITE_CODE));
        runCallback(EventStep::ET_CONNECT, WRITE_CODE);
        close();
        return;
    }
}

void HttpClient::onShutdown(Err code)
{
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    close();
}

void HttpClient::onWrite(Err code)
{
    if (code != Err::E_SUCCESS) {
        tDLogE("HttpClient::onWrite() {} error.", getErrName(code));
        runCallback(EventStep::ET_WRITE, code);
        close();
        return;
    }

    Err const START_CODE = start();
    if (START_CODE != Err::E_SUCCESS) {
        tDLogE("HttpClient::onWrite() start {} error.", getErrName(START_CODE));
        runCallback(EventStep::ET_WRITE, START_CODE);
        close();
        return;
    }
}

void HttpClient::onRead(Err code, ReadPacket const & packet)
{
    if (code == Err::E_EOF) {
        tDLogI("HttpClient::onRead() EOF.");
        if (_call_once == false) {
            runCallback(EventStep::ET_READ, Err::E_NO_RESPONSE);
        }
        close();
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("HttpServer::onRead() {} error", getErrName(code));
        runCallback(EventStep::ET_READ, code);
        close();
        return;
    }

    assert(code == Err::E_SUCCESS);
    _parser.execute(packet.buffer, packet.size);

    if (_parser.isComplete()) {
        tDLogD("HttpClient::onRead() Completed http parsing (HTTP STATUS: {}).", _parser.getStatusCode());
        runCallback(EventStep::ET_READ, Err::E_SUCCESS);
        close();
        return;
    }

    tDLogD("HttpClient::onRead() Incomplete. Wait for response data.");
}

void HttpClient::onClose()
{
    tDLogD("HttpClient::onClose()");
}

void HttpClient::onTimer()
{
    tDLogD("HttpClient::onTimer()");
    runCallback(EventStep::ET_TIMER, Err::E_TIMEOUT);
    close();
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

