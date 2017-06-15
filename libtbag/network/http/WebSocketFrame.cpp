/**
 * @file   WebSocketFrame.cpp
 * @brief  WebSocketFrame class implementation.
 * @author zer0
 * @date   2017-06-11
 */

#include <libtbag/network/http/WebSocketFrame.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/bitwise/Endian.hpp>

#include <cassert>
#include <cstring>

#include <algorithm>
#include <utility>
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WebSocketFrame::WebSocketFrame()
        : _fin (false), _rsv1(false), _rsv2(false), _rsv3(false),
          _opcode(OpCode::OC_CONTINUATION_FRAME), _mask(false),
          _payload_length(0), _masking_key(0)
{
    // EMPTY.
}

WebSocketFrame::WebSocketFrame(WebSocketFrame const & obj)
{
    (*this) = obj;
}

WebSocketFrame::WebSocketFrame(WebSocketFrame && obj)
{
    (*this) = std::move(obj);
}

WebSocketFrame::~WebSocketFrame()
{
    // EMPTY.
}

WebSocketFrame & WebSocketFrame::operator =(WebSocketFrame const & obj)
{
    if (this != &obj) {
        _fin = obj._fin;
        _rsv1 = obj._rsv1;
        _rsv2 = obj._rsv2;
        _rsv3 = obj._rsv3;
        _opcode = obj._opcode;
        _mask = obj._mask;
        _payload_length = obj._payload_length;
        _masking_key = obj._masking_key;
        _payload_buffer = obj._payload_buffer;
    }
    return *this;
}

WebSocketFrame & WebSocketFrame::operator =(WebSocketFrame && obj)
{
    if (this != &obj) {
        std::swap(_fin, obj._fin);
        std::swap(_rsv1, obj._rsv1);
        std::swap(_rsv2, obj._rsv2);
        std::swap(_rsv3, obj._rsv3);
        std::swap(_opcode, obj._opcode);
        std::swap(_mask, obj._mask);
        std::swap(_payload_length, obj._payload_length);
        std::swap(_masking_key, obj._masking_key);
        _payload_buffer.swap(obj._payload_buffer);
    }
    return *this;
}

Err WebSocketFrame::execute(uint8_t const * data, std::size_t size)
{
    if (size < 2) {
        return Err::E_SMALLBUF;
    }

    _fin    = static_cast<  bool>(data[0] & 0x80 /* 0b10000000 */);
    _rsv1   = static_cast<  bool>(data[0] & 0x40 /* 0b01000000 */);
    _rsv2   = static_cast<  bool>(data[0] & 0x20 /* 0b00100000 */);
    _rsv3   = static_cast<  bool>(data[0] & 0x10 /* 0b00010000 */);
    _opcode = static_cast<OpCode>(data[0] & 0x0F /* 0b00001111 */);
    _mask   = static_cast<  bool>(data[1] & 0x80 /* 0b10000000 */);

    auto const PAYLOAD_LENGTH_7BIT = static_cast<uint8_t>(data[1] & 0x7F /* 0b01111111 */);
    auto const PAYLOAD_BIT    = getPayloadBit(PAYLOAD_LENGTH_7BIT);
    auto const MASK_KEY_INDEX = getMaskingKeyByteIndex(PAYLOAD_BIT);
    auto const DATA_INDEX     = getPayloadDataByteIndex(PAYLOAD_BIT, _mask);

    if (size < MASK_KEY_INDEX) {
        return Err::E_SMALLBUF;
    }

    // Update payload length.
    if (PAYLOAD_BIT == PayloadBit::PL_BIT_7) {
        _payload_length = PAYLOAD_LENGTH_7BIT;
    } else if (PAYLOAD_BIT == PayloadBit::PL_BIT_16) {
        uint16_t temp = 0;
        ::memcpy(&temp, &data[2], sizeof(uint16_t));
        _payload_length = bitwise::toHost(temp);
    } else if (PAYLOAD_BIT == PayloadBit::PL_BIT_64) {
        uint64_t temp = 0;
        ::memcpy(&temp, &data[2], sizeof(uint64_t));
        _payload_length = bitwise::toHost(temp);
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }

    // Update masking key.
    if (_mask) {
        if (size < DATA_INDEX) {
            return Err::E_SMALLBUF;
        }
        _masking_key = libtbag::network::http::getMaskingKey(&data[MASK_KEY_INDEX]);
    } else {
        _masking_key = 0;
    }

    // Update payload data.
    if (_payload_length > 0) {
        if (size < DATA_INDEX + _payload_length) {
            return Err::E_SMALLBUF;
        }
        if (_payload_buffer.size() < _payload_length) {
            _payload_buffer.resize(_payload_length);
        }
        _payload_buffer.assign(&data[DATA_INDEX], &data[DATA_INDEX] + _payload_length);
        if (_mask) {
            updatePayloadData(_masking_key, _payload_buffer.data(), _payload_length);
        }
    }

    return Err::E_SUCCESS;
}

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

