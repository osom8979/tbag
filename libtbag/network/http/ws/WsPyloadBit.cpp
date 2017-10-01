/**
 * @file   WsPyloadBit.cpp
 * @brief  WsPyloadBit class implementation.
 * @author zer0
 * @date   2017-10-01
 */

#include <libtbag/network/http/ws/WsPyloadBit.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/bitwise/Endian.hpp>

#include <cstring>
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

static_assert(WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE == 2,
              "The minimum header size for a WebSocket is 2 byte.");

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
#if defined(max)
    TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif

    TBAG_CONSTEXPR std::size_t const MAX_UINT16_BYTE_SIZE = std::numeric_limits<uint16_t>::max();
    TBAG_CONSTEXPR std::size_t const MAX_UINT64_BYTE_SIZE = std::numeric_limits<uint64_t>::max();

#if defined(__RESTORE_MAX__)
    TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif

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
    case WsPayloadBit::WSPL_BIT_7 : return WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE;
    case WsPayloadBit::WSPL_BIT_16: return WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE + sizeof(uint16_t);
    case WsPayloadBit::WSPL_BIT_64: return WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE + sizeof(uint64_t);
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

std::string getPayloadData(uint32_t mask, std::string const & data)
{
    util::Buffer const INPUT(data.begin(), data.end());
    util::Buffer const OUTPUT = getPayloadData(mask, INPUT);
    return std::string(OUTPUT.begin(), OUTPUT.end());
}

util::Buffer getPayloadData(uint32_t mask, util::Buffer const & data)
{
    return getPayloadData(mask, data.data(), data.size());
}

util::Buffer getPayloadData(uint32_t mask, char const * data, std::size_t size)
{
    util::Buffer result(data, data + size);
    updatePayloadData(mask, result.data(), result.size());
    return result;
}

void updatePayloadData(uint32_t mask, char * result, std::size_t size)
{
    static_assert(sizeof(uint32_t) == 4, "Why not?");
    uint8_t const * mask_ptr = reinterpret_cast<uint8_t const *>(&mask);
    for (std::size_t i = 0; i < size; ++i) {
        *(result + i) ^= mask_ptr[i % sizeof(uint32_t)];
    }
}

uint64_t getPayloadLength(char const * total_data)
{
    auto const PAYLOAD_LENGTH_7BIT = static_cast<uint8_t>(total_data[1] & 0x7F /* 0b01111111 */);
    auto const PAYLOAD_BIT = getWsPayloadBit(PAYLOAD_LENGTH_7BIT);
    return getPayloadLength(total_data + WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE, PAYLOAD_LENGTH_7BIT, PAYLOAD_BIT);
}

uint64_t getPayloadLength(char const * data, uint8_t payload_length_7bit, WsPayloadBit payload_bit)
{
    // Update payload length.
    if (payload_bit == WsPayloadBit::WSPL_BIT_7) {
        return payload_length_7bit;

    } else if (payload_bit == WsPayloadBit::WSPL_BIT_16) {
        uint16_t temp = 0;
        ::memcpy(&temp, data, sizeof(uint16_t));
        return bitwise::toHost(temp);

    } else if (payload_bit == WsPayloadBit::WSPL_BIT_64) {
        uint64_t temp = 0;
        ::memcpy(&temp, data, sizeof(uint64_t));
        return bitwise::toHost(temp);
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

std::size_t calculateBufferSize(uint64_t payload_length, bool is_mask)
{
    std::size_t const DEFAULT_SIZE = WEBSOCKET_MINIMUM_HEADER_BYTE_SIZE
                                     + payload_length + (is_mask ? sizeof(uint32_t) : 0);
    switch (getWsPayloadBitWithPayloadLength(payload_length)) {
    case WsPayloadBit::WSPL_BIT_7:  return DEFAULT_SIZE;
    case WsPayloadBit::WSPL_BIT_16: return DEFAULT_SIZE + sizeof(uint16_t);
    case WsPayloadBit::WSPL_BIT_64: return DEFAULT_SIZE + sizeof(uint64_t);
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return DEFAULT_SIZE;
}

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

