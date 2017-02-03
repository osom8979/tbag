/**
 * @file   Udp.cpp
 * @brief  Udp class implementation.
 * @author zer0
 * @date   2017-01-11
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#include <libtbag/uvpp/Udp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_udp_send_cb__(uv_udp_send_t * request, int status)
{
    // Type definition for callback passed to uv_udp_send(),
    // which is called after the data was sent.

    UdpSendRequest * req = static_cast<UdpSendRequest*>(request->data);
    if (req == nullptr) {
        __tbag_error("__global_uv_udp_send_cb__() request.data is nullptr.");
    } else if (isDeletedAddress(req)) {
        __tbag_error("__global_uv_udp_send_cb__() request.data is deleted.");
    } else {
        Udp * s = static_cast<Udp*>(req->getOwner());
        if (s == nullptr) {
            __tbag_error("__global_uv_udp_send_cb__() request.data.owner is nullptr.");
        } else if (isDeletedAddress(s)) {
            __tbag_error("__global_uv_udp_send_cb__() request.data.owner is deleted.");
        } else {
            s->onSend(*req, getUerr(status));
        }
    }
}

static void __global_uv_udp_alloc_cb__(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf)
{
    // Type definition for callback passed to uv_read_start() and uv_udp_recv_start().
    // The user must allocate memory and fill the supplied uv_buf_t structure.
    // If NULL is assigned as the buffer's base or 0 as its length, a UV_ENOBUFS error
    // will be triggered in the uv_udp_recv_cb or the uv_read_cb callback.
    //
    // A suggested size (65536 at the moment in most cases) is provided,
    // but it's just an indication, not related in any way to the pending data to be read.
    // The user is free to allocate the amount of memory they decide.

    Udp * u = static_cast<Udp*>(handle->data);
    if (u == nullptr) {
        __tbag_error("__global_uv_udp_alloc_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(u)) {
        __tbag_error("__global_uv_udp_alloc_cb__() handle.data is deleted.");
    } else {
        auto result_buffer = u->onAlloc(suggested_size);
        buf->base = result_buffer.buffer;
        buf->len  = result_buffer.size;
    }
}

static void __global_uv_udp_recv_cb__(uv_udp_t       * handle,
                                      ssize_t          nread,
                                      uv_buf_t const * buf,
                                      sockaddr const * addr,
                                      unsigned int     flags)
{
    // Type definition for callback passed to uv_udp_recv_start(),
    // which is called when the endpoint receives data.
    //  - handle: UDP handle
    //  - nread : Number of bytes that have been received.
    //            0 if there is no more data to read.
    //            You may discard or repurpose the read buffer.
    //            Note that 0 may also mean that an empty datagram was received (in this case addr is not NULL).
    //            < 0 if a transmission error was detected.
    //  - buf   : uv_buf_t with the received data.
    //  - addr  : struct sockaddr* containing the address of the sender.
    //            Can be NULL. Valid for the duration of the callback only.
    //  - flags : One or more or'ed UV_UDP_* constants. Right now only UV_UDP_PARTIAL is used.
    //
    // Note:
    // The receive callback will be called with nread == 0 and addr == NULL when there is nothing to read,
    // and with nread == 0 and addr != NULL when an empty UDP packet is received.

    Udp * u = static_cast<Udp*>(handle->data);
    if (u == nullptr) {
        __tbag_error("__global_uv_udp_recv_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(u)) {
        __tbag_error("__global_uv_udp_recv_cb__() handle.data is deleted.");
    } else {
        uerr code;
        if (nread >= 0){
            code = uerr::UVPP_SUCCESS;
        } else {
            code = getUerr(static_cast<int>(nread));
        }

        u->onRead(code, buf->base, static_cast<std::size_t>(nread), addr, flags);
    }
}

// ----------------------
// Stream implementation.
// ----------------------

Udp::Udp() : Handle(uhandle::UDP)
{
    // EMPTY.
}

Udp::Udp(Loop & loop) : Udp()
{
    if (init(loop) != uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

Udp::~Udp()
{
    // EMPTY.
}

std::size_t Udp::getSendQueueSize() const TBAG_NOEXCEPT
{
    // This field strictly shows how much information is currently queued.
    return Parent::cast<uv_udp_t>()->send_queue_size;
}

std::size_t Udp::getSendQueueCount() const TBAG_NOEXCEPT
{
    return Parent::cast<uv_udp_t>()->send_queue_count;
}

uerr Udp::init(Loop & loop)
{
    // The actual socket is created lazily. Returns 0 on success.
    int const CODE = ::uv_udp_init(loop.cast<uv_loop_t>(), Parent::cast<uv_udp_t>());
    return getUerr2("Udp::init()", CODE);
}

uerr Udp::open(usock sock)
{
    // Unix only: The only requirement of the sock argument is that it follows
    // the datagram contract (works in unconnected mode, supports sendmsg()/recvmsg(), etc).
    // In other words, other datagram-type sockets like raw sockets
    // or netlink sockets can also be passed to this function.
    //
    // Changed in version 1.2.1: the file descriptor is set to non-blocking mode.
    //
    // Note: The passed file descriptor or SOCKET is not checked for its type,
    // but it's required that it represents a valid datagram socket.

    int const CODE  = ::uv_udp_open(Parent::cast<uv_udp_t>(), static_cast<uv_os_sock_t>(sock));
    return getUerr2("Udp::open()", CODE);
}

uerr Udp::bind(sockaddr const * addr, unsigned int flags)
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  addr   - struct sockaddr_in or struct sockaddr_in6 with the address and port to bind to.
    //  flags  - Indicate how the socket will be bound,
    //           UV_UDP_IPV6ONLY and UV_UDP_REUSEADDR are supported.
    //
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_bind(Parent::cast<uv_udp_t>(), addr, flags);
    return getUerr2("Udp::bind()", CODE);
}

uerr Udp::getSockName(sockaddr * name, int * namelen)
{
    // Parameters:
    //  handle  - UDP handle. Should have been initialized with uv_udp_init() and bound.
    //  name    - Pointer to the structure to be filled with the address data.
    //            In order to support IPv4 and IPv6 struct sockaddr_storage should be used.
    //  namelen - On input it indicates the data of the name field. On output it indicates how much of it was filled.
    //
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_getsockname(Parent::cast<uv_udp_t>(), name, namelen);
    return getUerr2("Udp::getSockName()", CODE);
}

uerr Udp::setMembership(char const * multicast_addr, char const * interface_addr, Membership membership)
{
    uv_membership native_membership;
    if (membership == Membership::LEAVE_GROUP) {
        native_membership = UV_LEAVE_GROUP;
    } else /*if (membership == Membership::JOIN_GROUP)*/ {
        native_membership = UV_JOIN_GROUP;
    }

    // Parameters:
    //  handle         - UDP handle. Should have been initialized with uv_udp_init().
    //  multicast_addr - Multicast address to set membership for.
    //  interface_addr - Interface address.
    //  membership     - Should be UV_JOIN_GROUP or UV_LEAVE_GROUP.
    //
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_membership(Parent::cast<uv_udp_t>(), multicast_addr, interface_addr, native_membership);
    return getUerr2("Udp::setMembership()", CODE);
}

