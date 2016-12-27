/**
 * @file   Stream.hpp
 * @brief  Stream class prototype.
 * @author zer0
 * @date   2016-12-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_STREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_STREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

/**
 * Stream class prototype.
 *
 * @author zer0
 * @date   2016-12-27
 */
class TBAG_API Stream : public Handle
{
public:
    using Parent = Handle;

public:
    Stream(UvHandleType type);
    virtual ~Stream();

public:
    /** Returns true if the stream is readable, false otherwise. */
    bool isReadable() const TBAG_NOEXCEPT;

    /** Returns true if the stream is writable, false otherwise. */
    bool isWritable() const TBAG_NOEXCEPT;
};

#ifndef _TBAG_UV_STREAM_EX
#define _TBAG_UV_STREAM_EX(type, name)                  \
    struct name##Stream : public Stream {               \
        name##Stream() : Stream(UvHandleType::type) { } \
        ~name##Stream() { }                             \
    }
#endif

_TBAG_UV_STREAM_EX(TCP , Tcp );
_TBAG_UV_STREAM_EX(PIPE, Pipe);
_TBAG_UV_STREAM_EX(TTY , Tty );

#undef _TBAG_UV_STREAM_EX

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_STREAM_HPP__

