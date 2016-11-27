/**
 * @file   UvEventDispatcher.hpp
 * @brief  UvEventDispatcher class prototype.
 * @author zer0
 * @date   2016-11-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTDISPATCHER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTDISPATCHER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/pattern/Singleton2.hpp>

#include <libtbag/container/Pointer.hpp>
#include <libtbag/lock/RwLock.hpp>

#include <unordered_map>
#include <map>

#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

/**
 * UvEventDispatcher class prototype.
 *
 * @author zer0
 * @date   2016-11-27
 */
template <typename HandleType, typename ReceiverType>
class UvEventDispatcher : public Noncopyable
{
public:
    using Handle  = HandleType;
    using Receiver = ReceiverType;

    using Self = UvEventDispatcher<Handle, Receiver>;

    template <typename T>
    using Pointer = libtbag::container::Pointer<T>;

    using Key   = Pointer<Handle>;
    using Value = Pointer<Receiver>;

#if defined(__OS_MACOS__) && !defined(NDEBUG)
    // Only debugging.
    using HandleMap = std::map<Key, Value, typename Key::Less>;
#else
    using HandleMap = std::unordered_map<Key, Value, typename Key::Hash, typename Key::EqualTo>;
#endif

    using RwLock     = libtbag::lock::RwLock;
    using WriteGuard = libtbag::lock::WriteLockGuard;
    using ReadGuard  = libtbag::lock::ReadLockGuard;

private:
    mutable RwLock  _rwlock;
    HandleMap _handlers;

public:
    UvEventDispatcher()
    { /* EMPTY. */ }
    ~UvEventDispatcher()
    { /* EMPTY. */ }

public:
    void add(Handle * handle, Receiver * receiver)
    {
        WriteGuard guard(_rwlock);
        _handlers.insert(std::make_pair(Key(handle), Value(receiver)));
    }

    void remove(Key key)
    {
        WriteGuard guard(_rwlock);
        _handlers.erase(key);
    }

public:
    inline Receiver * get(Handle * handle)
    {
        ReadGuard guard(_rwlock);
        auto find_itr = _handlers.find(Key(handle));
        if (find_itr != _handlers.end()) {
            return find_itr->second.get();
        }
        return nullptr;
    }

public:
    template <typename Manager>
    inline static void add(Handle * handle, Receiver * receiver)
    {
        static_assert(std::is_base_of<Self, Manager>::value, "---");
        Manager::getInstance()->add(handle, receiver);
    }

    template <typename Manager>
    inline static void remove(Handle * handle, Receiver * receiver)
    {
        static_assert(std::is_base_of<Self, Manager>::value, "---");
        Manager::getInstance()->remove(handle, receiver);
    }
};

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

// ----------------------------------------
// WARNING: Don't use into the header file.
// ----------------------------------------

#define TBAG_EVENT_IMPLEMENT_OPEN(manager_name, class_name) \
    class manager_name : public ::libtbag::loop::UvEventDispatcher<void, class_name> \
    { SINGLETON2_PROTOTYPE(manager_name); private:
#define TBAG_EVENT_IMPLEMENT_CLOSE(manager_name) \
    }; SINGLETON2_IMPLEMENT(manager_name);

#define TBAG_EVENT_PARAM0(event, name)  \
    static void event(void * h)         \
    { getInstance()->get(h)->name(h); }
#define TBAG_EVENT_PARAM1(event, name, p1)  \
    static void event(void * h, p1 a1)      \
    { getInstance()->get(h)->name(h, a1); }
#define TBAG_EVENT_PARAM2(event, name, p1, p2)  \
    static void event(void * h, p1 a1, p2 a2)   \
    { getInstance()->get(h)->name(h, a1, a2); }
#define TBAG_EVENT_PARAM3(event, name, p1, p2, p3)   \
    static void event(void * h, p1 a1, p2 a2, p3 a3) \
    { getInstance()->get(h)->name(h, a1, a2, a3); }

