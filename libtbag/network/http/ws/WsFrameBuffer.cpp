/**
 * @file   WsFrameBuffer.cpp
 * @brief  WsFrameBuffer class implementation.
 * @author zer0
 * @date   2017-10-01
 */

#include <libtbag/network/http/ws/WsFrameBuffer.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace ws      {

WsFrameBuffer::WsFrameBuffer() : _buffer_size(0), _frames_size(0)
{
    // EMPTY.
}

WsFrameBuffer::WsFrameBuffer(WsFrameBuffer const & obj)
{
    (*this) = obj;
}

WsFrameBuffer::WsFrameBuffer(WsFrameBuffer && obj)
{
    (*this) = std::move(obj);
}

WsFrameBuffer::~WsFrameBuffer()
{
    // EMPTY.
}

WsFrameBuffer & WsFrameBuffer::operator =(WsFrameBuffer const & obj)
{
    if (this != &obj) {
        _buffer      = obj._buffer;
        _buffer_size = obj._buffer_size;
        _frames      = obj._frames;
        _frames_size = obj._frames_size;
    }
    return *this;
}

WsFrameBuffer & WsFrameBuffer::operator =(WsFrameBuffer && obj)
{
    if (this != &obj) {
        swap(obj);
    }
    return *this;
}

void WsFrameBuffer::swap(WsFrameBuffer & obj)
{
    _buffer.swap(obj._buffer);
    std::swap(_buffer_size, obj._buffer_size);
    _frames.swap(obj._frames);
    std::swap(_frames_size, obj._frames_size);
}

void WsFrameBuffer::clear()
{
    _buffer.clear();
    _buffer_size = 0;
    _frames.clear();
    _frames_size = 0;
}

void WsFrameBuffer::clearCache()
{
    __cache__.buffer.clear();
    __cache__.opcode = WsOpCode::WSOC_CONTINUATION_FRAME;
    __cache__.payload.clear();
}

void WsFrameBuffer::push(char const * buffer, std::size_t size)
{
    if (_buffer.size() < _buffer_size + size) {
        _buffer.resize(_buffer_size + size);
    }
    ::memcpy(&_buffer[_buffer_size], buffer, size);
    _buffer_size += size;
}

bool WsFrameBuffer::next(Err * code, std::size_t * size)
{
    WsFrame      & current_buffer = __cache__.buffer;
    WsOpCode     & result_opcode  = __cache__.opcode;
    util::Buffer & result_payload = __cache__.payload;

    std::size_t read_size = 0;
    Err const CODE = current_buffer.execute(&_buffer[0], _buffer_size, &read_size);
    if (isFailure(CODE)) {
        if (code != nullptr) { (*code) = CODE; }
        if (size != nullptr) { (*size) = 0; }
        return false;
    }

    if (read_size == _buffer_size) {
        _buffer_size = 0;
    } else {
        std::swap_ranges(_buffer.begin() + read_size, _buffer.begin() + _buffer_size, _buffer.begin());
        _buffer_size -= read_size;
    }

    if (current_buffer.fin == false) {
        // It is temporarily stored in the buffer until 'Finish' is confirmed.
        if (_frames.size() < _frames_size + 1) {
            _frames.resize(_frames_size + 1);
        }
        _frames[_frames_size].swap(current_buffer);
        ++_frames_size;

        if (code != nullptr) { (*code) = Err::E_CONTINUE; }
        if (size != nullptr) { (*size) = 0; }
        return false;
    }

    result_payload.clear();
    if (_frames_size > 0) {
        result_opcode = _frames[0].opcode;
        for (auto const & f : _frames) {
            result_payload.insert(result_payload.end(), f.payload.begin(), f.payload.end());
        }
    } else {
        result_opcode = current_buffer.opcode;
    }
    result_payload.insert(result_payload.end(), current_buffer.payload.begin(), current_buffer.payload.end());
    _frames_size = 0;

    if (code != nullptr) { (*code) = Err::E_SUCCESS; }
    if (size != nullptr) { (*size) = read_size; }
    return true;
}

} // namespace ws
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

