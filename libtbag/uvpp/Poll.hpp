/**
 * @file   Poll.hpp
 * @brief  Poll class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_POLL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_POLL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>
#include <libtbag/bitwise/BitFlags.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

/**
 * Poll class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 *
 * @remarks
 *  Poll handles are used to watch file descriptors for readability, @n
 *  writability and disconnection similar to the purpose of poll(2). @n
 *  @n
 *  The purpose of poll handles is to enable integrating external libraries @n
 *  that rely on the event loop to signal it about the socket status changes, @n
 *  like c-ares or libssh2. Using uv_poll_t for any other purpose is not recommended; @n
 *  uv_tcp_t, uv_udp_t, etc. provide an implementation that is faster and more scalable @n
 *  than what can be achieved with uv_poll_t, especially on Windows. @n
 *  @n
 *  It is possible that poll handles occasionally signal that a file descriptor is readable @n
 *  or writable even when it isn’t. The user should therefore always be prepared to handle EAGAIN @n
 *  or equivalent when it attempts to read from or write to the fd. @n
 *  @n
 *  It is not okay to have multiple active poll handles for the same socket, @n
 *  this can cause libuv to busyloop or otherwise malfunction. @n
 *  @n
 *  The user should not close a file descriptor while it is being polled by an active poll handle. @n
 *  This can cause the handle to report an error, but it might also start polling another socket. @n
 *  However the fd can be safely closed immediately after a call to uv_poll_stop() or uv_close().
 *
 * @warning
 *  - On windows only sockets can be polled with poll handles.
 *    On Unix any file descriptor that would be accepted by poll(2) can be used. @n
 *  - On AIX, watching for disconnection is not supported.
 */
class TBAG_API Poll : public Handle
{
public:
    friend class Loop;

public:
    using Parent = Handle;
    using EventType = int;

public:
    // @formatter:off
    struct init_fd
    {
        int fd;
        init_fd(int f) : fd(f) { /* EMPTY. */ }
        init_fd & operator = (int f) { fd = f; return *this; }
    };
    struct init_sock
    {
        usock sock;
        init_sock(usock s) : sock(s) { /* EMPTY. */ }
        init_sock & operator = (usock s) { sock = s; return *this; }
    };
    // @formatter:on

public:
    TBAG_CONSTEXPR static EventType const EVENT_READABLE   = 1;
    TBAG_CONSTEXPR static EventType const EVENT_WRITABLE   = 2;
    TBAG_CONSTEXPR static EventType const EVENT_DISCONNECT = 4;
    TBAG_CONSTEXPR static EventType const EVENT_RW = (EVENT_READABLE | EVENT_WRITABLE);

protected:
    Poll();
    explicit Poll(Loop & loop, init_fd fd);
    explicit Poll(Loop & loop, init_sock sock);

public:
    virtual ~Poll();

public:
    /**
     * Initialize the handle using a file descriptor.
     *
     * @warning
     *  Changed in version 1.2.2: the file descriptor is set to non-blocking mode.
     */
    uerr init(Loop & loop, int fd);

    /**
     * Initialize the handle using a socket descriptor.
     *
     * @warning
     *  Changed in version 1.2.2: the socket is set to non-blocking mode.
     */
    uerr initSocket(Loop & loop, usock sock);

    /** Starts polling the file descriptor. */
    uerr start(EventType events = EVENT_RW);

    /** Stop polling the file descriptor, the callback will no longer be called. */
    uerr stop();

public:
    // @formatter:off
    inline static bool isEventReadable(EventType event) TBAG_NOEXCEPT
    { return bitwise::checkFlag(event, EVENT_READABLE); }
    inline static bool isEventWritable(EventType event) TBAG_NOEXCEPT
    { return bitwise::checkFlag(event, EVENT_WRITABLE); }
    inline static bool isEventDisconnect(EventType event) TBAG_NOEXCEPT
    { return bitwise::checkFlag(event, EVENT_DISCONNECT); }
    // @formatter:on

// Event methods.
public:
    virtual void onPoll(uerr status, EventType events);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_POLL_HPP__

