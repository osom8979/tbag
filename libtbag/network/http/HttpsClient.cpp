/**
 * @file   HttpsClient.cpp
 * @brief  HttpsClient class implementation.
 * @author zer0
 * @date   2018-02-04
 */

#include <libtbag/network/http/HttpsClient.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpsClient::HttpsClient(Loop & loop, StreamType type)
        : Parent(loop, type), _reader(this), _tls(), _state(TlsState::TS_NOT_READY)
{
    // EMPTY.
}

HttpsClient::~HttpsClient()
{
    // EMPTY.
}

std::string HttpsClient::getKey() const
{
    return _reader.getKey();
}

Err HttpsClient::writeRequest()
{
    return writeRequest(HttpRequest());
}

Err HttpsClient::writeRequest(HttpRequest const & request)
{
    HttpRequest update_request = request;
    libtbag::http::updateDefaultRequest(update_request);
    std::string const & REQUEST_STRING = libtbag::http::toRequestString(update_request);
    return writeTls(REQUEST_STRING.data(), REQUEST_STRING.size());
}

Err HttpsClient::writeTls(void const * data, std::size_t size)
{
    Err encode_code = Err::E_UNKNOWN;
    auto encode_result = _tls.encode(data, size, &encode_code);
    if (isFailure(encode_code)) {
        return encode_code;
    }
    return write(encode_result.data(), encode_result.size());
}

// --------------
// Event methods.
// --------------

void HttpsClient::onConnect(Err code)
{
    if (isFailure(code)) {
        onError(EventType::ET_CONNECT, code);
        return;
    }

    _tls.connect();
    Err const HANDSHAKE_CODE = _tls.handshake();
    // Handshake is not finished, we can ignore it.
    assert(HANDSHAKE_CODE == Err::E_SSLWREAD);

    std::vector<char> buffer;
    Err const WRITE_BUFFER_CODE = _tls.readFromWriteBuffer(buffer);
    if (isFailure(WRITE_BUFFER_CODE)) {
        onError(EventType::ET_CONNECT, WRITE_BUFFER_CODE);
        return;
    }

    // HANDSHAKE 01. Client hello
    Err const WRITE_CODE = write(buffer.data(), buffer.size());
    if (isFailure(WRITE_CODE)) {
        onError(EventType::ET_CONNECT, WRITE_CODE);
        return;
    }

    Err const START_CODE = start();
    if (isFailure(START_CODE)) {
        onError(EventType::ET_CONNECT, START_CODE);
        return;
    }

    // [WARNING] Called after successful SSL Handshake.
    // onOpen();

    tDLogD("HttpsClient::onConnect() Update state: Not ready -> Hello");
    assert(_state == TlsState::TS_NOT_READY);
    _state = TlsState::TS_HANDSHAKE;
}

void HttpsClient::onRead(Err code, ReadPacket const & packet)
{
    if (code == Err::E_EOF) {
        onEof();
        return;
    } else if (code != Err::E_SUCCESS) {
        onError(EventType::ET_READ, code);
        return;
    }

    switch (_state) {
    case TlsState::TS_NOT_READY:
        onError(EventType::ET_READ, Err::E_ILLSTATE);
        return;

    case TlsState::TS_HANDSHAKE:
        onHandshakeHello(packet);
        return;

    case TlsState::TS_EXCHANGE_KEY:
        onHandshakeExchangeKey(packet);
        return;

    case TlsState::TS_FINISH:
        onApplication(packet);
        return;

    default:
        onError(EventType::ET_READ, Err::E_UNKNOWN);
        return;
    }
}

void HttpsClient::onParseError(Err code, void * arg)
{
    close();
}

void HttpsClient::onHandshakeHello(ReadPacket const & packet)
{
    assert(_state == TlsState::TS_HANDSHAKE);

    Err const READ_BUFFER_CODE = _tls.writeToReadBuffer(packet.buffer, packet.size);
    if (isFailure(READ_BUFFER_CODE)) {
        onError(EventType::ET_READ, READ_BUFFER_CODE);
        return;
    }

    Err const HANDSHAKE_CODE = _tls.handshake();
    if (HANDSHAKE_CODE != Err::E_SSLWREAD) {
        onError(EventType::ET_READ, HANDSHAKE_CODE);
        return;
    }

    // Handshake is not finished, we can ignore it.
    assert(HANDSHAKE_CODE == Err::E_SSLWREAD);

    std::vector<char> buffer;
    _tls.readFromWriteBuffer(buffer); // Skip error code check.

    if (buffer.size() > 0) {
        Err const WRITE_CODE = write(buffer.data(), buffer.size());
        if (isSuccess(WRITE_CODE)) {
            tDLogD("HttpsClient::onHandshakeHello() SSL connection using {}",
                   _tls.getCipherName());
            _state = TlsState::TS_EXCHANGE_KEY;
        } else {
            onError(EventType::ET_READ, WRITE_CODE);
            return;
        }
    }
}

void HttpsClient::onHandshakeExchangeKey(ReadPacket const & packet)
{
    assert(_state == TlsState::TS_EXCHANGE_KEY);

    Err const READ_BUFFER_CODE = _tls.writeToReadBuffer(packet.buffer, packet.size);
    if (isFailure(READ_BUFFER_CODE)) {
        onError(EventType::ET_READ, READ_BUFFER_CODE);
        return;
    }

    Err const HANDSHAKE_CODE = _tls.handshake(); // Skip error code check.

    if (_tls.isFinished()) {
        tDLogD("HttpsClient::onHandshakeExchangeKey() Handshake finish!");
        _state = TlsState::TS_FINISH;
        onOpen();
    } else {
        onError(EventType::ET_READ, HANDSHAKE_CODE);
    }
}

void HttpsClient::onApplication(ReadPacket const & packet)
{
    assert(_state == TlsState::TS_FINISH);

    Err code = Err::E_UNKNOWN;

    auto cursor = _tls.decode(packet.buffer, packet.size, &code);
    if (code == Err::E_SSLWREAD || code == Err::E_SSLEREAD) {
        tDLogD("HttpsClient::onApplication() Decode #1 result: {}", getErrName(code));
        return;
    } else if (isFailure(code)) {
        onError(EventType::ET_READ, code);
        return;
    }

    code = _reader.parse(cursor.data(), cursor.size());
    tDLogD("HttpsClient::onApplication() result({}), read({})", getErrName(code), cursor.size());

    while (code == Err::E_CONTINUE) {
        cursor = _tls.decode(&code);
        if (code == Err::E_SSLWREAD || code == Err::E_SSLEREAD) {
            tDLogD("HttpsClient::onApplication() Decode #2 result: {}", getErrName(code));
            return;
        } else if (isFailure(code)) {
            onError(EventType::ET_READ, code);
            return;
        }
        code = _reader.parse(cursor.data(), cursor.size());
        tDLogD("HttpsClient::onApplication() result({}), read({})", getErrName(code), cursor.size());
    }
}

// ---------------------------
// Extension callback methods.
// ---------------------------

void HttpsClient::onOpen()
{
    // EMPTY.
}

void HttpsClient::onEof()
{
    close();
}

void HttpsClient::onError(EventType from, Err code)
{
    tDLogE("HttpsClient::onError() EventType({}) Err({})", getEventTypeName(from), getErrName(code));
    close();
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

