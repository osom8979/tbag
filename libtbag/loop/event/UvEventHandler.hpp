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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>

#include <cstdlib>
#include <cstring>

#include <unordered_set>
#include <set>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace loop  {
namespace event {

/**
 * UvEventHandler class prototype.
 *
 * @author zer0
 * @date   2016-10-16
 */
struct UvEventHandler : public Noncopyable
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
    UvEventHandler() = default;
    virtual ~UvEventHandler() = default;

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
    virtual void onGetaddrinfo (void * req, int status, struct addrinfo* res){}
    virtual void onGetnameinfo (void * req, int status, char const * hostname, char const * service){}
    // formatter:on
};

} // namespace event
} // namespace loop

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOOP_EVENT_UVEVENTHANDLER_HPP__

