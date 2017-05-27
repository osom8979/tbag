/**
 * @file   TpotRunner.hpp
 * @brief  TpotRunner class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTRUNNER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTRUNNER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/network/http/HttpParser.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Process.hpp>

#include <libtbag/string/Environments.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotParams class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
struct TpotParams
{
    bool enable_service;
    bool enable_verbose;

    std::string server_bind;
    int server_port;
};

/**
 * TpotRunner class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 *
 * @remarks
 *  Separate initializer and business logic.
 */
class TBAG_API TpotRunner final : public Noncopyable
{
public:
    using Loop = uvpp::Loop;
    using Proc = uvpp::Process;

    using SharedProc = std::shared_ptr<Proc>;

    using HttpParser  = network::http::HttpParser;
    using HttpBuilder = network::http::HttpBuilder;
    using HttpServer  = network::http::HttpServer;
    using SharedHttpServer = std::shared_ptr<HttpServer>;

    using Node = HttpServer::WeakClient;

    using Environments = string::Environments;
    using EnvFlag = Environments::Flag;

private:
    TpotParams _params;
    Environments _envs;

private:
    Loop _loop;
    SharedHttpServer _server;

public:
    TpotRunner(TpotParams const & params);
    ~TpotRunner();

public:
    int run();

public:
    void onNodeOpen(Node node);
    void onNodeRequest(Err code, Node node, HttpParser const & request, HttpBuilder & response, uint64_t & timeout);
    void onNodeExecRequest(Err code, Node node, HttpParser const & request, HttpBuilder & response, uint64_t & timeout);
    void onNodeKillRequest(Err code, Node node, HttpParser const & request, HttpBuilder & response, uint64_t & timeout);
    void onNodeClose(Node node);
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTRUNNER_HPP__

