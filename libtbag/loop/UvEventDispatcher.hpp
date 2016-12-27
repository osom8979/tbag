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
#include <libtbag/uv/Type.hpp>
#include <libtbag/pattern/Singleton2.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/lock/RwLock.hpp>
#include <libtbag/Type.hpp>

#include <unordered_map>
#include <map>

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

#if defined(__PLATFORM_MACOS__) && !defined(NDEBUG)
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
};

namespace __uv_example {
void onAlloc       (/* uv_handle_t      */ void * handle, size_t suggested_size, /* uv_buf_t */ void * buf);
void onRead        (/* uv_stream_t      */ void * stream, ssize_t nread, /* uv_buf_t */ void const * buf);
void onWrite       (/* uv_write_t       */ void * req, int status);
void onConnect     (/* uv_connect_t     */ void * req, int status);
void onShutdown    (/* uv_shutdown_t    */ void * req, int status);
void onConnection  (/* uv_stream_t      */ void * server, int status);
void onClose       (/* uv_handle_t      */ void * handle);
void onPoll        (/* uv_poll_t        */ void * handle, int status, int events);
void onTimer       (/* uv_timer_t       */ void * handle);
void onAsync       (/* uv_async_t       */ void * handle);
void onPrepare     (/* uv_prepare_t     */ void * handle);
void onCheck       (/* uv_check_t       */ void * handle);
void onIdle        (/* uv_idle_t        */ void * handle);
void onExit        (/* uv_process_t     */ void * process, int64_t exit_status, int term_signal);
void onWalk        (/* uv_handle_t      */ void * handle, void * arg);
void onFs          (/* uv_fs_t          */ void * req);
void onWork        (/* uv_work_t        */ void * req);
void onAfterWork   (/* uv_work_t        */ void * req, int status);
void onGetaddrinfo (/* uv_getaddrinfo_t */ void * req, int status, /* struct addrinfo */ void * addr);
void onGetnameinfo (/* uv_getnameinfo_t */ void * req, int status, char const * hostname, char const * service);
} // namespace __uv_example

} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

// @formatter:off
#ifndef TBAG_UV_EVENT
#define TBAG_UV_EVENT

// ----------------------------------------
// WARNING: Don't use into the header file.
// ----------------------------------------

# define TBAG_UV_EVENT_IMPLEMENT_OPEN(manager_name, class_name) \
    namespace __uv_event_detail_##manager_name##class_name { \
    class manager_name : public ::libtbag::loop::UvEventDispatcher<void, class_name> \
    { SINGLETON2_PROTOTYPE(manager_name); public:
# define TBAG_UV_EVENT_IMPLEMENT_CLOSE(manager_name, class_name) \
    }; SINGLETON2_IMPLEMENT(manager_name); \
    } using namespace __uv_event_detail_##manager_name##class_name;

# define TBAG_UV_EVENT_PARAM0(event, name) \
    static void event(void * h) { getInstance()->get(h)->name(h); }
# define TBAG_UV_EVENT_PARAM1(event, name, p1) \
    static void event(void * h, p1 a1) { getInstance()->get(h)->name(h, a1); }
# define TBAG_UV_EVENT_PARAM2(event, name, p1, p2) \
    static void event(void * h, p1 a1, p2 a2) { getInstance()->get(h)->name(h, a1, a2); }
# define TBAG_UV_EVENT_PARAM3(event, name, p1, p2, p3) \
    static void event(void * h, p1 a1, p2 a2, p3 a3) { getInstance()->get(h)->name(h, a1, a2, a3); }

# define TBAG_UV_EVENT_REGISTER(manager_name, handle, receiver) manager_name::getInstance()->add(handle, receiver)
# define TBAG_UV_EVENT_UNREGISTER(manager_name, handle)         manager_name::getInstance()->remove(handle)

# define TBAG_UV_EVENT_ALLOC(name)         TBAG_UV_EVENT_PARAM2(name, name, size_t, void*)
# define TBAG_UV_EVENT_READ(name)          TBAG_UV_EVENT_PARAM2(name, name, ssize_t, void const*)
# define TBAG_UV_EVENT_WRITE(name)         TBAG_UV_EVENT_PARAM1(name, name, int)
# define TBAG_UV_EVENT_CONNECT(name)       TBAG_UV_EVENT_PARAM1(name, name, int)
# define TBAG_UV_EVENT_SHUTDOWN(name)      TBAG_UV_EVENT_PARAM1(name, name, int)
# define TBAG_UV_EVENT_CONNECTION(name)    TBAG_UV_EVENT_PARAM1(name, name, int)
# define TBAG_UV_EVENT_CLOSE(name)         TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_POLL(name)          TBAG_UV_EVENT_PARAM2(name, name, int, int)
# define TBAG_UV_EVENT_TIMER(name)         TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_ASYNC(name)         TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_PREPARE(name)       TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_CHECK(name)         TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_IDLE(name)          TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_EXIT(name)          TBAG_UV_EVENT_PARAM2(name, name, int64_t, int)
# define TBAG_UV_EVENT_WALK(name)          TBAG_UV_EVENT_PARAM1(name, name, void*)
# define TBAG_UV_EVENT_FS(name)            TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_WORK(name)          TBAG_UV_EVENT_PARAM0(name, name)
# define TBAG_UV_EVENT_AFTER_WORK(name)    TBAG_UV_EVENT_PARAM1(name, name, int)
# define TBAG_UV_EVENT_GETADDR_INFO(name)  TBAG_UV_EVENT_PARAM2(name, name, int, void*)
# define TBAG_UV_EVENT_GETNAME_INFO(name)  TBAG_UV_EVENT_PARAM3(name, name, int, char const*, char const*)

