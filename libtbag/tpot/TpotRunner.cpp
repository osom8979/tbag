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

TBAG_CONSTEXPR char const * const DEFAULT_HTML_BODY = R"HTML(
<html>
<head>
    <title>${title}</title>
</head>
<body>
    <h2>${title}</h2>
    <p>Now running...</p>
</body>
</html>
)HTML";

TBAG_CONSTEXPR char const * const NOT_FOUND_BODY = "NOT_FOUND";

TpotRunner::TpotRunner(TpotParams const & params) : _params(params)
{
    _envs.push(EnvFlag("title", std::string(LIBTBAG_MAIN_TITLE)));
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
    _server->setOnOpen   (std::bind(&TpotRunner::onNodeOpen   , this, _1));
    _server->setOnRequest(std::bind(&TpotRunner::onNodeRequest, this, _1, _2, _3, _4, _5));
    _server->setOnClose  (std::bind(&TpotRunner::onNodeClose  , this, _1));

    _server->setOnRequest(    Exec::getMethod(),     Exec::getPath(), std::bind(&TpotRunner::onNodeExecRequest    , this, _1, _2, _3, _4, _5));
    _server->setOnRequest(    Kill::getMethod(),     Kill::getPath(), std::bind(&TpotRunner::onNodeKillRequest    , this, _1, _2, _3, _4, _5));
    _server->setOnRequest(    List::getMethod(),     List::getPath(), std::bind(&TpotRunner::onNodeListRequest    , this, _1, _2, _3, _4, _5));
    _server->setOnRequest(Heartbit::getMethod(), Heartbit::getPath(), std::bind(&TpotRunner::onNodeHeartbitRequest, this, _1, _2, _3, _4, _5));

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
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeOpen() Expired client.");
        return;
    }
    tDLogN("TpotRunner::onNodeOpen(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());
}

void TpotRunner::onNodeRequest(Err code, Node node, HttpParser const & request,
                               HttpBuilder & response, uint64_t & timeout)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeRequest() Expired client.");
        return;
    }

    if (code != Err::E_SUCCESS) {
        return;
    }

    tDLogN("TpotRunner::onNodeRequest(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());

    response.setStatus(200);
    response.setReason("OK");
    response.setBody(_envs.convert(std::string(DEFAULT_HTML_BODY)));
}

void TpotRunner::onNodeExecRequest(Err code, Node node, HttpParser const & request, HttpBuilder & response, uint64_t & timeout)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeExecRequest() Expired client.");
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("TpotRunner::onNodeExecRequest() {} error.", getErrName(code));
        return;
    }

    tDLogN("TpotRunner::onNodeExecRequest(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());

    response.setStatus(404);
    response.setReason("Not found");
    response.setBody(NOT_FOUND_BODY);

    Exec body;
    if (body.fromRequestJsonString(request.getBody()) != Err::E_SUCCESS) {
        response.setStatus(400);
        response.setReason("Bad Request");
        return;
    }

    Proc::Options options;
    options.file = body.request.file;
    options.cwd  = body.request.cwd;
    options.args = body.request.args;
    options.envs = body.request.envs;
    options.uid  = body.request.uid;
    options.gid  = body.request.gid;

    auto proc = _loop.newHandle<Proc>(_loop, options);
    if (static_cast<bool>(proc) == false) {
        response.setStatus(500);
        response.setReason("Internal Server Error");
        return;
    }

    if (_procs.insert(ProcPair(proc->getPid(), proc)).second == false) {
        response.setStatus(500);
        response.setReason("Internal Server Error");
        return;
    }

    body.response.id = proc->getPid();

    std::string json;
    if (body.toResponseJsonString(json) != Err::E_SUCCESS) {
        response.setStatus(500);
        response.setReason("Internal Server Error");
        return;
    }

    response.setStatus(200);
    response.setReason("OK");
    response.setBody(json);
}

void TpotRunner::onNodeKillRequest(Err code, Node node, HttpParser const & request, HttpBuilder & response, uint64_t & timeout)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeKillRequest() Expired client.");
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("TpotRunner::onNodeKillRequest() {} error.", getErrName(code));
        return;
    }

    tDLogN("TpotRunner::onNodeKillRequest(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());

    Kill body;
    if (body.fromRequestJsonString(request.getBody()) != Err::E_SUCCESS) {
        response.setStatus(400);
        response.setReason("Bad Request");
        return;
    }

    auto itr = _procs.find(body.request.id);
    if (itr == _procs.end()) {
        response.setStatus(404);
        response.setReason("Not Found");
        return;
    }

    if (static_cast<bool>(itr->second) == false) {
        response.setStatus(500);
        response.setReason("Internal Server Error");
        return;
    }

    Err const RESULT_CODE = itr->second->processKill(signal::SIGNAL_TERMINATION);
    if (RESULT_CODE != Err::E_SUCCESS) {
        response.setStatus(500);
        response.setReason("Internal Server Error");
        return;
    }

    itr->second->close();
    _procs.erase(itr);

    response.setStatus(200);
    response.setReason("OK");
}

void TpotRunner::onNodeListRequest(Err code, Node node, HttpParser const & request, HttpBuilder & response, uint64_t & timeout)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeListRequest() Expired client.");
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("TpotRunner::onNodeListRequest() {} error.", getErrName(code));
        return;
    }

    tDLogN("TpotRunner::onNodeListRequest(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());

    List body;
    for (auto & cursor : _procs) {
        int id = cursor.first;
        body.response.ids.push_back(id);
    }

    std::string json;
    if (body.toResponseJsonString(json) == Err::E_SUCCESS) {
        response.setStatus(200);
        response.setReason("OK");
        response.setBody(json);
    } else {
        response.setStatus(500);
        response.setReason("Internal Server Error");
    }
}

void TpotRunner::onNodeHeartbitRequest(Err code, Node node, HttpParser const & request, HttpBuilder & response, uint64_t & timeout)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeHeartbitRequest() Expired client.");
        return;
    }

    if (code != Err::E_SUCCESS) {
        tDLogE("TpotRunner::onNodeHeartbitRequest() {} error.", getErrName(code));
        return;
    }

    tDLogN("TpotRunner::onNodeHeartbitRequest(ID:{}) {}:{}", shared->getId(), shared->getDestination(), shared->getPort());

    response.setStatus(200);
    response.setReason("OK");
}

void TpotRunner::onNodeClose(Node node)
{
    auto shared = node.lock();
    if (static_cast<bool>(shared) == false) {
        tDLogE("TpotRunner::onNodeOpen() Expired client.");
        return;
    }
    tDLogN("TpotRunner::onNodeClose(ID:{})", shared->getId());
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

