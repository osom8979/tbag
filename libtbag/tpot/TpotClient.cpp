/**
 * @file   TpotClient.cpp
 * @brief  TpotClient class implementation.
 * @author zer0
 * @date   2017-09-07
 */

#include <libtbag/tpot/TpotClient.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/http/SimpleHttpClient.hpp>
#include <libtbag/network/Uri.hpp>

#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>
#include <libtbag/util/Structures.hpp>
#include <libtbag/proto/FunctionalTpotPacket.hpp>
#include <libtbag/signal/SignalHandler.hpp>

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

// --------------------------
// TpotClient implementation.
// --------------------------

TpotClient::TpotClient(std::size_t capacity) : Parent(capacity)
{
    // EMPTY.
}

TpotClient::TpotClient(Param const & param, std::size_t capacity) : Parent(capacity), _param(param)
{
    // EMPTY.
}

TpotClient::~TpotClient()
{
    // EMPTY.
}

Err TpotClient::request(std::string const & method, std::string const & path,
                        uint8_t const * buffer, std::size_t size, HttpResponse & response)
{
    using namespace network::http;
    std::stringstream ss;
    if (_param.type == StreamType::TCP) {
        ss << "http://" << _param.ip << ':' << _param.port << path;
    } else {
        ss << _param.ip;
    }

    HttpProperty request;
    request.setHttpMethod(getHttpMethod(method));
    request.body.assign(buffer, buffer + size);

    network::Uri uri(ss.str());
    return requestWithSync(uri, request, response, _param.timeout, _param.type);
}

Err TpotClient::requestCommon(std::string const & method, std::string const & path)
{
    using namespace proto;
    using namespace network::http;

    HttpResponse response;
    Err const CODE = request(method, path, this->point(), this->size(), response);
    if (isFailure(CODE)) {
        tDLogE("TpotClient::Internal::requestCommon({}) Request {} error", path, getErrName(CODE));
        return CODE;
    }

    if (!(100 <= COMPARE_AND(response.code) < 300)) { /* 1xx ~ 2xx */
        tDLogE("TpotClient::Internal::requestCommon({}) Response error status code: {}", path, response.code);
        if (300 <= COMPARE_AND(response.code) < 400) {
            return Err::E_HTTP_3XX;
        } else if (500 <= COMPARE_AND(response.code) < 500) {
            return Err::E_HTTP_4XX;
        } else {
            return Err::E_UNKNOWN;
        }
    }

    Err const PARSE_CODE = this->parse(response.body.data(), response.body.size());
    if (isFailure(PARSE_CODE)) {
        tDLogE("TpotClient::Internal::requestCommon({}) Response parse {} error", path, getErrName(PARSE_CODE));
        return PARSE_CODE;
    }
    return Err::E_SUCCESS;
}

Err TpotClient::requestVersion()
{
    buildVersionRequest(util::Header());
    return requestCommon(proto::VersionPath::getMethod(), proto::VersionPath::getPath());
}

Err TpotClient::requestEcho(std::string const & message)
{
    buildEchoRequest(util::Header(), message);
    return requestCommon(proto::EchoPath::getMethod(), proto::EchoPath::getPath());
}

Err TpotClient::requestLogin(std::string const & id, std::string const & pw)
{
    buildLoginRequest(util::Header(), id, pw);
    return requestCommon(proto::LoginPath::getMethod(), proto::LoginPath::getPath());
}

Err TpotClient::requestLogout()
{
    buildLogoutRequest(util::Header());
    return requestCommon(proto::LogoutPath::getMethod(), proto::LogoutPath::getPath());
}

Err TpotClient::requestExec(util::ExecParam const & param)
{
    buildExecRequest(util::Header(), param.file, param.args, param.envs, param.cwd, param.input);
    return requestCommon(proto::ExecPath::getMethod(), proto::ExecPath::getPath());
}

Err TpotClient::requestProcessList()
{
    buildProcessListRequest(util::Header());
    return requestCommon(proto::ProcessListPath::getMethod(), proto::ProcessListPath::getPath());
}

Err TpotClient::requestProcessKill(int pid, int signum)
{
    buildProcessKillRequest(util::Header(), pid, signum);
    return requestCommon(proto::ProcessKillPath::getMethod(), proto::ProcessKillPath::getPath());
}

