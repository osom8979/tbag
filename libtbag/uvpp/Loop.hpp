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
#include <libtbag/uvpp/Handle.hpp>

#include <cstdint>

#include <memory>
#include <atomic>
#include <unordered_map>
#include <thread>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

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
    inline ThreadId getOwnerThreadId() const TBAG_NOEXCEPT
    { return _owner_thread_id; }

    inline bool isRunning() const TBAG_NOEXCEPT_SP_OP(_running.load())
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

    /** Update the event loop's concept of "now". */
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

    using key_type        = typename HandleMap::key_type;
    using mapped_type     = typename HandleMap::mapped_type;
    using hasher          = typename HandleMap::hasher;
    using key_equal       = typename HandleMap::key_equal;
    using allocator_type  = typename HandleMap::allocator_type;
    using value_type      = typename HandleMap::value_type;
    using reference       = typename HandleMap::reference;
    using const_reference = typename HandleMap::const_reference;
    using pointer         = typename HandleMap::pointer;
    using const_pointer   = typename HandleMap::const_pointer;
    using size_type       = typename HandleMap::size_type;
    using difference_type = typename HandleMap::difference_type;
    using iterator        = typename HandleMap::iterator;
    using const_iterator  = typename HandleMap::const_iterator;

private:
    HandleMap _handles;

private:
    bool _auto_erase_handle;
    bool _print_internal_handle;
    bool _verbose;

public:
    Loop(bool auto_erase = true, bool print_internal = false, bool verbose = false);
    virtual ~Loop();

public:
    inline bool isAutoEraseHandle() const TBAG_NOEXCEPT
    { return _auto_erase_handle; }

    inline bool isAliveAndThisThread() const
    { return isRunning() && isAlive() && getOwnerThreadId() == std::this_thread::get_id(); }

public:
    /** @return close handle count. */
    std::size_t closeAllHandles();

    /** @return close handle count. */
    std::size_t tryCloseAllHandles();

private:
    /** @warning Don't use this method of user level developers. */
    void runCloseAllHandles();

private:
    WeakHandle findChildHandle(void * native_handle);
    bool eraseChildHandle(void * native_handle);

public:
    WeakHandle insertChildHandle(SharedHandle h);
    WeakHandle findChildHandle(Handle & h);
    bool eraseChildHandle(Handle & h);

// By-pass methods.
public:
    // clang-format off
    inline       iterator    begin()       TBAG_NOEXCEPT_SP_OP(_handles.   begin()) { return _handles.   begin(); }
    inline const_iterator    begin() const TBAG_NOEXCEPT_SP_OP(_handles.   begin()) { return _handles.   begin(); }
    inline       iterator      end()       TBAG_NOEXCEPT_SP_OP(_handles.     end()) { return _handles.     end(); }
    inline const_iterator      end() const TBAG_NOEXCEPT_SP_OP(_handles.     end()) { return _handles.     end(); }
    inline const_iterator   cbegin() const TBAG_NOEXCEPT_SP_OP(_handles.  cbegin()) { return _handles.  cbegin(); }
    inline const_iterator     cend() const TBAG_NOEXCEPT_SP_OP(_handles.    cend()) { return _handles.    cend(); }
    inline           bool    empty() const TBAG_NOEXCEPT_SP_OP(_handles.   empty()) { return _handles.   empty(); }
    inline      size_type     size() const TBAG_NOEXCEPT_SP_OP(_handles.    size()) { return _handles.    size(); }
    inline      size_type max_size() const TBAG_NOEXCEPT_SP_OP(_handles.max_size()) { return _handles.max_size(); }
    // clang-format on

public:
    /** @warning This function should be avoided. */
    void forceClear();

// Event methods.
public:
    virtual void onClosing(Handle * handle) { /* EMPTY. */ }
    virtual void onClosed (Handle * handle) { /* EMPTY. */ }

public:
    /** [INTERNAL] Create(new) & insert handle. */
    template <typename HandleType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<HandleType>::type> newInternalHandle(bool is_internal, Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(Handle, HandleType);
        typedef typename remove_cr<HandleType>::type ResultHandleType;
        typedef std::shared_ptr<ResultHandleType> ResultSharedHandleType;
        SharedHandle shared(new HandleType(std::forward<Args>(args) ...));
        if (static_cast<bool>(shared)) {
            shared->setInternal(is_internal);
            return std::static_pointer_cast<ResultHandleType, Handle>(insertChildHandle(shared).lock());
        }
        return ResultSharedHandleType();
    }

    /** Create(new) & insert handle. */
    template <typename HandleType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<HandleType>::type> newHandle(Args && ... args)
    {
        return newInternalHandle<HandleType, Args ...>(false, std::forward<Args>(args) ...);
    }
};

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_LOOP_HPP__

