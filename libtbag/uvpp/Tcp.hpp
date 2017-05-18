/**
 * @file   Tcp.hpp
 * @brief  Tcp class prototype.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Stream.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;
struct ConnectRequest;

/**
 * Tcp class prototype.
 *
 * @author zer0
 * @date   2016-12-28
 *
 * @remarks
 *  TCP handles are used to represent both TCP streams and servers.
 */
class TBAG_API Tcp : public Stream
{
public:
    friend class Loop;

public:
    using Parent = Stream;

protected:
    Tcp(Loop & loop);

public:
    virtual ~Tcp();

public:
    inline bool isDealsWithIo() const TBAG_NOEXCEPT
    { return isActive(); }

public:
    /** Initialize the handle. No socket is created as of yet. */
    uerr init(Loop & loop);

    /** Enable TCP_NODELAY, which disables Nagle's algorithm. */
    uerr setNodelay(bool enable = true);

    /** Enable/Disable TCP keep-alive. */
    uerr keepAlive(bool enable, unsigned int delay);

    /** Enable/Disable simultaneous asynchronous accept requests. */
    uerr acceptsSimultaneous(int enable = true);

    /** Bind the handle to an address and port. */
    uerr bind(sockaddr const * address, unsigned int flags = 0);

    /** Get the current address to which the handle is bound. */
    uerr getSockName(struct sockaddr * name, int * namelen);

    /** Get the address of the peer connected to the handle. */
    uerr getPeerName(struct sockaddr * name, int * namelen);

    std::string getSockIp();
    std::string getPeerIp();

    int getSockPort();
    int getPeerPort();

    /** Establish an IPv4 or IPv6 TCP connection. */
    uerr connect(ConnectRequest & request, sockaddr const * address);

// Event methods.
public:
    virtual void onConnect(ConnectRequest & request, uerr code);
};

// ----------------
// Utility methods.
// ----------------

// @formatter:off
TBAG_API bool initCommonServerSock(Tcp & tcp, struct sockaddr const * addr);
TBAG_API bool initCommonServerIpv4(Tcp & tcp, std::string const & ip, int port);
TBAG_API bool initCommonServerIpv6(Tcp & tcp, std::string const & ip, int port);
TBAG_API bool initCommonServer    (Tcp & tcp, std::string const & ip, int port);

TBAG_API bool initCommonClientSock(Tcp & tcp, ConnectRequest & request, struct sockaddr const * addr);
TBAG_API bool initCommonClientIpv4(Tcp & tcp, ConnectRequest & request, std::string const & ip, int port);
TBAG_API bool initCommonClientIpv6(Tcp & tcp, ConnectRequest & request, std::string const & ip, int port);
TBAG_API bool initCommonClient    (Tcp & tcp, ConnectRequest & request, std::string const & ip, int port);
// @formatter:on

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TCP_HPP__

