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

    using OnResponse = std::function<void(Err, HttpParser const &)>;

private:
    HttpBuilder _builder;
    HttpParser  _parser;

private:
    OnResponse _response_cb;
    Millisec   _timeout;
    TimePoint  _start_time;

public:
    HttpClient(Loop & loop, StreamType type = StreamType::TCP);
    virtual ~HttpClient();

public:
    // @formatter:off
    inline HttpBuilder       & atBuilder()       TBAG_NOEXCEPT { return _builder; }
    inline HttpBuilder const & atBuilder() const TBAG_NOEXCEPT { return _builder; }
    inline HttpParser        & atParser ()       TBAG_NOEXCEPT { return _parser;  }
    inline HttpParser  const & atParser () const TBAG_NOEXCEPT { return _parser;  }
    // @formatter:on

public:
    void setup(HttpBuilder const & request, OnResponse const & response_cb, Millisec const & timeout = Millisec(0));

public:
    virtual void onConnect(Err code) override;
    virtual void onShutdown(Err code) override;
    virtual void onWrite(Err code) override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;
    virtual void onClose() override;
};

// ----------
// Utilities.
// ----------

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

