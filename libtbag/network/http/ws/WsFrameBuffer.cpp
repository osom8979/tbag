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
#include "WsFrameBuffer.hpp"

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
    std::size_t read_size = 0;
    Err const CODE = __cache__.buffer.execute(&_buffer[0], _buffer_size, &read_size);
    if (TBAG_ERR_FAILURE(CODE)) {
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

    if (__cache__.buffer.fin == false) {
        if (__cache__.buffer.opcode == WsOpCode::WSOC_CONTINUATION_FRAME) {
            if (_frames.size() < _frames_size + 1) {
                _frames.resize(_frames_size + 1);
            }
            _frames[_frames_size].swap(__cache__.buffer);
        }
        ++_frames_size;

        if (code != nullptr) { (*code) = Err::E_CONTINUE; }
        if (size != nullptr) { (*size) = 0; }
        return false;
    }

    __cache__.payload.clear();
    if (_frames_size > 0) {
        __cache__.opcode = _frames[0].opcode;
        for (auto const & f : _frames) {
            __cache__.payload.insert(__cache__.payload.end(), f.payload.begin(), f.payload.end());
        }
    } else {
        __cache__.opcode = __cache__.buffer.opcode;
    }
    __cache__.payload.insert(__cache__.payload.end(),
                             __cache__.buffer.payload.begin(),
                             __cache__.buffer.payload.end());
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

