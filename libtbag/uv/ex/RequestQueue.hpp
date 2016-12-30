/**
 * @file   RequestQueue.hpp
 * @brief  RequestQueue class prototype.
 * @author zer0
 * @date   2016-12-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_REQUESTQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_REQUESTQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/uv/Type.hpp>

#include <mutex>
#include <memory>
#include <queue>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uv {

// Forward declaration.
class Handle;
class Request;

namespace ex {

/**
 * RequestQueue class prototype.
 *
 * @author zer0
 * @date   2016-12-28
 */
class TBAG_API RequestQueue : public Noncopyable
{
public:
    using SharedRequest = std::shared_ptr<Request>;
    using WeakRequest   = std::weak_ptr<Request>;

    using RequestKey    = container::Pointer<Request>;
    using PrepareQueue  = std::queue<SharedRequest>;
    using ActiveMap     = std::unordered_map<RequestKey, SharedRequest, typename RequestKey::Hash, typename RequestKey::EqualTo>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    UvRequestType _type;

private:
    PrepareQueue _prepare;
    ActiveMap    _active;

private:
    mutable Mutex _mutex;

public:
    RequestQueue(UvRequestType type);
    virtual ~RequestQueue();

public:
    WeakRequest find(Request * request) const;

public:
    WeakRequest create(Handle * owner);
    void release(Request * request);
};

#ifndef _TBAG_UV_REQUEST_QUEUE_EX
#define _TBAG_UV_REQUEST_QUEUE_EX(type, name)       \
    struct name##Queue : public RequestQueue {      \
        name##Queue()                               \
                : RequestQueue(UvRequestType::type) \
        { /* EMPTY. */ }                            \
        ~name##Queue() { /* EMPTY. */ }             \
    }
#endif

_TBAG_UV_REQUEST_QUEUE_EX(WRITE, Write);
_TBAG_UV_REQUEST_QUEUE_EX(FS   , Fs   );
_TBAG_UV_REQUEST_QUEUE_EX(WORK , Work );

//_TBAG_UV_REQUEST_QUEUE_EX(UDP_SEND   , UdpSend    );
//_TBAG_UV_REQUEST_QUEUE_EX(CONNECT    , Connect    );
//_TBAG_UV_REQUEST_QUEUE_EX(SHUTDOWN   , Shutdown   );
//_TBAG_UV_REQUEST_QUEUE_EX(GETADDRINFO, GetAddrInfo);
//_TBAG_UV_REQUEST_QUEUE_EX(GETNAMEINFO, GetNameInfo);

#undef _TBAG_UV_REQUEST_QUEUE_EX

} // namespace ex
} // namespace uv

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UV_EX_REQUESTQUEUE_HPP__

