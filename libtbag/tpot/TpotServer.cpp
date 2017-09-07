/**
 * @file   TpotServer.cpp
 * @brief  TpotServer class implementation.
 * @author zer0
 * @date   2017-09-06
 */

#include <libtbag/tpot/TpotServer.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/Version.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR static char const * const DEFAULT_HTML_BAD_REQUEST_BODY = R"HTML(
<html>
<head><title>${TITLE}</title></head>
<body><h2>${TITLE}</h2><p>Bad Request</p></body>
</html>
)HTML";

TBAG_CONSTEXPR static char const * const DEFAULT_HTML_INTERNAL_SERVER_ERROR_BODY = R"HTML(
<html>
<head><title>${TITLE}</title></head>
<body><h2>${TITLE}</h2><p>Internal Server Error</p></body>
</html>
)HTML";

TBAG_CONSTEXPR static char const * const TPOT_ENVS_EXE_PATH = "EXE_PATH";
TBAG_CONSTEXPR static char const * const TPOT_ENVS_EXE_DIR  = "EXE_DIR";
TBAG_CONSTEXPR static char const * const TPOT_ENVS_WORK_DIR = "WORK_DIR";
TBAG_CONSTEXPR static char const * const TPOT_ENVS_HOME_DIR = "HOME_DIR";
TBAG_CONSTEXPR static char const * const TPOT_ENVS_TITLE    = "TITLE";

TpotServer::TpotServer(Param const & param) : _param(param)
{
    _envs.push(EnvFlag(TPOT_ENVS_EXE_PATH, filesystem::Path::getExePath()));
    _envs.push(EnvFlag(TPOT_ENVS_EXE_DIR , filesystem::Path::getExeDir()));
    _envs.push(EnvFlag(TPOT_ENVS_WORK_DIR, filesystem::Path::getWorkDir()));
    _envs.push(EnvFlag(TPOT_ENVS_HOME_DIR, filesystem::Path::getHomeDir()));
    _envs.push(EnvFlag(TPOT_ENVS_TITLE   , std::string(LIBTBAG_MAIN_TITLE)));

    _body_4xx = _envs.convert(std::string(DEFAULT_HTML_BAD_REQUEST_BODY));
    _body_5xx = _envs.convert(std::string(DEFAULT_HTML_INTERNAL_SERVER_ERROR_BODY));
}

TpotServer::~TpotServer()
{
    // EMPTY.
}

int TpotServer::run()
{
    _server.reset(new (std::nothrow) Server(this, _loop, StreamType::TCP));
    assert(static_cast<bool>(_server));

    if (_param.verbose) {
        tDLogN("TpotServer::run() initialize({}:{}) ...", _param.bind, _param.port);
    }
    if (_server->init(_param.bind.c_str(), _param.port) != Err::E_SUCCESS) {
        return EXIT_FAILURE;
    }

    // @formatter:off
    using namespace proto;
    _server->setRequest( VersionPath::getMethod(),  VersionPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onVersionRequest (n, c, p); });
    _server->setRequest(    ExecPath::getMethod(),     ExecPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onExecRequest    (n, c, p); });
    _server->setRequest(HeartbitPath::getMethod(), HeartbitPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onHeartbitRequest(n, c, p); });
    _server->setRequest(    ListPath::getMethod(),     ListPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onListRequest    (n, c, p); });
    _server->setRequest(    KillPath::getMethod(),     KillPath::getPath(), [&](WeakClient n, Err c, HttpPacket & p){ onKillRequest    (n, c, p); });
    _packet.setOnPacketVersionRequest([&](Header const & h, PacketVersionRequest const & p, void * a) { onPacketVersionRequest(h, p, *(HttpPacket*)a); });
    _packet.setOnExecRequest         ([&](Header const & h, ExecRequest          const & p, void * a) { onExecRequest         (h, p, *(HttpPacket*)a); });
    _packet.setOnHeartbitRequest     ([&](Header const & h, HeartbitRequest      const & p, void * a) { onHeartbitRequest     (h, p, *(HttpPacket*)a); });
    _packet.setOnListRequest         ([&](Header const & h, ListRequest          const & p, void * a) { onListRequest         (h, p, *(HttpPacket*)a); });
    _packet.setOnKillRequest         ([&](Header const & h, KillRequest          const & p, void * a) { onKillRequest         (h, p, *(HttpPacket*)a); });
    // @formatter:on

    tDLogN("TpoT is run! (BIND: {}, PORT: {})", _server->dest(), _server->port());
    Err const RESULT = _loop.run();
    if (RESULT != Err::E_SUCCESS) {
        return EXIT_FAILURE;
    }

    {   // Check the last state.
        // XXXX
    }
    return EXIT_SUCCESS;
}

