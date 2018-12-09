/**
 * @file   Ip.hpp
 * @brief  Ip class prototype.
 * @author zer0
 * @date   2018-12-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NET_IP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NET_IP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net {

TBAG_CONSTEXPR char const * const ANY_IPV4 = "0.0.0.0";
TBAG_CONSTEXPR char const * const BROADCAST_SUBNET_IPV4 = "255.255.255.255";

TBAG_CONSTEXPR char const * const ANY_SUBNET_MASK_IPV4 = "0.0.0.0";

TBAG_CONSTEXPR char const * const LOOPBACK_NAME = "localhost";
TBAG_CONSTEXPR char const * const LOOPBACK_IPV4 = "127.0.0.1";
TBAG_CONSTEXPR char const * const LOOPBACK_IPV6 = "::1";

TBAG_API bool isIpv4(std::string const & ip);
TBAG_API bool isIpv6(std::string const & ip);

inline bool isIp(std::string const & ip)
{
    return isIpv4(ip) || isIpv6(ip);
}

// ------------------------
// Miscellaneous utilities.
// ------------------------

inline bool isWellKnownPort(int port) TBAG_NOEXCEPT
{
    return 0 <= COMPARE_AND(port) <= 1023;
}

inline bool isRegisteredPort(int port) TBAG_NOEXCEPT
{
    return 1024 <= COMPARE_AND(port) <= 49151;
}

inline bool isDynamicPort(int port) TBAG_NOEXCEPT
{
    return 49152 <= COMPARE_AND(port) <= 65535;
}

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NET_IP_HPP__

