/**
 * @file   HttpClient.hpp
 * @brief  HttpClient class prototype.
 * @author zer0
 * @date   2017-05-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/random/MaskingDevice.hpp>
#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/http/base/HttpReader.hpp>
#include <libtbag/uvpp/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * HttpClient class prototype.
 *
 * @author zer0
 * @date   2017-05-19
 */
class TBAG_API HttpClient : public stream::StreamClient, public HttpReaderInterface
{
public:
    using StreamType = details::StreamType;
    using Parent     = stream::StreamClient;
    using Loop       = uvpp::Loop;
    using Masking    = random::MaskingDevice;
    using WsFrame    = WsFrame;

    enum class EventType
    {
        ET_CONNECT,
        ET_START,
        ET_READ
    };

private:
    HttpReaderForCallback<HttpClient> _reader;
    Masking _device;

public:
    HttpClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpClient();

public:
    std::string getKey() const;

public:
    Err writeRequest(HttpRequest const & request);
    Err writeWsRequest(HttpRequest const & request);
    Err writeWsFrame(WsFrame const & frame);

public:
    Err writeText(char const * buffer, std::size_t size, bool finish = true);
    Err writeText(std::string const & text, bool finish = true);

    Err writeBinary(char const * buffer, std::size_t size, bool finish = true);
    Err writeBinary(util::Buffer const & binary, bool finish = true);

    Err writeClose();

public:
    virtual void onConnect(Err code) override;
    virtual void onRead   (Err code, ReadPacket const & packet) override;

public:
    virtual void onParseError(Err code, void * arg) override;

public:
    virtual void onOpen();
    virtual void onEof();

public:
    virtual void onError(EventType from, Err code);
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCLIENT_HPP__