std::size_t WebSocketFrame::calculateWriteBufferSize() const
{
    std::size_t default_size = 2/*HEADER*/ + _payload_length + (_mask ? sizeof(uint32_t) : 0);
    switch(getPayloadBitWithPayloadLength(_payload_length)) {
    case PayloadBit::PL_BIT_7:  return default_size;
    case PayloadBit::PL_BIT_16: return default_size + MAX_UINT16_BYTE_SIZE;
    case PayloadBit::PL_BIT_64: return default_size + MAX_UINT64_BYTE_SIZE;
    default:                    return default_size;
    }
}

std::size_t WebSocketFrame::write(uint8_t * data, std::size_t size)
{
    ::memset(data, 0x00, size);

    if (size < 2) {
        return 0;
    }

    *(data + 0) |= ( _fin ? 0x80 : 0);
    *(data + 0) |= (_rsv1 ? 0x40 : 0);
    *(data + 0) |= (_rsv2 ? 0x20 : 0);
    *(data + 0) |= (_rsv3 ? 0x10 : 0);
    *(data + 0) |= static_cast<uint8_t>(_opcode);
    *(data + 1) |= (_mask ? 0x80 : 0);

    // Next index (Written size)
    std::size_t index = 2;

    // Update payload length.
    if (_payload_length <= 125) {
        if (size < index) {
            return 0;
        }
        *(data + 1) |= _payload_length;

    } else if (_payload_length <= MAX_UINT16_BYTE_SIZE) {
        if (size < index + sizeof(uint16_t)) {
            return 0;
        }
        *(data + 1) |= 0x7E;
        uint16_t temp = bitwise::toNetwork(static_cast<uint16_t>(_payload_length));
        ::memcpy(data + index, &temp, sizeof(uint16_t));
        index += sizeof(uint16_t);

    } else {
        assert(MAX_UINT16_BYTE_SIZE < COMPARE_AND(_payload_length) <= MAX_UINT64_BYTE_SIZE);
        if (size < index + sizeof(uint64_t)) {
            return 0;
        }

        *(data + 1) |= 0x7F;
        uint64_t temp = bitwise::toNetwork(static_cast<uint64_t>(_payload_length));
        ::memcpy(data + index, &temp, sizeof(uint64_t));
        index += sizeof(uint64_t);
    }

    // Update masking key.
    if (_mask) {
        if (size < index + sizeof(uint32_t)) {
            return 0;
        }
        ::memcpy(data + index, &_masking_key, sizeof(uint32_t));
        index += sizeof(uint32_t);
    }

    // Update payload data.
    if (_payload_length > 0) {
        if (size < index + _payload_length) {
            return 0;
        }
        ::memcpy(data + index, _payload_buffer.data(), _payload_length);
        if (_mask) {
            updatePayloadData(_masking_key, data + index, _payload_length);
        }
        index += _payload_length;
    }

    return index;
}

std::size_t WebSocketFrame::write(Buffer & buffer)
{
    return write(buffer.data(), buffer.size());
}

