/**
 * @file   Pipe.hpp
 * @brief  Pipe class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PIPE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PIPE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Stream.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;
struct ConnectRequest;

/**
 * Pipe class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 *
 * @remarks
 *  Pipe handles provide an abstraction over local domain sockets on Unix and named pipes on Windows.
 */
class TBAG_API Pipe : public Stream
{
public:
    friend class Loop;

public:
    using Parent = Stream;

protected:
    Pipe(Loop & loop, bool is_ipc = false);

public:
    virtual ~Pipe();

public:
    /** Initialize a pipe handle. */
    uerr init(Loop & loop, bool is_ipc = false);

    /**
     * Open an existing file descriptor or HANDLE as a pipe.
     *
     * @warning
     *  Changed in version 1.2.1: the file descriptor is set to non-blocking mode.
     */
    uerr open(ufile file);

    /** Bind the pipe to a file path (Unix) or a name (Windows). */
    uerr bind(char const * name);

    /** Connect to the Unix domain socket or the named pipe. */
    void connect(ConnectRequest & request, char const * name);

    /** Get the name of the Unix domain socket or the named pipe. */
    std::string getSockName() const;

    /** Get the name of the Unix domain socket or the named pipe to which the handle is connected. */
    std::string getPeerName() const;

    /** Set the number of pending pipe instance handles when the pipe server is waiting for connections. */
    void pendingInstances(int count);

    /** Obtain the count of pending. */
    int getPendingCount();

    /**
     * Used to receive handles over IPC pipes.
     *
     * @warning
     *  Return value is native(libuv) handle type. (e.g. UV_FILE)
     */
    int getPendingType();

// Event methods.
public:
    virtual void onConnect(ConnectRequest & request, uerr code);
};

// ----------------
// Utility methods.
// ----------------

// @formatter:off
TBAG_API bool initPipeServer(Pipe & pipe, std::string const & path);
TBAG_API bool initPipeClient(Pipe & pipe, ConnectRequest & request, std::string const & path);
// @formatter:on

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PIPE_HPP__

