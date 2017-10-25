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

#include <libtbag/network/http/base/HttpProperty.hpp>
#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/proto/TpotPacket.hpp>

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
class TBAG_API TpotClient : protected proto::TpotPacket
{
public:
    using Parent = proto::TpotPacket;

    using HttpClient   = network::http::HttpClient;
    using HttpRequest  = network::http::HttpRequest;
    using HttpResponse = network::http::HttpResponse;
    using StreamType   = HttpClient::StreamType;

public:
    struct Param
    {
        std::string ip;
        int         port;
        StreamType  type;
        uint64_t    timeout;
        bool        verbose;

        Param(std::string const & i = network::details::LOOPBACK_IPV4,
              int p = proto::DEFAULT_TPOT_SERVER_PORT_NUMBER,
              StreamType t = StreamType::TCP,
              uint64_t o = proto::DEFAULT_TPOT_TIMEOUT_MILLISEC,
              bool v = false) : ip(i), port(p), type(t), timeout(o), verbose(v)
        { /* EMPTY. */ }
        ~Param()
        { /* EMPTY. */ }
    };

private:
    Param _param;

public:
    TpotClient(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    TpotClient(Param const & param, std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~TpotClient();

public:
    inline void setParam(Param const & param) { _param = param; }
    inline Param getParam() const { return _param; }

private:
    Err request(std::string const & method, std::string const & path,
                uint8_t const * buffer, std::size_t size, HttpResponse & response);
    Err requestCommon(std::string const & method, std::string const & path);

public:
    Err requestVersion();
    Err requestEcho(std::string const & message);
    Err requestLogin(std::string const & id, std::string const & pw);
    Err requestLogout();
    Err requestExec(util::ExecParam const & param);
    Err requestProcessList();
    Err requestProcessKill(int pid, int signum);
    Err requestProcessRemove(int pid);

private:
    // @formatter:off
    virtual void onVersionRequest      (util::Header const & header, void * arg) override { /* EMPTY. */ }
    virtual void onEchoRequest         (util::Header const & header, std::string const & message, void * arg) override { /* EMPTY. */ }
    virtual void onLoginRequest        (util::Header const & header, std::string const & id, std::string const & pw, void * arg) override { /* EMPTY. */ }
    virtual void onLogoutRequest       (util::Header const & header, void * arg) override { /* EMPTY. */ }
    virtual void onExecRequest         (util::Header const & header, util::ExecParam const & exec, void * arg) override { /* EMPTY. */ }
    virtual void onProcessListRequest  (util::Header const & header, void * arg) override { /* EMPTY. */ }
    virtual void onProcessKillRequest  (util::Header const & header, int pid, int signum, void * arg) override { /* EMPTY. */ }
    virtual void onProcessRemoveRequest(util::Header const & header, int pid, void * arg) override { /* EMPTY. */ }
    // @formatter:on
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

