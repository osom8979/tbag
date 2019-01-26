/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2019-01-11
 */

#include <libtbag/http/HttpClient.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

using Loop     = HttpClient::Loop;
using MqParams = HttpClient::MqParams;

HttpClient::HttpClient(std::string const & address, int port, Callbacks const & callbacks)
        : HttpClient(Params(address, port, callbacks))
{
    assert(static_cast<bool>(_client));
}

HttpClient::HttpClient(Params const & params)
        : PARAMS(params), _reader(this, params.getWebSocketKey(), params.enable_websocket),
          _state(TlsState::TS_NOT_READY)
{
    NetStreamClient::Callbacks client_cbs;
    client_cbs.begin_cb = [&]() {
        if (PARAMS.enable_tls) {
            onBeginTls();
        } else {
            onBegin();
        }
    };
    client_cbs.end_cb = [&]() {
        onEnd();
    };
    client_cbs.recv_cb = [&](char const * b, std::size_t l){
        if (PARAMS.enable_tls) {
            onRecvTls(b, l);
        } else {
            onRecv(b, l);
        }
    };
    _client = std::make_unique<NetStreamClient>(params, client_cbs);
    assert(static_cast<bool>(_client));
}

HttpClient::~HttpClient()
{
    _client.reset();
}

Loop & HttpClient::loop()
{
    assert(static_cast<bool>(_client));
    return _client->loop();
}

Loop const & HttpClient::loop() const
{
    assert(static_cast<bool>(_client));
    return _client->loop();
}

Err HttpClient::sendTls(void const * data, std::size_t size)
{
    Err encode_code = Err::E_UNKNOWN;
    auto encode_result = _tls.encode(data, size, &encode_code);
    if (isFailure(encode_code)) {
        return encode_code;
    }
    return _client->send(encode_result.data(), encode_result.size());
}

Err HttpClient::sendTls(std::string const & text)
{
    return sendTls(text.data(), text.size());
}

void HttpClient::onBegin()
{
    if (PARAMS.begin_cb) {
        PARAMS.begin_cb();
    }
}

void HttpClient::onBeginTls()
{
    _tls.connect();
    Err const HANDSHAKE_CODE = _tls.handshake();
    // Handshake is not finished, we can ignore it.
    assert(HANDSHAKE_CODE == Err::E_SSLWREAD);

    Buffer buffer;
    Err const WRITE_BUFFER_CODE = _tls.readFromWriteBuffer(buffer);
    if (isFailure(WRITE_BUFFER_CODE)) {
        onError(WRITE_BUFFER_CODE);
        return;
    }

    // HANDSHAKE 01. Client hello
    Err const WRITE_CODE = _client->send(buffer.data(), buffer.size());
    if (isFailure(WRITE_CODE)) {
        onError(WRITE_CODE);
        return;
    }

    // [WARNING] Called after successful SSL Handshake.
    // PARENT->onBegin();

    tDLogD("HttpClient::onBeginTls() Update state: Not ready -> Hello");
    assert(_state == TlsState::TS_NOT_READY);
    _state = TlsState::TS_HELLO;
}

void HttpClient::onEnd()
{
    if (PARAMS.end_cb) {
        PARAMS.end_cb();
    }
}

void HttpClient::onRecv(char const * buffer, std::size_t size)
{
    _reader.parse(buffer, size);
}

void HttpClient::onRecvTls(char const * buffer, std::size_t size)
{
    switch (_state) {
    case TlsState::TS_NOT_READY:
        onError(Err::E_ILLSTATE);
        return;

    case TlsState::TS_HELLO:
        onHandshakeHello(buffer, size);
        return;

    case TlsState::TS_EXCHANGE_KEY:
        onHandshakeExchangeKey(buffer, size);
        return;

    case TlsState::TS_FINISH:
        onApplication(buffer, size);
        return;

    default:
        onError(Err::E_UNKNOWN);
        return;
    }
}

void HttpClient::onHandshakeHello(char const * buffer, std::size_t size)
{
    assert(_state == TlsState::TS_HELLO);

    Err const READ_BUFFER_CODE = _tls.writeToReadBuffer(buffer, size);
    if (isFailure(READ_BUFFER_CODE)) {
        onError(READ_BUFFER_CODE);
        return;
    }

    Err const HANDSHAKE_CODE = _tls.handshake();
    if (HANDSHAKE_CODE != Err::E_SSLWREAD) {
        onError(HANDSHAKE_CODE);
        return;
    }

    // Handshake is not finished, we can ignore it.
    assert(HANDSHAKE_CODE == Err::E_SSLWREAD);

    Buffer write_buffer;
    _tls.readFromWriteBuffer(write_buffer); // Skip error code check.

    if (!write_buffer.empty()) {
        Err const WRITE_CODE = _client->send(write_buffer.data(), write_buffer.size());
        if (isSuccess(WRITE_CODE)) {
            tDLogD("HttpClient::onHandshakeHello() SSL connection using {}", _tls.getCipherName());
            _state = TlsState::TS_EXCHANGE_KEY;
        } else {
            onError(WRITE_CODE);
            return;
        }
    }
}

void HttpClient::onHandshakeExchangeKey(char const * buffer, std::size_t size)
{
    assert(_state == TlsState::TS_EXCHANGE_KEY);

    Err const READ_BUFFER_CODE = _tls.writeToReadBuffer(buffer, size);
    if (isFailure(READ_BUFFER_CODE)) {
        onError(READ_BUFFER_CODE);
        return;
    }

    Err const HANDSHAKE_CODE = _tls.handshake(); // Skip error code check.

    if (_tls.isFinished()) {
        tDLogD("HttpClient::onHandshakeExchangeKey() Handshake finish!");
        _state = TlsState::TS_FINISH;
        if (PARAMS.begin_cb) {
            PARAMS.begin_cb();
        }
    } else {
        onError(HANDSHAKE_CODE);
    }
}

