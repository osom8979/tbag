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
#include <cstdint>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace bitwise {

TBAG_API  int16_t toHost( int16_t network) TBAG_NOEXCEPT;
TBAG_API uint16_t toHost(uint16_t network) TBAG_NOEXCEPT;
TBAG_API  int32_t toHost( int32_t network) TBAG_NOEXCEPT;
TBAG_API uint32_t toHost(uint32_t network) TBAG_NOEXCEPT;

TBAG_API  int16_t toNetwork( int16_t host) TBAG_NOEXCEPT;
TBAG_API uint16_t toNetwork(uint16_t host) TBAG_NOEXCEPT;
TBAG_API  int32_t toNetwork( int32_t host) TBAG_NOEXCEPT;
TBAG_API uint32_t toNetwork(uint32_t host) TBAG_NOEXCEPT;

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

