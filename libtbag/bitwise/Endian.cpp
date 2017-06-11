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
#elif defined(TBAG_PLATFORM_LINUX)
# include <endian.h>
#else
# include <netinet/in.h> // or <arpa/inet.h>
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

int64_t toHost(int64_t network) TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_LINUX)
    return be64toh(network);
#else
    return ntohll(network);
#endif
}

uint64_t toHost(uint64_t network) TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_LINUX)
    return be64toh(network);
#else
    return ntohll(network);
#endif
}

int64_t toNetwork(int64_t host) TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_LINUX)
    return htobe64(network);
#else
    return htonll(host);
#endif
}

uint64_t toNetwork(uint64_t host) TBAG_NOEXCEPT
{
#if defined(TBAG_PLATFORM_LINUX)
    return htobe64(network);
#else
    return htonll(host);
#endif
}

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

