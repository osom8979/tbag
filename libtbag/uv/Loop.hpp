/**
 * @file   Loop.hpp
 * @brief  BaseLoop & Loop class prototype.
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
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/Type.hpp>

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
class Handle;

/**
 * BaseLoop class prototype.
 *
 * @author zer0
 * @date   2016-12-28
 *
 * @remarks
 *  The event loop is the central part of libuv’s functionality.
 *  It takes care of polling for i/o and scheduling callbacks
 *  to be run based on different sources of events.
 *
 * @warning
 *  This class is used only in Loop class.
 */
class TBAG_API BaseLoop : public Native
{
public:
    using Parent     = Native;
    using ThreadId   = std::thread::id;
    using AtomicBool = std::atomic_bool;

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

protected: // IMPORTANT!!
    BaseLoop();
    virtual ~BaseLoop();

public:
    inline std::thread::id getOwnerThreadId() const TBAG_NOEXCEPT
    { return _owner_thread_id; }
    inline bool isRunning() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_running.load()))
    { return _running.load(); }

public:
    /** Releases all internal loop resources. */
    bool close(Err * result = nullptr);

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

/**
 * Loop class prototype.
 *
 * @author zer0
 * @date   2016-12-31
 */
class TBAG_API Loop : public BaseLoop
{
public:
    using Parent = BaseLoop;

    using SharedHandle = std::shared_ptr<Handle>;
    using WeakHandle   = std::weak_ptr<Handle>;
    using HandleSet    = std::set<SharedHandle>;

    using iterator               = typename HandleSet::iterator;
    using const_iterator         = typename HandleSet::const_iterator;
    using reverse_iterator       = typename HandleSet::reverse_iterator;
    using const_reverse_iterator = typename HandleSet::const_reverse_iterator;

private:
    HandleSet _handles;

public:
    Loop();
    virtual ~Loop();

public:
    /** @return close handle count. */
    std::size_t closeAllHandles();

private:
    /** @warning Don't use this method of user level developers. */
    void runCloseAllHandles();

public:
    WeakHandle   findChildHandle(Handle     * handle);
    WeakHandle insertChildHandle(SharedHandle handle);
    WeakHandle insertChildHandle(Handle     * handle);
    bool        eraseChildHandle(WeakHandle   handle);
    bool        eraseChildHandle(Handle     * handle);

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

    TBAG_UV_LOOP_BY_PASS_METHOD(       bool,    empty, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(std::size_t,     size, const)
    TBAG_UV_LOOP_BY_PASS_METHOD(std::size_t, max_size, const)

#undef EMPTY_QUALIFIER
#undef TBAG_UV_LOOP_BY_PASS_METHOD

public:
    /** @warning This function should be avoided. */
    void forceClear();

public:
    virtual void onClosing(Handle * handle);
    virtual void onClosed(Handle * handle);

public:
    /** Create(new) & insert handle. */
    template <typename HandleType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<HandleType>::type> newHandle(Args && ... args)
    {
        typedef typename remove_cr<HandleType>::type ResultHandleType;
        HandleType * handle = new HandleType(std::forward<Args>(args) ...);
        auto shared = insertChildHandle(handle).lock();
        return std::static_pointer_cast<ResultHandleType, Handle>(shared);
    }
};

} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_LOOP_HPP__

