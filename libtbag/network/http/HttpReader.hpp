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

#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/network/http/common/HttpParser.hpp>
#include <libtbag/network/http/ws/WsCommon.hpp>
#include <libtbag/network/http/ws/WsFrameBuffer.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpReader class prototype.
 *
 * @author zer0
 * @date   2017-10-03
 */
class TBAG_API HttpReader : public common::HttpParser
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
    inline bool isEnableWebsocket  () const TBAG_NOEXCEPT { return _enable_websocket;   }
    inline bool isSwitchingProtocol() const TBAG_NOEXCEPT { return _switching_protocol; }

public:
    Err parse(char const * buffer, std::size_t size, void * arg = nullptr);

protected:
    /** Parsing is not finish. Wait for the next packet. */
    void onContinue(void * arg) { /* EMPTY. */ }

    /** When a socket has opened, i.e. after TCP three-way handshake and WebSocket handshake. */
    bool onSwitchingProtocol(common::HttpProperty const & response, void * arg) { return true; }

    /** When a message has been received from WebSocket client/server. */
    void onWsMessage(ws::WsOpCode opcode, util::Buffer const & payload, void * arg) { /* EMPTY. */ }

    /** Regular http message. */
    void onRegularHttp(common::HttpProperty const & response, void * arg) { /* EMPTY. */ }

    /** Triggered when error occurred. */
    void onParseError(Err code, void * arg) { /* EMPTY. */ }
};

// ------------------------
// Miscellaneous utilities.
// ------------------------

TBAG_API bool testWsRequest (common::HttpProperty const & request, int test_version = ws::WEBSOCKET_VERSION_HYBI13);
TBAG_API bool testWsResponse(common::HttpProperty const & response, std::string const & key);
TBAG_API bool testWsVersion (common::HttpProperty const & property, int test_version);

TBAG_API Err updateWsRequest (common::HttpProperty & request , std::string const & key);
TBAG_API Err updateWsResponse(common::HttpProperty & response, std::string const & key);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPREADER_HPP__

