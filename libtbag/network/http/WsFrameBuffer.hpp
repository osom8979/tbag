/**
 * @file   WsFrameBuffer.hpp
 * @brief  WsFrameBuffer class prototype.
 * @author zer0
 * @date   2017-08-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSFRAMEBUFFER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSFRAMEBUFFER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/container/CircularBuffer.hpp>
#include <libtbag/network/http/WsFrame.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {

/**
 * WsFrameBuffer class prototype.
 *
 * @author zer0
 * @date   2017-08-07
 */
class TBAG_API WsFrameBuffer : private Noncopyable
{
public:
    using CircularBuffer = container::CircularBuffer<char>;
    using Buffer = std::vector<char>;

private:
    CircularBuffer _buffer;
    Buffer _read;

private:
    WsFrame _frame;

public:
    WsFrameBuffer();
    virtual ~WsFrameBuffer();

public:
    // @formatter:off
    inline CircularBuffer       & atBuffer()       TBAG_NOEXCEPT { return _buffer; }
    inline CircularBuffer const & atBuffer() const TBAG_NOEXCEPT { return _buffer; }
    // @formatter:on

private:
    std::size_t readNextDatagramSize();

// Event by-pass methods.
public:
    void alloc(std::size_t suggested_size);
    void push(char const * buffer, std::size_t size);
    bool next(WsFrame * result);
};

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSFRAMEBUFFER_HPP__

