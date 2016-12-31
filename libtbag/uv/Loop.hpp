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

#include <memory>
#include <atomic>
#include <set>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
struct Handle;

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
    using SharedHandle = std::shared_ptr<Handle>;
    using WeakHandle   = std::weak_ptr<Handle>;
    using HandleSet    = std::set<SharedHandle>;

    using ThreadId     = std::thread::id;
    using AtomicBool   = std::atomic_bool;

    using iterator               = typename HandleSet::iterator;
    using const_iterator         = typename HandleSet::const_iterator;
    using reverse_iterator       = typename HandleSet::reverse_iterator;
    using const_reverse_iterator = typename HandleSet::const_reverse_iterator;

public:
    enum class RunMode
    {
        RUN_DEFAULT,
        RUN_ONCE,
        RUN_NOWAIT,
    };

private:
    ThreadId   _owner_thread_id;
    AtomicBool _running;

private:
    HandleSet  _handles;

public:
    Loop();
    virtual ~Loop();

public:
    inline std::thread::id getOwnerThreadId() const TBAG_NOEXCEPT
    { return _owner_thread_id; }
    inline bool isRunning() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_running.load()))
    { return _running.load(); }

private:
    /** @warning Don't use this method of user level developers. */
    void runCloseAllHandles();

public:
    WeakHandle createChildHandle(UvHandleType type);
    WeakHandle insertChildHandle(Handle * handle);
    void eraseChildHandle(WeakHandle handle);

// By-pass methods.
public:

#ifndef TBAG_UV_LOOP_BY_PASS_METHOD
#define TBAG_UV_LOOP_BY_PASS_METHOD(retval, name, qualifier) \
    inline retval name() qualifier TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_handles.name())) { return _handles.name(); }
#define EMPTY_QUALIFIER
#endif

    TBAG_UV_LOOP_BY_PASS_METHOD(      iterator, begin, EMPTY_QUALIFIER)
    TBAG_UV_LOOP_BY_PASS_METHOD(const_iterator, begin, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(      iterator,   end, EMPTY_QUALIFIER)
    TBAG_UV_LOOP_BY_PASS_METHOD(const_iterator,   end, const)

    TBAG_UV_LOOP_BY_PASS_METHOD(      reverse_iterator, rbegin, EMPTY_QUALIFIER)
    TBAG_UV_LOOP_BY_PASS_METHOD(const_reverse_iterator, rbegin, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(      reverse_iterator,   rend, EMPTY_QUALIFIER)
    TBAG_UV_LOOP_BY_PASS_METHOD(const_reverse_iterator,   rend, const)

    TBAG_UV_LOOP_BY_PASS_METHOD(        const_iterator,  cbegin, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(        const_iterator,    cend, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(const_reverse_iterator, crbegin, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(const_reverse_iterator,   crend, const)

    TBAG_UV_LOOP_BY_PASS_METHOD(bool, empty, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(std::size_t, size, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(std::size_t, max_size, const)

#undef EMPTY_QUALIFIER
#undef TBAG_UV_LOOP_BY_PASS_METHOD

public:
    bool close();

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
    void printAllHandles(FILE * file);

    /** This is the same as uv_print_all_handles() except only active handles are printed. */
    void printActiveHandles(FILE * file);

public:
    /** Returns the size of the uv_loop_t structure. */
    static std::size_t getNativeSize() TBAG_NOEXCEPT;
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_LOOP_HPP__

