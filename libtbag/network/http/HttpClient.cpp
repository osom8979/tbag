/**
 * @file   HttpClient.cpp
 * @brief  HttpClient class implementation.
 * @author zer0
 * @date   2017-05-20
 */

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/log/Log.hpp>

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

Err HttpClient::writeRequest()
{
    return writeRequest(HttpRequest());
}

Err HttpClient::writeRequest(HttpRequest const & request)
{
    HttpRequest update_request = request;
    update_request.updateDefaultRequest();
    std::string const & REQUEST_STRING = update_request.toRequestString();
    return write(REQUEST_STRING.data(), REQUEST_STRING.size());
}

Err HttpClient::writeWsRequest()
{
    HttpRequest request;
    request.setHttpMethod(HttpMethod::M_GET);
    request.insert(HEADER_HOST, std::string());
    request.insert(HEADER_ORIGIN, std::string());
    return writeWsRequest(request);
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

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

