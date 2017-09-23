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
    using WriteState = details::WriteState;
    using WriteInfo  = details::WriteInfo;

    using Loop   = uvpp::Loop;
    using Stream = uvpp::Stream;
    using binf   = uvpp::binf;

    using SharedClientBackend = std::shared_ptr<Stream>;
    using WeakClientBackend   = std::weak_ptr<Stream>;

    using SafetyAsync       = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using WeakSafetyAsync   = std::weak_ptr<SafetyAsync>;

    using Id = id::Id;

public:
    class Internal;
    friend class Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

public:
    struct WriteReady { /* EMPTY. */ };

private:
    UniqueInternal _internal;

public:
    StreamClient(Loop & loop, StreamType type);
    StreamClient(Loop & loop, StreamType type, WriteReady const & UNUSED_PARAM(ready));
    virtual ~StreamClient();

public:
    WriteState getWriteState() const;
    char const * getWriteStateName() const;

public:
    WeakClientBackend getClient();
    WeakSafetyAsync getAsync();

public:
    virtual Id          id   () const override;
    virtual std::string dest () const override;
    virtual int         port () const override;
    virtual void *      udata() override;

public:
    virtual Err init  (char const * destination, int port = 0) override;
    virtual Err start () override;
    virtual Err stop  () override;
    virtual Err close () override;
    virtual Err cancel() override;
    virtual Err write (char const * buffer, std::size_t size) override;

public:
    virtual void setWriteTimeout(uint64_t millisec) override;
    virtual bool isActiveTimer() override;
    virtual Err  startTimer(uint64_t millisec) override;
    virtual void stopTimer() override;

// Event backend.
protected:
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
    ~TcpClient()
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
    ~PipeClient()
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