#ifndef _TPOT_SERVER_CHECK_ERROR
#define _TPOT_SERVER_CHECK_ERROR(prefix, code, node, shared)                               \
    auto shared = node.lock();                                                             \
    if (static_cast<bool>(shared) == false) { tDLogE(prefix " Expired client."); return; } \
    if (code != Err::E_SUCCESS) { tDLogE(prefix " {} error.", getErrName(code)); return; } \
    tDLogN(prefix " [ID:{}] {}:{}", shared->id(), shared->dest(), shared->port()); \
    /* END */
#endif

void TpotServer::onServerClose()
{
    // EMPTY.
}

void TpotServer::onHttpOpen(WeakClient node)
{
    // EMPTY.
}

void TpotServer::onHttpRequest(WeakClient node, Err code, HttpPacket & packet)
{
    _TPOT_SERVER_CHECK_ERROR("TpotServer::onHttpRequest()", code, node, shared);
    packet.response.setStatus(network::http::HttpStatus::SC_NOT_FOUND);
    packet.response.setBody(_body_4xx);
}

void TpotServer::onHttpClose(WeakClient node)
{
    // EMPTY.
}

void TpotServer::onHttpTimer(WeakClient node)
{
    // EMPTY.
}

void TpotServer::onHttpShutdown(WeakClient node, Err code)
{
    // EMPTY.
}

void TpotServer::onHttpWrite(WeakClient node, Err code)
{
    // EMPTY.
}

void TpotServer::onVersionRequest(WeakClient node, Err code, HttpPacket & packet)
{
    _TPOT_SERVER_CHECK_ERROR("TpotServer::onVersionRequest()", code, node, shared);
    auto body = packet.request.getBody();
    if (_packet.parse(body.data(), body.size(), &packet) != Err::E_SUCCESS) {
        tDLogW("TpotServer::onVersionRequest() Bad request.");
        packet.response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        packet.response.setBody(_body_4xx);
    }
}

void TpotServer::onExecRequest(WeakClient node, Err code, HttpPacket & packet)
{
    _TPOT_SERVER_CHECK_ERROR("TpotServer::onExecRequest()", code, node, shared);
    auto body = packet.request.getBody();
    if (_packet.parse(body.data(), body.size(), &packet) != Err::E_SUCCESS) {
        tDLogW("TpotServer::onExecRequest() Bad request.");
        packet.response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        packet.response.setBody(_body_4xx);
    }
}

void TpotServer::onHeartbitRequest(WeakClient node, Err code, HttpPacket & packet)
{
    _TPOT_SERVER_CHECK_ERROR("TpotServer::onHeartbitRequest()", code, node, shared);
    auto body = packet.request.getBody();
    if (_packet.parse(body.data(), body.size(), &packet) != Err::E_SUCCESS) {
        tDLogW("TpotServer::onHeartbitRequest() Bad request.");
        packet.response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        packet.response.setBody(_body_4xx);
    }
}

void TpotServer::onListRequest(WeakClient node, Err code, HttpPacket & packet)
{
    _TPOT_SERVER_CHECK_ERROR("TpotServer::onListRequest()", code, node, shared);
    auto body = packet.request.getBody();
    if (_packet.parse(body.data(), body.size(), &packet) != Err::E_SUCCESS) {
        tDLogW("TpotServer::onListRequest() Bad request.");
        packet.response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        packet.response.setBody(_body_4xx);
    }
}

