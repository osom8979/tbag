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
#include <libtbag/util/Version.hpp>
#include <libtbag/util/ProcInfo.hpp>

#include <cstdint>
#include <vector>
#include <string>
#include <memory>

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

public:
    struct Internal;
    friend struct Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    UniqueInternal _internal;

public:
    TpotClient(Param const & param, StreamType type = StreamType::TCP);
    virtual ~TpotClient();

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