bool WebSocketFrame::updateRequest(bool fin, bool rsv1, bool rsv2, bool rsv3,
                                   OpCode opcode, uint32_t masking_key, uint8_t const * data, std::size_t size)
{
    _fin  = fin;
    _rsv1 = rsv1;
    _rsv2 = rsv2;
    _rsv3 = rsv3;
    _opcode = opcode;
    _mask = true;
    _payload_length = size;
    _masking_key = masking_key;
    if (_payload_buffer.size() < _payload_length) {
        _payload_buffer.resize(_payload_length);
    }
    _payload_buffer.assign(data, data + _payload_length);
    return true;
}

bool WebSocketFrame::updateResponse(bool fin, bool rsv1, bool rsv2, bool rsv3,
                                    OpCode opcode, uint8_t const * data, std::size_t size)
{
    _fin  = fin;
    _rsv1 = rsv1;
    _rsv2 = rsv2;
    _rsv3 = rsv3;
    _opcode = opcode;
    _mask = false;
    _payload_length = size;
    _masking_key = 0;
    if (_payload_buffer.size() < _payload_length) {
        _payload_buffer.resize(_payload_length);
    }
    _payload_buffer.assign(data, data + _payload_length);
    return true;
}

// ----------
// Utilities.
// ----------

uint8_t getPayloadDataByteIndex(PayloadBit payload_bit, bool is_mask) TBAG_NOEXCEPT
{
    return getMaskingKeyByteIndex(payload_bit) + (is_mask ? sizeof(uint32_t) : 0);
}

uint8_t getMaskingKeyByteIndex(PayloadBit payload_bit) TBAG_NOEXCEPT
{
    switch (payload_bit) {
    case PayloadBit::PL_BIT_7:  return 2;
    case PayloadBit::PL_BIT_16: return 2 + sizeof(uint16_t);
    case PayloadBit::PL_BIT_64: return 2 + sizeof(uint64_t);
    }
    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return 0;
}

PayloadBit getPayloadBit(uint8_t payload_length_7bit) TBAG_NOEXCEPT
{
    if (payload_length_7bit <= 125) {
        return PayloadBit::PL_BIT_7;
    } else if (payload_length_7bit == 126) {
        return PayloadBit::PL_BIT_16;
    } else if (payload_length_7bit == 127) {
        return PayloadBit::PL_BIT_64;
    } else {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    return PayloadBit::PL_BIT_7;
}

PayloadBit getPayloadBitWithPayloadLength(uint64_t payload_length) TBAG_NOEXCEPT
{
    if (payload_length <= 125) {
        return PayloadBit::PL_BIT_7;
    } else if (payload_length <= MAX_UINT16_BYTE_SIZE) {
        return PayloadBit::PL_BIT_16;
    }

    assert(MAX_UINT16_BYTE_SIZE < COMPARE_AND(payload_length) <= MAX_UINT64_BYTE_SIZE);
    return PayloadBit::PL_BIT_64;
}

uint32_t getMaskingKey(uint8_t const * data) TBAG_NOEXCEPT
{
    uint32_t network_32byte_size = 0;
    ::memcpy(&network_32byte_size, data, sizeof(uint32_t));
    return network_32byte_size;
}

std::string getPayloadData(uint32_t mask, std::string const & data)
{
    std::vector<uint8_t> const INPUT(data.begin(), data.end());
    std::vector<uint8_t> const OUTPUT = getPayloadData(mask, INPUT);
    return std::string(OUTPUT.begin(), OUTPUT.end());
}

std::vector<uint8_t> getPayloadData(uint32_t mask, std::vector<uint8_t> const & data)
{
    return getPayloadData(mask, data.data(), data.size());
}

std::vector<uint8_t> getPayloadData(uint32_t mask, uint8_t const * data, std::size_t size)
{
    std::vector<uint8_t> result(data, data + size);
    updatePayloadData(mask, result.data(), result.size());
    return result;
}

void updatePayloadData(uint32_t mask, uint8_t * result, std::size_t size)
{
    static_assert(sizeof(uint32_t) == 4, "Why not?");
    uint8_t const * mask_ptr = reinterpret_cast<uint8_t const *>(&mask);
    for (std::size_t i = 0; i < size; ++i) {
        *(result + i) ^= mask_ptr[i % sizeof(uint32_t)];
    }
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

