/**
 * @file   UvEventLoop.hpp
 * @brief  UvEventLoop class prototype.
 * @author zer0
 * @date   2016-05-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTLOOP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTLOOP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Exception.hpp>
#include <libtbag/pattern/Singleton.hpp>
#include <libtbag/lock/RwLock.hpp>

#include <cassert>
#include <cstring>

#include <list>
#include <mutex>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop {

// Forward declaration.
class UvEventHandler;
class UvEventManager;
class UvEventLoop;

/**
 * UvEventHandler class prototype.
 *
 * @author zer0
 * @date   2016-05-18
 */
class UvEventHandler : public Noncopyable
{
public:
    using HandleList = std::list<uv_handle_t*>;

private:
    mutable lock::RwLock _rwlock;
    HandleList _handles;

public:
    UvEventHandler() = default;
    virtual ~UvEventHandler() = default;

public:
    void clear() {
        lock::WriteLockGuard guard(this->_rwlock);
        this->_handles.clear();
    }

    std::size_t getHandleSize() const noexcept {
        lock::ReadLockGuard guard(this->_rwlock);
        return this->_handles.size();
    }

    bool existsHandle(uv_handle_t const * handle) const {
        lock::ReadLockGuard guard(this->_rwlock);
        for (HandleList::const_iterator itr = _handles.cbegin(), end = _handles.cend(); itr != end; ++itr) {
            if ((*itr) == handle) {
                return true;
            }
        }
        return false;
    }

    void addHandle(uv_handle_t * handle) {
        lock::WriteLockGuard guard(this->_rwlock);
        this->_handles.push_back(handle);
    }

    void removeHandle(uv_handle_t const * handle) {
        lock::WriteLockGuard guard(this->_rwlock);
        for (HandleList::const_iterator itr = _handles.cbegin(), end = _handles.cend(); itr != end; ++itr) {
            if ((*itr) == handle) {
                this->_handles.erase(itr);
                return;
            }
        }
    }

public:
    /**
     * @remarks
     *  Type definition for callback passed to uv_read_start() and uv_udp_recv_start(). @n
     *  The user must fill the supplied uv_buf_t structure with whatever size,          @n
     *  as long as it's > 0. A suggested size (65536 at the moment) is provided,        @n
     *  but it doesn't need to be honored. Setting the buffer's length to 0 will        @n
     *  trigger a UV_ENOBUFS error in the uv_udp_recv_cb or uv_read_cb callback.
     */
    virtual void onAlloc(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf) {
        __EMPTY_BLOCK__
    }

    /**
     * Callback called when data was read on a stream.
     *
     * @remarks
     *  nread is > 0 if there is data available or < 0 on error.                @n
     *  When we've reached EOF, nread will be set to UV_EOF. When nread < 0,    @n
     *  the buf parameter might not point to a valid buffer;                    @n
     *  in that case buf.len and buf.base are both set to 0.
     *
     * - NOTE: @n
     *   nread might be 0, which does not indicate an error or EOF. @n
     *   This is equivalent to EAGAIN or EWOULDBLOCK under read(2).
     *
     *  The callee is responsible for stopping closing the stream       @n
     *  when an error happens by calling uv_read_stop() or uv_close().  @n
     *  Trying to read from the stream again is undefined.
     *
     *  The callee is responsible for freeing the buffer, libuv does not reuse it. @n
     *  The buffer may be a null buffer (where buf->base=NULL and buf->len=0) on error.
     */
    virtual void onRead(uv_stream_t * stream, ssize_t nread, uv_buf_t const * buf) {
        __EMPTY_BLOCK__
    }

    /**
     * Callback called after data was written on a stream. @n
     * @c status will be 0 in case of success, < 0 otherwise.
     */
    virtual void onWrite(uv_write_t * req, int status) {
        __EMPTY_BLOCK__
    }

