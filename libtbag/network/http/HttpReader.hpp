/**
 * @file   HttpReader.hpp
 * @brief  HttpReader class prototype.
 * @author zer0
 * @date   2017-10-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPREADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPREADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/http/HttpCommon.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/ws/WsFrameBuffer.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpReader interface.
 *
 * @author zer0
 * @date   2017-10-06
 */
struct HttpReaderInterface
{
    /** Incomplete (Parsing is not finish). Wait for the next packet. */
    virtual void onContinue(void * arg) { /* EMPTY. */ }

    /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
    virtual bool onSwitchingProtocol(HttpProperty const & response, void * arg) { return true; }

    /** When a message has been received from WebSocket client/server. */
    virtual void onWsMessage(ws::WsOpCode opcode, util::Buffer const & payload, void * arg) { /* EMPTY. */ }

    /** Regular http message. */
    virtual void onRegularHttp(HttpProperty const & response, void * arg) { /* EMPTY. */ }

    /** Triggered when error occurred. */
    virtual void onParseError(Err code, void * arg) { /* EMPTY. */ }
};

/**
 * HttpReader class prototype.
 *
 * @author zer0
 * @date   2017-10-03
 */
class TBAG_API HttpReader : public HttpParser, public HttpReaderInterface
{
private:
    ws::WsFrameBuffer _frame_buffer;
    std::string _key;

private:
    bool _enable_websocket;
    bool _switching_protocol;

public:
    HttpReader(bool use_websocket = true);
    HttpReader(std::string const & key, bool use_websocket = true);
    virtual ~HttpReader();

public:
    std::string getKey() const { return _key; }

public:
    inline bool isEnableWebsocket  () const TBAG_NOEXCEPT { return _enable_websocket;   }
    inline bool isSwitchingProtocol() const TBAG_NOEXCEPT { return _switching_protocol; }
    inline bool isWsReady() const TBAG_NOEXCEPT { return _enable_websocket && _switching_protocol; }

public:
    Err parse(char const * buffer, std::size_t size, void * arg = nullptr);

protected:
    virtual void onContinue(void * arg) override { /* EMPTY. */ }
    virtual bool onSwitchingProtocol(HttpProperty const & response, void * arg) override { return true; }
    virtual void onWsMessage(ws::WsOpCode opcode, util::Buffer const & payload, void * arg) override { /* EMPTY. */ }
    virtual void onRegularHttp(HttpProperty const & response, void * arg) override { /* EMPTY. */ }
    virtual void onParseError(Err code, void * arg) override { /* EMPTY. */ }
};

/**
 * HttpReader for the callback style.
 *
 * @author zer0
 * @date   2017-10-03
 */
template <typename ByPassType>
struct HttpReaderForCallback : public HttpReader
{
    using Parent = ByPassType;

    Parent * parent;

    HttpReaderForCallback(Parent * p, bool use_websocket = true) : HttpReader(use_websocket), parent(p)
    { /* EMPTY. */ }
    HttpReaderForCallback(Parent * p, std::string const & key, bool use_websocket = true) : HttpReader(key, use_websocket), parent(p)
    { /* EMPTY. */ }
    virtual ~HttpReaderForCallback()
    { /* EMPTY. */ }

    virtual void onContinue(void * arg) override
    { parent->onContinue(arg); }
    virtual bool onSwitchingProtocol(HttpProperty const & response, void * arg) override
    { return parent->onSwitchingProtocol(response, arg); }
    virtual void onWsMessage(ws::WsOpCode opcode, util::Buffer const & payload, void * arg) override
    { parent->onWsMessage(opcode, payload, arg); }
    virtual void onRegularHttp(HttpProperty const & response, void * arg) override
    { parent->onRegularHttp(response, arg); }
    virtual void onParseError(Err code, void * arg) override
    { parent->onParseError(code, arg); }
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPREADER_HPP__

