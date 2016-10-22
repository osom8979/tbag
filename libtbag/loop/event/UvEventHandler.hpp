/**
 * @file   UvEventHandler.hpp
 * @brief  UvEventHandler class prototype.
 * @author zer0
 * @date   2016-10-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVEVENTHANDLER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVEVENTHANDLER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>

#include <cstdlib>
#include <cstring>

#include <unordered_set>
#include <set>

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
 *  Type definition for callback passed to uv_read_start() and uv_udp_recv_start(). @n
 *  The user must fill the supplied uv_buf_t structure with whatever size,          @n
 *  as long as it's > 0. A suggested size (65536 at the moment) is provided,        @n
 *  but it doesn't need to be honored. Setting the buffer's length to 0 will        @n
 *  trigger a UV_ENOBUFS error in the uv_udp_recv_cb or uv_read_cb callback.
 */
TBAG_EXPORTS void onAlloc(/* uv_handle_t */ void * handle, size_t suggested_size, /* uv_buf_t */ void * buf);

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
TBAG_EXPORTS void onRead(/* uv_stream_t */ void * stream, ssize_t nread, /* uv_buf_t */ void const * buf);

/**
 * Callback called after data was written on a stream. @n
 * @c status will be 0 in case of success, < 0 otherwise.
 */
TBAG_EXPORTS void onWrite(/* uv_write_t */ void * req, int status);

TBAG_EXPORTS void onConnect     (/* uv_connect_t     */ void * req, int status);
TBAG_EXPORTS void onShutdown    (/* uv_shutdown_t    */ void * req, int status);
TBAG_EXPORTS void onConnection  (/* uv_stream_t      */ void * server, int status);
TBAG_EXPORTS void onClose       (/* uv_handle_t      */ void * handle);
TBAG_EXPORTS void onPoll        (/* uv_poll_t        */ void * handle, int status, int events);
TBAG_EXPORTS void onTimer       (/* uv_timer_t       */ void * handle);
TBAG_EXPORTS void onAsync       (/* uv_async_t       */ void * handle);
TBAG_EXPORTS void onPrepare     (/* uv_prepare_t     */ void * handle);
TBAG_EXPORTS void onCheck       (/* uv_check_t       */ void * handle);
TBAG_EXPORTS void onIdle        (/* uv_idle_t        */ void * handle);
TBAG_EXPORTS void onExit        (/* uv_process_t     */ void * process, int64_t exit_status, int term_signal);
TBAG_EXPORTS void onWalk        (/* uv_handle_t      */ void * handle, void * arg);
TBAG_EXPORTS void onFs          (/* uv_fs_t          */ void * req);
TBAG_EXPORTS void onWork        (/* uv_work_t        */ void * req);
TBAG_EXPORTS void onAfterWork   (/* uv_work_t        */ void * req, int status);
TBAG_EXPORTS void onGetaddrinfo (/* uv_getaddrinfo_t */ void * req, int status, /* struct addrinfo */ void * addr);
TBAG_EXPORTS void onGetnameinfo (/* uv_getnameinfo_t */ void * req, int status, char const * hostname, char const * service);

} // namespace uv

/**
 * UvEventHandler class prototype.
 *
 * @author zer0
 * @date   2016-10-16
 */
struct TBAG_EXPORTS UvEventHandler : public Noncopyable
{
public:
    using UvHandle = libtbag::container::Pointer<void>;

#if defined(__OS_MACOS__) && !defined(NDEBUG)
    using UvHandleSet = std::set<UvHandle>;
#else
    using UvHandleSet = std::unordered_set<UvHandle, UvHandle::Hash>;
#endif

private:
    UvHandleSet _handles;

public:
    UvEventHandler();
    virtual ~UvEventHandler();

public:
    void add(void * handle);
    void remove(void * handle);
    bool exists(void * handle) const;

public:
    // formatter:off
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
    // formatter:on
};

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVEVENTHANDLER_HPP__

