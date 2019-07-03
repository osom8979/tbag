/**
 * @file   Endian.cpp
 * @brief  Endian class implementation.
 * @author zer0
 * @date   2017-04-25
 */

#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/config-ex.h>

// For the Windows platform:
// https://msdn.microsoft.com/en-us/library/a3140177.aspx
#include <stdlib.h>

#if defined(TBAG_PLATFORM_WINDOWS)
#include <winsock2.h>
#endif
#if defined(HAVE_ENDIAN_H)
#include <endian.h>
#endif
#if defined(HAVE_NETINET_IN_H)
#include <netinet/in.h>
#endif
#if defined(HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

// clang-format off
 int16_t    toHost( int16_t network) TBAG_NOEXCEPT { return ntohs(network); }
uint16_t    toHost(uint16_t network) TBAG_NOEXCEPT { return ntohs(network); }
 int16_t toNetwork( int16_t    host) TBAG_NOEXCEPT { return htons(host); }
uint16_t toNetwork(uint16_t    host) TBAG_NOEXCEPT { return htons(host); }

 int32_t    toHost( int32_t network) TBAG_NOEXCEPT { return ntohl(network); }
uint32_t    toHost(uint32_t network) TBAG_NOEXCEPT { return ntohl(network); }
 int32_t toNetwork( int32_t    host) TBAG_NOEXCEPT { return htonl(host); }
uint32_t toNetwork(uint32_t    host) TBAG_NOEXCEPT { return htonl(host); }
// clang-format on

int64_t toHost(int64_t network) TBAG_NOEXCEPT
{
    return toHost((uint64_t)network);
}

uint64_t toHost(uint64_t network) TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_WINDOWS)
    if (isBigEndianSystem()) {
        return network;
    } else {
        return _byteswap_uint64(network);
    }
#elif defined(TBAG_PLATFORM_LINUX)
    if (isBigEndianSystem()) {
        return network;
    } else {
        return be64toh(network);
    }
#else
    return ntohll(network);
#endif
}

int64_t toNetwork(int64_t host) TBAG_NOEXCEPT
{
    return toNetwork((uint64_t)host);
}

uint64_t toNetwork(uint64_t host) TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_WINDOWS)
    if (isBigEndianSystem()) {
        return host;
    } else {
        return _byteswap_uint64(host);
    }
#elif defined(TBAG_PLATFORM_LINUX)
    if (isBigEndianSystem()) {
        return host;
    } else {
        return htobe64(host);
    }
#else
    return htonll(host);
#endif
}

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

