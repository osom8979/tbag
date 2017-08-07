/**
 * @file   WsFrameBuffer.cpp
 * @brief  WsFrameBuffer class implementation.
 * @author zer0
 * @date   2017-08-07
 */

#include <libtbag/network/http/WsFrameBuffer.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WsFrameBuffer::WsFrameBuffer()
{
    // EMPTY.
}

WsFrameBuffer::~WsFrameBuffer()
{
    // EMPTY.
}

std::size_t WsFrameBuffer::readNextDatagramSize()
{
//    // Section 01: Exists next_read_size.
//    if (_next_read_size != NO_NEXT_READ_SIZE) {
//        return _next_read_size;
//    }
//
//    // Section 02: Find the next_read_size in buffer.
//    if (_data_buffer.size() < DATAGRAM_HEADER_SIZE) {
//        return NO_NEXT_READ_SIZE;
//    }
//
//    // Section 03: Update next_read_size.
//    Size network_byte_size = 0;
//    _data_buffer.pop((char*)&network_byte_size, DATAGRAM_HEADER_SIZE);
//    _next_read_size = bitwise::toHost(network_byte_size);
//
//    return _next_read_size;
    return 0;
}

void WsFrameBuffer::alloc(std::size_t suggested_size)
{
    if (_buffer.capacity() < suggested_size) {
        _buffer.extendCapacity(suggested_size - _buffer.capacity());
    }
}

void WsFrameBuffer::push(char const * buffer, std::size_t size)
{
    _buffer.extendPush(buffer, size);
}

bool WsFrameBuffer::next(WsFrame * result)
{
//    Err const & CODE = _frame.execute();
//    std::size_t const READ_SIZE = readNextDatagramSize();
//    if (READ_SIZE == NO_NEXT_READ_SIZE) {
//        return false;
//    }
//
//    if (_buffer.size() < READ_SIZE) {
//        return false;
//    }
//
//    // Realloc with read buffer.
//    if (_read.size() < READ_SIZE) {
//        _read.resize(READ_SIZE);
//    }
//
//    std::size_t real_read_size = _buffer.pop(&_read_buffer[0], READ_SIZE);
//    if (real_read_size != READ_SIZE) {
//        return false;
//    }
//
//    if (result != nullptr) {
//        result->buffer = &_read_buffer[0];
//        result->size   = real_read_size;
//    }
//
//    // Clear next datagram size.
//    _next_read_size = NO_NEXT_READ_SIZE;
    return false;
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