Err TpotClient::requestProcessRemove(int pid)
{
    buildProcessRemoveRequest(util::Header(), pid);
    return requestCommon(proto::ProcessRemovePath::getMethod(), proto::ProcessRemovePath::getPath());
}

// ------------
// Entry-point.
// ------------

TBAG_CONSTEXPR static char const * const _TPOT_REQ_CMD_PREFIX    = "-";
TBAG_CONSTEXPR static char const * const _TPOT_REQ_CMD_DELIMITER = "=";
TBAG_CONSTEXPR static char const * const _TPOT_REQ_CMD_SYNOPSIS  = "Usage: test [flags] request";
TBAG_CONSTEXPR static char const * const _TPOT_REQ_CMD_REMARKS   = "\nThe command line interface of the TpoT server.";

int requestTpotClient(TpotClient::Param const & param, std::vector<std::string> const & cmd_args)
{
    if (cmd_args.empty()) {
        std::cerr << "Empty arguments.\n";
        return EXIT_FAILURE;
    }

    // @formatter:off
    std::string const        VERSION_CMD = proto::      VersionPath::getName();
    std::string const           ECHO_CMD = proto::         EchoPath::getName();
    std::string const          LOGIN_CMD = proto::        LoginPath::getName();
    std::string const         LOGOUT_CMD = proto::       LogoutPath::getName();
    std::string const           EXEC_CMD = proto::         ExecPath::getName();
    std::string const   PROCESS_LIST_CMD = proto::  ProcessListPath::getName();
    std::string const   PROCESS_KILL_CMD = proto::  ProcessKillPath::getName();
    std::string const PROCESS_REMOVE_CMD = proto::ProcessRemovePath::getName();
    // @formatter:on

    std::string const MERGE_ARGS = string::mergeTokens(cmd_args, std::string(" "));
    std::vector<std::string> commands;

    std::string const DEFAULT_ECHO_MESSAGE = "TpoT";

    std::string file;
    std::string cwd;
    std::vector<std::string> args;
    std::vector<std::string> envs;
    std::string input;
    std::string msg = DEFAULT_ECHO_MESSAGE;
    std::string id;
    std::string pw;
    std::string key;
    int signum = signal::TBAG_SIGNAL_TERMINATION;
    int pid = 0;
    bool help = false;

    using namespace string;
    HelpCommander commander(_TPOT_REQ_CMD_PREFIX,
                            _TPOT_REQ_CMD_DELIMITER,
                            _TPOT_REQ_CMD_SYNOPSIS,
                            _TPOT_REQ_CMD_REMARKS);
    {   // Initialize commander.
        using namespace libtbag::string;
        // @formatter:off
        commander.insertDefault("file" , &file , std::string(), std::string("Executable file path [")  + EXEC_CMD + "]");
        commander.insertDefault("cwd"  , &cwd  , std::string(), std::string("Working directory [")     + EXEC_CMD + "]");
        commander.insertDefault("input", &input, std::string(), std::string("Standard input string [") + EXEC_CMD + "]");
        commander.insert("args", [&](Arguments const & a){ args = a.getStrings(); }, std::string("Command-line arguments [") + EXEC_CMD + "]");
        commander.insert("envs", [&](Arguments const & a){ envs = a.getStrings(); }, std::string("Environment variables [")  + EXEC_CMD + "]");
        commander.insertDefault(   "msg", &msg   , DEFAULT_ECHO_MESSAGE, std::string("Message [") + ECHO_CMD + "]");
        commander.insertDefault("signum", &signum, signal::TBAG_SIGNAL_TERMINATION, std::string("Kill signal [") + PROCESS_KILL_CMD + "]");
        commander.insertDefault(   "pid", &pid   , 0, std::string("Process id [") + PROCESS_KILL_CMD + "," + PROCESS_REMOVE_CMD + "]");
        commander.insertDefault(    "id", &id    , std::string(), std::string("Login ID [") + LOGIN_CMD + "]");
        commander.insertDefault(    "pw", &pw    , std::string(), std::string("Login PW [") + LOGIN_CMD + "]");
        commander.insertDefault(   "key", &key   , std::string(), std::string("Authentication KEY CODE"));
        commander.insertDefault(  "help", &help  , true, std::string("Help message"));
        commander.setDefaultCallbackForLeftArguments(&commands);
        commander.request(MERGE_ARGS);
        // @formatter:on
    }

    if (help) {
        std::cout << commander.help(true)
                  << "Request list: "
                  <<        VERSION_CMD << ", "
                  <<           ECHO_CMD << ", "
                  <<          LOGIN_CMD << ", "
                  <<         LOGOUT_CMD << ", "
                  <<           EXEC_CMD << ", "
                  <<   PROCESS_LIST_CMD << ", "
                  <<   PROCESS_KILL_CMD << ", "
                  << PROCESS_REMOVE_CMD << std::endl;
        return EXIT_FAILURE;
    }

    if (commands.empty()) {
        std::cerr << "Empty command: " << MERGE_ARGS << std::endl;
        return EXIT_FAILURE;
    }

    proto::FunctionalTpotPacket<TpotClient> client;
    client.setParam(param);

    util::Header header;
    Err request_code = Err::E_UNKNOWN;
    std::string request_name;
    std::string request_value;

    if (commands[0] == VERSION_CMD) {
        util::Version version;
        client.set_onVersionResponse([&](util::Header const & h, util::Version const & v, util::Pairs const & p, void * a){
            header = h;
            version = v;
        });
        request_code = client.requestVersion();
        request_name = VERSION_CMD;
        request_value = version.toString();

    } else if (commands[0] == ECHO_CMD) {
        std::string message;
        client.set_onEchoResponse([&](util::Header const & h, std::string const & m, void * a){
            header = h;
            message = m;
        });
        request_code = client.requestEcho(msg);
        request_name = VERSION_CMD;
        request_value = message;

    } else if (commands[0] == LOGIN_CMD) {
        std::string key;
        client.set_onLoginResponse([&](util::Header const & h, std::string const & k, void * a){
            header = h;
            key = k;
        });
        request_code = client.requestLogin(id, pw);
        request_name = LOGIN_CMD;
        request_value = key;

    } else if (commands[0] == LOGOUT_CMD) {
        client.set_onLogoutResponse([&](util::Header const & h, void * a){
            header = h;
        });
        request_code = client.requestLogout();
        request_name = LOGOUT_CMD;
        request_value = "[EMPTY]";

    } else if (commands[0] == EXEC_CMD) {
        int pid = 0;
        client.set_onExecResponse([&](util::Header const & h, int p, void * a){
            header = h;
            pid = p;
        });
        util::ExecParam param;
        param.file  = file;
        param.args  = args;
        param.envs  = envs;
        param.cwd   = cwd;
        param.input = input;
        request_code = client.requestExec(param);
        request_name = EXEC_CMD;
        request_value = std::to_string(pid);

    } else if (commands[0] == PROCESS_LIST_CMD) {
        std::vector<util::ProcessInfo> procs;
        client.set_onProcessListResponse([&](util::Header const & h, std::vector<util::ProcessInfo> const & p, void * a){
            header = h;
            procs = p;
        });
        request_code = client.requestProcessList();
        request_name = PROCESS_LIST_CMD;

        std::stringstream ss;
        for (std::size_t i = 0; i < procs.size(); ++i) {
            ss << string::fformat("\n * [{}] {} ({})", i, procs[i].pid, (procs[i].active ? "active" : "inactive"));
            for (auto & info : procs[i].infos) {
                ss << string::fformat("[{}:{}]", info.key, info.val);
            }
        }
        request_value = string::fformat("SIZE({})", procs.size()) + ss.str();

    } else if (commands[0] == PROCESS_KILL_CMD) {
        client.set_onProcessKillResponse([&](util::Header const & h, void * a){
            header = h;
        });
        request_code = client.requestProcessKill(pid, signum);
        request_name = PROCESS_KILL_CMD;
        request_value = "[EMPTY]";

    } else if (commands[0] == PROCESS_REMOVE_CMD) {
        client.set_onProcessRemoveResponse([&](util::Header const & h, void * a){
            header = h;
        });
        request_code = client.requestProcessRemove(pid);
        request_name = PROCESS_REMOVE_CMD;
        request_value = "[EMPTY]";

    } else {
        request_code = Err::E_UNKNOWN;
        std::cerr << "Unknown command: " << commands[0] << std::endl;
    }

    if (isSuccess(request_code)) {
        std::cout << string::fformat("REQUEST [ID({}) CODE({})] {}: {}\n", header.id, header.code, request_name, request_value);
    } else {
        std::cerr << string::fformat("REQUEST({}) ERROR: {}\n", request_name, getErrName(request_code));
    }
    return isSuccess(request_code) ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

