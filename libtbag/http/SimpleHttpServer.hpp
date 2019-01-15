/**
 * @file   SimpleHttpServer.hpp
 * @brief  SimpleHttpServer class prototype.
 * @author zer0
 * @date   2019-01-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_SIMPLEHTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_SIMPLEHTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/http/HttpCommon.hpp>
#include <libtbag/http/HttpServer.hpp>

#include <string>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * SimpleHttpServer class prototype.
 *
 * @author zer0
 * @date   2019-01-14
 */
class TBAG_API SimpleHttpServer : public HttpServer
{
public:
    using OnBegin  = std::function<void(void)>;
    using OnEnd    = std::function<void(void)>;

    using OnAccept = std::function<bool(std::intptr_t, std::string const &)>;
    using OnClose  = std::function<void(std::intptr_t)>;

    using OnHttp  = std::function<HttpResponse(std::intptr_t, HttpRequest const &)>;
    using OnError = std::function<void(std::intptr_t, Err)>;

public:
    struct Callbacks
    {
        // @formatter:off
        OnBegin  begin_cb;
        OnEnd    end_cb;
        OnAccept accept_cb;
        OnClose  close_cb;
        OnHttp   http_cb;
        OnError  error_cb;
        // @formatter:on
    };

private:
    Callbacks _callbacks;

public:
    SimpleHttpServer(std::string const & host, int port, Callbacks const & cbs);
    virtual ~SimpleHttpServer();

private:
    /**
     * @remarks
     *  Tricky method used to prevent reordering warnings.
     *
     *  CLANG WARNING:
     *  @code
     *   warning: field '_callbacks' will be initialized after base 'HttpServer' [-Wreorder]
     *  @endcode
     */
    MqParams initAndParams(std::string const & host, int port, Callbacks const & cbs);

protected:
    virtual void onBegin() override;
    virtual void onEnd() override;
    virtual bool onAccept(std::intptr_t id, std::string const & ip) override;
    virtual void onClose(std::intptr_t id) override;
    virtual HttpResponse onRegularHttp(std::intptr_t id, HttpRequest const & request) override;
    virtual void onError(std::intptr_t id, Err code) override;
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_SIMPLEHTTPSERVER_HPP__

