/**
 * @file   ActiveChecker.cpp
 * @brief  ActiveChecker class implementation.
 * @author zer0
 * @date   2017-07-27
 */

#include <libtbag/network/ex/ActiveChecker.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/func/FunctionalUdp.hpp>
#include <libtbag/uvpp/func/FunctionalTimer.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/SocketAddress.hpp>
#include <libtbag/util/Version.hpp>
#include <libtbag/3rd/jsoncpp/json.h>

#include <cassert>
#include <vector>
#include <memory>

#ifndef _INTERNAL_PREFIX_
#define _INTERNAL_PREFIX_ "ActiveChecker::Internal::"
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace ex      {

/**
 * ActiveChecker::Internal class implementation.
 *
 * @author zer0
 * @date   2017-07-27
 */
class ActiveChecker::Internal : private Noncopyable
{
public:
    using Loop = uvpp::Loop;
    using binf = uvpp::binf;

    using FuncUdp     = uvpp::func::FuncUdp;
    using FuncTimer   = uvpp::func::FuncTimer;
    using SafetyAsync = uvpp::ex::SafetyAsync;

    using SharedFuncUdp     = std::shared_ptr<FuncUdp>;
    using SharedFuncTimer   = std::shared_ptr<FuncTimer>;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;

    using SocketAddress = network::SocketAddress;
    using Buffer        = std::vector<char>;

private:
    ActiveChecker * _parent;

private:
    SharedFuncUdp     _receiver;
    SharedFuncUdp     _sender;
    SharedFuncTimer   _timer;
    SharedSafetyAsync _async;

    Buffer _buffer;
    std::string _json;

public:
    Internal(ActiveChecker * parent) : _parent(parent)
    {
        // EMPTY.
    }

    virtual ~Internal()
    {
        // EMPTY.
    }

public:
    std::string getRecvIp() const
    {
        if (static_cast<bool>(_receiver)) {
            return _receiver->getSockIp();
        }
        return std::string();
    }

    int getRecvPort() const
    {
        if (static_cast<bool>(_receiver)) {
            return _receiver->getSockPort();
        }
        return 0;
    }

public:
    void setJsonString(std::string const & json)
    {
        _json = json;
    }

public:
    static bool bindUdp(FuncUdp & udp, std::string const & ip, int port)
    {
        SocketAddress addr;
        Err const ADDR_INIT_CODE = addr.init(ip, port);
        if (TBAG_ERR_FAILURE(ADDR_INIT_CODE)) {
            tDLogE(_INTERNAL_PREFIX_"bindUdp() SocketAddress init {} error", getErrName(ADDR_INIT_CODE));
            return false;
        }

        if (udp.isInit() == false) {
            tDLogE(_INTERNAL_PREFIX_"bindUdp() Udp is not initialized.");
            return false;
        }

        unsigned int const BIND_FLAGS = 0;
        Err const BIND_CODE = udp.bind(addr.getCommon(), BIND_FLAGS);
        if (TBAG_ERR_FAILURE(BIND_CODE)) {
            tDLogE(_INTERNAL_PREFIX_"bindUdp() Udp bind {} error.", getErrName(BIND_CODE));
            return false;
        }

        return true;
    }

    static bool startReceiver(FuncUdp & udp)
    {
        Err const START_RECV_RESULT = udp.startRecv();
        if (TBAG_ERR_FAILURE(START_RECV_RESULT)) {
            tDLogE(_INTERNAL_PREFIX_"startReceiver() Start receive {} error.", getErrName(START_RECV_RESULT));
            return false;
        }
        return true;
    }

    static bool setBroadcast(FuncUdp & udp)
    {
        Err const SET_BROADCAST_CODE = udp.setBroadcast(true);
        if (SET_BROADCAST_CODE != Err::E_SUCCESS) {
            tDLogE(_INTERNAL_PREFIX_"setBroadcast() Broadcast flag {} error", getErrName(SET_BROADCAST_CODE));
            return false;
        }
        return true;
    }

    static bool broadcast(FuncUdp & udp, int port, std::string const & data)
    {
        SocketAddress addr;
        Err const ADDR_INIT_CODE = addr.init(network::details::BROADCAST_SUBNET_IPV4, port);
        if (TBAG_ERR_FAILURE(ADDR_INIT_CODE)) {
            tDLogE(_INTERNAL_PREFIX_"broadcast() SocketAddress init {} error", getErrName(ADDR_INIT_CODE));
            return false;
        }

        Err send_result = Err::E_UNKNOWN;
        std::size_t const SEND_SIZE = udp.trySend(&data[0], data.size(), addr.getIpv4(), &send_result);
        if (send_result != Err::E_SUCCESS) {
            tDLogE(_INTERNAL_PREFIX_"broadcast() Send {} error", getErrName(send_result));
            return false;
        }

        if (SEND_SIZE == data.size()) {
            tDLogD(_INTERNAL_PREFIX_"broadcast() Send success." );
        } else {
            tDLogW(_INTERNAL_PREFIX_"broadcast() The send size is not correct ({}/{})", SEND_SIZE, data.size());
        }

        return true;
    }

public:
    bool initServer(Loop & loop, std::string const & bind_ip, int bind_port, Response const & response)
    {
        _json     = response.toJsonString();
        _receiver = loop.newHandle<FuncUdp>(loop);
        _sender   = loop.newHandle<FuncUdp>(loop);
        _async    = loop.newHandle<SafetyAsync>(loop);

        assert(_json.empty() == false);
        assert(static_cast<bool>(_receiver));
        assert(static_cast<bool>(_sender));
        assert(static_cast<bool>(_async));

        _receiver->setOnAlloc([&](std::size_t suggested_size) -> binf {
            return onRecvAlloc(suggested_size);
        });
        _receiver->setOnRecv([&](Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags){
            onRecvRequest(code, buffer, size, addr, flags);
        });
        _receiver->setOnClose([&](){
            _parent->onClose();
        });

        if (bindUdp(*_receiver, bind_ip, bind_port) == false) {
            return false;
        }
        if (startReceiver(*_receiver) == false) {
            return false;
        }
        return true;
    }

    bool initClient(Loop & loop, std::string const & bind_ip, int bind_port, uint64_t timeout, Request const & request)
    {
        _json     = request.toJsonString();
        _receiver = loop.newHandle<FuncUdp>(loop);
        _sender   = loop.newHandle<FuncUdp>(loop);
        _async    = loop.newHandle<SafetyAsync>(loop);

        if (timeout > 0U) {
            _timer = loop.newHandle<FuncTimer>(loop);
            assert(static_cast<bool>(_timer));

            _timer->setOnTimer([&](){
                _parent->onTimeout();
            });
            _timer->start(timeout);
        }

        assert(_json.empty() == false);
        assert(static_cast<bool>(_receiver));
        assert(static_cast<bool>(_sender));
        assert(static_cast<bool>(_async));

        _receiver->setOnAlloc([&](std::size_t suggested_size) -> binf {
            return onRecvAlloc(suggested_size);
        });
        _receiver->setOnRecv([&](Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags){
            onRecvResponse(code, buffer, size, addr, flags);
        });
        _receiver->setOnClose([&](){
            _parent->onClose();
        });

        if (_sender->isInit() == false) {
            tDLogE(_INTERNAL_PREFIX_"initClient() Sender is not initialized.");
            return false;
        }

        if (bindUdp(*_receiver, bind_ip, bind_port) == false) {
            return false;
        }
        if (startReceiver(*_receiver) == false) {
            return false;
        }
        if (bindUdp(*_sender, details::ANY_IPV4, 0) == false) {
            return false;
        }
        if (setBroadcast(*_sender) == false) {
            return false;
        }
        return true;
    }

    bool broadcast(int port)
    {
        return broadcast(*_sender, port, _json);
    }

protected:
    binf onRecvAlloc(std::size_t suggested_size)
    {
        return uvpp::defaultOnAlloc(_buffer, suggested_size);
    }

    void onRecvRequest(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
    {
        if (code == Err::E_EOF) {
            return;
        }
        if (TBAG_ERR_FAILURE(code)) {
            tDLogE(_INTERNAL_PREFIX_"onRecvRequest() recv {} error", getErrName(code));
            return;
        }
        if (size == 0 || addr == nullptr) {
            return;
        }

        assert(code == Err::E_SUCCESS);
        assert(size > 0);
        assert(addr != nullptr);

        Request request;
        if (request.fromJsonString(std::string(buffer, buffer + size)) == false) {
            tDLogE(_INTERNAL_PREFIX_"onRecvRequest() Parsing error of 'request data'.");
            return;
        }

        SocketAddress client_addr;
        Err const ADDR_INIT_CODE = client_addr.init(request.recv_ip, request.recv_port);
        if (TBAG_ERR_FAILURE(ADDR_INIT_CODE)) {
            tDLogE(_INTERNAL_PREFIX_"onRecvRequest() SocketAddress init {} error", getErrName(ADDR_INIT_CODE));
            return;
        }

        Err send_result = Err::E_UNKNOWN;
        std::size_t const SEND_SIZE = _sender->trySend(&_json[0], _json.size(), client_addr.getCommon(), &send_result);

        if (TBAG_ERR_FAILURE(send_result)) {
            tDLogE(_INTERNAL_PREFIX_"onRecvRequest() Send {} error", getErrName(send_result));
            return;
        }

        if (SEND_SIZE == _json.size()) {
            tDLogD(_INTERNAL_PREFIX_"onRecvRequest() Send success." );
        } else {
            tDLogW(_INTERNAL_PREFIX_"onRecvRequest() The send size is not correct ({}/{})", SEND_SIZE, _json.size());
        }
    }

    void onRecvResponse(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
    {
        if (code == Err::E_EOF) {
            return;
        }
        if (TBAG_ERR_FAILURE(code)) {
            tDLogE(_INTERNAL_PREFIX_"onRecvResponse() recv {} error", getErrName(code));
            return;
        }
        if (size == 0 || addr == nullptr) {
            return;
        }

        assert(code == Err::E_SUCCESS);
        assert(size > 0);
        assert(addr != nullptr);

        Response response;
        if (response.fromJsonString(std::string(buffer, buffer + size)) == false) {
            tDLogE(_INTERNAL_PREFIX_"onRecvResponse() Parsing error of 'response data'.");
            return;
        }

        _parent->onResponse(response.server_type, response.server_ip, response.server_port);
    }

private:
    void closeByNoSafe()
    {
        // @formatter:off
        if (static_cast<bool>(   _async)) {    _async->close();    _async.reset(); }
        if (static_cast<bool>(   _timer)) {    _timer->close();    _timer.reset(); }
        if (static_cast<bool>(  _sender)) {   _sender->close();   _sender.reset(); }
        if (static_cast<bool>(_receiver)) { _receiver->close(); _receiver.reset(); }
        // @formatter:on
    }

public:
    void close()
    {
        if (static_cast<bool>(_async)) {
            Loop * loop = _async->getLoop();
            assert(loop != nullptr);
            if (loop->isAliveAndThisThread() == false) {
                _async->newSendFunc([&](){
                    closeByNoSafe();
                });
                return;
            }
        }
        closeByNoSafe();
    }
};

// --------------------------------------
// ActiveChecker::Request implementation.
// --------------------------------------

ActiveChecker::Request::Request() : version(), recv_ip(), recv_port()
{
    // EMPTY.
}

ActiveChecker::Request::~Request()
{
    // EMPTY.
}

std::string ActiveChecker::Request::toJsonString() const
{
    Json::Value root;
    root[VERSION_KEY]   = version.toString();
    root[RECV_IP_KEY]   = recv_ip;
    root[RECV_PORT_KEY] = recv_port;
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

bool ActiveChecker::Request::fromJsonString(std::string const & json)
{
    Json::Value root;
    Json::Reader reader;

    // @formatter:off
    if (reader.parse(json, root)     == false) { return false; }
    if (root.isMember(VERSION_KEY)   == false || root[VERSION_KEY].isString() == false) { return false; }
    if (root.isMember(RECV_IP_KEY)   == false || root[RECV_IP_KEY].isString() == false) { return false; }
    if (root.isMember(RECV_PORT_KEY) == false || root[RECV_PORT_KEY].isInt()  == false) { return false; }
    // @formatter:on

    version.fromString(root[VERSION_KEY].asString());
    recv_ip   = root[RECV_IP_KEY].asString();
    recv_port = root[RECV_PORT_KEY].asInt();
    return true;
}

// ---------------------------------------
// ActiveChecker::Response implementation.
// ---------------------------------------

ActiveChecker::Response::Response()
        : version(), server_type(EndPointType::EPT_UNKNOWN),
          server_ip(), server_port()
{
    // EMPTY.
}

ActiveChecker::Response::~Response()
{
    // EMPTY.
}

std::string ActiveChecker::Response::toJsonString() const
{
    Json::Value root;
    root[VERSION_KEY]     = version.toString();
    root[SERVER_TYPE_KEY] = static_cast<int>(server_type);
    root[SERVER_IP_KEY]   = server_ip;
    root[SERVER_PORT_KEY] = server_port;
    Json::StreamWriterBuilder writer;
    return Json::writeString(writer, root);
}

bool ActiveChecker::Response::fromJsonString(std::string const & json)
{
    Json::Value root;
    Json::Reader reader;

    // @formatter:off
    if (reader.parse(json, root)       == false) { return false; }
    if (root.isMember(VERSION_KEY)     == false || root[VERSION_KEY].isString()   == false) { return false; }
    if (root.isMember(SERVER_TYPE_KEY) == false || root[SERVER_TYPE_KEY].isInt()  == false) { return false; }
    if (root.isMember(SERVER_IP_KEY)   == false || root[SERVER_IP_KEY].isString() == false) { return false; }
    if (root.isMember(SERVER_PORT_KEY) == false || root[SERVER_PORT_KEY].isInt()  == false) { return false; }
    // @formatter:on

    version.fromString(root[VERSION_KEY].asString());
    server_type = static_cast<EndPointType>(root[SERVER_TYPE_KEY].asInt());
    server_ip   = root[SERVER_IP_KEY].asString();
    server_port = root[SERVER_PORT_KEY].asInt();
    return true;
}

// -----------------------------
// ActiveChecker implementation.
// -----------------------------

ActiveChecker::ActiveChecker() : _internal(new Internal(this)), _is_init(false)
{
    // EMPTY.
}

ActiveChecker::ActiveChecker(Loop & loop, std::string const & ip, int port, Response const & response)
        : ActiveChecker()
{
    initServer(loop, ip, port, response);
}

ActiveChecker::ActiveChecker(Loop & loop, std::string const & ip, int port,
                             uint64_t timeout, Request const & request) : ActiveChecker()
{
    initClient(loop, ip, port, timeout, request);
}

ActiveChecker::ActiveChecker(Loop & loop, std::string const & ip, int port,
                             uint64_t timeout, Version const & version) : ActiveChecker()
{
    initClient(loop, ip, port, timeout, version);
}

ActiveChecker::~ActiveChecker()
{
    // EMPTY.
}

std::string ActiveChecker::getRecvIp() const
{
    assert(static_cast<bool>(_internal));
    if (_is_init) {
        return _internal->getRecvIp();
    } else {
        return std::string();
    }
}

int ActiveChecker::getRecvPort() const
{
    assert(static_cast<bool>(_internal));
    if (_is_init) {
        return _internal->getRecvPort();
    } else {
        return 0;
    }
}

bool ActiveChecker::initServer(Loop & loop, std::string const & ip, int port, Response const & response)
{
    assert(static_cast<bool>(_internal));
    _is_init = _internal->initServer(loop, ip, port, response);
    if (_is_init == false) {
        tDLogE("ActiveChecker::ActiveChecker() init server error.");
    }
    return _is_init;
}

bool ActiveChecker::initClient(Loop & loop, std::string const & ip, int port,
                               uint64_t timeout, Request const & request)
{
    assert(static_cast<bool>(_internal));
    _is_init = _internal->initClient(loop, ip, port, timeout, request);
    if (_is_init == false) {
        tDLogE("ActiveChecker::initClient() init error.");
    }
    return _is_init;
}

bool ActiveChecker::initClient(Loop & loop, std::string const & ip, int port,
                               uint64_t timeout, Version const & version)
{
    Request request;
    request.version   = version;
    request.recv_ip   = ip;
    request.recv_port = port;
    return initClient(loop, ip, port, timeout, request);
}

void ActiveChecker::setJsonString(std::string const & json)
{
    assert(static_cast<bool>(_internal));
    if (_is_init) {
        _internal->setJsonString(json);
    }
}

bool ActiveChecker::broadcast(int port)
{
    assert(static_cast<bool>(_internal));
    if (_is_init) {
        return _internal->broadcast(port);
    }
    return false;
}

void ActiveChecker::close()
{
    assert(static_cast<bool>(_internal));
    if (_is_init) {
        _internal->close();
    }
}

void ActiveChecker::onResponse(EndPointType type, std::string const & ip, int port)
{
    // EMPTY.
}

void ActiveChecker::onTimeout()
{
    // EMPTY.
}

void ActiveChecker::onClose()
{
    // EMPTY.
}

} // namespace ex
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