# define TBAG_UV_EVENT_CALLBACK_ALLOC(mgr, name)        (uv_alloc_cb)       &mgr::name
# define TBAG_UV_EVENT_CALLBACK_READ(mgr, name)         (uv_read_cb)        &mgr::name
# define TBAG_UV_EVENT_CALLBACK_WRITE(mgr, name)        (uv_write_cb)       &mgr::name
# define TBAG_UV_EVENT_CALLBACK_CONNECT(mgr, name)      (uv_connect_cb)     &mgr::name
# define TBAG_UV_EVENT_CALLBACK_SHUTDOWN(mgr, name)     (uv_shutdown_cb)    &mgr::name
# define TBAG_UV_EVENT_CALLBACK_CONNECTION(mgr, name)   (uv_connection_cb)  &mgr::name
# define TBAG_UV_EVENT_CALLBACK_CLOSE(mgr, name)        (uv_close_cb)       &mgr::name
# define TBAG_UV_EVENT_CALLBACK_POLL(mgr, name)         (uv_poll_cb)        &mgr::name
# define TBAG_UV_EVENT_CALLBACK_TIMER(mgr, name)        (uv_timer_cb)       &mgr::name
# define TBAG_UV_EVENT_CALLBACK_ASYNC(mgr, name)        (uv_async_cb)       &mgr::name
# define TBAG_UV_EVENT_CALLBACK_PREPARE(mgr, name)      (uv_prepare_cb)     &mgr::name
# define TBAG_UV_EVENT_CALLBACK_CHECK(mgr, name)        (uv_check_cb)       &mgr::name
# define TBAG_UV_EVENT_CALLBACK_IDLE(mgr, name)         (uv_idle_cb)        &mgr::name
# define TBAG_UV_EVENT_CALLBACK_EXIT(mgr, name)         (uv_exit_cb)        &mgr::name
# define TBAG_UV_EVENT_CALLBACK_WALK(mgr, name)         (uv_walk_cb)        &mgr::name
# define TBAG_UV_EVENT_CALLBACK_FS(mgr, name)           (uv_fs_cb)          &mgr::name
# define TBAG_UV_EVENT_CALLBACK_WORK(mgr, name)         (uv_work_cb)        &mgr::name
# define TBAG_UV_EVENT_CALLBACK_AFTERWORK(mgr, name)    (uv_after_work_cb)  &mgr::name
# define TBAG_UV_EVENT_CALLBACK_GETADDRINFO(mgr, name)  (uv_getaddrinfo_cb) &mgr::name
# define TBAG_UV_EVENT_CALLBACK_GETNAMEINFO(mgr, name)  (uv_getnameinfo_cb) &mgr::name

# define TBAG_UV_EVENT_DEFAULT_NAME __uv_event_dispatcher_manager__
# define TBAG_UV_EVENT_DEFAULT_IMPLEMENT_OPEN(name)         TBAG_UV_EVENT_IMPLEMENT_OPEN(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_IMPLEMENT_CLOSE(name)        TBAG_UV_EVENT_IMPLEMENT_CLOSE(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_REGISTER(handle, receiver)   TBAG_UV_EVENT_REGISTER(TBAG_UV_EVENT_DEFAULT_NAME, handle, receiver)
# define TBAG_UV_EVENT_DEFAULT_UNREGISTER(handle)           TBAG_UV_EVENT_UNREGISTER(TBAG_UV_EVENT_DEFAULT_NAME, handle)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_ALLOC(name)         TBAG_UV_EVENT_CALLBACK_ALLOC(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_READ(name)          TBAG_UV_EVENT_CALLBACK_READ(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_WRITE(name)         TBAG_UV_EVENT_CALLBACK_WRITE(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_CONNECT(name)       TBAG_UV_EVENT_CALLBACK_CONNECT(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_SHUTDOWN(name)      TBAG_UV_EVENT_CALLBACK_SHUTDOWN(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_CONNECTION(name)    TBAG_UV_EVENT_CALLBACK_CONNECTION(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_CLOSE(name)         TBAG_UV_EVENT_CALLBACK_CLOSE(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_POLL(name)          TBAG_UV_EVENT_CALLBACK_POLL(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_TIMER(name)         TBAG_UV_EVENT_CALLBACK_TIMER(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_ASYNC(name)         TBAG_UV_EVENT_CALLBACK_ASYNC(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_PREPARE(name)       TBAG_UV_EVENT_CALLBACK_PREPARE(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_CHECK(name)         TBAG_UV_EVENT_CALLBACK_CHECK(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_IDLE(name)          TBAG_UV_EVENT_CALLBACK_IDLE(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_EXIT(name)          TBAG_UV_EVENT_CALLBACK_EXIT(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_WALK(name)          TBAG_UV_EVENT_CALLBACK_WALK(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_FS(name)            TBAG_UV_EVENT_CALLBACK_FS(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_WORK(name)          TBAG_UV_EVENT_CALLBACK_WORK(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_AFTERWORK(name)     TBAG_UV_EVENT_CALLBACK_AFTERWORK(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_GETADDRINFO(name)   TBAG_UV_EVENT_CALLBACK_GETADDRINFO(TBAG_UV_EVENT_DEFAULT_NAME, name)
# define TBAG_UV_EVENT_DEFAULT_CALLBACK_GETNAMEINFO(name)   TBAG_UV_EVENT_CALLBACK_GETNAMEINFO(TBAG_UV_EVENT_DEFAULT_NAME, name)
#endif // TBAG_UV_EVENT
// @formatter:on

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTDISPATCHER_HPP__

