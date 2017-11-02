/**
 * @file   FunctionalHttpServer.hpp
 * @brief  FunctionalHttpServer class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNC_FUNCTIONALHTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNC_FUNCTIONALHTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/functional/CallbackHelper.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace func    {

/**
 * FunctionalHttpServer class prototype.
 *
 * @author zer0
 * @date   2017-06-30
 * @date   2017-08-09 (Rename: FunctionalHttpServer -> FuncHttpServer)
 * @date   2017-10-16 (Change to the class template)
 */
template <typename HttpServerType>
class FunctionalHttpServer : public HttpServerType
{
public:
    using Parent     = HttpServerType;
    using Loop       = HttpServer::Loop;
    using StreamType = HttpServer::StreamType;
    using WeakClient = HttpServer::WeakClient;

    STATIC_ASSERT_CHECK_IS_BASE_OF(HttpServer, Parent);

public:
    template <typename ... Args>
    FunctionalHttpServer(Args && ... args) : Parent(std::forward<Args>(args) ...)
    { /* EMPTY. */ }
    virtual ~FunctionalHttpServer()
    { /* EMPTY. */ }

public:
    using       onConnection_func = std::function<void(Err)>;
    using        onClientEof_func = std::function<void(WeakClient)>;
    using  onClientReadError_func = std::function<void(WeakClient, Err)>;
    using onClientParseError_func = std::function<void(WeakClient, Err)>;

private:
          onConnection_func        __onConnection_cb;
           onClientEof_func         __onClientEof_cb;
     onClientReadError_func   __onClientReadError_cb;
    onClientParseError_func  __onClientParseError_cb;

public:
    inline void       set_onConnection(      onConnection_func const & cb) {       __onConnection_cb = cb; }
    inline void        set_onClientEof(       onClientEof_func const & cb) {        __onClientEof_cb = cb; }
    inline void  set_onClientReadError( onClientReadError_func const & cb) {  __onClientReadError_cb = cb; }
    inline void set_onClientParseError(onClientParseError_func const & cb) { __onClientParseError_cb = cb; }

protected:
    virtual void onConnection(Err code) override
    {
        if (static_cast<bool>(__onConnection_cb)) {
            __onConnection_cb(code);
        } else {
            Parent::onConnection(code);
        }
    }

    virtual void onClientEof(WeakClient node) override
    {
        if (static_cast<bool>(__onClientEof_cb)) {
            __onClientEof_cb(node);
        } else {
            Parent::onClientEof(node);
        }
    }

    virtual void onClientReadError(WeakClient node, Err code) override
    {
        if (static_cast<bool>(__onClientReadError_cb)) {
            __onClientReadError_cb(node, code);
        } else {
            Parent::onClientReadError(node, code);
        }
    }

    virtual void onClientParseError(WeakClient node, Err code) override
    {
        if (static_cast<bool>(__onClientParseError_cb)) {
            __onClientParseError_cb(node, code);
        } else {
            Parent::onClientParseError(node, code);
        }
    }

public:
    TBAG_VOID_CALLBACK_HELPER(onClientShutdown, WeakClient, Err);
    TBAG_VOID_CALLBACK_HELPER(onClientWrite   , WeakClient, Err);
    TBAG_VOID_CALLBACK_HELPER(onClientClose   , WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onClientTimer   , WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onServerClose);

    TBAG_VOID_CALLBACK_HELPER(onClientOpen        , WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onClientContinue    , WeakClient);
    TBAG_RETN_CALLBACK_HELPER(onClientSwitchingProtocol, bool, true, WeakClient, HttpRequest const &);
    TBAG_VOID_CALLBACK_HELPER(onClientWsMessage   , WeakClient, ws::WsOpCode, util::Buffer const &);
    TBAG_VOID_CALLBACK_HELPER(onClientRequest     , WeakClient, HttpRequest const &);
};

using FuncHttpServer = FunctionalHttpServer<HttpServer>;

} // namespace func
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNC_FUNCTIONALHTTPSERVER_HPP__

