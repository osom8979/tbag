/**
 * @file   HttpCacheData.cpp
 * @brief  HttpCacheData class implementation.
 * @author zer0
 * @date   2017-07-02
 */

#include <libtbag/network/http/HttpCacheData.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

HttpCacheData::HttpCacheData(ClientInterface * client) : _client(client)
{
    clear();
}

HttpCacheData::~HttpCacheData()
{
    // EMPTY.
}

void HttpCacheData::clear()
{
    builder.clear();
    parser.clear();
    parser.clearCache();
    ws.upgrade = false;
    ws.sender.clear();
    ws.receiver.clear();
    ws.write_buffer.clear();
    ws.key.clear();
    ws.protocols.clear();
}

void HttpCacheData::generateWebSocketKey()
{
    ws.key = generateRandomWebSocketKey();
}

void HttpCacheData::clearProtocol()
{
    ws.protocols.clear();
}

void HttpCacheData::addProtocol(std::string const & protocol)
{
    ws.protocols.push_back(protocol);
}

Err HttpCacheData::writeSendWsFrame()
{
    assert(_client != nullptr);
    std::size_t const SIZE = ws.sender.copyTo(ws.write_buffer);
    return _client->write((char const *)&ws.write_buffer[0], SIZE);
}

Err HttpCacheData::writeTextRequest(std::string const & text, bool continuation, bool finish)
{
    Err const CODE = ws.sender.text(text, ws.device.gen(), continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeTextRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeTextResponse(std::string const & text, bool continuation, bool finish)
{
    Err const CODE = ws.sender.text(text, continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeTextResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeBinaryRequest(Buffer const & buffer, bool continuation, bool finish)
{
    Err const CODE = ws.sender.binary(buffer, ws.device.gen(), continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeBinaryRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeBinaryResponse(Buffer const & buffer, bool continuation, bool finish)
{
    Err const CODE = ws.sender.binary(buffer, continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeBinaryResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePingRequest(std::string const & str)
{
    Err const CODE = ws.sender.ping(str, ws.device.gen());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePingRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePingResponse(std::string const & str)
{
    Err const CODE = ws.sender.ping(str);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePingResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePongRequest(std::string const & str)
{
    Err const CODE = ws.sender.pong(str, ws.device.gen());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePongRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePongResponse(std::string const & str)
{
    Err const CODE = ws.sender.pong(str);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePongResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeCloseRequest()
{
    Err const CODE = ws.sender.close(ws.device.gen());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeCloseRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeCloseResponse(uint16_t status_code, std::string const & reason)
{
    Err const CODE = ws.sender.close(status_code, reason);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeCloseResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeCloseResponse(WebSocketStatusCode code)
{
    return writeCloseResponse(getWsStatusCodeNumber(code), std::string(getWsStatusCodeName(code)));
}

Err HttpCacheData::writeCloseResponse()
{
    return writeCloseResponse(ws.code);
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