uerr Udp::setMulticastLoop(bool on)
{
    // Makes multicast packets loop back to local sockets.
    //
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  on     - 1 for on, 0 for off.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_multicast_loop(Parent::cast<uv_udp_t>(), (on ? 1 : 0));
    return getUerr2("Udp::setMulticastLoop()", CODE);
}

uerr Udp::setMulticastTtl(int ttl)
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  ttl    - 1 through 255.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_multicast_ttl(Parent::cast<uv_udp_t>(), ttl);
    return getUerr2("Udp::setMulticastTtl()", CODE);
}

uerr Udp::setMulticastInterface(char const * interface_addr)
{
    // Parameters:
    //  handle         - UDP handle. Should have been initialized with uv_udp_init().
    //  interface_addr - interface address.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_multicast_interface(Parent::cast<uv_udp_t>(), interface_addr);
    return getUerr2("Udp::setMulticastInterface()", CODE);
}

uerr Udp::setBroadcast(bool on)
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  on     - 1 for on, 0 for off.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_broadcast(Parent::cast<uv_udp_t>(), (on ? 1 : 0));
    return getUerr2("Udp::setBroadcast()", CODE);
}

uerr Udp::setTtl(int ttl)
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  ttl    - 1 through 255.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_ttl(Parent::cast<uv_udp_t>(), ttl);
    return getUerr2("Udp::setTtl()", CODE);
}

