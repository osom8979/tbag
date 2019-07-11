/**
 * @file   Endian.hpp
 * @brief  Endian class prototype.
 * @author zer0
 * @date   2017-04-25
 *
 * @remarks
 * Host <--> Network converter:
 *  - htonl, htons, ntohl, ntohs - convert values between host and network byte order:
 *  - POSIX.1-2001. <netinet/in.h> or <arpa/inet.h>
 *  - Windows <Winsock2.h> with Ws2_32.lib
 *
 * @see <https://linux.die.net/man/3/ntohs>
 * @see <https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms740075(v=vs.85).aspx>
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_ENDIAN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_ENDIAN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <cassert>
#include <cstdint>

#ifndef TBAG_BYTE_SWAP_CONST_UINT16
#define TBAG_BYTE_SWAP_CONST_UINT16(x) \
    ((uint16_t)((((uint16_t)(x) & 0xFF00) >> 8) | \
                (((uint16_t)(x) & 0x00FF) << 8)))
#endif

#ifndef TBAG_BYTE_SWAP_CONST_UINT32
#define TBAG_BYTE_SWAP_CONST_UINT32(x) \
    ((uint32_t)((((uint32_t)(x) & 0xFF000000) >> 24) | \
                (((uint32_t)(x) & 0x00FF0000) >>  8) | \
                (((uint32_t)(x) & 0x0000FF00) <<  8) | \
                (((uint32_t)(x) & 0x000000FF) << 24)))
#endif

#ifndef TBAG_BYTE_SWAP_CONST_UINT64
#define TBAG_BYTE_SWAP_CONST_UINT64(x) \
    ((uint64_t)((((uint64_t)(x) & 0xFF00000000000000ULL) >> 56) | \
                (((uint64_t)(x) & 0x00FF000000000000ULL) >> 40) | \
                (((uint64_t)(x) & 0x0000FF0000000000ULL) >> 24) | \
                (((uint64_t)(x) & 0x000000FF00000000ULL) >>  8) | \
                (((uint64_t)(x) & 0x00000000FF000000ULL) <<  8) | \
                (((uint64_t)(x) & 0x0000000000FF0000ULL) << 24) | \
                (((uint64_t)(x) & 0x000000000000FF00ULL) << 40) | \
                (((uint64_t)(x) & 0x00000000000000FFULL) << 56)))
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

TBAG_CONSTEXPR uint32_t const TEST_ONE = 1U;

inline bool isLittleEndianSystem() TBAG_NOEXCEPT
{
    return ((*(volatile uint8_t *)&TEST_ONE) == 1);
}

inline bool isBigEndianSystem() TBAG_NOEXCEPT
{
    return !isLittleEndianSystem();
}

inline bool isMiddleEndianSystem() TBAG_NOEXCEPT
{
    assert(false && "Unsupported operation.");
    return false;
}

// --------------
// 16Bit convert.
// --------------

TBAG_API  int16_t    toHost( int16_t network) TBAG_NOEXCEPT;
TBAG_API uint16_t    toHost(uint16_t network) TBAG_NOEXCEPT;
TBAG_API  int16_t toNetwork( int16_t    host) TBAG_NOEXCEPT;
TBAG_API uint16_t toNetwork(uint16_t    host) TBAG_NOEXCEPT;

// --------------
// 32Bit convert.
// --------------

TBAG_API  int32_t    toHost( int32_t network) TBAG_NOEXCEPT;
TBAG_API uint32_t    toHost(uint32_t network) TBAG_NOEXCEPT;
TBAG_API  int32_t toNetwork( int32_t    host) TBAG_NOEXCEPT;
TBAG_API uint32_t toNetwork(uint32_t    host) TBAG_NOEXCEPT;

// --------------
// 64Bit convert.
// --------------

TBAG_API  int64_t    toHost( int64_t network) TBAG_NOEXCEPT;
TBAG_API uint64_t    toHost(uint64_t network) TBAG_NOEXCEPT;
TBAG_API  int64_t toNetwork( int64_t    host) TBAG_NOEXCEPT;
TBAG_API uint64_t toNetwork(uint64_t    host) TBAG_NOEXCEPT;

} // namespace bitwise

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BITWISE_ENDIAN_HPP__

