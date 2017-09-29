/**
 * @file   WsFrameBuffer.cpp
 * @brief  WsFrameBuffer class implementation.
 * @author zer0
 * @date   2017-08-07
 */

#include <libtbag/network/http/WsFrameBuffer.hpp>
#include <libtbag/log/Log.hpp>

#include <cstring>
#include <algorithm>
#include <iterator>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

WsFrameBuffer::WsFrameBuffer() : _buffer(), _size(0), _opcode(OpCode::OC_CONTINUATION_FRAME), _finish(false), _payload()
{
    // EMPTY.
}

WsFrameBuffer::~WsFrameBuffer()
{
    // EMPTY.
}

void WsFrameBuffer::clearBuffer()
{
    _buffer.clear();
    _size = 0;
}

void WsFrameBuffer::clearCache()
{
    __cache__.frame.clear();
}

void WsFrameBuffer::clear()
{
    clearBuffer();
    clearCache();
}

void WsFrameBuffer::alloc(std::size_t suggested_size)
{
    if (_buffer.size() < suggested_size) {
        _buffer.resize(suggested_size);
    }
}

void WsFrameBuffer::push(char const * buffer, std::size_t size)
{
    if (_buffer.size() < _size + size) {
        _buffer.resize(_size + size);
    }
    ::memcpy(&_buffer[_size], buffer, size);
    _size += size;
}

bool WsFrameBuffer::next()
{
    std::size_t read_size = 0;
    Err const CODE = __cache__.frame.execute(&_buffer[0], _size, &read_size);
    if (TBAG_ERR_FAILURE(CODE)) {
        return false;
    }

    std::swap_ranges(_buffer.begin() + read_size, _buffer.begin() + _size, _buffer.begin());
    _size -= read_size;
    return true;
}

void WsFrameBuffer::update()
{
    char const * payload_data = __cache__.frame.getPayloadDataPtr();
    std::size_t payload_size = __cache__.frame.getPayloadSize();

    _finish = __cache__.frame.fin;
    if (__cache__.frame.opcode == OpCode::OC_CONTINUATION_FRAME) {
        _payload.insert(_payload.end(), payload_data, payload_data + payload_size);
    } else {
        _opcode = __cache__.frame.opcode;
        _payload.assign(payload_data, payload_data + payload_size);
    }
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