uerr Udp::send(UdpSendRequest & request, binf * infos, std::size_t infos_size, sockaddr const * addr)
{
    if (infos_size > getBufferInfoSizeMax()) {
        __tbag_error("Udp::send() buffer info size too large.");
        return uerr::UVPP_ILLARGS;
    }

    request.setOwner(this); // IMPORTANT!!

    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    // If the socket has not previously been bound with uv_udp_bind()
    // it will be bound to 0.0.0.0 (the "all interfaces" IPv4 address) and a random port number.
    //
    // Parameters:
    //  req     - UDP request handle. Need not be initialized.
    //  handle  - UDP handle. Should have been initialized with uv_udp_init().
    //  bufs    - List of buffers to send.
    //  nbufs   - Number of buffers in bufs.
    //  addr    - struct sockaddr_in or struct sockaddr_in6 with the address and port of the remote peer.
    //  send_cb - Callback to invoke when the data has been sent out.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_send(request.cast<uv_udp_send_t>(),
                                   Parent::cast<uv_udp_t>(),
                                   &uv_infos[0],
                                   static_cast<unsigned int>(uv_infos.size()),
                                   addr,
                                   __global_uv_udp_send_cb__);
    return getUerr2("Udp::send()", CODE);
}

uerr Udp::send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr const * addr)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return send(request, &info, 1U, addr);
}

std::size_t Udp::trySend(binf * infos, std::size_t infos_size, sockaddr const * addr, uerr * result)
{
    if (infos_size > getBufferInfoSizeMax()) {
        __tbag_error("Udp::trySend() buffer info size too large.");
        if (result != nullptr) {
            *result = uerr::UVPP_ILLARGS;
        }
        return 0U;
    }

    // Same as uv_udp_send(), but won't queue a send request if it can't be completed immediately.
    // Returns:
    //  >= 0: number of bytes sent (it matches the given buffer size).
    //  <  0: negative error code (UV_EAGAIN is returned when the message can't be sent immediately).

    std::vector<uv_buf_t> uv_infos;
    uv_infos.resize(infos_size);
    for (std::size_t i = 0; i < infos_size; ++i) {
        uv_infos[i].base = (infos + i)->buffer;
        uv_infos[i].len  = (infos + i)->size;
    }

    int  const WRITE_SIZE = ::uv_udp_try_send(Parent::cast<uv_udp_t>(),
                                              &uv_infos[0],
                                              static_cast<unsigned int>(uv_infos.size()),
                                              addr);
    uerr const ERROR_CODE = getUerr2("Udp::trySend()", WRITE_SIZE);

    if (result != nullptr) {
        *result = ERROR_CODE;
    }
    return static_cast<std::size_t>(WRITE_SIZE);
}

std::size_t Udp::trySend(char const * buffer, std::size_t size, sockaddr const * addr, uerr * result)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return trySend(&info, 1U, addr, result);
}

uerr Udp::startRecv()
{
    // If the socket has not previously been bound with uv_udp_bind()
    // it is bound to 0.0.0.0 (the "all interfaces" IPv4 address) and a random port number.
    //
    // Parameters:
    //  handle   - UDP handle. Should have been initialized with uv_udp_init().
    //  alloc_cb - Callback to invoke when temporary storage is needed.
    //  recv_cb  - Callback to invoke with received data.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_recv_start(Parent::cast<uv_udp_t>(), __global_uv_udp_alloc_cb__, __global_uv_udp_recv_cb__);
    return getUerr2("Udp::startRecv()", CODE);
}

uerr Udp::stopRecv()
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    // Returns:
    //  0 on success, or an error code < 0 on failure.
    int const CODE = ::uv_udp_recv_stop(Parent::cast<uv_udp_t>());
    return getUerr2("Udp::stopRecv()", CODE);
}

// --------------
// Event methods.
// --------------

void Udp::onSend(UdpSendRequest & request, uerr code)
{
    __tbag_debug("Udp::onSend({}) called.", getErrorName(code));
}

binf Udp::onAlloc(std::size_t suggested_size)
{
    __tbag_debug("Udp::onAlloc() called (suggested_size:{}).", suggested_size);
    return binf((char*)::malloc(suggested_size), suggested_size);
}

void Udp::onRead(uerr code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
{
    __tbag_debug("Udp::onRead({}) called (size:{}).", getErrorName(code), size);
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

