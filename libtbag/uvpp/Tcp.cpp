/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-12-28
 */

#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_connect_cb__(uv_connect_t * request, int status)
{
    // Callback called after a connection started by uv_connect() is done.
    // status will be 0 in case of success, < 0 otherwise.

    ConnectRequest * req = static_cast<ConnectRequest*>(request->data);
    if (req == nullptr) {
        __tbag_error("__global_uv_connect_cb__() request data is nullptr.");
        return;
    }

    Tcp * s = static_cast<Tcp*>(req->getOwner());
    if (s == nullptr) {
        __tbag_error("__global_uv_connect_cb__() request owner is nullptr.");
        return;
    }
    s->onConnect(*req, status == 0 ? Err::SUCCESS : Err::FAILURE);
}

// -------------------
// Tcp implementation.
// -------------------

Tcp::Tcp() : Stream(UvHandleType::TCP)
{
    // EMPTY.
}

Tcp::Tcp(Loop & loop) : Tcp()
{
    if (init(loop) == false) {
        throw std::bad_alloc();
    }
}

Tcp::~Tcp()
{
    // EMPTY.
}

bool Tcp::init(Loop & loop)
{
    int const CODE = ::uv_tcp_init(loop.cast<uv_loop_t>(), Parent::cast<uv_tcp_t>());
    if (CODE != 0) {
        __tbag_error("Tcp::init() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Tcp::setNodelay(bool enable)
{
    int const CODE = ::uv_tcp_nodelay(Parent::cast<uv_tcp_t>(), enable ? 1 : 0);
    if (CODE != 0) {
        __tbag_error("Tcp::setNodelay() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Tcp::keepAlive(bool enable, unsigned int delay)
{
    // delay is the initial delay in seconds, ignored when enable is zero.
    int const CODE = ::uv_tcp_keepalive(Parent::cast<uv_tcp_t>(), enable ? 1 : 0, delay);
    if (CODE != 0) {
        __tbag_error("Tcp::keepAlive() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Tcp::acceptsSimultaneous(int enable)
{
    // Enable/Disable simultaneous asynchronous accept requests that
    // are queued by the operating system when listening for new TCP connections.
    //
    // This setting is used to tune a TCP server for the desired performance.
    // Having simultaneous accepts can significantly improve the rate of accepting
    // connections (which is why it is enabled by default) but may lead to uneven
    // load distribution in multi-process setups.

    int const CODE = ::uv_tcp_simultaneous_accepts(Parent::cast<uv_tcp_t>(), enable ? 1 : 0);
    if (CODE != 0) {
        __tbag_error("Tcp::acceptsSimultaneous() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Tcp::bind(sockaddr const * address, unsigned int flags)
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
    if (CODE != 0) {
        __tbag_error("Tcp::bind() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

std::string Tcp::getSockName()
{
    sockaddr address = {0,};
    int length = sizeof(address);

    // addr must point to a valid and big enough chunk of memory,
    // struct sockaddr_storage is recommended for IPv4 and IPv6 support.

    int const CODE = ::uv_tcp_getsockname(Parent::cast<uv_tcp_t>(), &address, &length);
    if (CODE != 0) {
        __tbag_error("Tcp::getSockName() error [{}] {}", CODE, getUvErrorName(CODE));
        return std::string();
    }

    if (address.sa_family == AF_INET) {
        return getIpName((sockaddr_in const *)&address);
    } else if (address.sa_family == AF_INET6) {
        return getIpName((sockaddr_in6 const *)&address);
    }
    return std::string();
}

std::string Tcp::getPeerName()
{
    sockaddr address = {0,};
    int length = sizeof(address);

    // addr must point to a valid and big enough chunk of memory,
    // struct sockaddr_storage is recommended for IPv4 and IPv6 support.

    int const CODE = ::uv_tcp_getpeername(Parent::cast<uv_tcp_t>(), &address, &length);
    if (CODE != 0) {
        __tbag_error("Tcp::getPeerName() error [{}] {}", CODE, getUvErrorName(CODE));
        return std::string();
    }

    if (address.sa_family == AF_INET) {
        return getIpName((sockaddr_in const *)&address);
    } else if (address.sa_family == AF_INET6) {
        return getIpName((sockaddr_in6 const *)&address);
    }
    return std::string();
}

bool Tcp::connect(ConnectRequest & request, sockaddr const * address)
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
                                      __global_uv_connect_cb__);
    if (CODE != 0) {
        __tbag_error("Tcp::connect() error [{}] {}", CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

// --------------
// Event methods.
// --------------

void Tcp::onConnect(ConnectRequest & request, Err code)
{
    __tbag_debug("Tcp::onConnect({}) called.", static_cast<int>(code));
}

// ----------
// Utilities.
// ----------

std::string Tcp::getIpName(sockaddr const * address)
{
    if (address->sa_family == AF_INET) {
        return getIpName((sockaddr_in const *)&address);
    } else if (address->sa_family == AF_INET6) {
        return getIpName((sockaddr_in6 const *)&address);
    }
    return std::string();
}

std::string Tcp::getIpName(sockaddr_in const * address)
{
    char name[16] = {0,}; // e.g. 255.255.255.255
    if (::uv_ip4_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

std::string Tcp::getIpName(sockaddr_in6 const * address)
{
    char name[40] = {0,}; // e.g. 2001:0db8:85a3:08d3:1319:8a2e:0370:7334
    if (::uv_ip6_name(address, name, sizeof(name)) == 0) {
        return std::string(name);
    }
    return std::string();
}

bool Tcp::initAddress(std::string const & ip, int port, sockaddr_in * addr)
{
    int const CODE = ::uv_ip4_addr(ip.c_str(), port, addr);
    if (CODE != 0) {
        __tbag_error("Tcp::initAddress({}, {}) ipv4 error [{}] {}", ip, port, CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Tcp::initAddress(std::string const & ip, int port, sockaddr_in6 * addr)
{
    int const CODE = ::uv_ip6_addr(ip.c_str(), port, addr);
    if (CODE != 0) {
        __tbag_error("Tcp::initAddress({}, {}) ipv6 error [{}] {}", ip, port, CODE, getUvErrorName(CODE));
        return false;
    }
    return true;
}

bool Tcp::isIpv4(std::string const & ip)
{
    auto tokens = string::splitTokens(ip, ".");
    if (tokens.size() != 4) {
        return false;
    }
    for (auto cursor : tokens) {
        if (!std::all_of(cursor.begin(), cursor.end(), ::isdigit)) {
            return false;
        }
    }
    return true;
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

