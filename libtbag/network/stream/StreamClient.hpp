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

#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/network/stream/client/WriteStateClient.hpp>

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
    using WriteStateClient = client::WriteStateClient;

    using StreamType  = WriteStateClient::StreamType;
    using ReadPacket  = WriteStateClient::ReadPacket;
    using UpdateReady = WriteStateClient::UpdateReady;
    using WriteState  = client::WriteState;

    using SharedStream      = WriteStateClient::SharedStream;
    using WeakStream        = WriteStateClient::WeakStream;
    using SharedSafetyAsync = WriteStateClient::SharedSafetyAsync;
    using WeakSafetyAsync   = WriteStateClient::WeakSafetyAsync;
    using SharedUserTimer   = WriteStateClient::SharedUserTimer;

    using Loop = uvpp::Loop;
    using Id   = id::Id;

private:
    WriteStateClient _client;

public:
    StreamClient(Loop & loop, StreamType type);
    StreamClient(Loop & loop, StreamType type, UpdateReady const & UNUSED_PARAM(ready));
    virtual ~StreamClient();

public:
    WriteState getWriteState() const;
    char const * getWriteStateName() const;

public:
    WeakStream getClient();
    WeakSafetyAsync getAsync();

public:
    virtual Id          id   () const override;
    virtual std::string dest () const override;
    virtual int         port () const override;

public:
    virtual void * udata(void * data = nullptr) override;

public:
    virtual Err init  (char const * destination, int port = 0) override;
    virtual Err start () override;
    virtual Err stop  () override;
    virtual Err close () override;
    virtual Err cancel() override;
    virtual Err write (char const * buffer, std::size_t size) override;

public:
    virtual void setWriteTimeout(uint64_t millisec) override;
    virtual Err  startTimer     (uint64_t millisec) override;
    virtual void stopTimer      () override;
    virtual bool isActiveTimer  () override;

// Event backend.
protected:
    virtual void backConnect (Err code) override;
    virtual void backShutdown(Err code) override;
    virtual void backWrite   (Err code) override;
    virtual void backRead    (Err code, ReadPacket const & packet) override;
    virtual void backClose   () override;
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

using FuncTcpClient  = details::FunctionalClient<TcpClient>;
using FuncPipeClient = details::FunctionalClient<PipeClient>;

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMCLIENT_HPP__

