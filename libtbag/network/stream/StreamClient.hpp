/**
 * @file   StreamClient.hpp
 * @brief  StreamClient class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/id/Id.hpp>

#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>

#include <string>
#include <vector>
#include <memory>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamClient class prototype.
 *
 * @author zer0
 * @date   2017-05-10
 */
class TBAG_API StreamClient : public details::ClientInterface
{
public:
    using StreamType = details::StreamType;
    using ReadPacket = details::ReadPacket;

    using Loop   = uvpp::Loop;
    using Stream = uvpp::Stream;
    using binf   = uvpp::binf;

public:
    using SharedClientBackend = std::shared_ptr<Stream>;
    using   WeakClientBackend =   std::weak_ptr<Stream>;

public:
    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync = std::shared_ptr<SafetyAsync>;

public:
    using Id      = id::Id;
    using Buffer  = std::vector<char>;
    using Buffers = std::vector<Buffer>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    enum class WriteStatus
    {
        WS_NOT_READY,
        WS_READY,           ///< Next: call write.
        WS_ASYNC,           ///< Next: onTimeout or onAsync.
        WS_ASYNC_CANCEL,    ///< Next: onAsync.
        WS_WRITE,           ///< Next: onTimeout or onWrite.
        WS_SHUTDOWN,        ///< Next: onShutdown.
        WS_END,
    };

public:
    struct WriteReady { /* EMPTY. */ };

public:
    struct Internal;
    friend struct Internal;

public:
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    StreamType const STREAM_TYPE;

private:
    UniqueInternal _internal;
    mutable Mutex _mutex;

public:
    StreamClient(Loop & loop, StreamType type);
    StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async, WriteReady const & UNUSED_PARAM(ready));
    StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async);
    virtual ~StreamClient();

private:
    void onShutdownTimer();
    void onAsyncWrite();

public:
    static char const * getWriteStatusName(WriteStatus status) TBAG_NOEXCEPT;

public:
    WriteStatus getWriteStatus() const;
    char const * getWriteStatusName() const;

    WeakClientBackend getClient();
    WeakSafetyAsync getAsync();

public:
    virtual Id          id   () const override;
    virtual std::string dest () const override;
    virtual int         port () const override;
    virtual void *      udata() override;

public:
    virtual Err  init  (char const * destination, int port = 0) override;
    virtual Err  start () override;
    virtual Err  stop  () override;
    virtual void close () override;
    virtual void cancel() override;
    virtual Err  write (binf const * buffer, std::size_t size) override;
    virtual Err  write (char const * buffer, std::size_t size) override;

public:
    virtual void setWriteTimeout(uint64_t millisec) override;
    virtual bool isActiveTimer() override;
    virtual Err  startTimer(uint64_t millisec) override;
    virtual void stopTimer() override;

// Event backend.
public:
    virtual void backConnect(Err code) override;
    virtual void backShutdown(Err code) override;
    virtual void backWrite(Err code) override;
    virtual void backRead(Err code, ReadPacket const & packet) override;
    virtual void backClose() override;
};

/**
 * TcpClient class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
struct TcpClient : public stream::StreamClient
{
    TcpClient(Loop & loop) : stream::StreamClient(loop, StreamType::TCP)
    { /* EMPTY. */ }
};

/**
 * PipeClient class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
struct PipeClient : public stream::StreamClient
{
    PipeClient(Loop & loop) : stream::StreamClient(loop, StreamType::PIPE)
    { /* EMPTY. */ }
};

using FunctionalTcpClient  = details::FunctionalClient<TcpClient>;
using FunctionalPipeClient = details::FunctionalClient<PipeClient>;

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__

