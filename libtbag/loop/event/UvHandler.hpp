/**
 * @file   UvHandler.hpp
 * @brief  libuv event handling class prototype.
 * @author zer0
 * @date   2016-10-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVHANDLER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVHANDLER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>

#include <cstdint>
#include <cstdlib>

#include <vector>

#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
typedef intptr_t ssize_t;
# define _SSIZE_T_
# define _SSIZE_T_DEFINED
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop  {
namespace event {

namespace uv {

/**
 * @remarks
 *  void definition for callback passed to uv_read_start() and uv_udp_recv_start(). @n
 *  The user must fill the supplied uv_buf_t structure with whatever size,          @n
 *  as long as it's > 0. A suggested size (65536 at the moment) is provided,        @n
 *  but it doesn't need to be honored. Setting the buffer's length to 0 will        @n
 *  trigger a UV_ENOBUFS error in the uv_udp_recv_cb or uv_read_cb callback.
 */
TBAG_API void onAlloc(/* uv_handle_t */ void * handle, size_t suggested_size, /* uv_buf_t */ void * buf);

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
TBAG_API void onRead(/* uv_stream_t */ void * stream, ssize_t nread, /* uv_buf_t */ void const * buf);

/**
 * Callback called after data was written on a stream. @n
 * @c status will be 0 in case of success, < 0 otherwise.
 */
TBAG_API void onWrite(/* uv_write_t */ void * req, int status);

TBAG_API void onConnect     (/* uv_connect_t     */ void * req, int status);
TBAG_API void onShutdown    (/* uv_shutdown_t    */ void * req, int status);
TBAG_API void onConnection  (/* uv_stream_t      */ void * server, int status);
TBAG_API void onClose       (/* uv_handle_t      */ void * handle);
TBAG_API void onPoll        (/* uv_poll_t        */ void * handle, int status, int events);
TBAG_API void onTimer       (/* uv_timer_t       */ void * handle);
TBAG_API void onAsync       (/* uv_async_t       */ void * handle);
TBAG_API void onPrepare     (/* uv_prepare_t     */ void * handle);
TBAG_API void onCheck       (/* uv_check_t       */ void * handle);
TBAG_API void onIdle        (/* uv_idle_t        */ void * handle);
TBAG_API void onExit        (/* uv_process_t     */ void * process, int64_t exit_status, int term_signal);
TBAG_API void onWalk        (/* uv_handle_t      */ void * handle, void * arg);
TBAG_API void onFs          (/* uv_fs_t          */ void * req);
TBAG_API void onWork        (/* uv_work_t        */ void * req);
TBAG_API void onAfterWork   (/* uv_work_t        */ void * req, int status);
TBAG_API void onGetaddrinfo (/* uv_getaddrinfo_t */ void * req, int status, /* struct addrinfo */ void * addr);
TBAG_API void onGetnameinfo (/* uv_getnameinfo_t */ void * req, int status, char const * hostname, char const * service);

} // namespace uv

/**
 * UvHandler class prototype.
 *
 * @author zer0
 * @date   2016-10-16
 * @date   2016-11-03 (Rename: UvEventHandler -> UvHandler)
 */
struct TBAG_API UvHandler : public libtbag::Noncopyable
{
private:
    using Handle  = libtbag::container::Pointer<void>;
    using Handles = std::vector<Handle>;

public:
    Handles _handles;

public:
    UvHandler();
    UvHandler(void * h);
    ~UvHandler();

public:
    void clear();
    bool add(void * h);
    bool remove(void * h);

public:
    // @formatter:off
    virtual void onAlloc       (void * handle, size_t suggested_size, void * buf){}
    virtual void onRead        (void * stream, ssize_t nread, void const * buf){}
    virtual void onWrite       (void * req, int status){}
    virtual void onConnect     (void * req, int status){}
    virtual void onShutdown    (void * req, int status){}
    virtual void onConnection  (void * server, int status){}
    virtual void onClose       (void * handle){}
    virtual void onPoll        (void * handle, int status, int events){}
    virtual void onTimer       (void * handle){}
    virtual void onAsync       (void * handle){}
    virtual void onPrepare     (void * handle){}
    virtual void onCheck       (void * handle){}
    virtual void onIdle        (void * handle){}
    virtual void onExit        (void * process, int64_t exit_status, int term_signal){}
    virtual void onWalk        (void * handle, void * arg){}
    virtual void onFs          (void * req){}
    virtual void onWork        (void * req){}
    virtual void onAfterWork   (void * req, int status){}
    virtual void onGetaddrinfo (void * req, int status, void * addr){}
    virtual void onGetnameinfo (void * req, int status, char const * hostname, char const * service){}
    // @formatter:on
};

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

// WARNING: Don't use into the header file.
#define TBAG_UV_EVENT_CALLBACK_ALLOC       (uv_alloc_cb)       &libtbag::loop::event::uv::onAlloc
#define TBAG_UV_EVENT_CALLBACK_READ        (uv_read_cb)        &libtbag::loop::event::uv::onRead
#define TBAG_UV_EVENT_CALLBACK_WRITE       (uv_write_cb)       &libtbag::loop::event::uv::onWrite
#define TBAG_UV_EVENT_CALLBACK_CONNECT     (uv_connect_cb)     &libtbag::loop::event::uv::onConnect
#define TBAG_UV_EVENT_CALLBACK_SHUTDOWN    (uv_shutdown_cb)    &libtbag::loop::event::uv::onShutdown
#define TBAG_UV_EVENT_CALLBACK_CONNECTION  (uv_connection_cb)  &libtbag::loop::event::uv::onConnection
#define TBAG_UV_EVENT_CALLBACK_CLOSE       (uv_close_cb)       &libtbag::loop::event::uv::onClose
#define TBAG_UV_EVENT_CALLBACK_POLL        (uv_poll_cb)        &libtbag::loop::event::uv::onPoll
#define TBAG_UV_EVENT_CALLBACK_TIMER       (uv_timer_cb)       &libtbag::loop::event::uv::onTimer
#define TBAG_UV_EVENT_CALLBACK_ASYNC       (uv_async_cb)       &libtbag::loop::event::uv::onAsync
#define TBAG_UV_EVENT_CALLBACK_PREPARE     (uv_prepare_cb)     &libtbag::loop::event::uv::onPrepare
#define TBAG_UV_EVENT_CALLBACK_CHECK       (uv_check_cb)       &libtbag::loop::event::uv::onCheck
#define TBAG_UV_EVENT_CALLBACK_IDLE        (uv_idle_cb)        &libtbag::loop::event::uv::onIdle
#define TBAG_UV_EVENT_CALLBACK_EXIT        (uv_exit_cb)        &libtbag::loop::event::uv::onExit
#define TBAG_UV_EVENT_CALLBACK_WALK        (uv_walk_cb)        &libtbag::loop::event::uv::onWalk
#define TBAG_UV_EVENT_CALLBACK_FS          (uv_fs_cb)          &libtbag::loop::event::uv::onFs
#define TBAG_UV_EVENT_CALLBACK_WORK        (uv_work_cb)        &libtbag::loop::event::uv::onWork
#define TBAG_UV_EVENT_CALLBACK_AFTERWORK   (uv_after_work_cb)  &libtbag::loop::event::uv::onAfterWork
#define TBAG_UV_EVENT_CALLBACK_GETADDRINFO (uv_getaddrinfo_cb) &libtbag::loop::event::uv::onGetaddrinfo
#define TBAG_UV_EVENT_CALLBACK_GETNAMEINFO (uv_getnameinfo_cb) &libtbag::loop::event::uv::onGetnameinfo

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVHANDLER_HPP__