    virtual void onConnect     (uv_connect_t     * req, int status){}
    virtual void onShutdown    (uv_shutdown_t    * req, int status){}
    virtual void onConnection  (uv_stream_t      * server, int status){}
    virtual void onClose       (uv_handle_t      * handle){}
    virtual void onPoll        (uv_poll_t        * handle, int status, int events){}
    virtual void onTimer       (uv_timer_t       * handle){}
    virtual void onAsync       (uv_async_t       * handle){}
    virtual void onPrepare     (uv_prepare_t     * handle){}
    virtual void onCheck       (uv_check_t       * handle){}
    virtual void onIdle        (uv_idle_t        * handle){}
    virtual void onExit        (uv_process_t     * process, int64_t exit_status, int term_signal){}
    virtual void onWalk        (uv_handle_t      * handle, void * arg){}
    virtual void onFs          (uv_fs_t          * req){}
    virtual void onWork        (uv_work_t        * req){}
    virtual void onAfterWork   (uv_work_t        * req, int status){}
    virtual void onGetaddrinfo (uv_getaddrinfo_t * req, int status, struct addrinfo* res){}
    virtual void onGetnameinfo (uv_getnameinfo_t * req, int status, char const * hostname, char const * service){}
};

/**
 * UvEventManager class prototype.
 *
 * @author zer0
 * @date   2016-05-18
 */
class UvEventManager : SINGLETON_INHERITANCE(UvEventManager)
{
public:
    SINGLETON_RESTRICT(UvEventManager);

public:
    friend UvEventLoop;

public:
    using Handler     = UvEventHandler;
    using HandlerList = std::list<Handler*>;

private:
    mutable lock::RwLock _rwlock;
    HandlerList _list;

public:
    Handler * get(uv_handle_t * handle) {
        lock::ReadLockGuard guard(this->_rwlock);
        for (HandlerList::const_iterator itr = _list.cbegin(), end = _list.cend(); itr != end; ++itr) {
            if ((*itr)->existsHandle(handle)) {
                return (*itr);
            }
        }
        return nullptr;
    }

    void addHandler(Handler * handler) {
        lock::WriteLockGuard guard(this->_rwlock);
        this->_list.push_back(handler);
    }

    void removeHandler(Handler * handler) {
        lock::WriteLockGuard guard(this->_rwlock);
        for (HandlerList::const_iterator itr = _list.cbegin(), end = _list.cend(); itr != end; ++itr) {
            if ((*itr) == handler) {
                this->_list.erase(itr);
                return;
            }
        }
    }
};

#ifndef __TBAG_UV_EVNET_IMPLEMENT
#define __TBAG_UV_EVNET_IMPLEMENT
#define __TBAG_UV_EVNET_IMPLEMENT_PARAM0(name, handle_type)                         \
    inline void name(handle_type * h) {                                             \
        using Handler = typename UvEventManager::Handler;                           \
        Handler * c = UvEventManager::getInstance()->get((uv_handle_t*)h);          \
        if (c != nullptr) { c->name(h); }                                           \
    }
#define __TBAG_UV_EVNET_IMPLEMENT_PARAM1(name, handle_type, param1)                 \
    inline void name(handle_type * h, param1 p1) {                                  \
        using Handler = typename UvEventManager::Handler;                           \
        Handler * c = UvEventManager::getInstance()->get((uv_handle_t*)h);          \
        if (c != nullptr) { c->name(h, p1); }                                       \
    }
#define __TBAG_UV_EVNET_IMPLEMENT_PARAM2(name, handle_type, param1, param2)         \
    inline void name(handle_type * h, param1 p1, param2 p2) {                       \
        using Handler = typename UvEventManager::Handler;                           \
        Handler * c = UvEventManager::getInstance()->get((uv_handle_t*)h);          \
        if (c != nullptr) { c->name(h, p1, p2); }                                   \
    }
#define __TBAG_UV_EVNET_IMPLEMENT_PARAM3(name, handle_type, param1, param2, param3) \
    inline void name(handle_type * h, param1 p1, param2 p2, param3 p3) {            \
        using Handler = typename UvEventManager::Handler;                           \
        Handler * c = UvEventManager::getInstance()->get((uv_handle_t*)h);          \
        if (c != nullptr) { c->name(h, p1, p2, p3); }                               \
    }
