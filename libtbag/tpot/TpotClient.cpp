/**
 * @file   TpotClient.cpp
 * @brief  TpotClient class implementation.
 * @author zer0
 * @date   2017-09-07
 */

#include <libtbag/tpot/TpotClient.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/string/StringUtils.hpp>

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

    HttpRequest request;
    request.setMethod(getHttpMethod(method));
    request.body.assign(buffer, buffer + size);
    return requestWithSync(_param.type, _param.ip, _param.port, network::Uri(ss.str()), request, _param.timeout, response);
}

Err TpotClient::requestCommon(std::string const & method, std::string const & path)
{
    using namespace proto;
    HttpResponse response;
    Err const CODE = request(method, path, this->point(), this->size(), response);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("TpotClient::Internal::requestCommon({}) Request {} error", path, getErrName(CODE));
        return CODE;
    }

    if (!(100 <= COMPARE_AND(response.status) < 300)) { /* 1xx ~ 2xx */
        tDLogE("TpotClient::Internal::requestCommon({}) Response error status code: {}", path, response.status);
        if (300 <= COMPARE_AND(response.status) < 400) {
            return Err::E_HTTP_3XX;
        } else if (500 <= COMPARE_AND(response.status) < 500) {
            return Err::E_HTTP_4XX;
        } else {
            return Err::E_UNKNOWN;
        }
    }

    Err const PARSE_CODE = this->parse(response.body.data(), response.body.size());
    if (TBAG_ERR_FAILURE(PARSE_CODE)) {
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

Err TpotClient::requestExec(std::string const & file,
                            std::vector<std::string> const & args,
                            std::vector<std::string> const & envs,
                            std::string const & cwd,
                            std::string const & input)
{
    buildExecRequest(util::Header(), file, args, envs, cwd, input);
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

    std::string file;
    std::string cwd;
    std::vector<std::string> args;
    std::vector<std::string> envs;
    std::string input;
    std::string msg = "TpoT";
    std::string id;
    std::string pw;
    std::string key;
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
        commander.insertDefault("file" , &file , std::string(), std::string("Executable file path [")  + EXEC_CMD     + "]");
        commander.insertDefault("cwd"  , &cwd  , std::string(), std::string("Working directory [")     + EXEC_CMD     + "]");
        commander.insertDefault("input", &input, std::string(), std::string("Standard input string [") + EXEC_CMD     + "]");
        commander.insert("args", [&](Arguments const & a){ args = a.getStrings(); }, std::string("Command-line arguments [") + EXEC_CMD + "]");
        commander.insert("envs", [&](Arguments const & a){ envs = a.getStrings(); }, std::string("Environment variables [")  + EXEC_CMD + "]");
        commander.insertDefault("msg" , &msg ,        "TpoT", std::string("Message [") + ECHO_CMD + "]");
        commander.insertDefault("pid" , &pid ,             0, std::string("Process id [") + PROCESS_KILL_CMD + "," + PROCESS_REMOVE_CMD + "]");
        commander.insertDefault("id"  , &id  , std::string(), std::string("Login ID [") + LOGIN_CMD + "]");
        commander.insertDefault("pw"  , &pw  , std::string(), std::string("Login PW [") + LOGIN_CMD + "]");
        commander.insertDefault("key" , &key , std::string(), std::string("Authentication KEY CODE"));
        commander.insertDefault("help", &help,          true, std::string("Help message"));
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
    Err request_code = Err::E_UNKNOWN;

//    if (commands[0] == VERSION_CMD) {
//        request_code = client.requestVersion(&result);
//        std::cout << string::fformat("Request version (ID:{}, CODE:{}, VER:{})\n", result.response_id, result.code, result.response.version->toString());
//
//    } else if (commands[0] == EXEC_CMD) {
//        request_code = client.requestExec(file, args, envs, cwd, input, &result);
//        std::cout << string::fformat("Request exec (ID:{}, CODE:{}, PID:{})\n", result.response_id, result.code, *(result.response.pid));
//
//    } else if (commands[0] == HEARTBIT_CMD) {
//        request_code = client.requestHeartbit(msg, &result);
//        std::cout << string::fformat("Request heartbit (ID:{}, CODE:{}, ECHO:{})\n", result.response_id, result.code, *(result.response.echo));
//
//    } else if (commands[0] == LIST_CMD) {
//        request_code = client.requestList(&result);
//        std::cout << string::fformat("Request list (ID:{}, CODE:{}, Size:{})\n", result.response_id, result.code, result.response.procs->size());
//        for (auto & proc : *(result.response.procs)) {
//            std::cout << proc.pid << "[" << proc.active << "],";
//        }
//        std::cout << std::endl;
//
//    } else if (commands[0] == KILL_CMD) {
//        request_code = client.requestKill(pid, &result);
//        std::cout << string::fformat("Request kill (ID:{}, CODE:{})\n", result.response_id, result.code);
//
//    } else {
//        request_code = Err::E_UNKNOWN;
//        std::cerr << "Unknown command: " << commands[0] << std::endl;
//    }

    return TBAG_ERR_SUCCESS(request_code) ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

