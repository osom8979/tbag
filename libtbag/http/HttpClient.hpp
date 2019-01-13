/**
 * @file   HttpClient.hpp
 * @brief  HttpClient class prototype.
 * @author zer0
 * @date   2019-01-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/http/HttpCommon.hpp>
#include <libtbag/http/WsFrame.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * HttpClient class prototype.
 *
 * @author zer0
 * @date   2019-01-11
 */
class TBAG_API HttpClient : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl   = std::unique_ptr<Impl>;
    using MqParams     = libtbag::mq::details::MqParams;
    using HttpRequest  = libtbag::http::HttpRequest;
    using HttpResponse = libtbag::http::HttpResponse;
    using Buffer       = libtbag::util::Buffer;
    using WsFrame      = libtbag::http::WsFrame;

private:
    UniqueImpl _impl;

public:
    HttpClient(std::string const & uri, bool use_websocket = false, bool enable_tls = false);
    HttpClient(std::string const & uri, std::string const & key, bool use_websocket = false, bool enable_tls = false);
    HttpClient(MqParams const & params, bool use_websocket = false, bool enable_tls = false);
    HttpClient(MqParams const & params, std::string const & key, bool use_websocket = false, bool enable_tls = false);
    ~HttpClient();

protected:
    virtual void onBegin();
    virtual void onEnd();

protected:
    virtual void onRegularHttp(HttpResponse const & response);

protected:
    virtual bool onSwitchingProtocol(HttpResponse const & response);
    virtual void onWsMessage(WsOpCode opcode, Buffer const & payload);

protected:
    virtual void onError(Err code);

public:
    Err writeRequest();
    Err writeRequest(HttpRequest const & request);

public:
    Err writeWsRequest();
    Err writeWsRequest(HttpRequest const & request);

public:
    Err writeWsFrame(WsFrame const & frame);
    Err writeText   (char const * buffer, std::size_t size, bool finish = true);
    Err writeText   (std::string const & text, bool finish = true);
    Err writeBinary (char const * buffer, std::size_t size, bool finish = true);
    Err writeBinary (Buffer const & buffer, bool finish = true);
    Err writeClose  ();
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPCLIENT_HPP__

