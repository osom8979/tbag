/**
 * @file   Udp.hpp
 * @brief  Udp class prototype.
 * @author zer0
 * @date   2017-01-11
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/uvpp/Handle.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/network/Uri.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

TBAG_CONSTEXPR unsigned int const UDP_FLAG_IPV6ONLY  = 0x01;
TBAG_CONSTEXPR unsigned int const UDP_FLAG_PARTIAL   = 0x02;
TBAG_CONSTEXPR unsigned int const UDP_FLAG_REUSEADDR = 0x04;

/**
 * Udp class prototype.
 *
 * @author zer0
 * @date   2017-01-11
 */
class TBAG_API Udp : public Handle
{
public:
    using Parent = Handle;

public:
    /** Membership type for a multicast address. */
    enum class Membership
    {
        LEAVE_GROUP = 0,
        JOIN_GROUP,
    };

public:
    Udp();
    Udp(Loop & loop);
    virtual ~Udp();

public:
    /** Number of bytes queued for sending. */
    std::size_t getSendQueueSize() const TBAG_NOEXCEPT;

    /** Number of send requests currently in the queue awaiting to be processed. */
    std::size_t getSendQueueCount() const TBAG_NOEXCEPT;

public:
    // Initialize a new UDP handle. */
    Err init(Loop & loop);

    /** Opens an existing file descriptor or Windows SOCKET as a UDP handle. */
    Err open(usock sock);

    /** Bind the UDP handle to an IP address and port. */
    Err bind(sockaddr const * addr, unsigned int flags = 0);

    // @formatter:off
    inline Err bind(sockaddr_in  const * addr, unsigned int flags = 0)
    { return bind((struct sockaddr const *)addr, flags); }
    inline Err bind(sockaddr_in6 const * addr, unsigned int flags = 0)
    { return bind((struct sockaddr const *)addr, flags); }
    // @formatter:on

    /** Get the local IP and port of the UDP handle. */
    Err getSockName(sockaddr * name, int * namelen);

    std::string getSockIp();
    int getSockPort();

    /** Set membership for a multicast address. */
    Err setMembership(char const * multicast_addr, char const * interface_addr, Membership membership);

    /** Set IP multicast loop flag. */
    Err setMulticastLoop(bool on = true);

    /** Set the multicast ttl. */
    Err setMulticastTtl(int ttl);

    /** Set the multicast interface to send or receive data on. */
    Err setMulticastInterface(char const * interface_addr);

    /** Set broadcast on or off. */
    Err setBroadcast(bool on = true);

    /** Set the time to live. */
    Err setTtl(int ttl);

    /** Send data over the UDP socket. */
    Err send(UdpSendRequest & request, binf const * infos, std::size_t infos_size, sockaddr const * addr);
    Err send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr const * addr);

    // @formatter:off
    inline Err send(UdpSendRequest & request, binf const * infos, std::size_t infos_size, sockaddr_in const * addr)
    { return send(request, infos, infos_size, (struct sockaddr const *)addr); }
    inline Err send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr_in const * addr)
    { return send(request, buffer, size, (struct sockaddr const *)addr); }
    inline Err send(UdpSendRequest & request, binf const * infos, std::size_t infos_size, sockaddr_in6 const * addr)
    { return send(request, infos, infos_size, (struct sockaddr const *)addr); }
    inline Err send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr_in6 const * addr)
    { return send(request, buffer, size, (struct sockaddr const *)addr); }
    // @formatter:on

    /** Same as send(), but won't queue a send request if it can't be completed immediately. */
    std::size_t trySend(binf * infos, std::size_t infos_size, sockaddr const * addr, Err * result = nullptr);
    std::size_t trySend(char const * buffer, std::size_t size, sockaddr const * addr, Err * result = nullptr);

    // @formatter:off
    inline std::size_t trySend(binf * infos, std::size_t infos_size, sockaddr_in const * addr, Err * result = nullptr)
    { return trySend(infos, infos_size, (struct sockaddr const *)addr, result); }
    inline std::size_t trySend(char const * buffer, std::size_t size, sockaddr_in const * addr, Err * result = nullptr)
    { return trySend(buffer, size, (struct sockaddr const *)addr, result); }
    inline std::size_t trySend(binf * infos, std::size_t infos_size, sockaddr_in6 const * addr, Err * result = nullptr)
    { return trySend(infos, infos_size, (struct sockaddr const *)addr, result); }
    inline std::size_t trySend(char const * buffer, std::size_t size, sockaddr_in6 const * addr, Err * result = nullptr)
    { return trySend(buffer, size, (struct sockaddr const *)addr, result); }
    // @formatter:on

    /** Prepare for receiving data. */
    Err startRecv();

    /** Stop listening for incoming datagrams. */
    Err stopRecv();

public:
    virtual void onSend(UdpSendRequest & request, Err code);
    virtual binf onAlloc(std::size_t suggested_size);
    virtual void onRecv(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags);
};

// ----------------
// Utility methods.
// ----------------

TBAG_API bool initRecvUdpSock(Udp & udp, struct sockaddr const * addr, unsigned int flags = 0);
TBAG_API bool initRecvUdp(Udp & udp, std::string const & host, int port, unsigned int flags = 0);

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UDP_HPP__

