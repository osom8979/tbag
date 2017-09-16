/**
 * @file   TpotServer.hpp
 * @brief  TpotServer class prototype.
 * @author zer0
 * @date   2017-09-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/proto/TpotPacket.hpp>
#include <libtbag/process/ProcessManager.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotServer class prototype.
 *
 * @author zer0
 * @date   2017-09-06
 */
class TBAG_API TpotServer : protected proto::TpotPacket,
                            protected process::ProcessManager
{
public:
    using TpotPacket     = proto::TpotPacket;
    using ProcessManager = process::ProcessManager;

    using Loop       = uvpp::Loop;
    using HttpServer = network::http::HttpServer;
    using StreamType = HttpServer::StreamType;
    using WeakClient = HttpServer::WeakClient;
    using HttpPacket = HttpServer::HttpPacket;

public:
    struct Param
    {
        std::string bind;
        int         port;
        StreamType  type;
        uint64_t    timeout;
        bool        verbose;

        Param(std::string const & i = network::details::ANY_IPV4,
              int p = proto::DEFAULT_TPOT_SERVER_PORT_NUMBER,
              StreamType t = StreamType::TCP,
              uint64_t o = proto::DEFAULT_TPOT_TIMEOUT_MILLISEC,
              bool v = false) : bind(i), port(p), type(t), timeout(o), verbose(v)
        { /* EMPTY. */ }
        ~Param()
        { /* EMPTY. */ }
    };

public:
    class Internal;
    friend class Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    UniqueInternal _internal;

private:
    Param _param;

public:
    TpotServer(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    TpotServer(Param const & param, std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~TpotServer();

public:
    inline void setParam(Param const & param) { _param = param; }
    inline Param getParam() const { return _param; }

public:
    inline char const * getTpotPacketPointer() const { return (char const *)TpotPacket::point(); }
    inline std::size_t  getTpotPacketSize   () const { return TpotPacket::size(); }
    inline std::size_t  getProcessSize      () const { return ProcessManager::size(); }

public:
    int run(Loop & loop);
    int run();

public:
    virtual void onVersionRequest      (util::Header const & header, void * arg) override;
    virtual void onEchoRequest         (util::Header const & header, std::string const & message, void * arg) override;
    virtual void onLoginRequest        (util::Header const & header, std::string const & id, std::string const & pw, void * arg) override;
    virtual void onLogoutRequest       (util::Header const & header, void * arg) override;
    virtual void onExecRequest         (util::Header const & header, util::ExecParam const & exec, void * arg) override;
    virtual void onProcessListRequest  (util::Header const & header, void * arg) override;
    virtual void onProcessKillRequest  (util::Header const & header, int pid, int signum, void * arg) override;
    virtual void onProcessRemoveRequest(util::Header const & header, int pid, void * arg) override;
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__

