/**
 * @file   HttpCacheData.hpp
 * @brief  HttpCacheData class prototype.
 * @author zer0
 * @date   2017-07-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCACHEDATA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCACHEDATA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/WebSocketFrame.hpp>
#include <libtbag/random/MaskingDevice.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpCacheData class prototype.
 *
 * @author zer0
 * @date   2017-07-02
 */
class TBAG_API HttpCacheData : private Noncopyable
{
public:
    using Buffer  = WebSocketFrame::Buffer;
    using Strings = std::vector<std::string>;

    using MaskingDevice   = random::MaskingDevice;
    using ClientInterface = details::ClientInterface;

private:
    ClientInterface * _client;

public:
    HttpBuilder builder;
    HttpParser  parser;

public:
    struct {
        bool upgrade;
        MaskingDevice device;

        WebSocketFrame sender;
        WebSocketFrame receiver;
        Buffer write_buffer;

        std::string key;
        Strings protocols;

        WebSocketStatusCode code;
    } ws; ///< WebSocket properties.

public:
    HttpCacheData(ClientInterface * client = nullptr);
    virtual ~HttpCacheData();

public:
    inline bool isUpgrade() const TBAG_NOEXCEPT
    { return ws.upgrade; }

public:
    void clear();

public:
    void generateWebSocketKey();

public:
    void clearProtocol();
    void addProtocol(std::string const & protocol);

public:
    Err writeSendWsFrame();

public:
    Err writeTextRequest(std::string const & text, bool continuation = false, bool finish = true);
    Err writeTextResponse(std::string const & text, bool continuation = false, bool finish = true);

    Err writeBinaryRequest(Buffer const & buffer, bool continuation = false, bool finish = true);
    Err writeBinaryResponse(Buffer const & buffer, bool continuation = false, bool finish = true);

    Err writePingRequest(std::string const & str);
    Err writePingResponse(std::string const & str);

    Err writePongRequest(std::string const & str);
    Err writePongResponse(std::string const & str);

    Err writeCloseRequest();
    Err writeCloseResponse(uint16_t status_code, std::string const & reason);
    Err writeCloseResponse(WebSocketStatusCode code);
    Err writeCloseResponse();
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCACHEDATA_HPP__

