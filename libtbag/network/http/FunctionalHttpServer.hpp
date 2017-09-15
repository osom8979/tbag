/**
 * @file   FunctionalHttpServer.hpp
 * @brief  FunctionalHttpServer class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNCTIONALHTTPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNCTIONALHTTPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/functional/CallbackHelper.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * FunctionalHttpServer class prototype.
 *
 * @author zer0
 * @date   2017-06-30
 * @date   2017-08-09 (Rename: FunctionalHttpServer -> FuncHttpServer)
 */
class FunctionalHttpServer : public HttpServer
{
public:
    using StreamType = HttpServer::StreamType;
    using WeakClient = HttpServer::WeakClient;
    using HttpPacket = HttpServer::HttpPacket;
    using WsPacket   = HttpServer::WsPacket;

public:
    FunctionalHttpServer(Loop & loop, StreamType type = StreamType::TCP) : HttpServer(loop, type)
    { /* EMPTY. */ }
    virtual ~FunctionalHttpServer()
    { /* EMPTY. */ }

public:
    TBAG_VOID_NOPARAM_CALLBACK_HELPER(onServerClose);
    TBAG_CALLBACK_HELPER(onClientUdataAlloc, void*, nullptr, WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onClientUdataDealloc, WeakClient, void*);
    TBAG_VOID_CALLBACK_HELPER(onHttpOpen    , WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onHttpRequest , WeakClient, Err, HttpPacket&);
    TBAG_VOID_CALLBACK_HELPER(onHttpClose   , WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onHttpTimer   , WeakClient);
    TBAG_VOID_CALLBACK_HELPER(onHttpShutdown, WeakClient, Err);
    TBAG_VOID_CALLBACK_HELPER(onHttpWrite   , WeakClient, Err);
    TBAG_VOID_CALLBACK_HELPER(onWsOpen      , WeakClient, Err, HttpPacket&);
    TBAG_VOID_CALLBACK_HELPER(onWsMessage   , WeakClient, OpCode, char const*, std::size_t);
};

using FuncHttpServer = FunctionalHttpServer;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_FUNCTIONALHTTPSERVER_HPP__

