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

TpotRunner::TpotRunner(TpotParams const & params) : _params(params)
{
    _envs.push(EnvFlag(TPOT_ENVS_EXE_PATH, filesystem::Path::getExePath()));
    _envs.push(EnvFlag(TPOT_ENVS_EXE_DIR , filesystem::Path::getExeDir()));
    _envs.push(EnvFlag(TPOT_ENVS_WORK_DIR, filesystem::Path::getWorkDir()));
    _envs.push(EnvFlag(TPOT_ENVS_HOME_DIR, filesystem::Path::getHomeDir()));
    _envs.push(EnvFlag(TPOT_ENVS_TITLE   , std::string(LIBTBAG_MAIN_TITLE)));

    _body_4xx = _envs.convert(std::string(DEFAULT_HTML_BAD_REQUEST_BODY));
    _body_5xx = _envs.convert(std::string(DEFAULT_HTML_INTERNAL_SERVER_ERROR_BODY));
}

TpotRunner::~TpotRunner()
{
    _server.reset();
}

int TpotRunner::run()
{
    _server.reset(new (std::nothrow) HttpServer(_loop, HttpServer::StreamType::TCP));
    assert(static_cast<bool>(_server));
    if (_server->init(_params.server_bind.c_str(), _params.server_port) == false) {
        return EXIT_FAILURE;
    }

    using namespace std::placeholders;
    _server->setOnOpen       (std::bind(&TpotRunner::onNodeOpen   , this, _1));
    _server->setOnClose      (std::bind(&TpotRunner::onNodeClose  , this, _1));
    _server->setOnServerClose(std::bind(&TpotRunner::onServerClose, this));

    auto const func_common   = std::bind(&TpotRunner::onNodeRequest        , this, _1, _2, _3, _4, _5);
    auto const func_exec     = std::bind(&TpotRunner::onNodeExecRequest    , this, _1, _2, _3, _4, _5);
    auto const func_kill     = std::bind(&TpotRunner::onNodeKillRequest    , this, _1, _2, _3, _4, _5);
    auto const func_list     = std::bind(&TpotRunner::onNodeListRequest    , this, _1, _2, _3, _4, _5);
    auto const func_heartbit = std::bind(&TpotRunner::onNodeHeartbitRequest, this, _1, _2, _3, _4, _5);

    _server->setOnRequest(ExecParser::getMethod(), ExecParser::getPath(), func_exec);
    _server->setOnRequest(KillParser::getMethod(), KillParser::getPath(), func_kill);
    _server->setOnRequest(ListParser::getMethod(), ListParser::getPath(), func_list);
    _server->setOnRequest(HbitParser::getMethod(), HbitParser::getPath(), func_heartbit);
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

void TpotRunner::onServerClose()
{
    tDLogI("TpotRunner::onServerClose()");
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

    Err const CODE = execProcess(request.getBody(), response);
    if (CODE == Err::E_SUCCESS) {
        tDLogI("TpotRunner::onNodeExecRequest(ID:{}) Success.", shared->getId());
    } else {
        tDLogE("TpotRunner::onNodeExecRequest(ID:{}) {} error", shared->getId(), getErrName(CODE));
    }
}

void TpotRunner::onNodeKillRequest(Err code, Node node, HttpParser const & request,
                                   HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeKillRequest()", code, node, shared);

    Err const CODE = killProcess(request.getBody(), response);
    if (CODE == Err::E_SUCCESS) {
        tDLogI("TpotRunner::onNodeKillRequest(ID:{}) Success.", shared->getId());
    } else {
        tDLogE("TpotRunner::onNodeKillRequest(ID:{}) {} error", shared->getId(), getErrName(CODE));
    }
}

void TpotRunner::onNodeListRequest(Err code, Node node, HttpParser const & request,
                                   HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeListRequest()", code, node, shared);

    Err const CODE = listProcess(request.getBody(), response);
    if (CODE == Err::E_SUCCESS) {
        tDLogI("TpotRunner::onNodeListRequest(ID:{}) Success.", shared->getId());
    } else {
        tDLogE("TpotRunner::onNodeListRequest(ID:{}) {} error", shared->getId(), getErrName(CODE));
    }
}

void TpotRunner::onNodeHeartbitRequest(Err code, Node node, HttpParser const & request,
                                       HttpBuilder & response, uint64_t & timeout)
{
    _TPOT_RUNNER_CHECK_ERROR("TpotRunner::onNodeHeartbitRequest()", code, node, shared);
    response.setStatus(network::http::HttpStatus::SC_OK);
}

// -----------------
// Process callback.
// -----------------

Err TpotRunner::execProcess(std::string const & body, HttpBuilder & response)
{
    ExecParser parser;

    Err const PARSE_RESULT = parser.fromRequestString(body);
    if (PARSE_RESULT != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        response.setBody(_body_4xx);
        return PARSE_RESULT;
    }

    if (parser.request.file.empty()) {
        response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        response.setBody(_body_4xx);
        return Err::E_ILLARGS;
    }

    Proc::Options options;
    options.file = _envs.convert(parser.request.file);

    if (parser.request.cwd.empty()) {
        options.cwd = filesystem::Path::getWorkDir().getString();
    } else {
        options.cwd = _envs.convert(parser.request.cwd);
    }

    for (auto & arg : parser.request.args) {
        options.args.push_back(_envs.convert(arg));
    }

    for (auto & env : parser.request.envs) {
        options.envs.push_back(_envs.convert(env));
    }

    options.uid  = parser.request.uid;
    options.gid  = parser.request.gid;
    options.setuid   = false;
    options.setgid   = false;
    options.detached = true;

    {   // Logging.
        std::stringstream ss;
        ss << options.file;
        for (auto & arg : options.args) {
            ss << ' ' << arg;
        }
        tDLogN("TpotRunner::execProcess() {}", ss.str());
    }

    auto proc = _loop.newHandle<Proc>(_loop, options);
    if (static_cast<bool>(proc) == false) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return Err::E_BADALLOC;
    }

    if (_procs.insert(ProcPair(proc->getPid(), proc)).second == false) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);

        proc->close();
        return Err::E_INSERT;
    }

    std::string json;
    Pid const PROCESS_ID = proc->getPid();
    parser.response.id = PROCESS_ID;

    Err const RESPONSE_RESULT = parser.toResponseString(json);
    if (RESPONSE_RESULT != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);

        proc->close();
        return RESPONSE_RESULT;
    }

    proc->setOnExit([PROCESS_ID, this](int64_t exit_status, int term_signal){
        this->onExitProcess(PROCESS_ID, exit_status, term_signal);
    });
    proc->setOnClose([PROCESS_ID, this](){
        this->onCloseProcess(PROCESS_ID);
    });

    response.setStatus(network::http::HttpStatus::SC_OK);
    response.setBody(json);
    return Err::E_SUCCESS;
}

