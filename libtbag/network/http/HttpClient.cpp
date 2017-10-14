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

Err HttpClient::writeRequest(HttpProperty const & property)
{
    std::string const & REQUEST_STRING = property.toRequestString();
    return write(REQUEST_STRING.data(), REQUEST_STRING.size());
}

void HttpClient::onConnect(Err code)
{
    if (TBAG_ERR_FAILURE(code)) {
        onError(EventType::ET_CONNECT, code);
        return;
    }

    Err const START_CODE = start();
    if (TBAG_ERR_FAILURE(START_CODE)) {
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

