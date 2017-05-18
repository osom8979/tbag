/**
 * @file   Loop.hpp
 * @brief  BaseLoop & Loop class prototype.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_LOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_LOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/uvpp/Native.hpp>

#include <cstdint>

#include <memory>
#include <atomic>
#include <unordered_map>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

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
    Err close();

    /** This function runs the event loop. */
    Err run(RunMode mode = RunMode::RUN_DEFAULT);

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

    using NativeHandle = container::Pointer<void>;
    using SharedHandle = std::shared_ptr<Handle>;
    using WeakHandle   = std::weak_ptr<Handle>;

    using HandleMap = std::unordered_map<
            NativeHandle,
            SharedHandle,
            typename NativeHandle::Hash,
            typename NativeHandle::EqualTo>;

    using iterator       = typename HandleMap::iterator;
    using const_iterator = typename HandleMap::const_iterator;

private:
    HandleMap _handles;

public:
    Loop();
    virtual ~Loop();

public:
    inline bool isAliveAndThisThread() const
    { return isRunning() && isAlive() && getOwnerThreadId() == std::this_thread::get_id(); }

public:
    /** @return close handle count. */
    std::size_t closeAllHandles();

private:
    /** @warning Don't use this method of user level developers. */
    void runCloseAllHandles();

// @formatter:off
private: WeakHandle findChildHandle(void * native_handle);
public : WeakHandle findChildHandle(Handle & h);
private: bool eraseChildHandle(void * native_handle);
public : bool eraseChildHandle(Handle & h);
public : WeakHandle insertChildHandle(SharedHandle h);
// @formatter:on

// By-pass methods.
public:

#ifndef TBAG_UV_LOOP_BY_PASS_METHOD
#define TBAG_UV_LOOP_BY_PASS_METHOD(retval, name, qualifier) \
    inline retval name() qualifier TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_handles.name())) { return _handles.name(); }
#define TBAG_EMPTY_QUALIFIER
#endif

    TBAG_UV_LOOP_BY_PASS_METHOD(      iterator, begin, TBAG_EMPTY_QUALIFIER);
    TBAG_UV_LOOP_BY_PASS_METHOD(const_iterator, begin, const);
    TBAG_UV_LOOP_BY_PASS_METHOD(      iterator,   end, TBAG_EMPTY_QUALIFIER);
    TBAG_UV_LOOP_BY_PASS_METHOD(const_iterator,   end, const);

    TBAG_UV_LOOP_BY_PASS_METHOD(const_iterator,  cbegin, const);
    TBAG_UV_LOOP_BY_PASS_METHOD(const_iterator,    cend, const);

    TBAG_UV_LOOP_BY_PASS_METHOD(       bool,    empty, const);
    TBAG_UV_LOOP_BY_PASS_METHOD(std::size_t,     size, const);
    TBAG_UV_LOOP_BY_PASS_METHOD(std::size_t, max_size, const);

#undef TBAG_EMPTY_QUALIFIER
#undef TBAG_UV_LOOP_BY_PASS_METHOD

public:
    /** @warning This function should be avoided. */
    void forceClear();

// Event methods.
public:
    virtual void onClosing(Handle * handle);
    virtual void onClosed(Handle * handle);

public:
    /** Create(new) & insert handle. */
    template <typename HandleType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<HandleType>::type> newHandle(Args && ... args)
    {
        typedef typename remove_cr<HandleType>::type ResultHandleType;
        SharedHandle shared(new (std::nothrow) HandleType(std::forward<Args>(args) ...));
        return std::static_pointer_cast<ResultHandleType, Handle>(insertChildHandle(shared).lock());
    }
};

// ----------
// Utilities.
// ----------

TBAG_API void busyWaitForAlive(Loop & loop);

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_LOOP_HPP__

