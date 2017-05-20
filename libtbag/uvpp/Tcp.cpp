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
#include <libtbag/network/details/NetCommon.hpp>

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

Tcp::Tcp(Loop & loop) : Stream(uhandle::TCP)
{
    if (init(loop) != Err::E_SUCCESS) {
        throw std::bad_alloc();
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

// -----------------------
// Client utility methods.
// -----------------------

bool initCommonClientSock(Tcp & tcp, ConnectRequest & request, struct sockaddr const * addr)
{
    if (tcp.isInit() == false) {
        tDLogE("initCommonClientSock() tcp is not initialized.");
        return false;
    }

    Err const CODE = tcp.connect(request, addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerSock() tcp connect {} error.", getErrName(CODE));
        return false;
    }
    return true;
}

bool initCommonClientIpv4(Tcp & tcp, ConnectRequest & request, std::string const & host, int port)
{
    sockaddr_in addr;
    Err const CODE = initAddress(host, port, &addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerIpv4() sockaddr init {} error.", getErrName(CODE));
        return false;
    }
    return initCommonClientSock(tcp, request, (sockaddr const *)&addr);
}

bool initCommonClientIpv6(Tcp & tcp, ConnectRequest & request, std::string const & host, int port)
{
    sockaddr_in6 addr;
    Err const CODE = initAddress(host, port, &addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerIpv6() sockaddr init {} error.", getErrName(CODE));
        return false;
    }
    return initCommonClientSock(tcp, request, (sockaddr const *)&addr);
}

bool initCommonClientName(Tcp & tcp, ConnectRequest & request, std::string const & host, int port)
{
    Loop loop;
    DnsAddrInfo addr;
    if (addr.requestAddrInfoWithSync(loop, host) != Err::E_SUCCESS) {
        return false;
    }

    addrinfo const * info = addr.getAddrInfo();
    assert(info != nullptr);
    if (info->ai_addrlen == 0) {
        return false;
    }

    addrinfo * next = addr.getAddrInfo()->ai_next;
    assert(next != nullptr);

    sockaddr * sa = info->ai_addr;
    assert(sa != nullptr);

    if (sa->sa_family == AF_INET) {
        sockaddr_in const * ipv4_sa = reinterpret_cast<sockaddr_in const *>(sa);
        return initCommonClientIpv4(tcp, request, getIpName(ipv4_sa), port);
    } else if (sa->sa_family == AF_INET6) {
        sockaddr_in6 const * ipv6_sa = reinterpret_cast<sockaddr_in6 const *>(sa);
        return initCommonClientIpv6(tcp, request, getIpName(ipv6_sa), port);
    }
    return false;
}

bool initCommonClient(Tcp & tcp, ConnectRequest & request, std::string const & host, int port)
{
    using namespace libtbag::network::details;
    if (isIpv4(host)) {
        return initCommonClientIpv4(tcp, request, host, port);
    } else if (isIpv6(host)) {
        return initCommonClientIpv6(tcp, request, host, port);
    }
    return initCommonClientName(tcp, request, host, port);
}

bool initCommonClient(Tcp & tcp, ConnectRequest & request, network::Uri const & uri)
{
    if (uri.isHost() == false) {
        tDLogE("initCommonClient() Unknown host from uri: {}.", uri.get());
        return false;
    }
    if (uri.isPort()) {
        return initCommonClientName(tcp, request, uri.getHost(), uri.getPortNumber());
    }

    if (uri.isSchema() == false) {
        tDLogE("initCommonClient() Unknown schema from uri: {}.", uri.get());
        return false;
    }

    std::string const SERVICE = uri.getSchema();
    std::string const HOST    = uri.getHost();

    assert(SERVICE.empty() == false);
    assert(HOST.empty() == false);

    Loop loop;
    DnsAddrInfo addr;
    if (addr.requestAddrInfoWithSync(loop, HOST, SERVICE) != Err::E_SUCCESS) {
        return false;
    }

    addrinfo const * info = addr.getAddrInfo();
    assert(info != nullptr);
    if (info->ai_addrlen == 0) {
        return false;
    }

    addrinfo * next = addr.getAddrInfo()->ai_next;
    assert(next != nullptr);

    sockaddr * sa = info->ai_addr;
    assert(sa != nullptr);

    if (sa->sa_family == AF_INET) {
        sockaddr_in const * ipv4_sa = reinterpret_cast<sockaddr_in const *>(sa);
        return initCommonClientIpv4(tcp, request, getIpName(ipv4_sa), ipv4_sa->sin_port);
    } else if (sa->sa_family == AF_INET6) {
        sockaddr_in6 const * ipv6_sa = reinterpret_cast<sockaddr_in6 const *>(sa);
        return initCommonClientIpv6(tcp, request, getIpName(ipv6_sa), ipv6_sa->sin6_port);
    }
    return false;
}

// -----------------------
// Server utility methods.
// -----------------------

bool initCommonServerSock(Tcp & tcp, struct sockaddr const * addr)
{
    if (tcp.isInit() == false) {
        tDLogE("initCommonServerSock() tcp is not initialized.");
        return false;
    }

    Err const BIND_CODE = tcp.bind(addr);
    if (BIND_CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerSock() tcp bind {} error.", getErrName(BIND_CODE));
        return false;
    }

    Err const LISTEN_CODE = tcp.listen();
    if (LISTEN_CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerSock() tcp listen {} error.", getErrName(LISTEN_CODE));
        return false;
    }
    return true;
}

bool initCommonServerIpv4(Tcp & tcp, std::string const & ip, int port)
{
    sockaddr_in addr;
    Err const CODE = initAddress(ip, port, &addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerIpv4() sockaddr init {} error.", getErrName(CODE));
        return false;
    }
    return initCommonServerSock(tcp, (sockaddr const *)&addr);
}

bool initCommonServerIpv6(Tcp & tcp, std::string const & ip, int port)
{
    sockaddr_in6 addr;
    Err const CODE = initAddress(ip, port, &addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initCommonServerIpv6() sockaddr init {} error.", getErrName(CODE));
        return false;
    }
    return initCommonServerSock(tcp, (sockaddr const *)&addr);
}

bool initCommonServer(Tcp & tcp, std::string const & ip, int port)
{
    using namespace libtbag::network::details;
    if (isIpv4(ip)) {
        return initCommonServerIpv4(tcp, ip, port);
    } else if (isIpv6(ip)) {
        return initCommonServerIpv6(tcp, ip, port);
    }
    return false;
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

