/**
 * @file   Signal.hpp
 * @brief  Signal class prototype.
 * @author zer0
 * @date   2017-05-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_SIGNAL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_SIGNAL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/Handle.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

/**
 * Signal class prototype.
 *
 * @author zer0
 * @date   2017-05-02
 *
 * @remarks
 *  Signal handles implement Unix style signal handling on a per-event loop bases. @n
 *  @n
 *  Reception of some signals is emulated on Windows: @n
 *  - SIGINT is normally delivered when the user presses CTRL+C. @n
 *    However, like on Unix, it is not generated when terminal raw mode is enabled. @n
 *  - SIGBREAK is delivered when the user pressed CTRL + BREAK. @n
 *  - SIGHUP is generated when the user closes the console window. @n
 *    On SIGHUP the program is given approximately 10 seconds to perform cleanup. @n
 *    After that Windows will unconditionally terminate it.
 *  - SIGWINCH is raised whenever libuv detects that the console has been resized. @n
 *    SIGWINCH is emulated by libuv when the program uses a uv_tty_t handle to write to the console. @n
 *    SIGWINCH may not always be delivered in a timely manner; @n
 *    libuv will only detect size changes when the cursor is being moved. @n
 *    When a readable uv_tty_t handle is used in raw mode, @n
 *    resizing the console buffer will also trigger a SIGWINCH signal. @n
 *  @n
 *  Watchers for other signals can be successfully created, but these signals are never received. @n
 *  These signals are: SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGTERM and SIGKILL. @n
 *  @n
 *  Calls to raise() or abort() to programmatically raise a signal are not detected by libuv; @n
 *  these will not trigger a signal watcher.
 *
 * @warning
 *  On Linux SIGRT0 and SIGRT1 (signals 32 and 33) are used by the NPTL pthreads library to manage threads. @n
 *  Installing watchers for those signals will lead to unpredictable behavior and is strongly discouraged. @n
 *  Future versions of libuv may simply reject them.
 */
class TBAG_API Signal : public Handle
{
public:
    friend class Loop;

public:
    using Parent = Handle;

protected:
    Signal(Loop & loop);

public:
    virtual ~Signal();

public:
    /** Signal being monitored by this handle. */
    int getSignalNumber() const TBAG_NOEXCEPT;

public:
    /** Initialize the handle. */
    uerr init(Loop & loop);

    /** Start the handle with the given callback, watching for the given signal. */
    uerr start(int signum);

    /** Same functionality as uv_signal_start() but the signal handler is reset the moment the signal is received. */
    uerr startOneshot(int signum);

    /** Stop the handle, the callback will no longer be called. */
    uerr stop();

// Event methods.
public:
    virtual void onSignal(int signum);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_SIGNAL_HPP__

