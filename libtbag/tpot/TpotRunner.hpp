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
#include <libtbag/uvpp/func/FunctionalProcess.hpp>

#include <libtbag/string/Environments.hpp>
#include <libtbag/tpot/structure/TpotProtocol.hpp>

#include <memory>
#include <string>
#include <map>

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
class TBAG_API TpotRunner final : private Noncopyable
{
public:
    using Loop = uvpp::Loop;
    using Proc = uvpp::func::FuncProcess;

    using SharedProc = std::shared_ptr<Proc>;
    using Pid        = int;
    using ProcMap    = std::map<Pid, SharedProc>;
    using ProcPair   = ProcMap::value_type;

    using ExecParser = structure::Exec;
    using HbitParser = structure::Heartbit;
    using ListParser = structure::List;
    using KillParser = structure::Kill;

    using HttpParser       = network::http::HttpParser;
    using HttpBuilder      = network::http::HttpBuilder;
    using FuncHttpServer   = network::http::FuncHttpServer;
    using SharedHttpServer = std::shared_ptr<FuncHttpServer>;

    using Id = FuncHttpServer::Id;
    using WC = FuncHttpServer::WC;
    using HP = FuncHttpServer::HP;

    using Environments = string::Environments;
    using EnvFlag = Environments::Flag;

private:
    TpotParams   _params;
    Environments _envs;
    std::string  _body_4xx;
    std::string  _body_5xx;

private:
    Loop _loop;
    SharedHttpServer _server;
    ProcMap _procs;

public:
    TpotRunner(TpotParams const & params);
    ~TpotRunner();

public:
    int run();

public:
    void onNodeOpen (WC node);
    void onNodeClose(WC node);
    void onServerClose();

// HTTP Request callback.
public:
    void onNodeRequest        (WC node, Err code, HP const & packet);
    void onNodeExecRequest    (WC node, Err code, HP const & packet);
    void onNodeKillRequest    (WC node, Err code, HP const & packet);
    void onNodeListRequest    (WC node, Err code, HP const & packet);
    void onNodeHeartbitRequest(WC node, Err code, HP const & packet);

private:
    Err execProcess(std::string const & body, HttpBuilder & response);
    Err killProcess(std::string const & body, HttpBuilder & response);
    Err listProcess(std::string const & body, HttpBuilder & response);

// Process callback.
public:
    void onExitProcess(Pid id, int64_t exit_status, int term_signal);
    void onCloseProcess(Pid id);
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTRUNNER_HPP__

