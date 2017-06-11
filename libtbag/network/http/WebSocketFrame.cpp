/**
 * @file   WebSocketFrame.cpp
 * @brief  WebSocketFrame class implementation.
 * @author zer0
 * @date   2017-06-11
 */

#include <libtbag/network/http/WebSocketFrame.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/bitwise/Endian.hpp>

#include <cassert>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WebSocketFrame::WebSocketFrame()
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
        _buffer = obj._buffer;
    }
    return *this;
}

WebSocketFrame & WebSocketFrame::operator =(WebSocketFrame && obj)
{
    if (this != &obj) {
        _buffer.swap(obj._buffer);
    }
    return *this;
}

bool WebSocketFrame::fin() const
{
    return static_cast<bool>(_buffer[0] & 0x80);
}

bool WebSocketFrame::rsv1() const
{
    return static_cast<bool>(_buffer[0] & 0x40);
}

bool WebSocketFrame::rsv2() const
{
    return static_cast<bool>(_buffer[0] & 0x20);
}

bool WebSocketFrame::rsv3() const
{
    return static_cast<bool>(_buffer[0] & 0x10);
}

WebSocketFrame::OpCode WebSocketFrame::opcode() const
{
    return static_cast<OpCode>(_buffer[0] & 0x0F);
}

bool WebSocketFrame::mask() const
{
    return static_cast<bool>(_buffer[1] & 0x40);
}

WebSocketFrame::PayloadBit WebSocketFrame::payloadLengthBitSize() const
{
    uint8_t const LENGTH = static_cast<uint8_t>(_buffer[1] & 0x7F);
    if (LENGTH <= 125) {
        return PayloadBit::PL_BIT_7;
    } else if (LENGTH == 126) {
        return PayloadBit::PL_BIT_16;
    } else if (LENGTH == 127) {
        return PayloadBit::PL_BIT_64;
    } else {
        assert(false && "Inaccessible block.");
    }
    return PayloadBit::PL_BIT_7;
}

uint64_t WebSocketFrame::payloadLength() const
{
    uint8_t const LENGTH = static_cast<uint8_t>(_buffer[1] & 0x7F);
    if (LENGTH <= 125) {
        return LENGTH;

    } else if (LENGTH == 126) {
        uint16_t network_16byte_size = 0;
        ::memcpy(&network_16byte_size, &_buffer[2], sizeof(uint16_t));
        return bitwise::toHost(network_16byte_size);

    } else if (LENGTH == 127) {
        uint64_t network_64byte_size = 0;
        ::memcpy(&network_64byte_size, &_buffer[2], sizeof(uint64_t));
        return bitwise::toHost(network_64byte_size);

    } else {
        assert(false && "Inaccessible block.");
    }
    return LENGTH;
}

uint8_t WebSocketFrame::maskingKeyByteIndex() const
{
    switch (payloadLengthBitSize()) {
    case PayloadBit::PL_BIT_7:  return  2;
    case PayloadBit::PL_BIT_16: return  4;
    case PayloadBit::PL_BIT_64: return 10;
    }
    assert(false && "Inaccessible block.");
    return 2;
}

uint32_t WebSocketFrame::maskingKey() const
{
    uint32_t network_32byte_size = 0;
    ::memcpy(&network_32byte_size, &_buffer[maskingKeyByteIndex()], sizeof(uint32_t));
    //return bitwise::toHost(network_32byte_size);
    return network_32byte_size;
}

uint8_t WebSocketFrame::payloadDataByteIndex() const
{
    return maskingKeyByteIndex() + (mask() ? sizeof(uint32_t) : 0);
}

void const * WebSocketFrame::payloadData() const
{
    return &_buffer[payloadDataByteIndex()];
}

Err WebSocketFrame::readData(std::vector<uint8_t> & result)
{
    if (mask() == false) {
        uint8_t const * data = static_cast<uint8_t const *>(payloadData());
        result.assign(data, data + payloadLength());
        return Err::E_SUCCESS;
    }

    uint64_t length = payloadLength();
    uint32_t mask_key = maskingKey();

    uint8_t const * data_ptr = static_cast<uint8_t const *>(payloadData());
    uint8_t const * mask_ptr = reinterpret_cast<uint8_t const *>(&mask_key);

    result.resize(static_cast<std::size_t>(length));
    for (uint64_t i = 0; i < length; ++i) {
        result[i] = (*(data_ptr + i)) ^ mask_ptr[i % sizeof(uint32_t)];
    }
    return Err::E_SUCCESS;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

