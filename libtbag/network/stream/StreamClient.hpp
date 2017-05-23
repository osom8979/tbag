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
#include <libtbag/network/Client.hpp>

#include <libtbag/uvpp/ex/SafetyWriteAsync.hpp>
#include <libtbag/uvpp/ex/TimeoutClose.hpp>
#include <libtbag/uvpp/ex/TimeoutShutdown.hpp>

#include <string>
#include <memory>
#include <mutex>
#include <chrono>

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
class StreamClient : public Client
{
public:
    using StreamType = details::StreamType;
    using Loop       = uvpp::Loop;
    using Stream     = uvpp::Stream;
    using binf       = uvpp::binf;

public:
    using SharedClientBackend = std::shared_ptr<Stream>;
    using   WeakClientBackend =   std::weak_ptr<Stream>;

public:
    using SafetyAsync       = uvpp::ex::SafetyAsync;
    using SafetyWriteAsync  = uvpp::ex::SafetyWriteAsync;
    using TimeoutClose      = uvpp::ex::TimeoutClose;
    using TimeoutShutdown   = uvpp::ex::TimeoutShutdown;

    using SharedSafetyWriter = SafetyWriteAsync::SharedWriter;

    using SharedTimeoutClose     = std::shared_ptr<TimeoutClose>;
    using SharedTimeoutShutdown  = std::shared_ptr<TimeoutShutdown>;
    using SharedSafetyWriteAsync = std::shared_ptr<SafetyWriteAsync>;

    using WeakTimeoutClose     = std::weak_ptr<TimeoutClose>;
    using WeakTimeoutShutdown  = std::weak_ptr<TimeoutShutdown>;
    using WeakSafetyWriteAsync = std::weak_ptr<SafetyWriteAsync>;

public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using Seconds      = std::chrono::seconds;
    using Milliseconds = std::chrono::milliseconds;

    using Id = id::Id;

private:
    StreamType const STREAM_TYPE;

private:
    SharedClientBackend    _client;
    SharedSafetyWriteAsync _async;
    SharedTimeoutClose     _close;
    SharedTimeoutShutdown  _shutdown;

public:
    SharedSafetyWriter _last_writer;
    mutable Mutex _mutex;

public:
    StreamClient(Loop & loop, StreamType type);
    virtual ~StreamClient();

public:
    WeakClientBackend    getClient();
    WeakSafetyWriteAsync getAsync();
    WeakTimeoutClose     getClose();
    WeakTimeoutShutdown  getShutdown();

    bool isWriting() const;

private:
    void startTimeoutShutdown(Milliseconds const & millisec);
    void startTimeoutClose(Milliseconds const & millisec);
    void cancelTimeoutShutdown();
    void cancelTimeoutClose();
    bool write(SharedSafetyWriter writer, uint64_t millisec);
    void closeAll();

public:
    /** Obtain the Client handle id. */
    virtual Id getId() const override;

public:
    virtual bool init(std::string const & destination, int port = 0, uint64_t millisec = 0) override;

public:
    virtual bool  start() override;
    virtual bool   stop() override;
    virtual void  close() override;
    virtual void cancel() override;

public:
    virtual bool write(binf const * buffer, std::size_t size, uint64_t millisec = 0) override;
    virtual bool write(char const * buffer, std::size_t size, uint64_t millisec = 0) override;

public:
    virtual void * getUserData() override;

public:
    template <typename Predicated>
    void updateUserData(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_client->getUserData());
    }

public:
    virtual void runBackendConnect(Err code) override;
    virtual void runBackendShutdown(Err code) override;
    virtual void runBackendWrite(Err code) override;
    virtual void runBackendRead(Err code, char const * buffer, std::size_t size) override;
    virtual void runBackendClose() override;
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

using FunctionalTcpClient  = FunctionalClient<TcpClient>;
using FunctionalPipeClient = FunctionalClient<PipeClient>;

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__

