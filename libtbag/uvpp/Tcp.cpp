/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/network/SocketAddress.hpp>

#include <cstdint>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_tcp_connect_cb__(uv_connect_t * request, int status)
{
    // Callback called after a connection started by uv_connect() is done.
    // status will be 0 in case of success, < 0 otherwise.

    ConnectRequest * req = static_cast<ConnectRequest*>(request->data);
    if (req == nullptr) {
        tDLogE("__global_uv_tcp_connect_cb__() request.data is nullptr.");
    } else if (isDeletedAddress(req)) {
        tDLogE("__global_uv_tcp_connect_cb__() request.data is deleted.");
    } else {
        Tcp * s = static_cast<Tcp*>(req->getOwner());
        if (s == nullptr) {
            tDLogE("__global_uv_tcp_connect_cb__() request.data.owner is nullptr.");
        } else if (isDeletedAddress(s)) {
            tDLogE("__global_uv_tcp_connect_cb__() request.data.owner is deleted.");
        } else {
            s->onConnect(*req, convertUvErrorToErr(status));
        }
    }
}

// -------------------
// Tcp implementation.
// -------------------

Tcp::Tcp() : Stream(uhandle::TCP)
{
    // EMPTY.
}

Tcp::Tcp(Loop & loop) : Tcp()
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

Tcp::~Tcp()
{
    // EMPTY.
}

Err Tcp::init(Loop & loop)
{
    int const CODE = ::uv_tcp_init(loop.cast<uv_loop_t>(), Parent::cast<uv_tcp_t>());
    return convertUvErrorToErrWithLogging("Tcp::init()", CODE);
}

Err Tcp::setNodelay(bool enable)
{
    int const CODE = ::uv_tcp_nodelay(Parent::cast<uv_tcp_t>(), enable ? 1 : 0);
    return convertUvErrorToErrWithLogging("Tcp::setNodelay()", CODE);
}

Err Tcp::keepAlive(bool enable, unsigned int delay)
{
    // delay is the initial delay in seconds, ignored when enable is zero.
    int const CODE = ::uv_tcp_keepalive(Parent::cast<uv_tcp_t>(), enable ? 1 : 0, delay);
    return convertUvErrorToErrWithLogging("Tcp::keepAlive()", CODE);
}

Err Tcp::acceptsSimultaneous(int enable)
{
    // Enable/Disable simultaneous asynchronous accept requests that
    // are queued by the operating system when listening for new TCP connections.
    //
    // This setting is used to tune a TCP server for the desired performance.
    // Having simultaneous accepts can significantly improve the rate of accepting
    // connections (which is why it is enabled by default) but may lead to uneven
    // load distribution in multi-process setups.

    int const CODE = ::uv_tcp_simultaneous_accepts(Parent::cast<uv_tcp_t>(), enable ? 1 : 0);
    return convertUvErrorToErrWithLogging("Tcp::acceptsSimultaneous()", CODE);
}

Err Tcp::bind(sockaddr const * address, unsigned int flags)
{
    // addr should point to an initialized struct sockaddr_in or struct sockaddr_in6.
    // When the port is already taken, you can expect to see an UV_EADDRINUSE error
    // from either uv_tcp_bind(), uv_listen() or uv_tcp_connect().
    // That is, a successful call to this function does not guarantee that
    // the call to uv_listen() or uv_tcp_connect() will succeed as well.
    //
    // flags can contain UV_TCP_IPV6ONLY, in which case dual-stack support
    // is disabled and only IPv6 is used.

    int const CODE = ::uv_tcp_bind(Parent::cast<uv_tcp_t>(), address, flags);
    return convertUvErrorToErrWithLogging("Tcp::bind()", CODE);
}

Err Tcp::bind(sockaddr const * address, BindFlag flags)
{
    unsigned int uv_flags = 0;
    switch (flags) {
    case BindFlag::BF_IPV6_ONLY:
        uv_flags = UV_TCP_IPV6ONLY;
        break;
    case BindFlag::BF_NONE:
    default:
        uv_flags = 0;
        break;
    }

    return bind(address, uv_flags);
}

Err Tcp::getSockName(struct sockaddr * name, int * namelen)
{
    // addr must point to a valid and big enough chunk of memory,
    // struct sockaddr_storage is recommended for IPv4 and IPv6 support.

    int const CODE = ::uv_tcp_getsockname(Parent::cast<uv_tcp_t>(), name, namelen);
    return convertUvErrorToErrWithLogging("Tcp::getSockName()", CODE);
}

