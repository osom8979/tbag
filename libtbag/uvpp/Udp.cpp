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
#include <libtbag/uvpp/Dns.hpp>
#include <libtbag/network/details/NetCommon.hpp>

#include <cassert>
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
        tDLogE("__global_uv_udp_send_cb__() request.data is nullptr.");
    } else if (isDeletedAddress(req)) {
        tDLogE("__global_uv_udp_send_cb__() request.data is deleted.");
    } else {
        Udp * s = static_cast<Udp*>(req->getOwner());
        if (s == nullptr) {
            tDLogE("__global_uv_udp_send_cb__() request.data.owner is nullptr.");
        } else if (isDeletedAddress(s)) {
            tDLogE("__global_uv_udp_send_cb__() request.data.owner is deleted.");
        } else {
            s->onSend(*req, convertUvErrorToErr(status));
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
        tDLogE("__global_uv_udp_alloc_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(u)) {
        tDLogE("__global_uv_udp_alloc_cb__() handle.data is deleted.");
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
        tDLogE("__global_uv_udp_recv_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(u)) {
        tDLogE("__global_uv_udp_recv_cb__() handle.data is deleted.");
    } else {
        Err code;
        if (nread >= 0){
            code = Err::E_SUCCESS;
        } else {
            code = convertUvErrorToErr(static_cast<int>(nread));
        }

#if (0) // COMMENT SECTION.
        if (nread == 0 && addr == nullptr) {
            // Nothing to read.
        } else if (nread == 0 && addr != nullptr) {
            // Empty UDP packet is received.
        }
#endif

        u->onRecv(code, buf->base, static_cast<std::size_t>(nread), addr, flags);
    }
}

// ----------------------
// Stream implementation.
// ----------------------

Udp::Udp(Loop & loop) : Handle(uhandle::UDP)
{
    if (init(loop) != Err::E_SUCCESS) {
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

Err Udp::init(Loop & loop)
{
    // The actual socket is created lazily. Returns 0 on success.
    int const CODE = ::uv_udp_init(loop.cast<uv_loop_t>(), Parent::cast<uv_udp_t>());
    return convertUvErrorToErrWithLogging("Udp::init()", CODE);
}

Err Udp::open(usock sock)
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
    return convertUvErrorToErrWithLogging("Udp::open()", CODE);
}

Err Udp::bind(sockaddr const * addr, unsigned int flags)
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
    return convertUvErrorToErrWithLogging("Udp::bind()", CODE);
}

Err Udp::getSockName(sockaddr * name, int * namelen)
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
    return convertUvErrorToErrWithLogging("Udp::getSockName()", CODE);
}

std::string Udp::getSockIp()
{
    uint8_t buffer[SOCKADDR_MAX_BYTE_SIZE] = {0,};
    sockaddr * address = reinterpret_cast<struct sockaddr *>(buffer);
    int length = SOCKADDR_MAX_BYTE_SIZE;

    if (getSockName(address, &length) != Err::E_SUCCESS) {
        return std::string();
    }
    return getIpName(address);
}

int Udp::getSockPort()
{
    uint8_t buffer[SOCKADDR_MAX_BYTE_SIZE] = {0,};
    sockaddr * address = reinterpret_cast<struct sockaddr *>(buffer);
    int length = SOCKADDR_MAX_BYTE_SIZE;

    if (getSockName(address, &length) != Err::E_SUCCESS) {
        return UNKNOWN_PORT_NUMBER;
    }
    return getPortNumber(address);
}

Err Udp::setMembership(char const * multicast_addr, char const * interface_addr, Membership membership)
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
    return convertUvErrorToErrWithLogging("Udp::setMembership()", CODE);
}

Err Udp::setMulticastLoop(bool on)
{
    // Makes multicast packets loop back to local sockets.
    //
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  on     - 1 for on, 0 for off.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_multicast_loop(Parent::cast<uv_udp_t>(), (on ? 1 : 0));
    return convertUvErrorToErrWithLogging("Udp::setMulticastLoop()", CODE);
}

Err Udp::setMulticastTtl(int ttl)
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  ttl    - 1 through 255.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_multicast_ttl(Parent::cast<uv_udp_t>(), ttl);
    return convertUvErrorToErrWithLogging("Udp::setMulticastTtl()", CODE);
}

Err Udp::setMulticastInterface(char const * interface_addr)
{
    // Parameters:
    //  handle         - UDP handle. Should have been initialized with uv_udp_init().
    //  interface_addr - interface address.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_multicast_interface(Parent::cast<uv_udp_t>(), interface_addr);
    return convertUvErrorToErrWithLogging("Udp::setMulticastInterface()", CODE);
}

Err Udp::setBroadcast(bool on)
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  on     - 1 for on, 0 for off.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_broadcast(Parent::cast<uv_udp_t>(), (on ? 1 : 0));
    return convertUvErrorToErrWithLogging("Udp::setBroadcast()", CODE);
}

Err Udp::setTtl(int ttl)
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    //  ttl    - 1 through 255.
    // Returns:
    //  0 on success, or an error code < 0 on failure.

    int const CODE = ::uv_udp_set_ttl(Parent::cast<uv_udp_t>(), ttl);
    return convertUvErrorToErrWithLogging("Udp::setTtl()", CODE);
}

