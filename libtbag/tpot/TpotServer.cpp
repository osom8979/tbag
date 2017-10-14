/**
 * @file   TpotServer.cpp
 * @brief  TpotServer class implementation.
 * @author zer0
 * @date   2017-09-06
 */

#include <libtbag/tpot/TpotServer.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/network/http/FunctionalHttpServer.hpp>
#include <libtbag/util/Version.hpp>

#include <libtbag/uvpp/func/FunctionalSignal.hpp>
#include <libtbag/uvpp/UvCommon.hpp>
#include <libtbag/uvpp/UvUtils.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <libtbag/proto/TpotPacket.hpp>

#include <cassert>
#include <cstdlib>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR static char const * const TPOT_HTML_BAD_REQUEST_BODY = R"HTML(
<html>
<head><title>TpoT</title></head>
<body><h2>TpoT</h2><p>Bad Request</p></body>
</html>
)HTML";

TBAG_CONSTEXPR static char const * const TPOT_HTML_INTERNAL_SERVER_ERROR_BODY = R"HTML(
<html>
<head><title>TpoT</title></head>
<body><h2>TpoT</h2><p>Internal Server Error</p></body>
</html>
)HTML";

// ------------------------------------
// TpotServer::Internal implementation.
// ------------------------------------

class TpotServer::Internal
{
public:
    using SharedServer = std::shared_ptr<network::http::FuncHttpServer>;
    using SharedSignal = std::shared_ptr<uvpp::func::FuncSignal>;

private:
    TpotServer * _parent;
    SharedServer _server;
    SharedSignal _signal;

private:
    uint64_t _timeout;
    bool _verbose;

public:
    Internal(TpotServer * parent) : _parent(parent), _server(), _signal()
    { /* EMPTY. */ }
    ~Internal()
    { /* EMPTY. */ }

public:
    Err init(Loop & loop, Param const & param)
    {
        _timeout = param.timeout;
        _verbose = param.verbose;

        try {
            using namespace network::http;
            using namespace uvpp::func;
            _server.reset(new FuncHttpServer(loop, param.type));
            _signal = loop.newHandle<FuncSignal>(loop);
        } catch (...) {
            tDLogE("TpotServer::Internal::init() Server bad allocation.");
            return Err::E_BADALLOC;
        }

        assert(static_cast<bool>(_server));
        assert(static_cast<bool>(_signal));

        tDLogIfN(_verbose, "TpoT initialize ({}:{}) ...", param.bind, param.port);
        Err const INIT_CODE = _server->init(param.bind.c_str(), param.port);
        if (TBAG_ERR_FAILURE(INIT_CODE)) {
            tDLogE("TpotServer::Internal::init() Server init {} error", getErrName(INIT_CODE));
            return INIT_CODE;
        }

        BRACE("Close callbacks") {
            _signal->setOnSignal([&](int signum){
                tDLogW("TpotServer::Internal::init()::OnSignal() Signal: {}", libtbag::signal::getSignalName(signum));
                close();
            });
            _server->set_onServerClose([&](){
                tDLogN("TpotServer::Internal::init()::OnServerClose()");
                if (static_cast<bool>(_signal) && _signal->isClosing() == false) {
                    _signal->close();
                }
                onServerClose();
            });
        }

        // @formatter:off
        using namespace proto;
        _server->set_onHttpOpen([&](WeakClient n){ onHttpOpen(n); });
        _server->set_onHttpClose([&](WeakClient n){ onHttpClose(n); });
        _server->set_onHttpRequest([&](WeakClient n, Err c, HttpPacket & p){ onHttpRequest(n, c, p); });
        auto const TPOT_REQUEST_CB = [&](WeakClient n, Err c, HttpPacket & p){ onTpotRequest(n, c, p); };
        _server->setRequest(      VersionPath::getMethod(),       VersionPath::getPath(), TPOT_REQUEST_CB);
        _server->setRequest(         EchoPath::getMethod(),          EchoPath::getPath(), TPOT_REQUEST_CB);
        _server->setRequest(        LoginPath::getMethod(),         LoginPath::getPath(), TPOT_REQUEST_CB);
        _server->setRequest(       LogoutPath::getMethod(),        LogoutPath::getPath(), TPOT_REQUEST_CB);
        _server->setRequest(         ExecPath::getMethod(),          ExecPath::getPath(), TPOT_REQUEST_CB);
        _server->setRequest(  ProcessListPath::getMethod(),   ProcessListPath::getPath(), TPOT_REQUEST_CB);
        _server->setRequest(  ProcessKillPath::getMethod(),   ProcessKillPath::getPath(), TPOT_REQUEST_CB);
        _server->setRequest(ProcessRemovePath::getMethod(), ProcessRemovePath::getPath(), TPOT_REQUEST_CB);
        // @formatter:on

        tDLogN("TpoT initialize success! (BIND: {}, PORT: {})", _server->dest(), _server->port());
        tDLogIfN(_verbose, "===== TPOT INFORMATION =====");
        tDLogIfN(_verbose, "* Tbag version: {}", util::getTbagVersion().toString());
        tDLogIfN(_verbose, "* Packet version: {}", util::getTbagPacketVersion().toString());
        tDLogIfN(_verbose, "* Release version: {}", util::getTbagReleaseVersion().toString());
        return Err::E_SUCCESS;
    }