Err Tcp::getPeerName(struct sockaddr * name, int * namelen)
{
    // addr must point to a valid and big enough chunk of memory,
    // struct sockaddr_storage is recommended for IPv4 and IPv6 support.

    int const CODE = ::uv_tcp_getpeername(Parent::cast<uv_tcp_t>(), name, namelen);
    return convertUvErrorToErrWithLogging("Tcp::getPeerName()", CODE);
}

std::string Tcp::getSockIp()
{
    uint8_t buffer[SOCKADDR_MAX_BYTE_SIZE] = {0,};
    sockaddr * address = reinterpret_cast<struct sockaddr *>(buffer);
    int length = SOCKADDR_MAX_BYTE_SIZE;

    if (getSockName(address, &length) != Err::E_SUCCESS) {
        return std::string();
    }
    return getIpName(address);
}

std::string Tcp::getPeerIp()
{
    uint8_t buffer[SOCKADDR_MAX_BYTE_SIZE] = {0,};
    sockaddr * address = reinterpret_cast<struct sockaddr *>(buffer);
    int length = SOCKADDR_MAX_BYTE_SIZE;

    if (getPeerName(address, &length) != Err::E_SUCCESS) {
        return std::string();
    }
    return getIpName(address);
}


int Tcp::getSockPort()
{
    uint8_t buffer[SOCKADDR_MAX_BYTE_SIZE] = {0,};
    sockaddr * address = reinterpret_cast<struct sockaddr *>(buffer);
    int length = SOCKADDR_MAX_BYTE_SIZE;

    if (getSockName(address, &length) != Err::E_SUCCESS) {
        return UNKNOWN_PORT_NUMBER;
    }
    return getPortNumber(address);
}

int Tcp::getPeerPort()
{
    uint8_t buffer[SOCKADDR_MAX_BYTE_SIZE] = {0,};
    sockaddr * address = reinterpret_cast<struct sockaddr *>(buffer);
    int length = SOCKADDR_MAX_BYTE_SIZE;

    if (getPeerName(address, &length) != Err::E_SUCCESS) {
        return UNKNOWN_PORT_NUMBER;
    }
    return getPortNumber(address);
}

Err Tcp::connect(ConnectRequest & request, sockaddr const * address)
{
    request.setOwner(this); // IMPORTANT!!

    // Provide an initialized TCP handle and an uninitialized uv_connect_t.
    // addr should point to an initialized struct sockaddr_in or struct sockaddr_in6.
    //
    // The callback is made when the connection has been established
    // or when a connection error happened.

    int const CODE = ::uv_tcp_connect(request.cast<uv_connect_t>(),
                                      Parent::cast<uv_tcp_t>(),
                                      address,
                                      __global_uv_tcp_connect_cb__);
    return convertUvErrorToErrWithLogging("Tcp::connect()", CODE);
}

// --------------
// Event methods.
// --------------

void Tcp::onConnect(ConnectRequest & request, Err code)
{
    tDLogD("Tcp::onConnect({}) called.", getErrName(code));
}

// ----------------
// Utility methods.
// ----------------

Err initCommonClientSock(Tcp & tcp, ConnectRequest & request, struct sockaddr const * addr)
{
    if (tcp.isInit() == false) {
        tDLogE("initCommonClientSock() tcp is not initialized.");
        return Err::E_EINIT;
    }

    Err const CODE = tcp.connect(request, addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerSock() tcp connect {} error.", getErrName(CODE));
        return CODE;
    }

    return Err::E_SUCCESS;
}

Err initCommonClient(Tcp & tcp, ConnectRequest & request, std::string const & host, int port)
{
    libtbag::network::SocketAddress addr;
    Err const CODE = addr.init(host, port);
    if (CODE != Err::E_SUCCESS) {
        return CODE;
    }
    return initCommonClientSock(tcp, request, addr.getCommon());
}

Err initCommonServerSock(Tcp & tcp, struct sockaddr const * addr)
{
    if (tcp.isInit() == false) {
        tDLogE("initCommonServerSock() tcp is not initialized.");
        return Err::E_EINIT;
    }

    Err const BIND_CODE = tcp.bind(addr);
    if (BIND_CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerSock() tcp bind {} error.", getErrName(BIND_CODE));
        return BIND_CODE;
    }

    Err const LISTEN_CODE = tcp.listen();
    if (LISTEN_CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerSock() tcp listen {} error.", getErrName(LISTEN_CODE));
        return LISTEN_CODE;
    }

    return Err::E_SUCCESS;
}

Err initCommonServer(Tcp & tcp, std::string const & ip, int port)
{
    libtbag::network::SocketAddress addr;
    Err const CODE = addr.init(ip, port);
    if (CODE != Err::E_SUCCESS) {
        return CODE;
    }
    return initCommonServerSock(tcp, addr.getCommon());
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

