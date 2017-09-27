/**
 * @file   ClientProperty.hpp
 * @brief  ClientProperty class prototype.
 * @author zer0
 * @date   2017-09-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_CLIENTPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_CLIENTPROPERTY_HPP__

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
#include <libtbag/uvpp/Loop.hpp>

#include <memory>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

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

struct ClientTypes
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
 * ClientProperty structure.
 *
 * @author zer0
 * @date   2017-09-27
 */
struct ClientProperty : public ClientTypes
{
    ReuseQueue  queue;
    std::size_t max_queue_size;

    ConnectRequest    connect_req;
    WriteRequest        write_req;
    ShutdownRequest  shutdown_req;

    SharedStream         client_stream;
    SharedSafetyAsync    safety_async;
    SharedUserTimer      user_timer;
    SharedShutdownTimer  shutdown_timer;

    int  max_fail_count; ///< Maximum continuous failure count.
    int  cur_fail_count; ///< Current continuous failure count.

    uint64_t  timeout;   ///< Write timeout. (milliseconds)
    Buffer    wbuffer;   ///< Write buffer.

    WriteState  state;

    ClientProperty() : max_queue_size(details::MAXIMUM_WRITE_QUEUE_SIZE),
                       max_fail_count(details::MAXIMUM_CONTINUOUS_FAILURE_COUNT_OF_WRITE),
                       cur_fail_count(0), timeout(0), state(WriteState::WS_NOT_READY)
    { /* EMPTY. */ }
    ~ClientProperty()
    { /* EMPTY. */ }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_CLIENTPROPERTY_HPP__

