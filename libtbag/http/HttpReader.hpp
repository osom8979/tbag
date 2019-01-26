/**
 * @file   HttpReader.hpp
 * @brief  HttpReader class prototype.
 * @author zer0
 * @date   2017-10-03
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPREADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPREADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/http/HttpCommon.hpp>
#include <libtbag/http/HttpParser.hpp>
#include <libtbag/http/WsFrameBuffer.hpp>
#include <libtbag/Type.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpReader interface.
 *
 * @author zer0
 * @date   2017-10-06
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
 */
struct HttpReaderInterface
{
    using Buffer = libtbag::util::Buffer;

    STATIC_ASSERT_CHECK_IS_SAME(Buffer, HttpBuffer);

    HttpReaderInterface() { /* EMPTY. */ }
    virtual ~HttpReaderInterface() { /* EMPTY. */ }

    /** Incomplete (Parsing is not finish). Wait for the next packet. */
    virtual void onContinue(void * arg) { /* EMPTY. */ }

    /**
     * Protocol switching.
     *
     * @return
     *  Returns true to allow Protocol switching.
     *
     * @remarks
     *  When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake.
     */
    virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) { return true; }

    /** When a message has been received from WebSocket client/server. */
    virtual void onWsMessage(WsOpCode opcode, Buffer const & payload, void * arg) { /* EMPTY. */ }

    /** Regular http message. */
    virtual void onRegularHttp(HttpProperty const & property, void * arg) { /* EMPTY. */ }

    /** Triggered when error occurred. */
    virtual void onParseError(Err code, void * arg) { /* EMPTY. */ }
};

/**
 * HttpReader class prototype.
 *
 * @author zer0
 * @date   2017-10-03
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
 */
class TBAG_API HttpReader : public HttpParser, public HttpReaderInterface
{
private:
    WsFrameBuffer _frame_buffer;
    std::string _key;

private:
    bool _enable_websocket;
    bool _switching_protocol;

public:
    HttpReader(bool use_websocket = true);
    HttpReader(std::string const & key, bool use_websocket = true);
    virtual ~HttpReader();

public:
    void setKey(std::string const & key) { _key = key; }
    std::string getKey() const { return _key; }

public:
    inline bool isEnableWebsocket  () const TBAG_NOEXCEPT { return _enable_websocket;   }
    inline bool isSwitchingProtocol() const TBAG_NOEXCEPT { return _switching_protocol; }
    inline bool isWsReady() const TBAG_NOEXCEPT { return _enable_websocket && _switching_protocol; }

public:
    Err parse(char const * buffer, std::size_t size, void * arg = nullptr);
};

/**
 * HttpReader for the callback style.
 *
 * @author zer0
 * @date   2017-10-03
 * @date   2018-12-25 (Change namespace: libtbag::network::http::base -> libtbag::http)
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
    virtual bool onSwitchingProtocol(HttpProperty const & property, void * arg) override
    { return parent->onSwitchingProtocol(property, arg); }
    virtual void onWsMessage(WsOpCode opcode, util::Buffer const & payload, void * arg) override
    { parent->onWsMessage(opcode, payload, arg); }
    virtual void onRegularHttp(HttpProperty const & property, void * arg) override
    { parent->onRegularHttp(property, arg); }
    virtual void onParseError(Err code, void * arg) override
    { parent->onParseError(code, arg); }
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPREADER_HPP__

