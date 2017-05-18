/**
 * @file   Poll.cpp
 * @brief  Poll class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/uvpp/Poll.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_pool_cb__(uv_poll_t * handle, int status, int events)
{
    Poll * h = static_cast<Poll*>(handle->data);
    if (h == nullptr) {
        tDLogE("__global_uv_pool_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        tDLogE("__global_uv_pool_cb__() handle.data is deleted.");
    } else {
        h->onPoll(getUerr(status), static_cast<Poll::EventType>(events));
    }
}

// --------------------
// Poll implementation.
// --------------------

Poll::Poll(Loop & loop, init_fd fd) : Handle(uhandle::POLL)
{
    if (init(loop, fd.fd) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

Poll::Poll(Loop & loop, init_sock sock) : Handle(uhandle::POLL)
{
    if (initSocket(loop, sock.sock) != Err::E_SUCCESS) {
        throw std::bad_alloc();
    }
}

Poll::~Poll()
{
    // EMPTY.
}

Err Poll::init(Loop & loop, int fd)
{
    int const CODE = ::uv_poll_init(loop.cast<uv_loop_t>(), Parent::cast<uv_poll_t>(), fd);
    return getUerr2("Poll::init()", CODE);
}

Err Poll::initSocket(Loop & loop, usock sock)
{
    // On Unix this is identical to uv_poll_init().
    // On windows it takes a SOCKET handle.
    int const CODE = ::uv_poll_init_socket(loop.cast<uv_loop_t>(), Parent::cast<uv_poll_t>(), sock);
    return getUerr2("Poll::initSocket()", CODE);
}

Err Poll::start(EventType events)
{
    //  events is a bitmask consisting made up of UV_READABLE, UV_WRITABLE and UV_DISCONNECT.
    //  As soon as an event is detected the callback will be called with status set to 0,
    //  and the detected events set on the events field.
    //
    //  The UV_DISCONNECT event is optional in the sense that it may not be reported
    //  and the user is free to ignore it, but it can help optimize the shutdown path because
    //  an extra read or write call might be avoided.
    //
    //  If an error happens while polling, status will be < 0 and corresponds with one of the UV_E* error codes
    //  (see Error handling). The user should not close the socket while the handle is active.
    //  If the user does that anyway, the callback may be called reporting an error status,
    //  but this is not guaranteed.
    //
    //  Note: Calling uv_poll_start() on a handle that is already active is fine.
    //        Doing so will update the events mask that is being watched for.
    //  Note: Though UV_DISCONNECT can be set, it is unsupported on AIX and
    //        as such will not be set on the events field in the callback.
    //
    //  Changed in version 1.9.0: Added the UV_DISCONNECT event.
    int const CODE = ::uv_poll_start(Parent::cast<uv_poll_t>(), static_cast<int>(events), __global_uv_pool_cb__);
    return getUerr2("Poll::start()", CODE);
}

Err Poll::stop()
{
    int const CODE = ::uv_poll_stop(Parent::cast<uv_poll_t>());
    return getUerr2("Poll::stop()", CODE);
}

void Poll::onPoll(Err status, EventType events)
{
    tDLogD("Poll::onPoll({}, {}) called.", getErrorName(status), static_cast<int>(events));
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

