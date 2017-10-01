/**
 * @file   WsPyloadBit.cpp
 * @brief  WsPyloadBit class implementation.
 * @author zer0
 * @date   2017-10-01
 */

#include <libtbag/network/http/ws/WsPyloadBit.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <cstring>
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

#if defined(max)
TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif

TBAG_CONSTEXPR static std::size_t const MAX_UINT16_BYTE_SIZE = std::numeric_limits<uint16_t>::max();
TBAG_CONSTEXPR static std::size_t const MAX_UINT64_BYTE_SIZE = std::numeric_limits<uint64_t>::max();

#if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif

WsPayloadBit getWsPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT
{
    if (payload_length_7bit <= WS_PAYLOAD_7BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_7;
    } else if (payload_length_7bit == WS_PAYLOAD_16BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_16;
    } else if (payload_length_7bit == WS_PAYLOAD_64BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_64;
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return WsPayloadBit::WSPL_BIT_7;
}

WsPayloadBit getWsPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT
{
    if (payload_length <= WS_PAYLOAD_7BIT_TYPE_SIZE) {
        return WsPayloadBit::WSPL_BIT_7;
    } else if (payload_length <= MAX_UINT16_BYTE_SIZE) {
        return WsPayloadBit::WSPL_BIT_16;
    }

    assert(MAX_UINT16_BYTE_SIZE < COMPARE_AND(payload_length) <= MAX_UINT64_BYTE_SIZE);
    return WsPayloadBit::WSPL_BIT_64;
}

uint8_t getPayloadDataByteIndex(WsPayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT
{
    return getMaskingKeyByteIndex(payload_bit) + (is_mask ? sizeof(uint32_t) : 0);
}

uint8_t getMaskingKeyByteIndex(WsPayloadBit payload_bit) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (payload_bit) {
    case WsPayloadBit::WSPL_BIT_7 : return WS_MINIMUM_HEADER_BYTE_SIZE;
    case WsPayloadBit::WSPL_BIT_16: return WS_MINIMUM_HEADER_BYTE_SIZE + sizeof(uint16_t);
    case WsPayloadBit::WSPL_BIT_64: return WS_MINIMUM_HEADER_BYTE_SIZE + sizeof(uint64_t);
    }
    // @formatter:on

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

uint32_t copyMaskingKeyFromBuffer(char const * data) TBAG_NOEXCEPT
{
    uint32_t network_32byte_size = 0;
    ::memcpy(&network_32byte_size, data, sizeof(uint32_t));
    return network_32byte_size;
}

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

