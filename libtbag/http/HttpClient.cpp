/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2019-01-11
 */

#include <libtbag/http/HttpClient.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/net/socket/NetStreamClient.hpp>
#include <libtbag/http/HttpReader.hpp>
#include <libtbag/crypto/TlsReader.hpp>
#include <libtbag/random/MaskingDevice.hpp>
#include <libtbag/Type.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpClient::Impl class implementation.
 *
 * @author zer0
 * @date   2019-01-11
 */
struct HttpClient::Impl : public libtbag::net::socket::NetStreamClient,
                          public libtbag::http::HttpReaderInterface
{
public:
    using NetStreamClient     = libtbag::net::socket::NetStreamClient;
    using HttpReaderInterface = libtbag::http::HttpReaderInterface;
    using Reader              = libtbag::http::HttpReaderForCallback<Impl>;
    using Masking             = libtbag::random::MaskingDevice;
    using TlsReader           = libtbag::crypto::TlsReader;
    using TlsState            = libtbag::crypto::TlsState;

public:
    STATIC_ASSERT_CHECK_IS_SAME(Buffer, HttpReaderInterface::Buffer);

public:
    HttpClient * const PARENT;
    bool const ENABLE_TLS;

private:
    Reader    _reader;
    Masking   _masking;
    TlsReader _tls;
    TlsState  _state;

public:
    Impl(HttpClient * parent, MqParams const & params, std::string const & key, bool use_websocket, bool enable_tls)
            : NetStreamClient(params), PARENT(parent), ENABLE_TLS(enable_tls),
              _reader(this, key, use_websocket), _state(TlsState::TS_NOT_READY)
    {
        assert(PARENT != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    Err sendTls(void const * data, std::size_t size)
    {
        Err encode_code = Err::E_UNKNOWN;
        auto encode_result = _tls.encode(data, size, &encode_code);
        if (isFailure(encode_code)) {
            return encode_code;
        }
        return send(encode_result.data(), encode_result.size());
    }

    Err sendTls(std::string const & text)
    {
        return sendTls(text.data(), text.size());
    }

private:
    virtual void onBegin() override
    {
        if (ENABLE_TLS) {
            onBeginTls();
        } else {
            PARENT->onBegin();
        }
    }

    void onBeginTls()
    {
        _tls.connect();
        Err const HANDSHAKE_CODE = _tls.handshake();
        // Handshake is not finished, we can ignore it.
        assert(HANDSHAKE_CODE == Err::E_SSLWREAD);

        Buffer buffer;
        Err const WRITE_BUFFER_CODE = _tls.readFromWriteBuffer(buffer);
        if (isFailure(WRITE_BUFFER_CODE)) {
            PARENT->onError(WRITE_BUFFER_CODE);
            return;
        }

        // HANDSHAKE 01. Client hello
        Err const WRITE_CODE = send(buffer.data(), buffer.size());
        if (isFailure(WRITE_CODE)) {
            PARENT->onError(WRITE_CODE);
            return;
        }

        // [WARNING] Called after successful SSL Handshake.
        // PARENT->onBegin();

        tDLogD("HttpClient::Impl::onBeginTls() Update state: Not ready -> Hello");
        assert(_state == TlsState::TS_NOT_READY);
        _state = TlsState::TS_HELLO;
    }

    virtual void onEnd() override
    {
        PARENT->onEnd();
    }

    virtual void onRecv(char const * buffer, std::size_t size) override
    {
        if (ENABLE_TLS) {
            onRecvTls(buffer, size);
        } else {
            _reader.parse(buffer, size);
        }
    }

    void onRecvTls(char const * buffer, std::size_t size)
    {
        switch (_state) {
        case TlsState::TS_NOT_READY:
            PARENT->onError(Err::E_ILLSTATE);
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
            PARENT->onError(Err::E_UNKNOWN);
            return;
        }
    }

private:
    void onHandshakeHello(char const * buffer, std::size_t size)
    {
        assert(_state == TlsState::TS_HELLO);

        Err const READ_BUFFER_CODE = _tls.writeToReadBuffer(buffer, size);
        if (isFailure(READ_BUFFER_CODE)) {
            PARENT->onError(READ_BUFFER_CODE);
            return;
        }

        Err const HANDSHAKE_CODE = _tls.handshake();
        if (HANDSHAKE_CODE != Err::E_SSLWREAD) {
            PARENT->onError(HANDSHAKE_CODE);
            return;
        }

        // Handshake is not finished, we can ignore it.
        assert(HANDSHAKE_CODE == Err::E_SSLWREAD);

        Buffer write_buffer;
        _tls.readFromWriteBuffer(write_buffer); // Skip error code check.

        if (!write_buffer.empty()) {
            Err const WRITE_CODE = send(write_buffer.data(), write_buffer.size());
            if (isSuccess(WRITE_CODE)) {
                tDLogD("HttpClient::Impl::onHandshakeHello() SSL connection using {}", _tls.getCipherName());
                _state = TlsState::TS_EXCHANGE_KEY;
            } else {
                PARENT->onError(WRITE_CODE);
                return;
            }
        }
    }

    void onHandshakeExchangeKey(char const * buffer, std::size_t size)
    {
        assert(_state == TlsState::TS_EXCHANGE_KEY);

        Err const READ_BUFFER_CODE = _tls.writeToReadBuffer(buffer, size);
        if (isFailure(READ_BUFFER_CODE)) {
            PARENT->onError(READ_BUFFER_CODE);
            return;
        }

        Err const HANDSHAKE_CODE = _tls.handshake(); // Skip error code check.

        if (_tls.isFinished()) {
            tDLogD("HttpClient::Impl::onHandshakeExchangeKey() Handshake finish!");
            _state = TlsState::TS_FINISH;
            PARENT->onBegin();
        } else {
            PARENT->onError(HANDSHAKE_CODE);
        }
    }

    void onApplication(char const * buffer, std::size_t size)
    {
        assert(_state == TlsState::TS_FINISH);

        Err code = Err::E_UNKNOWN;

        auto cursor = _tls.decode(buffer, size, &code);
        if (code == Err::E_SSLWREAD || code == Err::E_SSLEREAD) {
            tDLogD("HttpClient::Impl::onApplication() Decode #1 result: {}", getErrName(code));
            return;
        } else if (isFailure(code)) {
            PARENT->onError(code);
            return;
        }

        code = _reader.parse(cursor.data(), cursor.size());
        tDLogD("HttpClient::Impl::onApplication() result({}), read({})", getErrName(code), cursor.size());

        while (code == Err::E_CONTINUE) {
            cursor = _tls.decode(&code);
            if (code == Err::E_SSLWREAD || code == Err::E_SSLEREAD) {
                tDLogD("HttpClient::Impl::onApplication() Decode #2 result: {}", getErrName(code));
                return;
            } else if (isFailure(code)) {
                PARENT->onError(code);
                return;
            }
            code = _reader.parse(cursor.data(), cursor.size());
            tDLogD("HttpClient::Impl::onApplication() result({}), read({})", getErrName(code), cursor.size());
        }
    }

public:
    virtual void onContinue(void * arg) override
    {
        // EMPTY.
    }

    virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
    {
        return PARENT->onSwitchingProtocol(static_cast<HttpResponse>(property));
    }

    virtual void onWsMessage(WsOpCode opcode, Buffer const & payload, void * arg) override
    {
        PARENT->onWsMessage(opcode, payload);
    }

    virtual void onRegularHttp(HttpProperty const & property, void * arg) override
    {
        PARENT->onRegularHttp(static_cast<HttpResponse>(property));
    }

    virtual void onParseError(Err code, void * arg) override
    {
        PARENT->onError(code);
    }

public:
    Err writeRequest()
    {
        return writeRequest(HttpRequest());
    }

    Err writeRequest(HttpRequest const & request)
    {
        HttpRequest update_request = request;
        libtbag::http::updateDefaultRequest(update_request);

        if (ENABLE_TLS) {
            return sendTls(libtbag::http::toRequestString(update_request));
        } else {
            return send(libtbag::http::toRequestString(update_request));
        }
    }

public:
    Err writeWsRequest()
    {
        HttpRequest request;
        request.method = libtbag::http::getHttpMethodName(libtbag::http::HttpMethod::M_GET);
        libtbag::http::insert(request.header, libtbag::http::HEADER_HOST, std::string());
        libtbag::http::insert(request.header, libtbag::http::HEADER_ORIGIN, std::string());
        return writeWsRequest(request);
    }

    Err writeWsRequest(HttpRequest const & request)
    {
        HttpRequest ws_request = request;
        libtbag::http::updateDefaultWsRequest(ws_request, _reader.getKey());

        if (!libtbag::http::exists(ws_request.header, libtbag::http::HEADER_ORIGIN)) {
            tDLogW("HttpClient::Impl::writeWsRequest() Not found {} header.", libtbag::http::HEADER_ORIGIN);
        }
        if (libtbag::http::getHttpMethod(ws_request.method) != libtbag::http::HttpMethod::M_GET) {
            tDLogW("HttpClient::Impl::writeWsRequest() Not a GET method: {}", request.method);
        }

        if (ENABLE_TLS) {
            return sendTls(libtbag::http::toRequestString(ws_request));
        } else {
            return send(libtbag::http::toRequestString(ws_request));
        }
    }

public:
    Err writeWsFrame(WsFrame const & frame)
    {
        if (!_reader.isEnableWebsocket()) {
            return Err::E_ILLSTATE;
        }

        Buffer buffer;
        auto const SIZE = frame.copyTo(buffer);

        if (SIZE == 0) {
            tDLogE("HttpClient::Impl::writeWsFrame() WsFrame -> Buffer copy error");
            return Err::E_ECOPY;
        }

        if (ENABLE_TLS) {
            return sendTls(buffer.data(), buffer.size());
        } else {
            return send(buffer.data(), buffer.size());
        }
    }

    Err writeText(char const * buffer, std::size_t size, bool finish = true)
    {
        WsFrame frame;
        frame.text(buffer, size, _masking.gen(), finish);
        return writeWsFrame(frame);
    }

    Err writeText(std::string const & text, bool finish = true)
    {
        return writeText(text.data(), text.size(), finish);
    }

    Err writeBinary(char const * buffer, std::size_t size, bool finish = true)
    {
        WsFrame frame;
        frame.binary(buffer, size, _masking.gen(), finish);
        return writeWsFrame(frame);
    }

    Err writeBinary(Buffer const & buffer, bool finish = true)
    {
        return writeBinary(buffer.data(), buffer.size(), finish);
    }

    Err writeClose()
    {
        WsFrame frame;
        frame.close(_masking.gen());
        return writeWsFrame(frame);
    }
};

// --------------------------
// HttpClient implementation.
// --------------------------

HttpClient::HttpClient(MqParams const & params, bool use_websocket, bool enable_tls)
        : HttpClient(params, libtbag::http::generateRandomWebSocketKey(), use_websocket, enable_tls)
{
    assert(static_cast<bool>(_impl));
}

HttpClient::HttpClient(MqParams const & params, std::string const & key, bool use_websocket, bool enable_tls)
        : _impl(std::make_unique<Impl>(this, params, key, use_websocket, enable_tls))
{
    assert(static_cast<bool>(_impl));
}

HttpClient::~HttpClient()
{
    // EMPTY.
}

void HttpClient::onBegin()
{
    // EMPTY.
}

void HttpClient::onEnd()
{
    // EMPTY.
}

void HttpClient::onRegularHttp(HttpResponse const & response)
{
    // EMPTY.
}

bool HttpClient::onSwitchingProtocol(HttpResponse const & response)
{
    return true;
}

void HttpClient::onWsMessage(WsOpCode opcode, Buffer const & payload)
{
    // EMPTY.
}

void HttpClient::onError(Err code)
{
    assert(static_cast<bool>(_impl));
    auto const EXIT_CODE = _impl->exit();
    tDLogE("HttpClient::onError({}) Exit request: {}", code, EXIT_CODE);
}

Err HttpClient::writeRequest()
{
    assert(static_cast<bool>(_impl));
    return _impl->writeRequest();
}

Err HttpClient::writeRequest(HttpRequest const & request)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeRequest(request);
}

Err HttpClient::writeWsRequest()
{
    assert(static_cast<bool>(_impl));
    return _impl->writeWsRequest();
}

Err HttpClient::writeWsRequest(HttpRequest const & request)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeWsRequest(request);
}

Err HttpClient::writeWsFrame(WsFrame const & frame)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeWsFrame(frame);
}

Err HttpClient::writeText(char const * buffer, std::size_t size, bool finish)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeText(buffer, size, finish);
}

Err HttpClient::writeText(std::string const & text, bool finish)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeText(text, finish);
}

Err HttpClient::writeBinary(char const * buffer, std::size_t size, bool finish)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeBinary(buffer, size, finish);
}

Err HttpClient::writeBinary(Buffer const & buffer, bool finish)
{
    assert(static_cast<bool>(_impl));
    return _impl->writeBinary(buffer, finish);
}

Err HttpClient::writeClose()
{
    assert(static_cast<bool>(_impl));
    return _impl->writeClose();
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

