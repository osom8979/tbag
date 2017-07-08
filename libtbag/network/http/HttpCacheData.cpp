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

HttpCacheData::HttpCacheData(ClientInterface * client)
        : _client(client), upgrade(false),
          send_frame(), recv_frame(), send_buffer(),
          key(), protocols(), builder(), parser()
{
    // EMPTY.
}

HttpCacheData::~HttpCacheData()
{
    // EMPTY.
}

Err HttpCacheData::writeSendWsFrame()
{
    assert(_client != nullptr);
    std::size_t const SIZE = send_frame.copyTo(send_buffer);
    return _client->write((char const *)&send_buffer[0], SIZE);
}

Err HttpCacheData::writeTextRequest(std::string const & text, bool continuation, bool finish)
{
    Err const CODE = send_frame.text(text, device.gen(), continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeTextRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeTextResponse(std::string const & text, bool continuation, bool finish)
{
    Err const CODE = send_frame.text(text, continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeTextResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeBinaryRequest(Buffer const & buffer, bool continuation, bool finish)
{
    Err const CODE = send_frame.binary(buffer, device.gen(), continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeBinaryRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeBinaryResponse(Buffer const & buffer, bool continuation, bool finish)
{
    Err const CODE = send_frame.binary(buffer, continuation, finish);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeBinaryResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePingRequest(Buffer const & buffer)
{
    Err const CODE = send_frame.ping(buffer.data(), buffer.size(), device.gen());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePingRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePingResponse(Buffer const & buffer)
{
    Err const CODE = send_frame.ping(buffer.data(), buffer.size());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePingResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePongRequest(Buffer const & buffer)
{
    Err const CODE = send_frame.pong(buffer.data(), buffer.size(), device.gen());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePongRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writePongResponse(Buffer const & buffer)
{
    Err const CODE = send_frame.pong(buffer.data(), buffer.size());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writePongResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeCloseRequest()
{
    Err const CODE = send_frame.close(device.gen());
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeCloseRequest() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

Err HttpCacheData::writeCloseResponse(uint16_t status_code, std::string const & reason)
{
    Err const CODE = send_frame.close(status_code, reason);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("HttpCacheData::writeCloseResponse() WebSocket frame {} error.", getErrName(CODE));
        return CODE;
    }
    return writeSendWsFrame();
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

