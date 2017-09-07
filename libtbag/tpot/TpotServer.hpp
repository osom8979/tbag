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

#include <libtbag/string/Environments.hpp>
#include <libtbag/process/ProcessManager.hpp>
#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/proto/TpotPacket.hpp>

#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <memory>
#include <string>
#include <map>

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
class TBAG_API TpotServer : private Noncopyable
{
public:
    struct Param
    {
        bool verbose;
        std::string bind;
        int port;
    };

public:
    using Loop = uvpp::Loop;

    using HttpParser  = network::http::HttpParser;
    using HttpBuilder = network::http::HttpBuilder;
    using HttpServer  = network::http::HttpServer;

    using Id           = HttpServer::Id;
    using StreamType   = HttpServer::StreamType;
    using Client       = HttpServer::ClientInterface;
    using SharedClient = HttpServer::SharedClient;
    using WeakClient   = HttpServer::WeakClient;
    using HttpPacket   = HttpServer::HttpPacket;
    using WsPacket     = HttpServer::WsPacket;

public:
    using Environments = string::Environments;
    using EnvFlag      = Environments::Flag;

public:
    using ProcessManager = process::FunctionalProcessManager;

public:
    using FuncTpotPacket        = proto::FunctionalTpotPacket;
    using Header                = FuncTpotPacket::Header;
    using PacketVersionRequest  = FuncTpotPacket::PacketVersionRequest;
    using PacketVersionResponse = FuncTpotPacket::PacketVersionResponse;
    using ExecRequest           = FuncTpotPacket::ExecRequest;
    using ExecResponse          = FuncTpotPacket::ExecResponse;
    using HeartbitRequest       = FuncTpotPacket::HeartbitRequest;
    using HeartbitResponse      = FuncTpotPacket::HeartbitResponse;
    using ListRequest           = FuncTpotPacket::ListRequest;
    using ListResponse          = FuncTpotPacket::ListResponse;
    using KillRequest           = FuncTpotPacket::KillRequest;
    using KillResponse          = FuncTpotPacket::KillResponse;

public:
    class Server : public HttpServer
    {
    private:
        TpotServer * _parent;

    public:
        Server(TpotServer * parent, Loop & loop, StreamType type = StreamType::TCP)
                : HttpServer(loop, type), _parent(parent)
        { /* EMPTY. */ }
        virtual ~Server()
        { /* EMPTY. */ }

    public:
        virtual void onServerClose() override
        { _parent->onServerClose(); }
        virtual void onHttpOpen(WeakClient node) override
        { _parent->onHttpOpen(node); }
        virtual void onHttpRequest(WeakClient node, Err code, HttpPacket & packet) override
        { _parent->onHttpRequest(node, code, packet); }
        virtual void onHttpClose(WeakClient node) override
        { _parent->onHttpClose(node); }
        virtual void onHttpTimer(WeakClient node) override
        { _parent->onHttpTimer(node); }
        virtual void onHttpShutdown(WeakClient node, Err code) override
        { _parent->onHttpShutdown(node, code); }
        virtual void onHttpWrite(WeakClient node, Err code) override
        { _parent->onHttpWrite(node, code); }
    };

    friend class Server;

    using SharedServer = std::shared_ptr<Server>;
    using WeakServer   = std::weak_ptr<Server>;

public:
    // --------------------
    // HTTP Path structure.
    // --------------------

#ifndef _TPOT_CREATE_PATH_STRUCTURE
#define _TPOT_CREATE_PATH_STRUCTURE(name, path, method) \
    struct name : public libtbag::network::http::HttpMethod##method \
    { TBAG_CONSTEXPR static char const * const getPath() TBAG_NOEXCEPT { return path; } };
#endif

    _TPOT_CREATE_PATH_STRUCTURE( VersionPath, "/ver" ,    GET)
    _TPOT_CREATE_PATH_STRUCTURE(    ExecPath, "/exec",    PUT)
    _TPOT_CREATE_PATH_STRUCTURE(HeartbitPath, "/hbit",    GET)
    _TPOT_CREATE_PATH_STRUCTURE(    ListPath, "/list",    GET)
    _TPOT_CREATE_PATH_STRUCTURE(    KillPath, "/kill", DELETE)

#undef _TPOT_CREATE_PATH_STRUCTURE

    TBAG_CONSTEXPR static char const * const getAcceptKey  () TBAG_NOEXCEPT { return "Accept"; }
    TBAG_CONSTEXPR static char const * const getAcceptValue() TBAG_NOEXCEPT { return "application/octet-stream"; }

private:
    Param _param;

private:
    Loop _loop;
    SharedServer _server;
    ProcessManager _procs;

private:
    FuncTpotPacket _packet;
    Environments   _envs;
    std::string    _body_4xx;
    std::string    _body_5xx;

public:
    TpotServer(Param param);
    virtual ~TpotServer();

public:
    int run();

protected:
    void onServerClose ();
    void onHttpOpen    (WeakClient node);
    void onHttpRequest (WeakClient node, Err code, HttpPacket & packet);
    void onHttpClose   (WeakClient node);
    void onHttpTimer   (WeakClient node);
    void onHttpShutdown(WeakClient node, Err code);
    void onHttpWrite   (WeakClient node, Err code);

    void onVersionRequest (WeakClient node, Err code, HttpPacket & packet);
    void onExecRequest    (WeakClient node, Err code, HttpPacket & packet);
    void onHeartbitRequest(WeakClient node, Err code, HttpPacket & packet);
    void onListRequest    (WeakClient node, Err code, HttpPacket & packet);
    void onKillRequest    (WeakClient node, Err code, HttpPacket & packet);

    void onPacketVersionRequest(Header const & header, PacketVersionRequest const & packet, HttpPacket & hp);
    void onExecRequest         (Header const & header, ExecRequest          const & packet, HttpPacket & hp);
    void onHeartbitRequest     (Header const & header, HeartbitRequest      const & packet, HttpPacket & hp);
    void onListRequest         (Header const & header, ListRequest          const & packet, HttpPacket & hp);
    void onKillRequest         (Header const & header, KillRequest          const & packet, HttpPacket & hp);

// Process callback.
public:
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTSERVER_HPP__