#define TBAG_EVENT_ALLOC(name)         TBAG_EVENT_PARAM2(name, name, size_t, void*)
#define TBAG_EVENT_READ(name)          TBAG_EVENT_PARAM2(name, name, ssize_t, void const*)
#define TBAG_EVENT_WRITE(name)         TBAG_EVENT_PARAM1(name, name, int)
#define TBAG_EVENT_CONNECT(name)       TBAG_EVENT_PARAM1(name, name, int)
#define TBAG_EVENT_SHUTDOWN(name)      TBAG_EVENT_PARAM1(name, name, int)
#define TBAG_EVENT_CONNECTION(name)    TBAG_EVENT_PARAM1(name, name, int)
#define TBAG_EVENT_CLOSE(name)         TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_POLL(name)          TBAG_EVENT_PARAM2(name, name, int, int)
#define TBAG_EVENT_TIMER(name)         TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_ASYNC(name)         TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_PREPARE(name)       TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_CHECK(name)         TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_IDLE(name)          TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_EXIT(name)          TBAG_EVENT_PARAM2(name, name, int64_t, int)
#define TBAG_EVENT_WALK(name)          TBAG_EVENT_PARAM1(name, name, void*)
#define TBAG_EVENT_FS(name)            TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_WORK(name)          TBAG_EVENT_PARAM0(name, name)
#define TBAG_EVENT_AFTER_WORK(name)    TBAG_EVENT_PARAM1(name, name, int)
#define TBAG_EVENT_GETADDR_INFO(name)  TBAG_EVENT_PARAM2(name, name, int, void*)
#define TBAG_EVENT_GETNAME_INFO(name)  TBAG_EVENT_PARAM3(name, name, int, char const*, char const*)

#define TBAG_EVENT_CALLBACK_ALLOC(mgr, name)        (uv_alloc_cb)       &mgr::name
#define TBAG_EVENT_CALLBACK_READ(mgr, name)         (uv_read_cb)        &mgr::name
#define TBAG_EVENT_CALLBACK_WRITE(mgr, name)        (uv_write_cb)       &mgr::name
#define TBAG_EVENT_CALLBACK_CONNECT(mgr, name)      (uv_connect_cb)     &mgr::name
#define TBAG_EVENT_CALLBACK_SHUTDOWN(mgr, name)     (uv_shutdown_cb)    &mgr::name
#define TBAG_EVENT_CALLBACK_CONNECTION(mgr, name)   (uv_connection_cb)  &mgr::name
#define TBAG_EVENT_CALLBACK_CLOSE(mgr, name)        (uv_close_cb)       &mgr::name
#define TBAG_EVENT_CALLBACK_POLL(mgr, name)         (uv_poll_cb)        &mgr::name
#define TBAG_EVENT_CALLBACK_TIMER(mgr, name)        (uv_timer_cb)       &mgr::name
#define TBAG_EVENT_CALLBACK_ASYNC(mgr, name)        (uv_async_cb)       &mgr::name
#define TBAG_EVENT_CALLBACK_PREPARE(mgr, name)      (uv_prepare_cb)     &mgr::name
#define TBAG_EVENT_CALLBACK_CHECK(mgr, name)        (uv_check_cb)       &mgr::name
#define TBAG_EVENT_CALLBACK_IDLE(mgr, name)         (uv_idle_cb)        &mgr::name
#define TBAG_EVENT_CALLBACK_EXIT(mgr, name)         (uv_exit_cb)        &mgr::name
#define TBAG_EVENT_CALLBACK_WALK(mgr, name)         (uv_walk_cb)        &mgr::name
#define TBAG_EVENT_CALLBACK_FS(mgr, name)           (uv_fs_cb)          &mgr::name
#define TBAG_EVENT_CALLBACK_WORK(mgr, name)         (uv_work_cb)        &mgr::name
#define TBAG_EVENT_CALLBACK_AFTERWORK(mgr, name)    (uv_after_work_cb)  &mgr::name
#define TBAG_EVENT_CALLBACK_GETADDRINFO(mgr, name)  (uv_getaddrinfo_cb) &mgr::name
#define TBAG_EVENT_CALLBACK_GETNAMEINFO(mgr, name)  (uv_getnameinfo_cb) &mgr::name

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTDISPATCHER_HPP__

