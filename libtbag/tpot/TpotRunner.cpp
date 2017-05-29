/**
 * @file   TpotRunner.cpp
 * @brief  TpotRunner class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/TpotRunner.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/tpot/structure/TpotProtocol.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR char const * const DEFAULT_HTML_BAD_REQUEST_BODY = R"HTML(
<html>
<head><title>${title}</title></head>
<body><h2>${title}</h2><p>Bad Request</p></body>
</html>
)HTML";

TBAG_CONSTEXPR char const * const DEFAULT_HTML_INTERNAL_SERVER_ERROR_BODY = R"HTML(
<html>
<head><title>${title}</title></head>
<body><h2>${title}</h2><p>Internal Server Error</p></body>
</html>
)HTML";

TpotRunner::TpotRunner(TpotParams const & params) : _params(params)
{
    _envs.push(EnvFlag("title", std::string(LIBTBAG_MAIN_TITLE)));
    _body_4xx = _envs.convert(std::string(DEFAULT_HTML_BAD_REQUEST_BODY));
    _body_5xx = _envs.convert(std::string(DEFAULT_HTML_INTERNAL_SERVER_ERROR_BODY));
}

TpotRunner::~TpotRunner()
{
    // EMPTY.
}

int TpotRunner::run()
{
    _server.reset(new (std::nothrow) HttpServer(_loop, HttpServer::StreamType::TCP));
    assert(static_cast<bool>(_server));
    if (_server->init(_params.server_bind.c_str(), _params.server_port) == false) {
        return EXIT_FAILURE;
    }

    using namespace std::placeholders;
    _server->setOnOpen   (std::bind(&TpotRunner::onNodeOpen , this, _1));
    _server->setOnClose  (std::bind(&TpotRunner::onNodeClose, this, _1));

    auto const func_common   = std::bind(&TpotRunner::onNodeRequest        , this, _1, _2, _3, _4, _5);
    auto const func_exec     = std::bind(&TpotRunner::onNodeExecRequest    , this, _1, _2, _3, _4, _5);
    auto const func_kill     = std::bind(&TpotRunner::onNodeKillRequest    , this, _1, _2, _3, _4, _5);
    auto const func_list     = std::bind(&TpotRunner::onNodeListRequest    , this, _1, _2, _3, _4, _5);
    auto const func_heartbit = std::bind(&TpotRunner::onNodeHeartbitRequest, this, _1, _2, _3, _4, _5);

    _server->setOnRequest(Exec::getMethod(), Exec::getPath(), func_exec);
    _server->setOnRequest(Kill::getMethod(), Kill::getPath(), func_kill);
    _server->setOnRequest(List::getMethod(), List::getPath(), func_list);
    _server->setOnRequest(Hbit::getMethod(), Hbit::getPath(), func_heartbit);
    _server->setOnRequest(func_common);

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

void TpotRunner::onNodeOpen(Node node)
{
    if (auto shared = node.lock()) {
        tDLogN("TpotRunner::onNodeOpen(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());
    } else {
        tDLogE("TpotRunner::onNodeOpen() Expired client.");
    }
}

void TpotRunner::onNodeClose(Node node)
{
    if (auto shared = node.lock()) {
        tDLogN("TpotRunner::onNodeClose(ID:{})", shared->getId());
    } else {
        tDLogE("TpotRunner::onNodeOpen() Expired client.");
    }
}

// ----------------------
// HTTP Request callback.
// ----------------------

#ifndef _TPOT_RUNNER_CHECK_ERROR
#define _TPOT_RUNNER_CHECK_ERROR(prefix, code, node, shared)                                       \
    auto shared = node.lock();                                                                     \
    if (static_cast<bool>(shared) == false) { tDLogE(prefix " Expired client."); return; }         \
    if (code != Err::E_SUCCESS) { tDLogE(prefix " {} error.", getErrName(code)); return; }         \
    tDLogN(prefix " [ID:{}] {}:{}", shared->getId(), shared->getDestination(), shared->getPort()); \
    /* END */
#endif

void TpotRunner::onNodeRequest(Err code, Node node, HttpParser const & request,
                               HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeRequest()", code, node, shared);
    response.setStatus(network::http::HttpStatus::SC_NOT_FOUND);
    response.setBody(_body_4xx);
}

void TpotRunner::onNodeExecRequest(Err code, Node node, HttpParser const & request,
                                   HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeExecRequest()", code, node, shared);

    Exec body;
    if (body.fromRequestJsonString(request.getBody()) != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        response.setBody(_body_4xx);
        return;
    }

    Proc::Options options;
    options.file = body.request.file;
    options.cwd  = body.request.cwd;
    options.args = body.request.args;
    options.envs = body.request.envs;
    options.uid  = body.request.uid;
    options.gid  = body.request.gid;

    tDLogN("TpotRunner::onNodeExecRequest(ID:{}) File: {}", shared->getId(), options.file);

    auto proc = _loop.newHandle<Proc>(_loop, options);
    if (static_cast<bool>(proc) == false) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return;
    }

    if (_procs.insert(ProcPair(proc->getPid(), proc)).second == false) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return;
    }

    body.response.id = proc->getPid();

    std::string json;
    if (body.toResponseJsonString(json) != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return;
    }

    response.setStatus(network::http::HttpStatus::SC_OK);
    response.setBody(json);
}

void TpotRunner::onNodeKillRequest(Err code, Node node, HttpParser const & request,
                                   HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeKillRequest()", code, node, shared);

    Kill body;
    if (body.fromRequestJsonString(request.getBody()) != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        response.setBody(_body_4xx);
        return;
    }

    auto itr = _procs.find(body.request.id);
    if (itr == _procs.end()) {
        response.setStatus(network::http::HttpStatus::SC_NOT_FOUND);
        response.setBody(_body_4xx);
        return;
    }

    if (static_cast<bool>(itr->second) == false) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return;
    }

    Err const RESULT_CODE = itr->second->processKill(signal::SIGNAL_TERMINATION);
    if (RESULT_CODE != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return;
    }

    itr->second->close();
    _procs.erase(itr);

    response.setStatus(network::http::HttpStatus::SC_OK);
}

void TpotRunner::onNodeListRequest(Err code, Node node, HttpParser const & request,
                                   HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeListRequest()", code, node, shared);

    List body;
    for (auto & cursor : _procs) {
        int id = cursor.first;
        body.response.ids.push_back(id);
    }

    std::string json;
    if (body.toResponseJsonString(json) == Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_OK);
        response.setBody(json);
    } else {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
    }
}

void TpotRunner::onNodeHeartbitRequest(Err code, Node node, HttpParser const & request,
                                       HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeHeartbitRequest()", code, node, shared);
    response.setStatus(network::http::HttpStatus::SC_OK);
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