#endif // __TBAG_UV_EVNET_IMPLEMENT

__TBAG_UV_EVNET_IMPLEMENT_PARAM2(onAlloc       , uv_handle_t      , size_t,  uv_buf_t       *);
__TBAG_UV_EVNET_IMPLEMENT_PARAM2(onRead        , uv_stream_t      , ssize_t, uv_buf_t const *);
__TBAG_UV_EVNET_IMPLEMENT_PARAM1(onWrite       , uv_write_t       , int);
__TBAG_UV_EVNET_IMPLEMENT_PARAM1(onConnect     , uv_connect_t     , int);
__TBAG_UV_EVNET_IMPLEMENT_PARAM1(onShutdown    , uv_shutdown_t    , int);
__TBAG_UV_EVNET_IMPLEMENT_PARAM1(onConnection  , uv_stream_t      , int);
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onClose       , uv_handle_t      );
__TBAG_UV_EVNET_IMPLEMENT_PARAM2(onPoll        , uv_poll_t        , int, int);
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onTimer       , uv_timer_t       );
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onAsync       , uv_async_t       );
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onPrepare     , uv_prepare_t     );
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onCheck       , uv_check_t       );
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onIdle        , uv_idle_t        );
__TBAG_UV_EVNET_IMPLEMENT_PARAM2(onExit        , uv_process_t     , int64_t, int);
__TBAG_UV_EVNET_IMPLEMENT_PARAM1(onWalk        , uv_handle_t      , void *);
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onFs          , uv_fs_t          );
__TBAG_UV_EVNET_IMPLEMENT_PARAM0(onWork        , uv_work_t        );
__TBAG_UV_EVNET_IMPLEMENT_PARAM1(onAfterWork   , uv_work_t        , int);
__TBAG_UV_EVNET_IMPLEMENT_PARAM2(onGetaddrinfo , uv_getaddrinfo_t , int, struct addrinfo *);
__TBAG_UV_EVNET_IMPLEMENT_PARAM3(onGetnameinfo , uv_getnameinfo_t , int, char const *, char const *);

/**
 * UvEventLoop class prototype.
 *
 * @author zer0
 * @date   2016-05-18
 *
 * @remarks
 *  Use the libuv event loop.
 */
class UvEventLoop : public UvEventHandler
{
private:
    std::mutex _runner_locker;
    uv_loop_t  _loop;

public:
    UvEventLoop() throw(InitializeException) {
        memset((void*)&this->_loop, 0x00, sizeof(this->_loop));
        if (init() == false) {
            throw InitializeException();
        }

        UvEventManager * em = UvEventManager::getInstance();
        assert(em != nullptr);
        em->addHandler(this);
    }

    virtual ~UvEventLoop() {
        UvEventManager * em = UvEventManager::getInstance();
        assert(em != nullptr);
        em->removeHandler(this);

        bool is_close = this->close();
        assert(is_close == true);
        REMOVE_UNUSED_VARIABLE(is_close);
    }

private:
    /** Fully close a loop. */
    static void onCloseStep(uv_handle_t * handle, void * UNUSED_PARAM(arg)) {
        if (uv_is_closing(handle) == false) {
            uv_close(handle, nullptr);
        }
    }

private:
    bool init() {
        return (uv_loop_init(&this->_loop) == 0 ? true : false);
    }

    bool close() {
        uv_walk(&this->_loop, &UvEventLoop::onCloseStep, nullptr);
        {
            std::lock_guard<std::mutex> guard(this->_runner_locker);
            uv_run(&this->_loop, UV_RUN_DEFAULT);
        }
        return (uv_loop_close(&this->_loop) == 0 ? true : false);
    }

public:
    uv_loop_t * getLoopPointer() noexcept {
        return &this->_loop;
    }

public:
    bool runDefault() {
        std::lock_guard<std::mutex> guard(this->_runner_locker);
        return (uv_run(&this->_loop, UV_RUN_DEFAULT) == 0 ? true : false);
    }
};

}; // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_UVEVENTLOOP_HPP__

