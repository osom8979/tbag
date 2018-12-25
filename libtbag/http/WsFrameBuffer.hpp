/**
 * @file   WsFrameBuffer.hpp
 * @brief  WsFrameBuffer class prototype.
 * @author zer0
 * @date   2017-10-01
 * @date   2018-12-25 (Change namespace: libtbag::network::http::ws -> libtbag::http)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_HTTP_WSFRAMEBUFFER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_HTTP_WSFRAMEBUFFER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/http/WsFrame.hpp>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace http {

/**
 * WsFrameBuffer class prototype.
 *
 * @author zer0
 * @date   2017-08-07
 * @date   2017-10-01 (Change namespace: libtbag::network::http -> libtbag::network::http::ws)
 * @date   2018-12-25 (Change namespace: libtbag::network::http::ws -> libtbag::http)
 */
class TBAG_API WsFrameBuffer
{
public:
    using Frames = std::vector<WsFrame>;

private:
    util::Buffer _buffer;
    std::size_t  _buffer_size;

    Frames      _frames;
    std::size_t _frames_size;

private:
    struct {
        WsFrame      buffer;
        WsOpCode     opcode;
        util::Buffer payload;
    } __cache__;

public:
    WsFrameBuffer();
    WsFrameBuffer(WsFrameBuffer const & obj);
    WsFrameBuffer(WsFrameBuffer && obj);
    virtual ~WsFrameBuffer();

public:
    WsFrameBuffer & operator =(WsFrameBuffer const & obj);
    WsFrameBuffer & operator =(WsFrameBuffer && obj);

public:
    void swap(WsFrameBuffer & obj);

public:
    inline WsOpCode getOpCode() const TBAG_NOEXCEPT { return __cache__.opcode; }

    inline util::Buffer       & atPayload()       TBAG_NOEXCEPT { return __cache__.payload; }
    inline util::Buffer const & atPayload() const TBAG_NOEXCEPT { return __cache__.payload; }

public:
    void clear();
    void clearCache();

public:
    void push(char const * buffer, std::size_t size);

    /**
     * The reason the return value is not an Err is that it is designed for a while() loop.
     *
     * @remarks
     *  Simple example:
     *  @code
     *   WsFrameBuffer wfb;
     *   // ...
     *   void onRead(char const * buffer, std::size_t size)
     *   {
     *       wfb.push(buffer, size);
     *       while (wfb.next()) {
     *           // on read next frame.
     *           auto opcode = wfb.getOpCode();
     *           auto & payload = wfb.atPayload();
     *           // ...
     *       }
     *   }
     *  @endcode
     */
    bool next(Err * code = nullptr, std::size_t * size = nullptr);
};

} // namespace http

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_HTTP_WSFRAMEBUFFER_HPP__

