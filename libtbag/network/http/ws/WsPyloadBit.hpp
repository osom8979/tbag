/**
 * @file   WsPyloadBit.hpp
 * @brief  WsPyloadBit class prototype.
 * @author zer0
 * @date   2017-10-01
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSPYLOADBIT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSPYLOADBIT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/network/http/base/HttpCommon.hpp>

#include <cstdint>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

enum class WsPayloadBit : uint8_t
{
    WSPL_BIT_7  =  7,
    WSPL_BIT_16 = 16,
    WSPL_BIT_64 = 64,
};

TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_7BIT_TYPE_SIZE  = 125;
TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_16BIT_TYPE_SIZE = 126;
TBAG_CONSTEXPR uint8_t const WS_PAYLOAD_64BIT_TYPE_SIZE = 127;

/**
 * @remarks
 *  The length of the "Payload data", in bytes: if 0-125, that is the
 *  payload length.  If 126, the following 2 bytes interpreted as a
 *  16-bit unsigned integer are the payload length.  If 127, the
 *  following 8 bytes interpreted as a 64-bit unsigned integer (the
 *  most significant bit MUST be 0) are the payload length.  Multibyte
 *  length quantities are expressed in network byte order.  Note that
 *  in all cases, the minimal number of bytes MUST be used to encode
 *  the length, for example, the length of a 124-byte-long string
 *  can't be encoded as the sequence 126, 0, 124.  The payload length
 *  is the length of the "Extension data" + the length of the
 *  "Application data".  The length of the "Extension data" may be
 *  zero, in which case the payload length is the length of the
 *  "Application data".
 *
 * @see <https://tools.ietf.org/html/rfc6455#section-5.2>
 */
TBAG_API WsPayloadBit getWsPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT;
TBAG_API WsPayloadBit getWsPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT;

TBAG_API uint8_t getPayloadDataByteIndex(WsPayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT;
TBAG_API uint8_t getMaskingKeyByteIndex (WsPayloadBit payload_bit) TBAG_NOEXCEPT;

TBAG_API uint32_t copyMaskingKeyFromBuffer(char const * data) TBAG_NOEXCEPT;

TBAG_API std::string  getPayloadData   (uint32_t mask, std::string const & data);
TBAG_API util::Buffer getPayloadData   (uint32_t mask, util::Buffer const & data);
TBAG_API util::Buffer getPayloadData   (uint32_t mask, char const * data, std::size_t size);
TBAG_API void         updatePayloadData(uint32_t mask, char * result, std::size_t size);

TBAG_API uint64_t getPayloadLength(char const * total_data);
TBAG_API uint64_t getPayloadLength(char const * data, uint8_t payload_length_7bit, WsPayloadBit payload_bit);

TBAG_API std::size_t calculateBufferSize(uint64_t payload_length, bool is_mask);

} // namespace ws

using namespace libtbag::network::http::ws;

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WS_WSPYLOADBIT_HPP__