void TpotServer::onKillRequest(WeakClient node, Err code, HttpPacket & packet)
{
    _TPOT_SERVER_CHECK_ERROR("TpotServer::onKillRequest()", code, node, shared);
    auto body = packet.request.getBody();
    if (_packet.parse(body.data(), body.size(), &packet) != Err::E_SUCCESS) {
        tDLogW("TpotServer::onKillRequest() Bad request.");
        packet.response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        packet.response.setBody(_body_4xx);
    }
}

void TpotServer::onPacketVersionRequest(Header const & header, PacketVersionRequest const & packet, HttpPacket & hp)
{
    util::Version const PACKET_VERSION = util::getTbagPacketVersion();
    _packet.buildPacketVersionResponse(PACKET_VERSION.getMajor(), PACKET_VERSION.getMinor());

    tDLogI("TpotServer::onPacketVersionRequest() Response OK (Version: {})", PACKET_VERSION.toString());
    hp.response.setStatus(network::http::HttpStatus::SC_OK);
    hp.response.appendBody((char const *)_packet.point(), _packet.size());
}

void TpotServer::onExecRequest(Header const & header, ExecRequest const & packet, HttpPacket & hp)
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

    int const PID = _procs.exec(file, args, envs, cwd, input);
    if (PID != 0) {
        _packet.buildExecResponse(PID);
        tDLogI("TpotServer::onExecRequest() Execute OK (PID: {})", PID);
    } else {
        using namespace proto::fbs::tpot;
        _packet.buildExecResponse(PID, ResultCode_UNKNOWN_ERROR);
        tDLogW("TpotServer::onExecRequest() Execute error");
    }
    hp.response.setStatus(network::http::HttpStatus::SC_OK);
    hp.response.appendBody((char const *)_packet.point(), _packet.size());
}

void TpotServer::onHeartbitRequest(Header const & header, HeartbitRequest const & packet, HttpPacket & hp)
{
    std::string const ECHO_MESSAGE = packet.echo()->str();
    _packet.buildHeartbitResponse(ECHO_MESSAGE);

    tDLogI("TpotServer::onHeartbitRequest() Response OK (Echo: {})", ECHO_MESSAGE);
    hp.response.setStatus(network::http::HttpStatus::SC_OK);
    hp.response.appendBody((char const *)_packet.point(), _packet.size());
}

void TpotServer::onListRequest(Header const & header, ListRequest const & packet, HttpPacket & hp)
{
    std::vector<ProcessInfo> pinfos;
    for (auto & pid : _procs.list()) {
        pinfos.emplace_back(pid, _procs.isActive(pid));
    }
    _packet.buildListResponse(pinfos);

    tDLogI("TpotServer::onListRequest() Response OK (List size: {})", pinfos.size());
    hp.response.setStatus(network::http::HttpStatus::SC_OK);
    hp.response.appendBody((char const *)_packet.point(), _packet.size());
}

void TpotServer::onKillRequest(Header const & header, KillRequest const & packet, HttpPacket & hp)
{
    using namespace proto::fbs::tpot;
    int const PID = packet.pid();

    if (_procs.exists(PID)) {
        Err const CODE = _procs.kill(PID, signal::TBAG_SIGNAL_TERMINATION);
        if (TBAG_ERR_SUCCESS(CODE)) {
            tDLogI("TpotServer::onKillRequest() Kill success (PID: {})", PID);
            _packet.buildKillResponse(ResultCode_SUCCESS);
        } else {
            tDLogE("TpotServer::onKillRequest() Kill {} error (PID: {})", getErrName(CODE), PID);
            _packet.buildKillResponse(ResultCode_UNKNOWN_ERROR);
        }
    } else {
        tDLogW("TpotServer::onKillRequest() Not exists process (PID: {})", PID);
        _packet.buildKillResponse(ResultCode_SUCCESS);
    }
    hp.response.setStatus(network::http::HttpStatus::SC_OK);
    hp.response.appendBody((char const *)_packet.point(), _packet.size());
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

