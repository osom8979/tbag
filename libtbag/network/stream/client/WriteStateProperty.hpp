/**
 * @file   WriteStateProperty.hpp
 * @brief  WriteStateProperty class prototype.
 * @author zer0
 * @date   2017-09-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITESTATEPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITESTATEPROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/id/Id.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/container/ReuseQueue.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Timer.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <memory>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {
namespace client  {

enum class WriteState
{
    WS_NOT_READY = 0,   ///< Next -> READY
    WS_READY,           ///< Next -> ASYNC or WRITE
    WS_ASYNC,           ///< Next -> CLOSING or ASYNC_CANCEL or WRITE
    WS_ASYNC_CANCEL,    ///< Next -> CLOSING or READY
    WS_WRITE,           ///< Next -> CLOSING or SHUTDOWN or READY
    WS_SHUTDOWN,        ///< Next -> CLOSING or READY
    WS_CLOSING,         ///< Next -> CLOSING or END
    WS_END,             ///< END
};

inline char const * getWriteStateName(WriteState state) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (state) {
    case WriteState::WS_NOT_READY:     return "NOT_READY";
    case WriteState::WS_READY:         return "READY";
    case WriteState::WS_ASYNC:         return "ASYNC";
    case WriteState::WS_ASYNC_CANCEL:  return "ASYNC_CANCEL";
    case WriteState::WS_WRITE:         return "WRITE";
    case WriteState::WS_SHUTDOWN:      return "SHUTDOWN";
    case WriteState::WS_CLOSING:       return "CLOSING";
    case WriteState::WS_END:           return "END";
    default: TBAG_INACCESSIBLE_BLOCK_ASSERT();
    }
    // @formatter:on
    return "UNKNOWN";
}

/**
 * Write information structure.
 *
 * @author zer0
 * @date   2017-09-23
 */
struct WriteInfo : private Noncopyable
{
    using ConnectRequest  = uvpp::ConnectRequest;
    using WriteRequest    = uvpp::WriteRequest;
    using ShutdownRequest = uvpp::ShutdownRequest;

    using binf   = uvpp::binf;
    using Buffer = std::vector<char>;

    int  max_fail_count; ///< Maximum continuous failure count.
    int  cur_fail_count; ///< Current continuous failure count.

    uint64_t timeout; ///< Write timeout. (milliseconds)

    WriteState  state;
    Buffer      buffer;

    ConnectRequest    connect_req;
    WriteRequest        write_req;
    ShutdownRequest  shutdown_req;

    WriteInfo() : max_fail_count(details::MAXIMUM_CONTINUOUS_FAILURE_COUNT_OF_WRITE),
                  cur_fail_count(0), timeout(0), state(WriteState::WS_NOT_READY), buffer()
    { /* EMPTY. */ }
    ~WriteInfo()
    { /* EMPTY. */ }

    inline void setNotReady   () TBAG_NOEXCEPT { state = WriteState::WS_NOT_READY;    }
    inline void setReady      () TBAG_NOEXCEPT { state = WriteState::WS_READY;        }
    inline void setAsync      () TBAG_NOEXCEPT { state = WriteState::WS_ASYNC;        }
    inline void setAsyncCancel() TBAG_NOEXCEPT { state = WriteState::WS_ASYNC_CANCEL; }
    inline void setWrite      () TBAG_NOEXCEPT { state = WriteState::WS_WRITE;        }
    inline void setShutdown   () TBAG_NOEXCEPT { state = WriteState::WS_SHUTDOWN;     }
    inline void setClosing    () TBAG_NOEXCEPT { state = WriteState::WS_CLOSING;      }
    inline void setEnd        () TBAG_NOEXCEPT { state = WriteState::WS_END;          }

    inline bool isNotReady   () const TBAG_NOEXCEPT { return state == WriteState::WS_NOT_READY;    }
    inline bool isReady      () const TBAG_NOEXCEPT { return state == WriteState::WS_READY;        }
    inline bool isAsync      () const TBAG_NOEXCEPT { return state == WriteState::WS_ASYNC;        }
    inline bool isAsyncCancel() const TBAG_NOEXCEPT { return state == WriteState::WS_ASYNC_CANCEL; }
    inline bool isWrite      () const TBAG_NOEXCEPT { return state == WriteState::WS_WRITE;        }
    inline bool isShutdown   () const TBAG_NOEXCEPT { return state == WriteState::WS_SHUTDOWN;     }
    inline bool isClosing    () const TBAG_NOEXCEPT { return state == WriteState::WS_CLOSING;      }
    inline bool isEnd        () const TBAG_NOEXCEPT { return state == WriteState::WS_END;          }

    inline WriteState getState() const TBAG_NOEXCEPT { return state; }
    inline char const * getStateName() const TBAG_NOEXCEPT { return getWriteStateName(state); }

    inline bool isReadyToWriteState() TBAG_NOEXCEPT_SP_OP(buffer.empty())
    { return isReady() || (isAsync() && buffer.empty() == false); }
};

struct BasicClientTypes
{
    using ClientInterface = details::ClientInterface;
    using StreamType      = details::StreamType;
    using ReadPacket      = details::ReadPacket;

    using ConnectRequest  = uvpp::ConnectRequest;
    using WriteRequest    = uvpp::WriteRequest;
    using ShutdownRequest = uvpp::ShutdownRequest;

