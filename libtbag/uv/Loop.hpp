/**
 * @file   Loop.hpp
 * @brief  Loop class prototype.
 * @author zer0
 * @date   2016-12-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_LOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_LOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uv/Native.hpp>

#include <cstdint>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

/**
 * Loop class prototype.
 *
 * @author zer0
 * @date   2016-12-28
 *
 * @remarks
 *  The event loop is the central part of libuv’s functionality.
 *  It takes care of polling for i/o and scheduling callbacks
 *  to be run based on different sources of events.
 */
class TBAG_API Loop : public Native
{
public:
    using Parent = Native;

public:
    enum class RunMode
    {
        RUN_DEFAULT,
        RUN_ONCE,
        RUN_NOWAIT,
    };

private:
    std::thread::id _owner_thread_id;

public:
    Loop();
    virtual ~Loop();

public:
    inline std::thread::id getOwnerThreadId() const TBAG_NOEXCEPT
    { return _owner_thread_id; }

private:
    void runCloseAllHandles();

public:
    /** This function runs the event loop. */
    bool run(RunMode mode = RunMode::RUN_DEFAULT);

    /** Returns true if there are active handles or request in the loop. */
    bool isAlive() const;

    /** Stop the event loop. */
    void stop();

    /** Get the poll timeout. The return value is in milliseconds, or -1 for no timeout. */
    int getPollTimeoutMilliseconds() const;

    /** Return the current timestamp in milliseconds. */
    uint64_t getNowMilliseconds() const;

    /** Returns the current high-resolution real time. This is expressed in nanoseconds. */
    uint64_t getNowNanoseconds() const;

    /** Update the event loop’s concept of "now". */
    void updateTime();

    /** Walk the list of handles. */
    void walk(void * arg = nullptr);

// Event methods.
public:
    virtual void onWalk(void * native_handle, void * arg);

// Debugging.
public:
    /** Prints all handles associated with the given loop to the given stream. */
    void printAllHandles(FILE * stream);

    /** This is the same as uv_print_all_handles() except only active handles are printed. */
    void printActiveHandles(FILE * stream);

public:
    /** Returns the size of the uv_loop_t structure. */
    static std::size_t getNativeSize() TBAG_NOEXCEPT;
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_LOOP_HPP__

