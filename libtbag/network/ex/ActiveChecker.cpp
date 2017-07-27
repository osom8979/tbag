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
class ActiveChecker::Internal : private libtbag::Noncopyable
{
public:
    using Loop = libtbag::uvpp::Loop;
    using binf = libtbag::uvpp::binf;

    using FuncUdp     = libtbag::uvpp::func::FuncUdp;
    using FuncTimer   = libtbag::uvpp::func::FuncTimer;
    using SafetyAsync = libtbag::uvpp::ex::SafetyAsync;

    using SharedFuncUdp     = std::shared_ptr<FuncUdp>;
    using SharedFuncTimer   = std::shared_ptr<FuncTimer>;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;

    using SocketAddress = libtbag::network::SocketAddress;
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

protected:
    binf onRecvAlloc(std::size_t suggested_size)
    {
        return uvpp::defaultOnAlloc(_buffer, suggested_size);
    }

    void onRecvRequest(Err code, std::string const & data, sockaddr const * addr)
    {
        Err send_result = Err::E_UNKNOWN;
        std::size_t const SEND_SIZE = _sender->trySend(&_json[0], _json.size(), addr, &send_result);

        if (send_result != Err::E_SUCCESS) {
            tDLogE("ActiveChecker::Internal::onRecvRequest() send {} error", libtbag::getErrName(send_result));
        }

        if (SEND_SIZE != _json.size()) {
            tDLogW("ActiveChecker::Internal::onRecvRequest() The sended size is not correct ({}/{})",
                   SEND_SIZE, _json.size());
        }
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

        _receiver->setOnAlloc([&](std::size_t suggested_size){
            return onRecvAlloc(suggested_size);
        });
        _receiver->setOnRecv([&](Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags){
            // The receive callback will be called with nread == 0 and addr == NULL when there is nothing to read,
            // and with nread == 0 and addr != NULL when an empty UDP packet is received.
            if (code != Err::E_SUCCESS) {
            }
            onRecvRequest(code, std::string(buffer, buffer + size), addr);
        });
        _receiver->setOnClose([&](){
            _parent->onClose();
        });

        if (uvpp::initRecvUdp(*_receiver, bind_ip, bind_port) == false) {
            closeByNoSafe();
            return false;
        }

        Err const START_RECV_RESULT = _receiver->startRecv();
        if (START_RECV_RESULT != Err::E_SUCCESS) {
            closeByNoSafe();
            return false;
        }

        return true;
    }

    bool initClient(Loop & loop, std::string const & bind_ip, int bind_port, int send_port, uint64_t timeout, Request const & request)
    {
        _json     = request.toJsonString();
        _receiver = loop.newHandle<FuncUdp>(loop);
        _sender   = loop.newHandle<FuncUdp>(loop);
        _async    = loop.newHandle<SafetyAsync>(loop);

        if (timeout > 0U) {
            _timer = loop.newHandle<FuncTimer>(loop);
        }

        Err const SET_BROADCAST_RESULT_CODE = _sender->setBroadcast(true);
        if (SET_BROADCAST_RESULT_CODE != Err::E_SUCCESS) {
            tDLogE("ActiveChecker::Internal::initClient() Set broadcast {} error", libtbag::getErrName(SET_BROADCAST_RESULT_CODE));
            closeByNoSafe();
            return false;
        }

        _receiver->setOnAlloc([&](std::size_t suggested_size){
            return libtbag::uvpp::defaultOnAlloc(_buffer, suggested_size);
        });
        _receiver->setOnRecv([&](Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags){
            Response res;
            if (res.fromJsonString(std::string(buffer, buffer + size))) {
                _parent->onResponse(res.server_type, res.server_ip, res.server_port);
            }
        });
        _receiver->setOnClose([&](){
            _parent->onClose();
        });

        if (libtbag::uvpp::initRecvUdp(*_receiver, bind_ip, bind_port) == false) {
            closeByNoSafe();
            return false;
        }

        Err const START_RECV_RESULT = _receiver->startRecv();
        if (START_RECV_RESULT != Err::E_SUCCESS) {
            closeByNoSafe();
            return false;
        }

        SocketAddress addr;
        addr.init(libtbag::network::details::BROADCAST_SUBNET_IPV4, send_port);

        Err send_result = Err::E_UNKNOWN;
        std::size_t const SEND_SIZE = _sender->trySend(&_json[0], _json.size(), addr.getIpv4(), &send_result);
        if (send_result != Err::E_SUCCESS) {
            tDLogE("ActiveChecker::Internal::initClient() send {} error", libtbag::getErrName(send_result));
            closeByNoSafe();
            return false;
        }

        _timer->setOnTimer([&](){
            closeByNoSafe();
        });
        _timer->start(timeout);

        return true;
    }

private:
    void closeByNoSafe()
    {
        // @formatter:off
        if (static_cast<bool>(_receiver)) { _receiver->close(); }
        if (static_cast<bool>(_sender))   {   _sender->close(); }
        if (static_cast<bool>(_timer))    {    _timer->close(); }
        if (static_cast<bool>(_async))    {    _async->close(); }
        _receiver.reset();
        _sender.reset();
        _timer.reset();
        _async.reset();
        // @formatter:on
    }

public:
    bool close()
    {
        if (static_cast<bool>(_async)) {
            auto job = _async->newSendFunc([&](){
                closeByNoSafe();
            });

            if (static_cast<bool>(job)) {
                return true;
            }
        }

        closeByNoSafe();
        return false;
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

ActiveChecker::ActiveChecker(Loop & loop, std::string const & ip, int port, Response const & response)
        : _internal(new Internal(this))
{
    assert(static_cast<bool>(_internal));
    _internal->initServer(loop, ip, port, response);
}

ActiveChecker::ActiveChecker(Loop & loop, std::string const & ip, int port, int broadcast_port,
                             uint64_t timeout, Request const & request)
        : _internal(new Internal(this))
{
    assert(static_cast<bool>(_internal));
    _internal->initClient(loop, ip, port, broadcast_port, timeout, request);
}

ActiveChecker::~ActiveChecker()
{
    // EMPTY.
}

void ActiveChecker::close()
{
    assert(static_cast<bool>(_internal));
    _internal->close();
}

void ActiveChecker::onResponse(EndPointType type, std::string const & ip, int port)
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

