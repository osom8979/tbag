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
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/uv/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
struct ShutdownRequest;
struct WriteRequest;

/** backlog indicates the number of connections the kernel might queue. */
int const BACKLOG_LIMIT = 128;

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
    /** Contains the amount of queued bytes waiting to be sent. */
    std::size_t getWriteQueueSize() const TBAG_NOEXCEPT;

public:
    /** Returns true if the stream is readable, false otherwise. */
    bool isReadable() const TBAG_NOEXCEPT;

    /** Returns true if the stream is writable, false otherwise. */
    bool isWritable() const TBAG_NOEXCEPT;

public:
    /** Shutdown the outgoing (write) side of a duplex stream. */
    bool shutdown(ShutdownRequest & request);

    /** Start listening for incoming connections. */
    bool listen(int backlog = BACKLOG_LIMIT);

    /** This call is used in conjunction with uv_listen() to accept incoming connections. */
    bool accept(Stream & client);

    /** Read data from an incoming stream. */
    bool startRead();

    /** Stop reading data from the stream. */
    bool stopRead();

    /** Write data to stream. Buffers are written in order. */
    bool write(WriteRequest & request, BufferInfo * infos, std::size_t infos_size);
    bool write(WriteRequest & request, char const * buffer, std::size_t size);

    /**
     * Same as uv_write(), but won’t queue a write request if it can’t be completed immediately.
     *
     * @return
     *  Number of bytes written.
     */
    std::size_t tryWrite(BufferInfo * infos, std::size_t infos_size, Err * result = nullptr);
    std::size_t tryWrite(char const * buffer, std::size_t size, Err * result = nullptr);

public:
    virtual void onShutdown(ShutdownRequest & request, Err code);
    virtual void onConnection(Err code);
    virtual BufferInfo onAlloc(std::size_t suggested_size);
    virtual void onRead(Err code, char const * buffer, std::size_t size);
    virtual void onWrite(WriteRequest & request, Err code);
};

#ifndef _TBAG_UV_STREAM_EX
#define _TBAG_UV_STREAM_EX(type, name)                  \
    struct name##Stream : public Stream {               \
        name##Stream() : Stream(UvHandleType::type) { } \
        ~name##Stream() { }                             \
    }
#endif

_TBAG_UV_STREAM_EX(PIPE, Pipe);
_TBAG_UV_STREAM_EX(TTY , Tty );

#undef _TBAG_UV_STREAM_EX

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_STREAM_HPP__

