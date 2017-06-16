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
#include <libtbag/Type.hpp>

#include <libtbag/network/stream/StreamClient.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <functional>
#include <chrono>
#include <vector>

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
class TBAG_API HttpClient : public stream::StreamClient
{
public:
    using StreamType = details::StreamType;
    using Parent     = stream::StreamClient;

public:
    using Loop   = uvpp::Loop;
    using Buffer = std::vector<char>;

    using SystemClock = std::chrono::system_clock;
    using TimePoint   = SystemClock::time_point;
    using Millisec    = std::chrono::milliseconds;
    using Seconds     = std::chrono::seconds;

    using OnConnect  = std::function<void(Err)>;
    using OnResponse = std::function<void(Err, HttpParser const &)>;
    using OnShutdown = std::function<void(Err)>;
    using OnWrite    = std::function<void(Err)>;
    using OnClose    = std::function<void(void)>;

private:
    HttpBuilder _builder;
    HttpParser  _parser;

private:
    OnConnect  _connect_cb;
    OnResponse _response_cb;
    OnShutdown _shutdown_cb;
    OnWrite    _write_cb;
    OnClose    _close_cb;

private:
    Millisec  _timeout;
    TimePoint _start_time;

public:
    HttpClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpClient();

public:
    // @formatter:off
    inline HttpBuilder       & atBuilder()       TBAG_NOEXCEPT { return _builder; }
    inline HttpBuilder const & atBuilder() const TBAG_NOEXCEPT { return _builder; }
    inline HttpParser        & atParser ()       TBAG_NOEXCEPT { return _parser;  }
    inline HttpParser  const & atParser () const TBAG_NOEXCEPT { return _parser;  }

    inline void setOnConnect (OnConnect  const & cb) { _connect_cb  = cb; }
    inline void setOnResponse(OnResponse const & cb) { _response_cb = cb; }
    inline void setOnShutdown(OnShutdown const & cb) { _shutdown_cb = cb; }
    inline void setOnWrite   (OnWrite    const & cb) { _write_cb    = cb; }
    inline void setOnClose   (OnClose    const & cb) { _close_cb    = cb; }

    inline void setRequest(HttpBuilder const & request) { _builder = request; }
    inline void setTimeout(Millisec const & timeout = Millisec(0)) { _timeout = timeout; }
    inline void markupTimer() { _start_time = SystemClock::now(); }
    // @formatter:on

public:
    void setup(HttpBuilder const & request, Millisec const & timeout = Millisec(0))
    {
        setRequest(request);
        setTimeout(timeout);
        markupTimer();
    }

public:
    virtual void onConnect(Err code) override;
    virtual void onShutdown(Err code) override;
    virtual void onWrite(Err code) override;
    virtual void onRead(Err code, ReadPacket const & packet) override;
    virtual void onClose() override;
};

// ----------
// Utilities.
// ----------

TBAG_API Err requestWithSync(HttpClient::StreamType type,
                             std::string const & host,
                             int port,
                             Uri const & uri,
                             HttpRequest const & request,
                             uint64_t timeout,
                             HttpResponse & result);

TBAG_API Err requestWithSync(Uri const & uri, HttpRequest const & request, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(std::string const & uri, HttpRequest const & request, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(Uri const & uri, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(std::string const & uri, uint64_t timeout, HttpResponse & result);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCLIENT_HPP__

