/**
 * @file   Stream.hpp
 * @brief  Stream class prototype.
 * @author zer0
 * @date   2016-12-27
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_STREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_STREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/uvpp/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

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
    Stream(uhandle type);
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
    /** Enable or disable blocking mode for a stream. */
    Err setBlocking(bool enable = true);

public:
    /** Shutdown the outgoing (write) side of a duplex stream. */
    Err shutdown(ShutdownRequest & request);

    /** Start listening for incoming connections. */
    Err listen(int backlog = BACKLOG_LIMIT);

    /** This call is used in conjunction with uv_listen() to accept incoming connections. */
    Err accept(Stream & client);

    /** Read data from an incoming stream. */
    Err startRead();

    /** Stop reading data from the stream. */
    Err stopRead();

    /** Write data to stream. Buffers are written in order. */
    Err write(WriteRequest & request, binf const * infos, std::size_t infos_size);
    Err write(WriteRequest & request, char const * buffer, std::size_t size);

    /**
     * Same as uv_write(), but won’t queue a write request if it can’t be completed immediately.
     *
     * @return
     *  Number of bytes written.
     */
    std::size_t tryWrite(binf * infos, std::size_t infos_size, Err * result = nullptr);
    std::size_t tryWrite(char const * buffer, std::size_t size, Err * result = nullptr);

public:
    virtual void onShutdown(ShutdownRequest & request, Err code);
    virtual void onConnection(Err code);
    virtual binf onAlloc(std::size_t suggested_size);
    virtual void onRead(Err code, char const * buffer, std::size_t size);
    virtual void onWrite(WriteRequest & request, Err code);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_STREAM_HPP__

