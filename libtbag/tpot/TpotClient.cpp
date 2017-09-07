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

TpotClient::TpotClient(Param const & param, StreamType type) : _param(param), _type(type), _timeout(0)
{
    // @formatter:off
    _packet.setOnPacketVersionResponse([&](Header const & h, PacketVersionResponse const & p, void * a) { onPacketVersionResponse(h, p, (Result*)a); });
    _packet.setOnExecResponse         ([&](Header const & h, ExecResponse          const & p, void * a) { onExecResponse         (h, p, (Result*)a); });
    _packet.setOnHeartbitResponse     ([&](Header const & h, HeartbitResponse      const & p, void * a) { onHeartbitResponse     (h, p, (Result*)a); });
    _packet.setOnListResponse         ([&](Header const & h, ListResponse          const & p, void * a) { onListResponse         (h, p, (Result*)a); });
    _packet.setOnKillResponse         ([&](Header const & h, KillResponse          const & p, void * a) { onKillResponse         (h, p, (Result*)a); });
    // @formatter:on
}

TpotClient::~TpotClient()
{
    // EMPTY.
}

void TpotClient::onPacketVersionResponse(Header const & header, PacketVersionResponse const & packet, Result * result)
{
    using namespace proto::fbs::tpot;
    if (header.code() == ResultCode_SUCCESS) {
        tDLogI("TpotClient::onPacketVersionResponse(ID:{}, CODE:{}) Success", header.id(), static_cast<int>(header.code()));
    } else {
        tDLogE("TpotClient::onPacketVersionResponse(ID:{}, CODE:{}) Failure", header.id(), static_cast<int>(header.code()));
        return;
    }

    assert(result != nullptr);
    result->id   = header.id();
    result->code = static_cast<uint32_t>(header.code());
    result->type = ResultType::PacketVersion;
    result->response.version->setMajor(packet.version()->minor());
    result->response.version->setMinor(packet.version()->minor());
}

void TpotClient::onExecResponse(Header const & header, ExecResponse const & packet, Result * result)
{
    using namespace proto::fbs::tpot;
    if (header.code() == ResultCode_SUCCESS) {
        tDLogI("TpotClient::onExecResponse(ID:{}, CODE:{}) Success", header.id(), static_cast<int>(header.code()));
    } else {
        tDLogE("TpotClient::onExecResponse(ID:{}, CODE:{}) Failure", header.id(), static_cast<int>(header.code()));
        return;
    }

    assert(result != nullptr);
    result->id   = header.id();
    result->code = static_cast<uint32_t>(header.code());
    result->type = ResultType::Exec;
    *(result->response.pid) = packet.pid();
}

void TpotClient::onHeartbitResponse(Header const & header, HeartbitResponse const & packet, Result * result)
{
    using namespace proto::fbs::tpot;
    if (header.code() == ResultCode_SUCCESS) {
        tDLogI("TpotClient::onHeartbitResponse(ID:{}, CODE:{}) Success", header.id(), static_cast<int>(header.code()));
    } else {
        tDLogE("TpotClient::onHeartbitResponse(ID:{}, CODE:{}) Failure", header.id(), static_cast<int>(header.code()));
        return;
    }

    assert(result != nullptr);
    result->id   = header.id();
    result->code = static_cast<uint32_t>(header.code());
    result->type = ResultType::Heartbit;
    result->response.echo->assign(packet.echo()->str());
}

void TpotClient::onListResponse(Header const & header, ListResponse const & packet, Result * result)
{
    using namespace proto::fbs::tpot;
    if (header.code() == ResultCode_SUCCESS) {
        tDLogI("TpotClient::onListResponse(ID:{}, CODE:{}) Success", header.id(), static_cast<int>(header.code()));
    } else {
        tDLogE("TpotClient::onListResponse(ID:{}, CODE:{}) Failure", header.id(), static_cast<int>(header.code()));
        return;
    }

    assert(result != nullptr);
    result->id   = header.id();
    result->code = static_cast<uint32_t>(header.code());
    result->type = ResultType::List;

    for (auto itr = packet.procs()->begin(); itr != packet.procs()->end(); ++itr) {
        result->response.procs->emplace_back(itr->pid(), itr->active());
    }
}

