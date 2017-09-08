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
#include <libtbag/proto/TpotPacket.hpp>

#include <cassert>
#include <cstdlib>
#include <sstream>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

// ------------------------------------
// TpotClient::Internal implementation.
// ------------------------------------

struct TpotClient::Internal
{
    using FuncTpotPacket   = proto::FunctionalTpotPacket;
    using Header           = FuncTpotPacket::Header;
    using VersionResponse  = FuncTpotPacket::VersionResponse;
    using ExecResponse     = FuncTpotPacket::ExecResponse;
    using HeartbitResponse = FuncTpotPacket::HeartbitResponse;
    using ListResponse     = FuncTpotPacket::ListResponse;
    using KillResponse     = FuncTpotPacket::KillResponse;

    TpotClient   * parent;
    Param          param;
    StreamType     type;
    uint64_t       timeout;
    FuncTpotPacket packet;

    Internal(TpotClient * p, Param pm, StreamType t) : parent(p), param(pm), type(t), timeout(0), packet()
    {
        // @formatter:off
        packet.setOnVersionResponse ([&](Header const & h, VersionResponse  const & p, void * a) { onVersionResponse (h, p, (Result*)a); });
        packet.setOnExecResponse    ([&](Header const & h, ExecResponse     const & p, void * a) { onExecResponse    (h, p, (Result*)a); });
        packet.setOnHeartbitResponse([&](Header const & h, HeartbitResponse const & p, void * a) { onHeartbitResponse(h, p, (Result*)a); });
        packet.setOnListResponse    ([&](Header const & h, ListResponse     const & p, void * a) { onListResponse    (h, p, (Result*)a); });
        packet.setOnKillResponse    ([&](Header const & h, KillResponse     const & p, void * a) { onKillResponse    (h, p, (Result*)a); });
        // @formatter:on
    }

    ~Internal()
    {
        // EMPTY.
    }

    Err request(std::string const & method, std::string const & path,
                uint8_t const * buffer, std::size_t size,
                HttpResponse & response)
    {
        using namespace network::http;
        std::stringstream ss;
        if (type == StreamType::TCP) {
            ss << "http://" << param.ip << ':' << param.port << path;
        } else {
            ss << param.ip;
        }

        HttpRequest request;
        request.setMethod(getHttpMethod(method));
        request.body.assign(buffer, buffer + size);
        return requestWithSync(type, param.ip, param.port, network::Uri(ss.str()), request, timeout, response);
    }

    Err requestCommon(std::string const & method, std::string const & path, Result * result)
    {
        using namespace proto;
        HttpResponse response;
        Err const CODE = request(method, path, packet.point(), packet.size(), response);
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

        Err const PARSE_CODE = packet.parse(response.body.data(), response.body.size(), result);
        if (TBAG_ERR_FAILURE(PARSE_CODE)) {
            tDLogE("TpotClient::Internal::requestCommon({}) Response parse {} error", path, getErrName(PARSE_CODE));
            return PARSE_CODE;
        }
        return Err::E_SUCCESS;
    }

    void onVersionResponse(Header const & header, VersionResponse const & packet, Result * result)
    {
        if (result == nullptr) { return; }
        result->response_id = header.id();
        result->code = static_cast<uint32_t>(header.code());
        result->type = ResultType::Version;
        result->response.version->setMajor(packet.version()->major());
        result->response.version->setMinor(packet.version()->minor());
    }

    void onExecResponse(Header const & header, ExecResponse const & packet, Result * result)
    {
        if (result == nullptr) { return; }
        result->response_id = header.id();
        result->code = static_cast<uint32_t>(header.code());
        result->type = ResultType::Exec;
        *(result->response.pid) = packet.pid();
    }

    void onHeartbitResponse(Header const & header, HeartbitResponse const & packet, Result * result)
    {
        if (result == nullptr) { return; }
        result->response_id = header.id();
        result->code = static_cast<uint32_t>(header.code());
        result->type = ResultType::Heartbit;
        result->response.echo->assign(packet.echo()->str());
    }

    void onListResponse(Header const & header, ListResponse const & packet, Result * result)
    {
        if (result == nullptr) { return; }
        result->response_id = header.id();
        result->code = static_cast<uint32_t>(header.code());
        result->type = ResultType::List;
        for (auto itr = packet.procs()->begin(); itr != packet.procs()->end(); ++itr) {
            result->response.procs->emplace_back(itr->pid(), itr->active());
        }
    }

    void onKillResponse(Header const & header, KillResponse const & packet, Result * result)
    {
        if (result == nullptr) { return; }
        result->response_id = header.id();
        result->code = static_cast<uint32_t>(header.code());
        result->type = ResultType::Kill;
    }
};

// --------------------------
// TpotClient implementation.
// --------------------------

TpotClient::TpotClient(Param const & param, StreamType type)
        : _internal(new Internal(this, param, type))
{
    assert(static_cast<bool>(_internal));
}

