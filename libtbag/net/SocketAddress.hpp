/**
 * @file   SocketAddress.hpp
 * @brief  SocketAddress class prototype.
 * @author zer0
 * @date   2017-06-18
 * @date   2019-01-19 (Move: libtbag/network -> libtbag/net)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKETADDRESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKETADDRESS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/net/Uri.hpp>

#include <string>

#if defined(TBAG_PLATFORM_WINDOWS)
# include <winsock2.h>
# include <WS2tcpip.h> // sockaddr_in6, addrinfo
# include <Windows.h>
#else
# include <netinet/in.h>
# include <netdb.h> // addrinfo
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net {

/**
 * SocketAddress class prototype.
 *
 * @author zer0
 * @date   2017-06-18
 * @date   2019-01-19 (Move: libtbag/network -> libtbag/net)
 */
class TBAG_API SocketAddress
{
private:
    union {
        sockaddr     common;
        sockaddr_in  ipv4;
        sockaddr_in6 ipv6;
    } _addr;

public:
    SocketAddress();
    explicit SocketAddress(struct sockaddr     const * addr);
    explicit SocketAddress(struct sockaddr_in  const * addr);
    explicit SocketAddress(struct sockaddr_in6 const * addr);
    SocketAddress(SocketAddress const & obj);
    SocketAddress(SocketAddress && obj) TBAG_NOEXCEPT;
    virtual ~SocketAddress();

public:
    SocketAddress & operator =(SocketAddress const & obj);
    SocketAddress & operator =(SocketAddress && obj) TBAG_NOEXCEPT;

public:
    Err init(struct sockaddr     const * addr);
    Err init(struct sockaddr_in  const * addr);
    Err init(struct sockaddr_in6 const * addr);

public:
    Err initIpv4(std::string const & ip, int port = 0);
    Err initIpv6(std::string const & ip, int port = 0);
    Err initName(std::string const & host, std::string const & service = "", int port = 0);

public:
    Err init(std::string const & host, int port);
    Err init(Uri const & uri);

public:
    inline struct sockaddr     const * getCommon() const TBAG_NOEXCEPT { return &_addr.common; }
    inline struct sockaddr_in  const * getIpv4  () const TBAG_NOEXCEPT { return &_addr.ipv4; }
    inline struct sockaddr_in6 const * getIpv6  () const TBAG_NOEXCEPT { return &_addr.ipv6; }

    inline bool isIpv4() const TBAG_NOEXCEPT { return _addr.ipv4.sin_port  == AF_INET;  }
    inline bool isIpv6() const TBAG_NOEXCEPT { return _addr.ipv6.sin6_port == AF_INET6; }

public:
    std::string getIpName() const;
    int getPortNumber() const;
};

// -----------------------
// Miscellaneous utilities
// -----------------------

/**
 * Find the network interface that the client IP is connected to and return the hostname of that interface.
 *
 * @param[in] ip
 *      IP address.
 * @param[in] flags
 *      Flag values for getnameinfo(). (e.g. NI_NUMERICHOST)
 * @param[out] result
 *      Interface host name.
 *
 * @return
 *  Result error code.
 */
TBAG_API Err findHostNameOfConnectedInterfaceByIpAddress(std::string const & ip, int flags, std::string & result);
TBAG_API std::string findHostNameOfConnectedInterfaceByIpAddress(std::string const & ip, int flags);

/**
 * Find the network interface that the client IP is connected to and return the hostname of that interface.
 *
 * Flag value is <code>NI_NUMERICHOST</code>.
 *
 * @param[in] ip
 *      IP address.
 *
 * @return
 *  Interface host name.
 */
TBAG_API std::string findHostNameOfConnectedInterfaceByIpAddress(std::string const & ip);

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKETADDRESS_HPP__

