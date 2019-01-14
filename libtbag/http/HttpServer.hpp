/**
 * @file   HttpServer.hpp
 * @brief  HttpServer class prototype.
 * @author zer0
 * @date   2019-01-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

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
 * HttpServer class prototype.
 *
 * @author zer0
 * @date   2019-01-11
 */
class TBAG_API HttpServer : private Noncopyable
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
    HttpServer(MqParams const & params, bool use_websocket = false);
    HttpServer(MqParams const & params, std::string const & key, bool use_websocket = false);
    ~HttpServer();

protected:
    virtual void onBegin();
    virtual void onEnd();

protected:
    virtual bool onAccept(std::intptr_t id, std::string const & ip);
    virtual void onClose(std::intptr_t id);

protected:
    virtual HttpResponse onRegularHttp(std::intptr_t id, HttpRequest const & request);

protected:
    virtual bool onSwitchingProtocol(std::intptr_t id, HttpRequest const & request);
    virtual void onWsMessage(std::intptr_t id, WsOpCode opcode, Buffer const & payload);

protected:
    virtual void onError(std::intptr_t id, Err code);

public:
    void join();

public:
    Err exit();
    Err close(std::intptr_t id);

public:
    Err writeWsFrame(std::intptr_t id, WsFrame const & frame);
    Err writeText   (std::intptr_t id, char const * buffer, std::size_t size, bool finish = true);
    Err writeText   (std::intptr_t id, std::string const & text, bool finish = true);
    Err writeBinary (std::intptr_t id, char const * buffer, std::size_t size, bool finish = true);
    Err writeBinary (std::intptr_t id, Buffer const & buffer, bool finish = true);
    Err writeClose  (std::intptr_t id, WsStatus const & status);
    Err writeClose  (std::intptr_t id, WsStatusCode code);
    Err writeClose  (std::intptr_t id);
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_HTTPSERVER_HPP__

