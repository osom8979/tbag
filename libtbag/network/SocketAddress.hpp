/**
 * @file   SocketAddress.hpp
 * @brief  SocketAddress class prototype.
 * @author zer0
 * @date   2017-06-18
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
#include <libtbag/network/Uri.hpp>

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

namespace network {

/**
 * SocketAddress class prototype.
 *
 * @author zer0
 * @date   2017-06-18
 */
class TBAG_API SocketAddress
{
private:
    union {
        sockaddr     common;
        sockaddr_in  ipv4;
        sockaddr_in6 ipv6;
    } addr;

public:
    SocketAddress();
    SocketAddress(SocketAddress const & obj);
    SocketAddress(SocketAddress && obj);
    virtual ~SocketAddress();

public:
    SocketAddress & operator =(SocketAddress const & obj);
    SocketAddress & operator =(SocketAddress && obj);

public:
    Err initIpv4(std::string const & ip, int port = 0);
    Err initIpv6(std::string const & ip, int port = 0);
    Err initName(std::string const & host, std::string const & service = "", int port = 0);

public:
    Err init(std::string const & host, int port);
    Err init(Uri const & uri);

public:
    inline struct sockaddr     const * getCommon() const TBAG_NOEXCEPT { return &addr.common; }
    inline struct sockaddr_in  const * getIpv4  () const TBAG_NOEXCEPT { return &addr.ipv4; }
    inline struct sockaddr_in6 const * getIpv6  () const TBAG_NOEXCEPT { return &addr.ipv6; }

    inline bool isIpv4() const TBAG_NOEXCEPT { return addr.ipv4.sin_port == AF_INET; }
    inline bool isIpv6() const TBAG_NOEXCEPT { return addr.ipv6.sin6_port == AF_INET6; }

public:
    std::string getIpName() const;
    int getPortNumber() const;
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKETADDRESS_HPP__

