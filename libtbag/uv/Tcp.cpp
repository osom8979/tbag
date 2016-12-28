/**
 * @file   Tcp.cpp
 * @brief  Tcp class implementation.
 * @author zer0
 * @date   2016-12-28
 */

#include <libtbag/uv/Tcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Loop.hpp>
#include <libtbag/uv/Stream.hpp>
#include <libtbag/uv/Request.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

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
    s->onConnect(status == 0 ? Err::SUCCESS : Err::FAILURE);
}

// -------------------
// Tcp implementation.
// -------------------

Tcp::Tcp(Loop & loop) : Stream(UvHandleType::TCP)
{
    // Initialize the handle. No socket is created as of yet.
    int const CODE = ::uv_tcp_init(loop.cast<uv_loop_t>(), Parent::cast<uv_tcp_t>());
    if (CODE != 0) {
        __tbag_error("Tcp::Tcp() error [{}] {}", CODE, getUvErrorName(CODE));
        throw std::bad_alloc();
    }
}

Tcp::~Tcp()
{
    // EMPTY.
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
    int name_length = 0;

    // addr must point to a valid and big enough chunk of memory,
    // struct sockaddr_storage is recommended for IPv4 and IPv6 support.

    int const CODE = ::uv_tcp_getsockname(Parent::cast<uv_tcp_t>(), &address, &name_length);
    if (CODE != 0) {
        __tbag_error("Tcp::getSockName() error [{}] {}", CODE, getUvErrorName(CODE));
        return std::string();
    }

    std::string result;
    return result;
}

std::string Tcp::getPeerName()
{
    sockaddr address = {0,};
    int name_length = 0;

    // addr must point to a valid and big enough chunk of memory,
    // struct sockaddr_storage is recommended for IPv4 and IPv6 support.

    int const CODE = ::uv_tcp_getpeername(Parent::cast<uv_tcp_t>(), &address, &name_length);
    if (CODE != 0) {
        __tbag_error("Tcp::getPeerName() error [{}] {}", CODE, getUvErrorName(CODE));
        return std::string();
    }

    std::string result;
    return result;
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

void Tcp::onConnect(Err code)
{
    __tbag_debug("Tcp::onConnect({}) called.", static_cast<int>(code));
}

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

