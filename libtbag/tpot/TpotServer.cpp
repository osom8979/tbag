/**
 * @file   TpotServer.cpp
 * @brief  TpotServer class implementation.
 * @author zer0
 * @date   2017-09-06
 */

#include <libtbag/tpot/TpotServer.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/util/Version.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <libtbag/proto/TpotPacket.hpp>

#include <cassert>
#include <cstdlib>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR static char const * const TPOT_HTML_BAD_REQUEST_BODY = R"HTML(
<html>
<head><title>TpoT</title></head>
<body><h2>TpoT</h2><p>Bad Request</p></body>
</html>
)HTML";

TBAG_CONSTEXPR static char const * const TPOT_HTML_INTERNAL_SERVER_ERROR_BODY = R"HTML(
<html>
<head><title>TpoT</title></head>
<body><h2>TpoT</h2><p>Internal Server Error</p></body>
</html>
)HTML";

// ------------------------------------
// TpotServer::Internal implementation.
// ------------------------------------

struct TpotServer::Internal
{
    using Loop = uvpp::Loop;

    using StreamType = network::http::HttpServer::StreamType;
    using WeakClient = network::http::HttpServer::WeakClient;
    using HttpPacket = network::http::HttpServer::HttpPacket;

    using SharedServer = std::shared_ptr<network::http::FuncHttpServer>;
    using FuncProcMgr  = process::FunctionalProcessManager;

    using FuncTpotPacket  = proto::FunctionalTpotPacket;
    using Header          = FuncTpotPacket::Header;
    using VersionRequest  = FuncTpotPacket::VersionRequest;
    using ExecRequest     = FuncTpotPacket::ExecRequest;
    using HeartbitRequest = FuncTpotPacket::HeartbitRequest;
    using ListRequest     = FuncTpotPacket::ListRequest;
    using KillRequest     = FuncTpotPacket::KillRequest;

    TpotServer * parent;
    Param param;

    Loop         loop;
    SharedServer server;
    FuncProcMgr  procs;

    FuncTpotPacket tpot;

    Internal(TpotServer * p, Param const & pm) : parent(p), param(pm), tpot()
    {
        // @formatter:off
        using namespace proto;
        tpot.setOnVersionRequest ([&](Header const & h, VersionRequest  const & p, void * a) { onVersionRequest (h, p, *(HttpPacket*)a); });
        tpot.setOnExecRequest    ([&](Header const & h, ExecRequest     const & p, void * a) { onExecRequest    (h, p, *(HttpPacket*)a); });
        tpot.setOnHeartbitRequest([&](Header const & h, HeartbitRequest const & p, void * a) { onHeartbitRequest(h, p, *(HttpPacket*)a); });
        tpot.setOnListRequest    ([&](Header const & h, ListRequest     const & p, void * a) { onListRequest    (h, p, *(HttpPacket*)a); });
        tpot.setOnKillRequest    ([&](Header const & h, KillRequest     const & p, void * a) { onKillRequest    (h, p, *(HttpPacket*)a); });
        // @formatter:on
    }

    ~Internal()
    {
        // EMPTY.
    }

