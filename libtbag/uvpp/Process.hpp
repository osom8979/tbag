/**
 * @file   Process.hpp
 * @brief  Process class prototype.
 * @author zer0
 * @date   2017-02-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PROCESS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PROCESS_HPP__

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
 * Process class prototype.
 *
 * @author zer0
 * @date   2017-02-03
 *
 * @remarks
 *  Process handles will spawn a new process and allow the user to control it @n
 *  and establish communication channels with it using streams.
 */
class TBAG_API Process : public Handle
{
public:
    using Parent = Handle;

public:
    /**
     * Process options prototype.
     *
     * @author zer0
     * @date   2017-02-04
     *
     * @remarks
     *  Options for spawning the process - passed to uv_spawn().
     */
    class TBAG_API Options
    {
    public:
        Options();
        virtual ~Options();

    public:
        Options(Options const & obj);
        Options(Options && obj);

    public:
        Options & operator =(Options const & obj);
        Options & operator =(Options && obj);

    public:
        friend void swap(Options & obj1, Options & obj2);
    };

public:
    Process();
    Process(Loop & loop, Options const & options);
    virtual ~Process();

public:
    /** Initializes the process handle and starts the process. */
    uerr spawn(Loop & loop, Options const & options);

    /** Sends the specified signal to the given process handle. */
    uerr processKill(int signum);

public:
    virtual void onExit(int64_t exit_status, int term_signal);

public:
    /** Disables inheritance for file descriptors / handles that this process inherited from its parent. */
    static void disableStdioInheritance();

    /** Sends the specified signal to the given PID. */
    static uerr kill(int pid, int signum);
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_PROCESS_HPP__

