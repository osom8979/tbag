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

#include <libtbag/network/tcp/TcpClient.hpp>
#include <libtbag/network/pipe/PipeClient.hpp>
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
template <typename ClientType>
class HttpClient : public ClientType
{
public:
    using Parent = ClientType;

public:
    using Loop     = uvpp::Loop;
    using String   = HttpParser::String;
    using Size     = HttpParser::Size;
    using Buffer   = std::vector<char>;

    using SystemClock = std::chrono::system_clock;
    using TimePoint   = SystemClock::time_point;
    using Millisec    = std::chrono::milliseconds;
    using Seconds     = std::chrono::seconds;

    using OnResponse = std::function<void(Err, HttpParser const &)>;

private:
    HttpBuilder _builder;
    HttpParser  _parser;

private:
    Buffer     _buffer;
    OnResponse _response_cb;
    Millisec   _timeout;
    TimePoint  _start_time;

public:
    HttpClient(Loop & loop) : Parent(loop), _builder(), _parser(HttpParser::Type::RESPONSE)
    { /* EMPTY. */ }
    virtual ~HttpClient()
    { /* EMPTY. */ }

public:
    // @formatter:off
    inline HttpBuilder       & atBuilder()       TBAG_NOEXCEPT { return _builder; }
    inline HttpBuilder const & atBuilder() const TBAG_NOEXCEPT { return _builder; }
    inline HttpParser        & atParser ()       TBAG_NOEXCEPT { return _parser;  }
    inline HttpParser  const & atParser () const TBAG_NOEXCEPT { return _parser;  }
    // @formatter:on

public:
    void setup(HttpBuilder const & request,
               OnResponse const & response_cb,
               Millisec const & timeout = Millisec(0))
    {
        _builder     = request;
        _response_cb = response_cb;
        _timeout     = timeout;
        _start_time  = SystemClock::now();
    }

// TcpClient callback.
public:
    virtual void onConnect(Err code) override
    {
        if (code == Err::E_SUCCESS) {
            using namespace std::chrono;

            auto buffer = _builder.request();
            Millisec left_time = _timeout - duration_cast<Millisec>(SystemClock::now() - _start_time);
            if (this->write(buffer.data(), buffer.size(), left_time.count()) == false) {
                _response_cb(Err::E_WRERR, _parser);
                this->close();
            }
        } else {
            _response_cb(code, _parser);
            this->close();
        }
    }

    virtual void onShutdown(Err code) override
    {
        _response_cb(code == Err::E_SUCCESS ? Err::E_SHUTDOWN : code, _parser);
        this->close();
    }

    virtual void onWrite(Err code) override
    {
        if (code == Err::E_SUCCESS) {
            this->start();
        } else {
            _response_cb(code, _parser);
            this->close();
        }
    }

    virtual void onRead(Err code, char const * buffer, Size size) override
    {
        if (code == Err::E_EOF) {
            _parser.execute(buffer, size);
            _response_cb(Err::E_SUCCESS, _parser);

            this->stop();
            this->close();
        } else if (code == Err::E_SUCCESS) {
            _parser.execute(buffer, size);
            if (_parser.isComplete()) {
                _response_cb(Err::E_SUCCESS, _parser);

                this->stop();
                this->close();
            }
        } else {
            _response_cb(code, _parser);
            this->stop();
            this->close();
        }
    }
};

using TcpHttpClient  = HttpClient<tcp::TcpClient>;
using PipeHttpClient = HttpClient<pipe::PipeClient>;

TBAG_API Err requestWithSync(Uri const & uri, HttpBuilder const & request, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(Uri const & uri, uint64_t timeout, HttpResponse & result);
TBAG_API Err requestWithSync(std::string const & uri, uint64_t timeout, HttpResponse & result);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCLIENT_HPP__

