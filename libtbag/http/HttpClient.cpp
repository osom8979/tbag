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

public:
    STATIC_ASSERT_CHECK_IS_SAME(Buffer, HttpReaderInterface::Buffer);

public:
    HttpClient * const PARENT;

private:
    Reader  _reader;
    Masking _masking;

public:
    Impl(HttpClient * parent, MqParams const & params, std::string const & key, bool use_websocket)
            : NetStreamClient(params), PARENT(parent), _reader(this, key, use_websocket)
    {
        assert(PARENT != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    inline bool isEnableWebSocket() const TBAG_NOEXCEPT
    {
        return _reader.isEnableWebsocket();
    }

    std::string getKey() const
    {
        return _reader.getKey();
    }

    uint32_t genMaskingKey()
    {
        return _masking.gen();
    }

private:
    virtual void onBegin() override
    {
        PARENT->onBegin();
    }

    virtual void onEnd() override
    {
        PARENT->onEnd();
    }

    virtual void onRecv(char const * buffer, std::size_t size) override
    {
        _reader.parse(buffer, size);
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
        return PARENT->onWsMessage(opcode, payload);
    }

    virtual void onRegularHttp(HttpProperty const & property, void * arg) override
    {
        return PARENT->onRegularHttp(static_cast<HttpResponse>(property));
    }

    virtual void onParseError(Err code, void * arg) override
    {
        return PARENT->onError(code);
    }
};

// ----------------------------
// HttpClient implementation.
// ----------------------------

HttpClient::HttpClient(std::string const & uri, bool use_websocket)
        : HttpClient(libtbag::mq::details::convertUriToParams(uri), use_websocket)
{
    assert(static_cast<bool>(_impl));
}

HttpClient::HttpClient(std::string const & uri, std::string const & key, bool use_websocket)
        : HttpClient(libtbag::mq::details::convertUriToParams(uri), key, use_websocket)
{
    assert(static_cast<bool>(_impl));
}

HttpClient::HttpClient(MqParams const & params, bool use_websocket)
        : HttpClient(params, libtbag::http::generateRandomWebSocketKey(), use_websocket)
{
    assert(static_cast<bool>(_impl));
}

HttpClient::HttpClient(MqParams const & params, std::string const & key, bool use_websocket)
        : _impl(std::make_unique<Impl>(this, params, key, use_websocket))
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
    // EMPTY.
}

Err HttpClient::writeRequest()
{
    return writeRequest(HttpRequest());
}

Err HttpClient::writeRequest(HttpRequest const & request)
{
    assert(static_cast<bool>(_impl));
    HttpRequest update_request = request;
    libtbag::http::updateDefaultRequest(update_request);
    return _impl->send(libtbag::http::toRequestString(update_request));
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
    assert(static_cast<bool>(_impl));
    HttpRequest ws_request = request;
    libtbag::http::updateDefaultWsRequest(ws_request, _impl->getKey());

    if (!libtbag::http::exists(ws_request.header, libtbag::http::HEADER_ORIGIN)) {
        tDLogW("HttpClient::writeWsRequest() Not found {} header.", libtbag::http::HEADER_ORIGIN);
    }
    if (libtbag::http::getHttpMethod(ws_request.method) != libtbag::http::HttpMethod::M_GET) {
        tDLogW("HttpClient::writeWsRequest() Not a GET method: {}", request.method);
    }

    return _impl->send(libtbag::http::toRequestString(ws_request));
}

Err HttpClient::writeWsFrame(WsFrame const & frame)
{
    assert(static_cast<bool>(_impl));
    if (!_impl->isEnableWebSocket()) {
        return Err::E_ILLSTATE;
    }

    Buffer buffer;
    auto const SIZE = frame.copyTo(buffer);

    if (SIZE == 0) {
        tDLogE("HttpClient::writeWsFrame() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return _impl->send(buffer.data(), buffer.size());
}

Err HttpClient::writeText(char const * buffer, std::size_t size, bool finish)
{
    assert(static_cast<bool>(_impl));
    WsFrame frame;
    frame.text(buffer, size, _impl->genMaskingKey(), finish);
    return writeWsFrame(frame);
}

Err HttpClient::writeText(std::string const & text, bool finish)
{
    return writeText(text.data(), text.size(), finish);
}

Err HttpClient::writeBinary(char const * buffer, std::size_t size, bool finish)
{
    assert(static_cast<bool>(_impl));
    WsFrame frame;
    frame.binary(buffer, size, _impl->genMaskingKey(), finish);
    return writeWsFrame(frame);
}

Err HttpClient::writeBinary(Buffer const & buffer, bool finish)
{
    return writeBinary(buffer.data(), buffer.size(), finish);
}

Err HttpClient::writeClose()
{
    assert(static_cast<bool>(_impl));
    WsFrame frame;
    frame.close(_impl->genMaskingKey());
    return writeWsFrame(frame);
}

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

