/**
 * @file   Udp.hpp
 * @brief  Udp class prototype.
 * @author zer0
 * @date   2017-01-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UDP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UDP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/uvpp/Handle.hpp>
#include <libtbag/uvpp/Request.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

unsigned int const UDP_FLAG_IPV6ONLY  = 0x01;
unsigned int const UDP_FLAG_PARTIAL   = 0x02;
unsigned int const UDP_FLAG_REUSEADDR = 0x04;

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
    bool init(Loop & loop);

    /** Opens an existing file descriptor or Windows SOCKET as a UDP handle. */
    bool open(OsSock sock);

    /** Bind the UDP handle to an IP address and port. */
    bool bind(sockaddr const * addr, unsigned int flags);

    /** Get the local IP and port of the UDP handle. */
    bool getSockName(sockaddr * name, int * namelen);

    /** Set membership for a multicast address. */
    bool setMembership(char const * multicast_addr, char const * interface_addr, Membership membership);

    /** Set IP multicast loop flag. */
    bool setMulticastLoop(bool on);

    /** Set the multicast ttl. */
    bool setMulticastTtl(int ttl);

    /** Set the multicast interface to send or receive data on. */
    bool setMulticastInterface(char const * interface_addr);

    /** Set broadcast on or off. */
    bool setBroadcast(bool on);

    /** Set the time to live. */
    bool setTtl(int ttl);

    /** Send data over the UDP socket. */
    bool send(UdpSendRequest & request, binf * infos, std::size_t infos_size, sockaddr const * addr);
    bool send(UdpSendRequest & request, char const * buffer, std::size_t size, sockaddr const * addr);

    /** Same as send(), but won't queue a send request if it can't be completed immediately. */
    std::size_t trySend(binf * infos, std::size_t infos_size, sockaddr const * addr, Err * result = nullptr);
    std::size_t trySend(char const * buffer, std::size_t size, sockaddr const * addr, Err * result = nullptr);

    /** Prepare for receiving data. */
    bool startRecv();

    /** Stop listening for incoming datagrams. */
    bool stopRecv();

public:
    virtual void onSend(UdpSendRequest & request, Err code);
    virtual binf onAlloc(std::size_t suggested_size);
    virtual void onRead(Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_UDP_HPP__