    int run()
    {
        using namespace network::http;
        server.reset(new (std::nothrow) FuncHttpServer(loop, StreamType::TCP));
        assert(static_cast<bool>(server));

        tDLogIfN(param.verbose, "TpotServer::Internal::run() initialize({}:{}) ...", param.bind, param.port);
        if (server->init(param.bind.c_str(), param.port) != Err::E_SUCCESS) {
            return EXIT_FAILURE;
        }

        // @formatter:off
        using namespace proto;
        server->setOnHttpRequest([&](WeakClient n, Err c, HttpPacket & p){ onHttpRequest(n, c, p); });
        server->setRequest( VersionPath::getMethod(),  VersionPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onVersionRequest (n, c, p); });
        server->setRequest(    ExecPath::getMethod(),     ExecPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onExecRequest    (n, c, p); });
        server->setRequest(HeartbitPath::getMethod(), HeartbitPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onHeartbitRequest(n, c, p); });
        server->setRequest(    ListPath::getMethod(),     ListPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onListRequest    (n, c, p); });
        server->setRequest(    KillPath::getMethod(),     KillPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onKillRequest    (n, c, p); });
        // @formatter:on

        tDLogN("TpoT is run! (BIND: {}, PORT: {})", server->dest(), server->port());
        Err const RESULT = loop.run();
        if (RESULT != Err::E_SUCCESS) {
            return EXIT_FAILURE;
        }

        {   // Check the last state.
            // XXXX
        }
        return EXIT_SUCCESS;
    }

    void onVersionRequest(Header const & header, VersionRequest const & packet, HttpPacket & hp)
    {
        util::Version const PACKET_VERSION = util::getTbagPacketVersion();
        tpot.buildVersionResponse(PACKET_VERSION.getMajor(), PACKET_VERSION.getMinor(), header.id());

        tDLogI("TpotServer::onVersionRequest() Response OK (Version: {})", PACKET_VERSION.toString());
        hp.response.setStatus(network::http::HttpStatus::SC_OK);
        hp.response.appendBody((char const *)tpot.point(), tpot.size());
    }

    void onExecRequest(Header const & header, ExecRequest const & packet, HttpPacket & hp)
    {
        std::string file  = packet.file ()->str();
        std::string cwd   = packet.cwd  ()->str();;
        std::string input = packet.input()->str();;

        std::vector<std::string> args;
        for (auto itr = packet.args()->begin(); itr != packet.args()->end(); ++itr) {
            args.push_back(itr->str());
        }

        std::vector<std::string> envs;
        for (auto itr = packet.envs()->begin(); itr != packet.envs()->end(); ++itr) {
            args.push_back(itr->str());
        }

        int const PID = procs.exec(file, args, envs, cwd, input);
        if (PID != 0) {
            tpot.buildExecResponse(PID, header.id());
            tDLogI("TpotServer::onExecRequest() Execute OK (PID: {})", PID);
        } else {
            using namespace proto::fbs::tpot;
            tpot.buildExecResponse(PID, header.id(), ResultCode_EXECUTE_ERROR);
            tDLogW("TpotServer::onExecRequest() Execute error");
        }
        hp.response.setStatus(network::http::HttpStatus::SC_OK);
        hp.response.appendBody((char const *)tpot.point(), tpot.size());
    }

    void onHeartbitRequest(Header const & header, HeartbitRequest const & packet, HttpPacket & hp)
    {
        std::string const ECHO_MESSAGE = packet.echo()->str();
        tpot.buildHeartbitResponse(ECHO_MESSAGE, header.id());

        tDLogI("TpotServer::onHeartbitRequest() Response OK (Echo: {})", ECHO_MESSAGE);
        hp.response.setStatus(network::http::HttpStatus::SC_OK);
        hp.response.appendBody((char const *)tpot.point(), tpot.size());
    }

    void onListRequest(Header const & header, ListRequest const & packet, HttpPacket & hp)
    {
        auto infos = procs.list();
        tpot.buildListResponse(infos, header.id());

        tDLogI("TpotServer::onListRequest() Response OK (List size: {})", infos.size());
        hp.response.setStatus(network::http::HttpStatus::SC_OK);
        hp.response.appendBody((char const *)tpot.point(), tpot.size());
    }

    void onKillRequest(Header const & header, KillRequest const & packet, HttpPacket & hp)
    {
        using namespace proto::fbs::tpot;
        int const PID = packet.pid();

        if (procs.exists(PID)) {
            Err const CODE = procs.kill(PID, signal::TBAG_SIGNAL_TERMINATION);
            if (TBAG_ERR_SUCCESS(CODE)) {
                tDLogI("TpotServer::onKillRequest() Kill success (PID: {})", PID);
                tpot.buildKillResponse(header.id(), ResultCode_SUCCESS);
            } else {
                tDLogE("TpotServer::onKillRequest() Kill {} error (PID: {})", getErrName(CODE), PID);
                tpot.buildKillResponse(header.id(), ResultCode_KILL_ERROR);
            }
        } else {
            tDLogW("TpotServer::onKillRequest() Not exists process (PID: {})", PID);
            tpot.buildKillResponse(header.id(), ResultCode_NOT_EXISTS);
        }
        hp.response.setStatus(network::http::HttpStatus::SC_OK);
        hp.response.appendBody((char const *)tpot.point(), tpot.size());
    }

    void response(HttpPacket & packet)
    {
        auto body = packet.request.getBody();
        if (tpot.parse(body.data(), body.size(), &packet) != Err::E_SUCCESS) {
            packet.response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
            packet.response.setBody(TPOT_HTML_BAD_REQUEST_BODY);
        }
    }

#ifndef _TPOT_SERVER_CHECK_ERROR
#define _TPOT_SERVER_CHECK_ERROR(prefix, code, node, shared)                               \
    auto shared = node.lock();                                                             \
    if (static_cast<bool>(shared) == false) { tDLogE(prefix " Expired client."); return; } \
    if (code != Err::E_SUCCESS) { tDLogE(prefix " {} error.", getErrName(code)); return; } \
    tDLogN(prefix " [ID:{}] {}:{}", shared->id(), shared->dest(), shared->port());         \
    /* END */
#endif

    void onHttpRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onHttpRequest()", code, node, shared);
        packet.response.setStatus(network::http::HttpStatus::SC_NOT_FOUND);
        packet.response.setBody(TPOT_HTML_BAD_REQUEST_BODY);
    }

    void onVersionRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onVersionRequest()", code, node, shared);
        response(packet);
    }

    void onExecRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onExecRequest()", code, node, shared);
        response(packet);
    }

    void onHeartbitRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onHeartbitRequest()", code, node, shared);
        response(packet);
    }

    void onListRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onListRequest()", code, node, shared);
        response(packet);
    }

    void onKillRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onKillRequest()", code, node, shared);
        response(packet);
    }

#undef _TPOT_SERVER_CHECK_ERROR
};

// --------------------------
// TpotServer implementation.
// --------------------------

TpotServer::TpotServer(Param const & param) : _internal(new Internal(this, param))
{
    assert(static_cast<bool>(_internal));
}

TpotServer::~TpotServer()
{
    // EMPTY.
}

int TpotServer::run()
{
    assert(static_cast<bool>(_internal));
    return _internal->run();
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