void TpotClient::onKillResponse(Header const & header, KillResponse const & packet, Result * result)
{
    using namespace proto::fbs::tpot;
    if (header.code() == ResultCode_SUCCESS) {
        tDLogI("TpotClient::onListResponse(ID:{}, CODE:{}) Success", header.id(), static_cast<int>(header.code()));
    } else {
        tDLogE("TpotClient::onListResponse(ID:{}, CODE:{}) Failure", header.id(), static_cast<int>(header.code()));
        return;
    }

    assert(result != nullptr);
    result->id   = header.id();
    result->code = static_cast<uint32_t>(header.code());
    result->type = ResultType::Kill;
}

Err TpotClient::request(std::string const & method,
                        std::string const & path,
                        uint8_t const * buffer,
                        std::size_t size,
                        HttpResponse & response)
{
    using namespace network::http;
    std::stringstream ss;
    if (_type == StreamType::TCP) {
        ss << "http://" << _param.ip << ':' << _param.port << path;
    } else {
        ss << _param.ip;
    }

    HttpRequest request;
    request.setMethod(getHttpMethod(method));
    request.body.assign(buffer, buffer + size);
    return requestWithSync(_type, _param.ip, _param.port, network::Uri(ss.str()), request, _timeout, response);
}

Err TpotClient::requestCommon(std::string const & prefix,
                              std::string const & method,
                              std::string const & path,
                              Result * result)
{
    using namespace proto;
    HttpResponse response;
    Err const CODE = request(method, path, _packet.point(), _packet.size(), response);
    if (TBAG_ERR_FAILURE(CODE)) {
        tDLogE("{} Request {} error", prefix, getErrName(CODE));
        return CODE;
    }

    if (!(100 <= COMPARE_AND(response.status) < 300)) { /* 1xx ~ 2xx */
        tDLogE("{} Response error status code: {}", prefix, response.status);
        if (300 <= COMPARE_AND(response.status) < 400) {
            return Err::E_HTTP_3XX;
        } else if (500 <= COMPARE_AND(response.status) < 500) {
            return Err::E_HTTP_4XX;
        } else {
            return Err::E_UNKNOWN;
        }
    }

    Err const PARSE_CODE = _packet.parse(response.body.data(), response.body.size(), result);
    if (TBAG_ERR_FAILURE(PARSE_CODE)) {
        tDLogE("{} Response parse {} error", prefix, getErrName(PARSE_CODE));
        return PARSE_CODE;
    }
    return Err::E_SUCCESS;
}

Err TpotClient::requestPacketVersion(Result * result)
{
    _packet.buildPacketVersionRequest();
    return requestCommon("TpotClient::requestPacketVersion()",
                         proto::VersionPath::getMethod(),
                         proto::VersionPath::getPath(),
                         result);
}

Err TpotClient::requestExec(std::string const & file,
                            std::vector<std::string> const & args,
                            std::vector<std::string> const & envs,
                            std::string const & cwd,
                            std::string const & input,
                            Result * result)
{
    _packet.buildExecRequest(file, args, envs, cwd, input);
    return requestCommon("TpotClient::requestExec()",
                         proto::ExecPath::getMethod(),
                         proto::ExecPath::getPath(),
                         result);
}

Err TpotClient::requestHeartbit(std::string const & echo, Result * result)
{
    _packet.buildHeartbitRequest(echo);
    return requestCommon("TpotClient::requestHeartbit()",
                         proto::HeartbitPath::getMethod(),
                         proto::HeartbitPath::getPath(),
                         result);
}

Err TpotClient::requestList(Result * result)
{
    _packet.buildListRequest();
    return requestCommon("TpotClient::requestList()",
                         proto::ListPath::getMethod(),
                         proto::ListPath::getPath(),
                         result);
}

Err TpotClient::requestKill(int pid, Result * result)
{
    _packet.buildKillRequest(pid);
    return requestCommon("TpotClient::requestKill()",
                         proto::KillPath::getMethod(),
                         proto::KillPath::getPath(),
                         result);
}

// ------------
// Entry-point.
// ------------

TBAG_CONSTEXPR static char const * const TPOT_CLIENT_REQUEST_COMMAND_PREFIX    = "-";
TBAG_CONSTEXPR static char const * const TPOT_CLIENT_REQUEST_COMMAND_DELIMITER = "=";
TBAG_CONSTEXPR static char const * const TPOT_CLIENT_REQUEST_COMMAND_SYNOPSIS  = "Usage: test [flags] request";
TBAG_CONSTEXPR static char const * const TPOT_CLIENT_REQUEST_COMMAND_REMARKS   = "\nThe command line interface of the TpoT server.";

