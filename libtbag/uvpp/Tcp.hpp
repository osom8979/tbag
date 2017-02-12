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
    using Parent = Stream;

public:
    Tcp();
    Tcp(Loop & loop);
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
    std::string getSockName();

    /** Get the address of the peer connected to the handle. */
    std::string getPeerName();

    /** Establish an IPv4 or IPv6 TCP connection. */
    uerr connect(ConnectRequest & request, sockaddr const * address);

// Event methods.
public:
    virtual void onConnect(ConnectRequest & request, uerr code);

// Utilities.
public:
    static std::string getIpName(sockaddr     const * address); // Don't use this method [IP RESULT BUG].
    static std::string getIpName(sockaddr_in  const * address);
    static std::string getIpName(sockaddr_in6 const * address);

    static uerr initAddress(std::string const & ip, int port, sockaddr_in * addr);
    static uerr initAddress(std::string const & ip, int port, sockaddr_in6 * addr);

    static bool isIpv4(std::string const & ip);
    static bool isIpv6(std::string const & ip);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_TCP_HPP__

