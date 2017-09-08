/**
 * @file   TpotClient.hpp
 * @brief  TpotClient class prototype.
 * @author zer0
 * @date   2017-09-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/proto/TpotPacket.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/util/ProcInfo.hpp>

#include <cstdint>
#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotClient class prototype.
 *
 * @author zer0
 * @date   2017-09-07
 */
class TBAG_API TpotClient : private Noncopyable
{
public:
    struct Param
    {
        bool verbose;
        std::string ip;
        int port;
    };

public:
    using HttpParser   = network::http::HttpParser;
    using HttpBuilder  = network::http::HttpBuilder;
    using HttpClient   = network::http::HttpClient;
    using HttpRequest  = network::http::HttpRequest;
    using HttpResponse = network::http::HttpResponse;

    using Id           = HttpClient::Id;
    using StreamType   = HttpClient::StreamType;
    using ProcInfo     = util::ProcInfo;

public:
    using FuncTpotPacket   = proto::FunctionalTpotPacket;
    using Header           = FuncTpotPacket::Header;
    using VersionResponse  = FuncTpotPacket::VersionResponse;
    using ExecResponse     = FuncTpotPacket::ExecResponse;
    using HeartbitResponse = FuncTpotPacket::HeartbitResponse;
    using ListResponse     = FuncTpotPacket::ListResponse;
    using KillResponse     = FuncTpotPacket::KillResponse;

public:
    enum class ResultType : int
    {
        Unknown,
        Version,
        Exec,
        Heartbit,
        List,
        Kill,
    };

    struct Result
    {
        uint64_t   request_id;
        uint64_t   response_id;
        uint32_t   code;
        ResultType type;

        union {
            util::Version * version;
            int * pid;
            std::string * echo;
            std::vector<ProcInfo> * procs;
        } response;

        Result(uint64_t req = 0, uint64_t res = 0, uint32_t c = 0, ResultType t = ResultType::Unknown)
                : request_id(req), response_id(res), code(c), type(t), response()
        { /* EMPTY. */ }
        ~Result()
        { /* EMPTY. */ }
    };

private:
    Param      _param;
    StreamType _type;
    uint64_t   _timeout;

private:
    FuncTpotPacket _packet;

public:
    TpotClient(Param const & param, StreamType type = StreamType::TCP);
    virtual ~TpotClient();

protected:
    void onVersionResponse (Header const & header, VersionResponse  const & packet, Result * result);
    void onExecResponse    (Header const & header, ExecResponse     const & packet, Result * result);
    void onHeartbitResponse(Header const & header, HeartbitResponse const & packet, Result * result);
    void onListResponse    (Header const & header, ListResponse     const & packet, Result * result);
    void onKillResponse    (Header const & header, KillResponse     const & packet, Result * result);

private:
    Err request(std::string const & method,
                std::string const & path,
                uint8_t const * buffer,
                std::size_t size,
                HttpResponse & response);
    Err requestCommon(std::string const & prefix,
                      std::string const & method,
                      std::string const & path,
                      Result * result = nullptr);

public:
    Err requestVersion(Result * result = nullptr);
    Err requestExec(std::string const & file,
                    std::vector<std::string> const & args,
                    std::vector<std::string> const & envs,
                    std::string const & cwd,
                    std::string const & input,
                    Result * result = nullptr);
    Err requestHeartbit(std::string const & echo, Result * result = nullptr);
    Err requestList(Result * result = nullptr);
    Err requestKill(int pid, Result * result = nullptr);
};

// ------------
// Entry-point.
// ------------

TBAG_API int requestTpotClient(TpotClient::Param const & param, std::vector<std::string> const & cmd_args);

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTCLIENT_HPP__

