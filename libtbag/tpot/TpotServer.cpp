/**
 * @file   TpotServer.cpp
 * @brief  TpotServer class implementation.
 * @author zer0
 * @date   2017-09-06
 */

#include <libtbag/tpot/TpotServer.hpp>
#include <libtbag/log/Log.hpp>

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

TpotServer::TpotServer() : _loop(), HttpServer(_loop, StreamType::TCP)
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

int TpotServer::run(std::string const & bind, int port)
{
    using namespace network::http;
    if (init(bind.c_str(), port) != Err::E_SUCCESS) {
        return EXIT_FAILURE;
    }

    tDLogN("TpoT is run!");
    Err const RESULT = _loop.run();
    if (RESULT != Err::E_SUCCESS) {
        return EXIT_FAILURE;
    }

    {   // Check the last state.
        // XXXX
    }
    return EXIT_SUCCESS;
}

#ifndef _TPOT_RUNNER_CHECK_ERROR
#define _TPOT_RUNNER_CHECK_ERROR(prefix, code, node, shared)                               \
    auto shared = node.lock();                                                             \
    if (static_cast<bool>(shared) == false) { tDLogE(prefix " Expired client."); return; } \
    if (code != Err::E_SUCCESS) { tDLogE(prefix " {} error.", getErrName(code)); return; } \
    tDLogN(prefix " [ID:{}] {}:{}", shared->id(), shared->dest(), shared->port()); \
    /* END */
#endif

void TpotServer::onServerClose()
{
}

void TpotServer::onHttpOpen(WeakClient node)
{
}

void TpotServer::onHttpRequest(WeakClient node, Err code, HttpPacket & packet)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotServer::onHttpRequest()", code, node, shared);
    packet.response.setStatus(network::http::HttpStatus::SC_NOT_FOUND);
    packet.response.setBody(_body_4xx);
}

void TpotServer::onHttpClose(WeakClient node)
{
}

void TpotServer::onHttpTimer(WeakClient node)
{
}

void TpotServer::onHttpShutdown(WeakClient node, Err code)
{
}

void TpotServer::onHttpWrite(WeakClient node, Err code)
{
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

