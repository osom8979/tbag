/**
 * @file   TpotServer.hpp
 * @brief  TpotServer class prototype.
 * @author zer0
 * @date   2017-09-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/string/Environments.hpp>
#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/proto/TpotPacket.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <memory>
#include <string>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotServer class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class TBAG_API TpotServer : private network::http::HttpServer
{
private:
    using Loop = uvpp::Loop;

    using HttpParser  = network::http::HttpParser;
    using HttpBuilder = network::http::HttpBuilder;
    using HttpServer  = network::http::HttpServer;

    using Id         = HttpServer::Id;
    using StreamType = HttpServer::StreamType;
    using WeakClient = HttpServer::WeakClient;
    using HttpPacket = HttpServer::HttpPacket;
    using WsPacket   = HttpServer::WsPacket;

    using Environments = string::Environments;
    using EnvFlag      = Environments::Flag;

private:
    Loop _loop;
    Environments _envs;
    std::string  _body_4xx;
    std::string  _body_5xx;

public:
    TpotServer();
    virtual ~TpotServer();

public:
    int run(std::string const & bind, int port);

protected:
    virtual void onServerClose() override;
    virtual void onHttpOpen(WeakClient node) override;
    virtual void onHttpRequest(WeakClient node, Err code, HttpPacket & packet) override;
    virtual void onHttpClose(WeakClient node) override;
    virtual void onHttpTimer(WeakClient node) override;
    virtual void onHttpShutdown(WeakClient node, Err code) override;
    virtual void onHttpWrite(WeakClient node, Err code) override;

// Process callback.
public:
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__