Err TpotRunner::killProcess(std::string const & body, HttpBuilder & response)
{
    KillParser parser;

    Err const PARSE_RESULT = parser.fromRequestString(body);
    if (PARSE_RESULT != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_BAD_REQUEST);
        response.setBody(_body_4xx);
        return PARSE_RESULT;
    }

    auto itr = _procs.find(parser.request.id);
    if (itr == _procs.end()) {
        response.setStatus(network::http::HttpStatus::SC_NOT_FOUND);
        response.setBody(_body_4xx);
        return Err::E_ENFOUND;
    }

    if (static_cast<bool>(itr->second) == false) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return Err::E_EXPIRED;
    }

    Err const KILL_RESULT = itr->second->processKill(signal::SIGNAL_TERMINATION);
    if (KILL_RESULT != Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
        return KILL_RESULT;
    }

    // Warning:
    // Don't close handle, Don't erase process. This is handled in onExitProcess() callback.

    response.setStatus(network::http::HttpStatus::SC_OK);
    return Err::E_SUCCESS;
}

Err TpotRunner::listProcess(std::string const & body, HttpBuilder & response)
{
    ListParser parser;
    for (auto & cursor : _procs) {
        int id = cursor.first;
        parser.response.ids.push_back(id);
    }

    std::string json;
    Err const CODE = parser.toResponseString(json);

    if (CODE == Err::E_SUCCESS) {
        response.setStatus(network::http::HttpStatus::SC_OK);
        response.setBody(json);
    } else {
        response.setStatus(network::http::HttpStatus::SC_INTERNAL_SERVER_ERROR);
        response.setBody(_body_5xx);
    }

    return CODE;
}

void TpotRunner::onExitProcess(Pid id, int64_t exit_status, int term_signal)
{
    tDLogI("TpotRunner::onExitProcess(PID:{}) status({}) signal({})", id, exit_status, term_signal);

    auto itr = _procs.find(id);
    if (itr == _procs.end()) {
        tDLogW("TpotRunner::onExitProcess(PID:{}) Not found process.", id);
        return;
    }

    if (static_cast<bool>(itr->second) == false) {
        tDLogE("TpotRunner::onExitProcess(PID:{}) Expired process.", id);
        return;
    }

    itr->second->close();
}

void TpotRunner::onCloseProcess(Pid id)
{
    if (_procs.erase(id) == 1U) {
        tDLogI("TpotRunner::onCloseProcess(PID:{}) Erase handle success.", id);
    } else {
        tDLogE("TpotRunner::onCloseProcess(PID:{}) Erase handle failure.", id);
    }
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

