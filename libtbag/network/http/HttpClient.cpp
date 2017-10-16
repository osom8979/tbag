/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2017-05-20
 */

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/log/Log.hpp>

//#define TBAG_WS_CLIENT_CHECK_BUFFER

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpClient::HttpClient(Loop & loop, StreamType type) : Parent(loop, type), _reader(this)
{
    // EMPTY.
}

HttpClient::~HttpClient()
{
    // EMPTY.
}

std::string HttpClient::getKey() const
{
    return _reader.getKey();
}

Err HttpClient::writeRequest(HttpRequest const & request)
{
    HttpRequest update_request = request;
    update_request.updateDefaultRequest();
    std::string const & REQUEST_STRING = update_request.toRequestString();
    return write(REQUEST_STRING.data(), REQUEST_STRING.size());
}

Err HttpClient::writeWsRequest(HttpRequest const & request)
{
    HttpRequest ws_request = request;
    ws_request.updateDefaultWsRequest(_reader.getKey());

    if (ws_request.exists(HEADER_ORIGIN) == false) {
        tDLogW("HttpClient::writeWsRequest() Not found {} header.", HEADER_ORIGIN);
    }
    if (ws_request.getHttpMethod() != HttpMethod::M_GET) {
        tDLogW("HttpClient::writeWsRequest() Not a GET method: {}", request.method);
    }

    std::string const & REQUEST_STRING = ws_request.toRequestString();
    return write(REQUEST_STRING.data(), REQUEST_STRING.size());
}

Err HttpClient::writeWsFrame(WsFrame const & frame)
{
    if (_reader.isEnableWebsocket() == false) {
        return Err::E_ILLSTATE;
    }

    util::Buffer buffer;
    std::size_t const SIZE = frame.copyTo(buffer);
    if (SIZE == 0) {
        tDLogE("HttpClient::writeWsFrame() WsFrame -> Buffer copy error");
        return Err::E_ECOPY;
    }
    return write(buffer.data(), buffer.size());
}

Err HttpClient::writeText(char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.text(buffer, size, _device.gen(), finish);
    return writeWsFrame(frame);
}

Err HttpClient::writeText(std::string const & text, bool finish)
{
    return writeText(text.data(), text.size(), finish);
}

Err HttpClient::writeBinary(char const * buffer, std::size_t size, bool finish)
{
    WsFrame frame;
    frame.binary(buffer, size, _device.gen(), finish);
    return writeWsFrame(frame);
}

Err HttpClient::writeBinary(util::Buffer const & binary, bool finish)
{
    return writeBinary(binary.data(), binary.size(), finish);
}

Err HttpClient::writeClose()
{
    WsFrame frame;
    frame.close(_device.gen());
    return writeWsFrame(frame);
}

void HttpClient::onConnect(Err code)
{
    if (isFailure(code)) {
        onError(EventType::ET_CONNECT, code);
        return;
    }

    Err const START_CODE = start();
    if (isFailure(START_CODE)) {
        onError(EventType::ET_START, START_CODE);
        return;
    }

    onOpen();
}

void HttpClient::onRead(Err code, ReadPacket const & packet)
{
    if (code == Err::E_EOF) {
        onEof();
    } else if (code != Err::E_SUCCESS) {
        onError(EventType::ET_READ, code);
    } else {
        _reader.parse(packet.buffer, packet.size);
    }
}

void HttpClient::onParseError(Err code, void * arg)
{
    close();
}

void HttpClient::onOpen()
{
    // EMPTY.
}

void HttpClient::onEof()
{
    close();
}

void HttpClient::onError(EventType from, Err code)
{
    close();
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