    using Loop   = uvpp::Loop;
    using Timer  = uvpp::Timer;
    using Stream = uvpp::Stream;
    using binf   = uvpp::binf;
    using Id     = id::Id;

    using Buffer = std::vector<char>;
    using Mutex  = std::mutex;
    using Guard  = std::lock_guard<Mutex>;

    using SharedBuffer = std::shared_ptr<Buffer>;
    using ReuseQueue   = container::ReuseQueue<SharedBuffer>;

    using SafetyAsync  = uvpp::ex::SafetyAsync;
    using JobInterface = SafetyAsync::JobInterface;

    using SharedStream = std::shared_ptr<Stream>;
    using WeakStream   = std::weak_ptr<Stream>;

    struct     OnUserTimerCallback { virtual void     onUserTimer() = 0; };
    struct OnShutdownTimerCallback { virtual void onShutdownTimer() = 0; };
    struct    OnAsyncWriteCallback { virtual void    onAsyncWrite() = 0; };

    struct UserTimer final : public Timer
    {
        OnUserTimerCallback * parent;
        UserTimer(Loop & loop, OnUserTimerCallback * p) : uvpp::Timer(loop), parent(p) { /* EMPTY. */ }
        virtual ~UserTimer() { /* EMPTY. */ }
        virtual void onTimer() override { parent->onUserTimer(); }
    };

    struct ShutdownTimer final : public Timer
    {
        OnShutdownTimerCallback * parent;
        ShutdownTimer(Loop & loop, OnShutdownTimerCallback * p) : uvpp::Timer(loop), parent(p) { /* EMPTY. */ }
        virtual ~ShutdownTimer() { /* EMPTY. */ }
        virtual void onTimer() override { parent->onShutdownTimer(); }
    };

    struct AsyncWrite final : public JobInterface
    {
        OnAsyncWriteCallback * parent;
        AsyncWrite(OnAsyncWriteCallback * p) : parent(p) { /* EMPTY. */ }
        virtual ~AsyncWrite() { /* EMPTY. */ }
        virtual void run() override { parent->onAsyncWrite(); }
    };

    using SharedSafetyAsync   = std::shared_ptr<SafetyAsync>;
    using WeakSafetyAsync     = std::weak_ptr<SafetyAsync>;

    using SharedUserTimer     = std::shared_ptr<UserTimer>;
    using WeakUserTimer       = std::weak_ptr<UserTimer>;

    using SharedShutdownTimer = std::shared_ptr<ShutdownTimer>;
    using WeakShutdownTimer   = std::weak_ptr<ShutdownTimer>;

    struct UpdateReady { /* EMPTY. */ };
};

/**
 * BasicClientProperty structure.
 *
 * @author zer0
 * @date   2017-09-27
 */
struct BasicClientProperty : public BasicClientTypes
{
    StreamType stream_type;

    ClientInterface * interface;

    int max_fail_count; ///< Maximum continuous failure count.
    int cur_fail_count; ///< Current continuous failure count.

    uint64_t timeout;   ///< Write timeout. (milliseconds)
    Buffer   buffer;    ///< Write buffer.

    ReuseQueue  queue;
    std::size_t max_queue_size;

    ConnectRequest    connect_req;
    WriteRequest        write_req;
    ShutdownRequest  shutdown_req;

    SharedStream         client_stream;
    SharedSafetyAsync    safety_async;
    SharedUserTimer      user_timer;
    SharedShutdownTimer  shutdown_timer;

    BasicClientProperty() : max_fail_count(details::MAXIMUM_CONTINUOUS_FAILURE_COUNT_OF_WRITE),
                            cur_fail_count(0), timeout(0), max_queue_size(details::MAXIMUM_WRITE_QUEUE_SIZE)
    { /* EMPTY. */ }
    virtual ~BasicClientProperty()
    { /* EMPTY. */ }
};

/**
 * SafetyClientProperty structure.
 *
 * @author zer0
 * @date   2017-09-27
 */
struct SafetyClientProperty : public BasicClientProperty
{
    using ClientGuard = std::lock_guard<SafetyClientProperty>;

    mutable Mutex mutex;

    SafetyClientProperty()
    { /* EMPTY. */ }
    virtual ~SafetyClientProperty()
    { /* EMPTY. */ }

    inline void     lock() const TBAG_NOEXCEPT_SP_OP(    mutex.lock()) {            mutex.lock(); }
    inline void   unlock() const TBAG_NOEXCEPT_SP_OP(  mutex.unlock()) {          mutex.unlock(); }
    inline bool try_lock() const TBAG_NOEXCEPT_SP_OP(mutex.try_lock()) { return mutex.try_lock(); }

    template <typename Predicated>
    void set(Predicated predicated)
    {
        Guard const LOCK(mutex);
        predicated(static_cast<BasicClientProperty&>(*this));
    }

    template <typename ReturnType, typename Predicated>
    ReturnType get(Predicated predicated)
    {
        Guard const LOCK(mutex);
        return predicated(static_cast<BasicClientProperty&>(*this));
    }

    template <typename ReturnType, typename Predicated>
    ReturnType get(Predicated predicated) const
    {
        Guard const LOCK(mutex);
        return predicated(static_cast<BasicClientProperty const &>(*this));
    }
};

} // namespace client
} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_CLIENT_WRITESTATEPROPERTY_HPP__