void HttpClient::onApplication(char const * buffer, std::size_t size)
{
    assert(_state == TlsState::TS_FINISH);

    Err code = Err::E_UNKNOWN;

    auto cursor = _tls.decode(buffer, size, &code);
    if (code == Err::E_SSLWREAD || code == Err::E_SSLEREAD) {
        tDLogD("HttpClient::onApplication() Decode #1 result: {}", getErrName(code));
        return;
    } else if (isFailure(code)) {
        onError(code);
        return;
    }

    code = _reader.parse(cursor.data(), cursor.size());
    tDLogD("HttpClient::onApplication() result({}), read({})", getErrName(code), cursor.size());

    while (code == Err::E_CONTINUE) {
        cursor = _tls.decode(&code);
        if (code == Err::E_SSLWREAD || code == Err::E_SSLEREAD) {
            tDLogD("HttpClient::onApplication() Decode #2 result: {}", getErrName(code));
            return;
        } else if (isFailure(code)) {
            onError(code);
            return;
        }
        code = _reader.parse(cursor.data(), cursor.size());
        tDLogD("HttpClient::onApplication() result({}), read({})", getErrName(code), cursor.size());
    }
}

void HttpClient::onError(Err code)
{
    if (PARAMS.error_cb) {
        PARAMS.error_cb(code);
    } else {
        auto const EXIT_CODE = exit();
        tDLogE("HttpClient::onError({}) Exit request: {}", code, EXIT_CODE);
    }
}

void HttpClient::onContinue(void * arg)
{
    // EMPTY.
}

bool HttpClient::onSwitchingProtocol(HttpProperty const & property, void * arg)
{
    if (PARAMS.switch_cb) {
        return PARAMS.switch_cb(static_cast<HttpResponse>(property));
    } else {
        return _reader.isEnableWebsocket();
    }
}

void HttpClient::onWsMessage(WsOpCode opcode, Buffer const & payload, void * arg)
{
    if (PARAMS.message_cb) {
        PARAMS.message_cb(opcode, payload);
    }
}

void HttpClient::onRegularHttp(HttpProperty const & property, void * arg)
{
    if (PARAMS.http_cb) {
        PARAMS.http_cb(static_cast<HttpResponse>(property));
    }
}

void HttpClient::onParseError(Err code, void * arg)
{
    onError(code);
}

void HttpClient::join()
{
    assert(static_cast<bool>(_client));
    _client->join();
}

Err HttpClient::exit()
{
    assert(static_cast<bool>(_client));
    return _client->exit();
}

Err HttpClient::writeRequest()
{
    return writeRequest(HttpRequest());
}

Err HttpClient::writeRequest(HttpRequest const & request)
{
    HttpRequest update_request = request;
    libtbag::http::updateDefaultRequest(update_request);

    if (PARAMS.enable_tls) {
        return sendTls(libtbag::http::toRequestString(update_request));
    } else {
        return _client->send(libtbag::http::toRequestString(update_request));
    }
}

Err HttpClient::writeWsRequest()
{
    HttpRequest request;
    request.method = libtbag::http::getHttpMethodName(libtbag::http::HttpMethod::M_GET);
    libtbag::http::insert(request.header, libtbag::http::HEADER_HOST, std::string());
    libtbag::http::insert(request.header, libtbag::http::HEADER_ORIGIN, std::string());
    return writeWsRequest(request);
}

Err HttpClient::writeWsRequest(HttpRequest const & request)
{
    HttpRequest ws_request = request;
    libtbag::http::updateDefaultWsRequest(ws_request, _reader.getKey());

    if (!libtbag::http::exists(ws_request.header, libtbag::http::HEADER_ORIGIN)) {
        tDLogW("HttpClient::writeWsRequest() Not found {} header.", libtbag::http::HEADER_ORIGIN);
    }
    if (libtbag::http::getHttpMethod(ws_request.method) != libtbag::http::HttpMethod::M_GET) {
        tDLogW("HttpClient::writeWsRequest() Not a GET method: {}", request.method);
    }

    if (PARAMS.enable_tls) {
        return sendTls(libtbag::http::toRequestString(ws_request));
    } else {
        return _client->send(libtbag::http::toRequestString(ws_request));
    }
}

Err HttpClient::writeWsFrame(WsFrame const & frame)
{
    if (!_reader.isEnableWebsocket()) {
        return Err::E_ILLSTATE;
    }

    Buffer buffer;
    auto const SIZE = frame.copyTo(buffer);

    if (SIZE == 0) {
        tDLogE("HttpClient::writeWsFrame() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }

    if (PARAMS.enable_tls) {
        return sendTls(buffer.data(), buffer.size());
    } else {
        return _client->send(buffer.data(), buffer.size());
    }
}

Err HttpClient::writeText(char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.text(buffer, size, _masking.gen(), finish);
    return writeWsFrame(frame);
}

Err HttpClient::writeText(std::string const & text, bool finish)
{
    return writeText(text.data(), text.size(), finish);
}

Err HttpClient::writeBinary(char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.binary(buffer, size, _masking.gen(), finish);
    return writeWsFrame(frame);
}

Err HttpClient::writeBinary(Buffer const & buffer, bool finish)
{
    return writeBinary(buffer.data(), buffer.size(), finish);
}

Err HttpClient::writeClose()
{
    WsFrame frame;
    frame.close(_masking.gen());
    return writeWsFrame(frame);
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

