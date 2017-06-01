/**
 * @file   RequestQueue.hpp
 * @brief  RequestQueue class prototype.
 * @author zer0
 * @date   2016-12-28
 * @date   2017-02-01 (Move package: libtbag/uv -> libtbag/uvpp)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_REQUESTQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_REQUESTQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <mutex>
#include <memory>
#include <queue>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

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
class TBAG_API RequestQueue : private Noncopyable
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
    ureq _type;

private:
    PrepareQueue _prepare;
    ActiveMap    _active;

private:
    mutable Mutex _mutex;

public:
    RequestQueue(ureq type);
    virtual ~RequestQueue();

public:
    inline bool getPrepareEmpty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_prepare.empty()))
    { Guard g(_mutex); return _prepare.empty(); }
    inline bool getActiveEmpty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_active.empty()))
    { Guard g(_mutex); return _active.empty(); }

    inline std::size_t getPrepareSize() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_prepare.size()))
    { Guard g(_mutex); return _prepare.size(); }
    inline std::size_t getActiveSize() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_active.size()))
    { Guard g(_mutex); return _active.size(); }

public:
    void clear();

public:
    WeakRequest find(RequestKey request) const;
    WeakRequest create(Handle * owner);
    void release(RequestKey request);

public:
    // @formatter:off
    WeakRequest find(Request     * request) const { return find(RequestKey(request)); }
    WeakRequest find(SharedRequest request) const { return find(request.get());       }
    WeakRequest find(WeakRequest   request) const { return find(request.lock());      }

    void release(Request     * request) { release(RequestKey(request)); }
    void release(SharedRequest request) { release(request.get());       }
    void release(WeakRequest   request) { release(request.lock());      }
    // @formatter:on
};

#ifndef _TBAG_UV_REQUEST_QUEUE_EX
#define _TBAG_UV_REQUEST_QUEUE_EX(type, name)       \
    struct name##Queue : public RequestQueue {      \
        name##Queue()                               \
                : RequestQueue(ureq::type) \
        { /* EMPTY. */ }                            \
        ~name##Queue() { /* EMPTY. */ }             \
    }
#endif

_TBAG_UV_REQUEST_QUEUE_EX(WRITE   , Write  );
_TBAG_UV_REQUEST_QUEUE_EX(FS      , Fs     );
_TBAG_UV_REQUEST_QUEUE_EX(WORK    , Work   );
_TBAG_UV_REQUEST_QUEUE_EX(UDP_SEND, UdpSend);

//_TBAG_UV_REQUEST_QUEUE_EX(CONNECT    , Connect    );
//_TBAG_UV_REQUEST_QUEUE_EX(SHUTDOWN   , Shutdown   );
//_TBAG_UV_REQUEST_QUEUE_EX(GETADDRINFO, GetAddrInfo);
//_TBAG_UV_REQUEST_QUEUE_EX(GETNAMEINFO, GetNameInfo);

#undef _TBAG_UV_REQUEST_QUEUE_EX

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_REQUESTQUEUE_HPP__

