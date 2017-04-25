/**
 * @file   Endian.cpp
 * @brief  Endian class implementation.
 * @author zer0
 * @date   2017-04-25
 */

#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/log/Log.hpp>

#if defined(TBAG_PLATFORM_WINDOWS)
# include <winsock2.h>
# include <WS2tcpip.h> // sockaddr_in6
# include <Windows.h>
#else
# include <netinet/in.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

// @formatter:off
 int16_t toHost( int16_t network) TBAG_NOEXCEPT { return ntohs (network); }
uint16_t toHost(uint16_t network) TBAG_NOEXCEPT { return ntohs (network); }
 int32_t toHost( int32_t network) TBAG_NOEXCEPT { return ntohl (network); }
uint32_t toHost(uint32_t network) TBAG_NOEXCEPT { return ntohl (network); }

 int16_t toNetwork( int16_t host) TBAG_NOEXCEPT { return htons (host); }
uint16_t toNetwork(uint16_t host) TBAG_NOEXCEPT { return htons (host); }
 int32_t toNetwork( int32_t host) TBAG_NOEXCEPT { return htonl (host); }
uint32_t toNetwork(uint32_t host) TBAG_NOEXCEPT { return htonl (host); }
// @formatter:on

// int64_t toHost( int64_t network) { return ntohll(network); }
//uint64_t toHost(uint64_t network) { return ntohll(network); }
// int64_t toNetwork( int64_t host) { return htonll(host); }
//uint64_t toNetwork(uint64_t host) { return htonll(host); }

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