TpotClient::~TpotClient()
{
    // EMPTY.
}

Err TpotClient::requestVersion(Result * result)
{
    using namespace proto;
    uint64_t const REQUEST_ID = TpotPacket::genId();
    if (result != nullptr) { result->request_id = REQUEST_ID; }
    _internal->packet.buildVersionRequest(REQUEST_ID);
    return _internal->requestCommon(VersionPath::getMethod(), VersionPath::getPath(), result);
}

Err TpotClient::requestExec(std::string const & file,
                            std::vector<std::string> const & args,
                            std::vector<std::string> const & envs,
                            std::string const & cwd,
                            std::string const & input,
                            Result * result)
{
    using namespace proto;
    uint64_t const REQUEST_ID = TpotPacket::genId();
    if (result != nullptr) { result->request_id = REQUEST_ID; }
    _internal->packet.buildExecRequest(file, args, envs, cwd, input, REQUEST_ID);
    return _internal->requestCommon(ExecPath::getMethod(), ExecPath::getPath(), result);
}

Err TpotClient::requestHeartbit(std::string const & echo, Result * result)
{
    using namespace proto;
    uint64_t const REQUEST_ID = TpotPacket::genId();
    if (result != nullptr) { result->request_id = REQUEST_ID; }
    _internal->packet.buildHeartbitRequest(echo, REQUEST_ID);
    return _internal->requestCommon(HeartbitPath::getMethod(), HeartbitPath::getPath(), result);
}

Err TpotClient::requestList(Result * result)
{
    using namespace proto;
    uint64_t const REQUEST_ID = TpotPacket::genId();
    if (result != nullptr) { result->request_id = REQUEST_ID; }
    _internal->packet.buildListRequest(REQUEST_ID);
    return _internal->requestCommon(ListPath::getMethod(), ListPath::getPath(), result);
}

Err TpotClient::requestKill(int pid, Result * result)
{
    using namespace proto;
    uint64_t const REQUEST_ID = TpotPacket::genId();
    if (result != nullptr) { result->request_id = REQUEST_ID; }
    _internal->packet.buildKillRequest(pid, REQUEST_ID);
    return _internal->requestCommon(KillPath::getMethod(), KillPath::getPath(), result);
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

    std::string const  VERSION_CMD = std::string(proto:: VersionPath::getName());
    std::string const     EXEC_CMD = std::string(proto::    ExecPath::getName());
    std::string const HEARTBIT_CMD = std::string(proto::HeartbitPath::getName());
    std::string const     LIST_CMD = std::string(proto::    ListPath::getName());
    std::string const     KILL_CMD = std::string(proto::    KillPath::getName());

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
        commander.insertDefault("msg" , &msg , "TpoT", std::string("Message [")    + HEARTBIT_CMD + "]");
        commander.insertDefault("pid" , &pid ,      0, std::string("Process id [") + KILL_CMD     + "]");
        commander.insertDefault("help", &help,   true, std::string("Help message"));
        commander.setDefaultCallbackForLeftArguments(&commands);
        commander.request(MERGE_ARGS);
        // @formatter:on
    }

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
    std::vector<util::ProcInfo> result_list;
    Err request_code = Err::E_UNKNOWN;

    if (commands[0] == VERSION_CMD) {
        result.response.version = &result_version;
        request_code = client.requestVersion(&result);
        std::cout << string::fformat("Request version (ID:{}, CODE:{}, VER:{})\n", result.response_id, result.code, result.response.version->toString());

    } else if (commands[0] == EXEC_CMD) {
        result.response.pid = &result_pid;
        request_code = client.requestExec(file, args, envs, cwd, input, &result);
        std::cout << string::fformat("Request exec (ID:{}, CODE:{}, PID:{})\n", result.response_id, result.code, *(result.response.pid));

    } else if (commands[0] == HEARTBIT_CMD) {
        result.response.echo = &result_echo;
        request_code = client.requestHeartbit(msg, &result);
        std::cout << string::fformat("Request heartbit (ID:{}, CODE:{}, ECHO:{})\n", result.response_id, result.code, *(result.response.echo));

    } else if (commands[0] == LIST_CMD) {
        result.response.procs = &result_list;
        request_code = client.requestList(&result);
        std::cout << string::fformat("Request list (ID:{}, CODE:{}, Size:{})\n", result.response_id, result.code, result.response.procs->size());
        for (auto & proc : *(result.response.procs)) {
            std::cout << proc.pid << "[" << proc.active << "],";
        }
        std::cout << std::endl;

    } else if (commands[0] == KILL_CMD) {
        request_code = client.requestKill(pid, &result);
        std::cout << string::fformat("Request kill (ID:{}, CODE:{})\n", result.response_id, result.code);

    } else {
        request_code = Err::E_UNKNOWN;
        std::cerr << "Unknown command: " << commands[0] << std::endl;
    }

    return TBAG_ERR_SUCCESS(request_code) ? EXIT_SUCCESS : EXIT_FAILURE;
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