    Err close()
    {
        tDLogN("TpotServer::Internal::close()");
        if (static_cast<bool>(_signal) && _signal->isClosing() == false) {
            _signal->close();
        }
        assert(static_cast<bool>(_server));
        return _server->close();
    }

#ifndef _TPOT_SERVER_CHECK_ERROR
#define _TPOT_SERVER_CHECK_ERROR(prefix, code, node, shared)                               \
    auto shared = node.lock();                                                             \
    if (static_cast<bool>(shared) == false) { tDLogE(prefix " Expired client."); return; } \
    if (code != Err::E_SUCCESS) { tDLogE(prefix " {} error.", getErrName(code)); return; } \
    tDLogN(prefix " [ID:{}] {}:{}", shared->id(), shared->dest(), shared->port());         \
    /* END */
#endif

    void onHttpRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onHttpRequest()", code, node, shared);
        packet.response.setHttpStatus(network::http::HttpStatus::SC_NOT_FOUND);
        packet.response.setBody(TPOT_HTML_BAD_REQUEST_BODY);
    }

    void onTpotRequest(WeakClient node, Err code, HttpPacket & packet)
    {
        _TPOT_SERVER_CHECK_ERROR("TpotServer::Internal::onTpotRequest()", code, node, shared);
        assert(_parent != nullptr);
        auto body = packet.request.body;
        if (_parent->parse(body.data(), body.size(), &packet) != Err::E_SUCCESS) {
            packet.response.setHttpStatus(network::http::HttpStatus::SC_BAD_REQUEST);
            packet.response.setBody(TPOT_HTML_BAD_REQUEST_BODY);
        }
    }

#undef _TPOT_SERVER_CHECK_ERROR

    void onHttpOpen(WeakClient node)
    {
        if (auto shared = node.lock()) {
            assert(_parent != nullptr);
            shared->setWriteTimeout(_parent->_param.timeout);
        } else {
            tDLogE("TpotServer::Internal::onHttpOpen() Expired client.");
        }
    }

    void onHttpClose(WeakClient node)
    {
        if (auto shared = node.lock()) {
            assert(_parent != nullptr);
            tDLogIfI(_verbose, "TpotServer::Internal::onHttpClose() [ID:{}]", shared->id());
        } else {
            tDLogE("TpotServer::Internal::onHttpClose() Expired client.");
        }
    }

    void onServerClose()
    {
        tDLogIfI(_verbose, "TpotServer::Internal::onServerClose()");
    }
};

// --------------------------
// TpotServer implementation.
// --------------------------

TpotServer::TpotServer(std::size_t capacity)
        : TpotServer(Param(), capacity)
{
    assert(static_cast<bool>(_internal));
}

TpotServer::TpotServer(Param const & param, std::size_t capacity)
        : TpotPacket(capacity), ProcessManager(), _internal(new Internal(this)), _param(param),
          _asset(res::getDynamicAsset(param.var, {TBAG_DYNAMIC_ASSERT_LAYOUT_LOG}))
{
    tDLogI("TpotServer::TpotServer() Variable directory: {}", param.var);
    auto const LOG_DIR = _asset.get(TBAG_DYNAMIC_ASSERT_LAYOUT_LOG);
    if (LOG_DIR.exists() == false) {
        tDLogI("TpotServer::TpotServer() Create log directory: {}", LOG_DIR.toString());
        LOG_DIR.createDir();
    }
    assert(static_cast<bool>(_internal));
}

TpotServer::~TpotServer()
{
    // EMPTY.
}

Err TpotServer::init(Loop & loop)
{
    assert(static_cast<bool>(_internal));
    return _internal->init(loop, _param);
}

// ---------------------
// ProcessManager event.
// ---------------------

void TpotServer::onOutRead(int pid, char const * buffer, std::size_t size)
{
    tDLogIfI(_param.verbose, "TpotServer::onOutRead({}) {}", pid, std::string(buffer, buffer + size));
}

void TpotServer::onErrRead(int pid, char const * buffer, std::size_t size)
{
    tDLogIfW(_param.verbose, "TpotServer::onErrRead({}) {}", pid, std::string(buffer, buffer + size));
}

void TpotServer::onExit(int pid, int64_t exit_status, int term_signal)
{
    tDLogI("TpotServer::onExit({}) Exit: {}, TermSignal: {}", pid, exit_status, term_signal);
}

// -----------------
// TpotPacket event.
// -----------------

void TpotServer::onVersionRequest(util::Header const & header, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);
    util::Version const PACKET_VERSION = util::getTbagPacketVersion();
    tDLogI("TpotServer::onVersionRequest() Response OK (Version: {})", PACKET_VERSION.toString());
    buildVersionResponse(util::Header(header.id), PACKET_VERSION.getMajor(), PACKET_VERSION.getMinor());
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

