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
#include <libtbag/network/tcp/TcpClient.hpp>
#include <libtbag/network/pipe/PipeClient.hpp>
#include <libtbag/network/http/HttpParser.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/Type.hpp>

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
class TBAG_API HttpClient : public ClientType
{
public:
    using Parent = ClientType;

public:
    using Loop     = uvpp::Loop;
    using String   = HttpParser::String;
    using Size     = HttpParser::Size;
    using Buffer   = std::vector<char>;
    using Millisec = std::chrono::milliseconds;
    using Seconds  = std::chrono::seconds;

    using OnResponse = std::function<void(Err, HttpParser const &)>;

private:
    HttpBuilder _request;
    HttpParser  _response;

private:
    Buffer     _buffer;
    OnResponse _callback;
    Millisec   _timeout;

public:
    HttpClient(Loop & loop) : Parent(loop), _request(), _response(HttpParser::Type::RESPONSE)
    { /* EMPTY. */ }
    virtual ~HttpClient()
    { /* EMPTY. */ }

public:
    // @formatter:off
    inline HttpBuilder       & atRequest ()       TBAG_NOEXCEPT { return  _request; }
    inline HttpBuilder const & atRequest () const TBAG_NOEXCEPT { return  _request; }
    inline HttpParser        & atResponse()       TBAG_NOEXCEPT { return _response; }
    inline HttpParser  const & atResponse() const TBAG_NOEXCEPT { return _response; }
    // @formatter:on

public:
    void setup(HttpBuilder const request, OnResponse const & callback, Millisec const & timeout = Millisec(0))
    {
        _request  = request;
        _callback = callback;
        _timeout  = timeout;
    }

// TcpClient callback.
public:
    virtual void onConnect(Err code) override
    {
        if (code == Err::E_SUCCESS) {
            auto buffer = _request.buildRequest();
            if (this->write(buffer.data(), buffer.size(), _timeout.count()) == false) {
                _callback(Err::E_WRERR, _response);
                this->close();
            }
        } else {
            _callback(code, _response);
            this->close();
        }
    }

    virtual void onShutdown(Err code) override
    {
        _callback(code == Err::E_SUCCESS ? Err::E_SHUTDOWN : code, _response);
        this->close();
    }

    virtual void onWrite(Err code) override
    {
        if (code == Err::E_SUCCESS) {
            this->start();
        } else {
            _callback(code, _response);
            this->close();
        }
    }

    virtual void onRead(Err code, char const * buffer, Size size) override
    {
        if (code == Err::E_EOF) {
            _response.execute(buffer, size);
            _callback(Err::E_SUCCESS, _response);

            this->stop();
            this->close();
        } else if (code == Err::E_SUCCESS) {
            _response.execute(buffer, size);
            if (_response.isComplete()) {
                _callback(Err::E_SUCCESS, _response);
                this->stop();
                this->close();
            }
        } else {
            _callback(code, _response);
            this->stop();
            this->close();
        }
    }
};

using TcpHttpClient  = HttpClient<tcp::TcpClient>;
using PipeHttpClient = HttpClient<pipe::PipeClient>;

// Forward declaration.
class Uri;

Err requestWithSync(Uri const & uri, uint64_t timeout, HttpParser & result);

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_HTTPCLIENT_HPP__