int requestTpotClient(TpotClient::Param const & param, std::vector<std::string> const & cmd_args)
{
    if (cmd_args.empty()) {
        std::cerr << "Empty arguments.\n";
        return EXIT_FAILURE;
    }

    std::string const MERGE_ARGS = string::mergeTokens(cmd_args, std::string(" "));
    std::vector<std::string> commands;

    std::string file;
    std::string cwd;
    std::vector<std::string> args;
    std::vector<std::string> envs;
    std::string input;
    std::string msg = "TpoT";
    int pid = 0;
    bool help = false;

    using namespace string;
    HelpCommander commander(TPOT_CLIENT_REQUEST_COMMAND_PREFIX,
                            TPOT_CLIENT_REQUEST_COMMAND_DELIMITER,
                            TPOT_CLIENT_REQUEST_COMMAND_SYNOPSIS,
                            TPOT_CLIENT_REQUEST_COMMAND_REMARKS);
    {   // Initialize commander.
        using namespace libtbag::string;
        commander.insertDefault("file" , &file ,     "", "Executable file path [exec]");
        commander.insertDefault("cwd"  , &cwd  ,     "", "Working directory [exec]");
        commander.insertDefault("input", &input,     "", "Standard input string [exec]");
        commander.insertDefault("msg"  , &msg  , "TpoT", "Message [echo]");
        commander.insertDefault("pid"  , &pid  ,      0, "Process id [kill]");
        commander.insertDefault("help" , &help ,   true, "Help message");
        commander.insert("args", [&](Arguments const & a){
            args = a.getStrings();
        }, "Command-line arguments [exec]");
        commander.insert("envs", [&](Arguments const & a){
            envs = a.getStrings();
        }, "Environment variables [exec]");
        commander.setDefaultCallbackForLeftArguments(&commands);
        commander.request(MERGE_ARGS);
    }

    std::string const  VERSION_CMD = std::string(proto:: VersionPath::getPath()).substr(1);
    std::string const     EXEC_CMD = std::string(proto::    ExecPath::getPath()).substr(1);
    std::string const HEARTBIT_CMD = std::string(proto::HeartbitPath::getPath()).substr(1);
    std::string const     LIST_CMD = std::string(proto::    ListPath::getPath()).substr(1);
    std::string const     KILL_CMD = std::string(proto::    KillPath::getPath()).substr(1);

    if (help) {
        std::cout << commander.help(true)
                  << "Request list: "
                  << VERSION_CMD  << ", "
                  << EXEC_CMD     << ", "
                  << HEARTBIT_CMD << ", "
                  << LIST_CMD     << ", "
                  << KILL_CMD     << std::endl;
        return EXIT_FAILURE;
    }

    if (commands.empty()) {
        std::cerr << "Empty command: " << MERGE_ARGS << std::endl;
        return EXIT_FAILURE;
    }

    TpotClient client(param);
    TpotClient::Result result;
    util::Version result_version;
    int result_pid;
    std::string result_echo;
    std::vector<TpotClient::ProcessInfo> result_list;
    Err request_code;

    if (commands[0] == VERSION_CMD) {
        result.response.version = &result_version;
        request_code = client.requestPacketVersion(&result);
        std::cout << string::fformat("Request version (ID:{}, CODE:{}, VER:{})\n", result.id, result.code, result.response.version->toString());

    } else if (commands[0] == EXEC_CMD) {
        result.response.pid = &result_pid;
        request_code = client.requestExec(file, args, envs, cwd, input, &result);
        std::cout << string::fformat("Request exec (ID:{}, CODE:{}, PID:{})\n", result.id, result.code, *(result.response.pid));

    } else if (commands[0] == HEARTBIT_CMD) {
        result.response.echo = &result_echo;
        request_code = client.requestHeartbit(msg, &result);
        std::cout << string::fformat("Request heartbit (ID:{}, CODE:{}, ECHO:{})\n", result.id, result.code, *(result.response.echo));

    } else if (commands[0] == LIST_CMD) {
        result.response.procs = &result_list;
        request_code = client.requestList(&result);
        std::cout << string::fformat("Request list (ID:{}, CODE:{}, Size:{})\n", result.id, result.code, result.response.procs->size());
        for (auto & proc : *(result.response.procs)) {
            std::cout << proc.pid() << "[" << proc.active() << "],";
        }
        std::cout << std::endl;

    } else if (commands[0] == KILL_CMD) {
        request_code = client.requestKill(pid, &result);
        std::cout << string::fformat("Request kill (ID:{}, CODE:{})\n", result.id, result.code);

    } else {
        std::cerr << "Unknown command: " << commands[0] << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

