/**
 * @file   InternalClient.hpp
 * @brief  InternalClient class prototype.
 * @author zer0
 * @date   2017-09-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_INTERNALCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_INTERNALCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/id/Id.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Timer.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

// Forward declaration.
class StreamClient;

/**
 * Internal StreamClient class prototype.
 *
 * @author zer0
 * @date   2017-06-16
 * @date   2017-09-24 (Rename: StreamClient::Internal -> InternalClient)
 *
 * @warning
 *  Do not use a mutex if the function name starts with '_'.
 */
class TBAG_API InternalClient : private Noncopyable
{
public:
    using ClientInterface = details::ClientInterface;
    using StreamType      = details::StreamType;
    using ReadPacket      = details::ReadPacket;
    using WriteState      = details::WriteState;
    using WriteInfo       = details::WriteInfo;

    using Loop   = uvpp::Loop;
    using Timer  = uvpp::Timer;
    using Stream = uvpp::Stream;
    using binf   = uvpp::binf;
    using Id     = id::Id;

    using SafetyAsync  = uvpp::ex::SafetyAsync;
    using JobInterface = SafetyAsync::JobInterface;

    using SharedClientBackend = std::shared_ptr<Stream>;
    using WeakClientBackend   = std::weak_ptr<Stream>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    struct UserTimer final : public Timer
    {
        InternalClient * parent;
        UserTimer(Loop & loop, InternalClient * p) : uvpp::Timer(loop), parent(p) { /* EMPTY. */ }
        virtual ~UserTimer() { /* EMPTY. */ }
        virtual void onTimer() override { parent->onUserTimer(); }
    };

    struct ShutdownTimer final : public Timer
    {
        InternalClient * parent;
        ShutdownTimer(Loop & loop, InternalClient * p) : uvpp::Timer(loop), parent(p) { /* EMPTY. */ }
        virtual ~ShutdownTimer() { /* EMPTY. */ }
        virtual void onTimer() override { parent->onShutdownTimer(); }
    };

    struct AsyncWrite final : public JobInterface
    {
        InternalClient * parent;
        AsyncWrite(InternalClient * p) : parent(p) { /* EMPTY. */ }
        virtual ~AsyncWrite() { /* EMPTY. */ }
        virtual void run() override { parent->onAsyncWrite(); }
    };

    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using WeakSafetyAsync   = std::weak_ptr<SafetyAsync>;

    using SharedUserTimer     = std::shared_ptr<UserTimer>;
    using SharedShutdownTimer = std::shared_ptr<ShutdownTimer>;

    struct WriteReady { /* EMPTY. */ };

private:
    StreamType const STREAM_TYPE;

private:
    ClientInterface * _parent;

private:
    mutable Mutex _mutex;

private:
    SharedClientBackend _client;
    SharedSafetyAsync   _safety_async;
    SharedUserTimer     _user_timer;
    SharedShutdownTimer _shutdown_timer;

private:
    WriteInfo _winfo;

public:
    InternalClient(ClientInterface * parent, Loop & loop, StreamType type);
    InternalClient(ClientInterface * parent, Loop & loop, StreamType type, WriteReady const & UNUSED_PARAM(ready));
    virtual ~InternalClient();

public:
    inline StreamType getStreamType() const TBAG_NOEXCEPT { return STREAM_TYPE; }

public:
    inline void     lock() const TBAG_NOEXCEPT_SP_OP(_mutex.  lock()) { _mutex.lock(); }
    inline void   unlock() const TBAG_NOEXCEPT_SP_OP(_mutex.unlock()) { _mutex.unlock(); }
    inline bool try_lock() const TBAG_NOEXCEPT_SP_OP(_mutex.try_lock()) { return _mutex.try_lock(); }

    inline WriteState   getState    () const TBAG_NOEXCEPT { Guard g(_mutex); return _winfo.getState    (); }
    inline char const * getStateName() const TBAG_NOEXCEPT { Guard g(_mutex); return _winfo.getStateName(); }

    inline WeakClientBackend   getClient        () { Guard g(_mutex); return WeakClientBackend  (        _client); }
    inline WeakSafetyAsync     getAsync         () { Guard g(_mutex); return WeakSafetyAsync    (  _safety_async); }
    inline SharedUserTimer     getUserTimer     () { Guard g(_mutex); return SharedUserTimer    (    _user_timer); }
    inline SharedShutdownTimer getShutdownTimer () { Guard g(_mutex); return SharedShutdownTimer(_shutdown_timer); }

public:
    void * getUserData() TBAG_NOEXCEPT;
    void setWriteTimeout(uint64_t millisec) TBAG_NOEXCEPT;

public:
    Id          getId() const;
    std::string  dest() const;
    int          port() const;

public:
    Err  initClient(StreamType type, std::string const & destination, int port = 0);
    Err  initInternalHandles();

    Err  startRead();
    Err  stopRead();

    bool isActiveUserTimer() const;
    Err  startUserTimer(uint64_t millisec);
    Err  stopUserTimer();

    Err  close();
    Err  cancel();

protected:
    inline WriteInfo       & _atWriteInfo()       TBAG_NOEXCEPT { return _winfo; }
    inline WriteInfo const & _atWriteInfo() const TBAG_NOEXCEPT { return _winfo; }

protected:
    Loop & _getLoop();
    Err    _writeReal(char const * buffer, std::size_t size);
    Err    _autoWrite(char const * buffer, std::size_t size);
    Err    _writeFromOnAsync();
    Err    _shutdownWrite();
    bool   _isActiveShutdownTimer() const;
    Err    _startShutdownTimer(uint64_t millisec);
    Err    _stopShutdownTimer();
    void   _closeClient();
    void   _closeInternal();

protected:
    void onUserTimer();
    void onShutdownTimer();
    void onAsyncWrite();

public:
    virtual void preConnect (Err code);
    virtual void preShutdown(Err code);
    virtual void preWrite   (Err code);
    virtual void preRead    (Err code, ReadPacket const & packet);
    virtual void preClose   ();
};

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_INTERNALCLIENT_HPP__

