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

#include <libtbag/network/http/WsFrame.hpp>
#include <vector>

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
    using Buffer = std::vector<char>;

private:
    Buffer _buffer;
    std::size_t _size;

private:
    OpCode _opcode;
    bool   _finish;
    Buffer _payload;

private:
    struct {
        WsFrame frame;
    } __cache__;

public:
    WsFrameBuffer();
    virtual ~WsFrameBuffer();

public:
    inline WsFrame       & atCachedFrame()       TBAG_NOEXCEPT { return __cache__.frame; }
    inline WsFrame const & atCachedFrame() const TBAG_NOEXCEPT { return __cache__.frame; }

public:
    void clearBuffer();
    void clearCache();
    void clear();

// Event by-pass methods.
public:
    void alloc(std::size_t suggested_size);
    void push(char const * buffer, std::size_t size);
    bool next();
    void update();

public:
    template <typename Predicated>
    std::size_t exec(char const * buffer, std::size_t size, Predicated predicated)
    {
        std::size_t counter = 0;
        bool is_continue = false;

        push(buffer, size);

        while (next()) {
            update();
            ++counter;
            is_continue = predicated(_opcode, _finish, _payload);
            if (is_continue == false) {
                break;
            }
            clearCache();
        }
        return counter;
    }
};

inline bool isWsWriteSuccess(Err code) TBAG_NOEXCEPT
{
    return code == Err::E_SUCCESS || code == Err::E_ENQASYNC || code == Err::E_ASYNCREQ;
}

inline bool isWsWriteFailure(Err code) TBAG_NOEXCEPT
{
    return !isWsWriteSuccess(code);
}

} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_WSFRAMEBUFFER_HPP__