Err Udp::send(UdpSendRequest & request, binf * infos, std::size_t infos_size, sockaddr const * addr)
{
    if (infos_size > getBufferInfoSizeMax()) {
        tDLogE("Udp::send() buffer info size too large.");
        return Err::E_ILLARGS;
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
    return convertUvErrorToErrWithLogging("Udp::send()", CODE);
}

Err Udp::send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr const * addr)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return send(request, &info, 1U, addr);
}

std::size_t Udp::trySend(binf * infos, std::size_t infos_size, sockaddr const * addr, Err * result)
{
    if (infos_size > getBufferInfoSizeMax()) {
        tDLogE("Udp::trySend() buffer info size too large.");
        if (result != nullptr) {
            *result = Err::E_ILLARGS;
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
    Err const ERROR_CODE = convertUvErrorToErrWithLogging("Udp::trySend()", WRITE_SIZE);

    if (result != nullptr) {
        *result = ERROR_CODE;
    }
    return static_cast<std::size_t>(WRITE_SIZE);
}

std::size_t Udp::trySend(char const * buffer, std::size_t size, sockaddr const * addr, Err * result)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return trySend(&info, 1U, addr, result);
}

Err Udp::startRecv()
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
    return convertUvErrorToErrWithLogging("Udp::startRecv()", CODE);
}

Err Udp::stopRecv()
{
    // Parameters:
    //  handle - UDP handle. Should have been initialized with uv_udp_init().
    // Returns:
    //  0 on success, or an error code < 0 on failure.
    int const CODE = ::uv_udp_recv_stop(Parent::cast<uv_udp_t>());
    return convertUvErrorToErrWithLogging("Udp::stopRecv()", CODE);
}

// --------------
// Event methods.
// --------------

void Udp::onSend(UdpSendRequest & request, Err code)
{
    tDLogD("Udp::onSend({}) called.", getErrName(code));
}

binf Udp::onAlloc(std::size_t suggested_size)
{
    tDLogD("Udp::onAlloc() called (suggested_size:{}).", suggested_size);
    return binf((char*)::malloc(suggested_size), suggested_size);
}

void Udp::onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags)
{
    tDLogD("Udp::onRecv({}) called (size:{}).", getErrName(code), size);
}

// ----------------
// Utility methods.
// ----------------

bool initRecvUdpSock(Udp & udp, struct sockaddr const * addr)
{
    if (udp.isInit() == false) {
        tDLogE("initRecvUdpSock() udp is not initialized.");
        return false;
    }

    Err const BIND_CODE = udp.bind(addr);
    if (BIND_CODE != Err::E_SUCCESS) {
        tDLogE("initRecvUdpSock() udp bind {} error.", getErrName(BIND_CODE));
        return false;
    }
    return true;
}

bool initRecvUdpIpv4(Udp & udp, std::string const & host, int port)
{
    sockaddr_in addr;
    Err const CODE = initAddress(host, port, &addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initRecvUdpIpv4() sockaddr init {} error.", getErrName(CODE));
        return false;
    }
    return initRecvUdpSock(udp, (struct sockaddr const *)&addr);
}

bool initRecvUdpIpv6(Udp & udp, std::string const & host, int port)
{
    sockaddr_in6 addr;
    Err const CODE = initAddress(host, port, &addr);
    if (CODE != Err::E_SUCCESS) {
        tDLogE("initRecvUdpIpv6() sockaddr init {} error.", getErrName(CODE));
        return false;
    }
    return initRecvUdpSock(udp, (struct sockaddr const *)&addr);
}

bool initRecvUdpName(Udp & udp, std::string const & host, int port)
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
        return initRecvUdpIpv4(udp, getIpName(ipv4_sa), port);
    } else if (sa->sa_family == AF_INET6) {
        sockaddr_in6 const * ipv6_sa = reinterpret_cast<sockaddr_in6 const *>(sa);
        return initRecvUdpIpv6(udp, getIpName(ipv6_sa), port);
    }
    return false;
}

bool initRecvUdp(Udp & udp, std::string const & host, int port)
{
    using namespace libtbag::network::details;
    if (isIpv4(host)) {
        return initRecvUdpIpv4(udp, host, port);
    } else if (isIpv6(host)) {
        return initRecvUdpIpv6(udp, host, port);
    }
    return initRecvUdpName(udp, host, port);
}

bool initRecvUdp(Udp & udp, network::Uri const & uri)
{
    if (uri.isHost() == false) {
        tDLogE("initRecvUdp() Unknown host from uri: {}.", uri.getString());
        return false;
    }
    if (uri.isPort()) {
        return initRecvUdpName(udp, uri.getHost(), uri.getPortNumber());
    }

    if (uri.isSchema() == false) {
        tDLogE("initRecvUdp() Unknown schema from uri: {}.", uri.getString());
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
        return initRecvUdpIpv4(udp, getIpName(ipv4_sa), ipv4_sa->sin_port);
    } else if (sa->sa_family == AF_INET6) {
        sockaddr_in6 const * ipv6_sa = reinterpret_cast<sockaddr_in6 const *>(sa);
        return initRecvUdpIpv6(udp, getIpName(ipv6_sa), ipv6_sa->sin6_port);
    }
    return false;
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