void TpotServer::onEchoRequest(util::Header const & header, std::string const & message, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);
    tDLogI("TpotServer::onEchoRequest() Response OK (Echo: {})", message);
    buildEchoResponse(util::Header(header.id), message);
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

void TpotServer::onLoginRequest(util::Header const & header, std::string const & id, std::string const & pw, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);

    std::string key;
    // TODO: LOGIN

    tDLogI("TpotServer::onLoginRequest() Response OK (ID: {})", id);
    buildLoginResponse(util::Header(header.id), key);
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

void TpotServer::onLogoutRequest(util::Header const & header, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);

    std::string id;
    // TODO: LOGOUT

    tDLogI("TpotServer::onLogoutRequest() Response OK (ID: {})", id);
    buildLogoutResponse(util::Header(header.id));
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

void TpotServer::onExecRequest(util::Header const & header, util::ExecParam const & exec, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);

    using ResultCode = proto::TpotPacketTypes::ResultCode;
    util::Header::Code code;

    int const PID = ProcessManager::exec(exec.file, exec.args, exec.envs, exec.cwd, exec.input);
    if (PID != 0) {
        code = static_cast<util::Header::Code>(ResultCode::RC_SUCCESS);
        tDLogI("TpotServer::onExecRequest() Execute OK (PID: {})", PID);
    } else {
        code = static_cast<util::Header::Code>(ResultCode::RC_EXECUTE_ERROR);
        tDLogE("TpotServer::onExecRequest() Execute error");
    }

    buildExecResponse(util::Header(header.id, code), PID);
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

void TpotServer::onProcessListRequest(util::Header const & header, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);

    auto const list = ProcessManager::list();
    tDLogI("TpotServer::onProcessListRequest() Response OK (List size: {})", list.size());

    buildProcessListResponse(util::Header(header.id), list);
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

void TpotServer::onProcessKillRequest(util::Header const & header, int pid, int signum, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);

    using ResultCode = proto::TpotPacketTypes::ResultCode;
    util::Header::Code code;

    if (ProcessManager::exists(pid)) {
        Err const CODE = ProcessManager::kill(pid, signum);
        if (TBAG_ERR_SUCCESS(CODE)) {
            code = static_cast<util::Header::Code>(ResultCode::RC_SUCCESS);
            tDLogI("TpotServer::onProcessKillRequest() Kill success (PID: {}, SIGNUM: {})", pid, signum);
        } else {
            code = static_cast<util::Header::Code>(ResultCode::RC_KILL_ERROR);
            tDLogE("TpotServer::onProcessKillRequest() Kill {} error (PID: {})", getErrName(CODE), pid);
        }
    } else {
        code = static_cast<util::Header::Code>(ResultCode::RC_NOT_EXISTS);
        tDLogW("TpotServer::onProcessKillRequest() Not exists process (PID: {})", pid);
    }

    buildProcessKillResponse(util::Header(header.id, code));
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

void TpotServer::onProcessRemoveRequest(util::Header const & header, int pid, void * arg)
{
    assert(arg != nullptr);
    HttpPacket * packet = static_cast<HttpPacket*>(arg);

    using ResultCode = proto::TpotPacketTypes::ResultCode;
    util::Header::Code code;

    if (ProcessManager::exists(pid)) {
        if (ProcessManager::erase(pid)) {
            code = static_cast<util::Header::Code>(ResultCode::RC_SUCCESS);
            tDLogI("TpotServer::onProcessRemoveRequest() Remove item (PID: {})", pid);
        } else {
            code = static_cast<util::Header::Code>(ResultCode::RC_REMOVE_ERROR);
            tDLogW("TpotServer::onProcessRemoveRequest() Remove error (PID: {})", pid);
        }
    } else {
        code = static_cast<util::Header::Code>(ResultCode::RC_NOT_EXISTS);
        tDLogW("TpotServer::onProcessRemoveRequest() Not exists process (PID: {})", pid);
    }

    buildProcessRemoveResponse(util::Header(header.id, code));
    packet->response.setHttpStatus(network::http::HttpStatus::SC_OK);
    packet->response.appendBody(getTpotPacketPointer(), getTpotPacketSize());
}

// ------------
// Entry-point.
// ------------

int runTpotServer(TpotServer::Param const & param)
{
    uvpp::Loop loop;
    TpotServer server(param);

    Err const INIT_SERVER_CODE = server.init(loop);
    if (TBAG_ERR_FAILURE(INIT_SERVER_CODE)) {
        tDLogE("runTpotServer() Server init {} error", getErrName(INIT_SERVER_CODE));
        return EXIT_FAILURE;
    }

    Err const LOOP_RUN_CODE = loop.run();
    if (TBAG_ERR_FAILURE(LOOP_RUN_CODE)) {
        tDLogE("runTpotServer() Loop {} error", getErrName(LOOP_RUN_CODE));
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

