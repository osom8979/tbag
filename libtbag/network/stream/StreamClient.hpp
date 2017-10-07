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
#include <libtbag/Noncopyable.hpp>

#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/network/details/ClientProperty.hpp>
#include <libtbag/container/ReuseQueue.hpp>

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
class TBAG_API StreamClient
        : public details::ClientInterface,
          public details::ClientTypes,
          public details::ClientTypes::OnUserTimerCallback,
          public details::ClientTypes::OnShutdownTimerCallback,
          public details::ClientTypes::OnAsyncWriteCallback,
          private Noncopyable
{
public:
    using StreamType  = details::StreamType;
    using ReadPacket  = details::ReadPacket;
    using WriteState  = details::WriteState;
    using ClientTypes = details::ClientTypes;

    using Id = ClientTypes::Id;

public:
    struct Internal;
    friend struct Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    mutable Mutex _mutex;
    UniqueInternal _internal;

private:
    void * _user_data;

public:
    StreamClient(Loop & loop, StreamType type);
    StreamClient(Loop & loop, StreamType type, UpdateReady const & UNUSED_PARAM(ready));
    virtual ~StreamClient();

public:
    void       * getUserData()       TBAG_NOEXCEPT { Guard g(_mutex); return _user_data; }
    void const * getUserData() const TBAG_NOEXCEPT { Guard g(_mutex); return _user_data; }

public:
    StreamType   getStreamType() const TBAG_NOEXCEPT;
    WriteState   getState     () const;
    char const * getStateName () const;

    WeakStream          getClient       ();
    WeakSafetyAsync     getAsync        ();
    SharedUserTimer     getUserTimer    ();
    SharedShutdownTimer getShutdownTimer();

public:
    Err sendJob(SharedJob job);

public:
    /** Create(new) & push & send job. */
    template <typename JobType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<JobType>::type> newSendJob(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(JobInterface, JobType);
        typedef typename remove_cr<JobType>::type ResultJobType;
        typedef std::shared_ptr<ResultJobType> ResultSharedJobType;
        SharedJob shared = SharedJob(new (std::nothrow) JobType(std::forward<Args>(args) ...));
        if (static_cast<bool>(shared)) {
            sendJob(shared);
            return std::static_pointer_cast<ResultJobType, JobInterface>(shared);
        }
        return ResultSharedJobType();
    }

    template <typename ... Args>
    inline std::shared_ptr<FunctionalJob> newSendFunc(Args && ... args)
    {
        return newSendJob<FunctionalJob, Args ...>(std::forward<Args>(args) ...);
    }

private:
    Err initClient(std::string const & destination, int port = 0);
    Err initInternalHandles();

public:
    virtual Id            id() const override;
    virtual std::string dest() const override;
    virtual int         port() const override;

public:
    virtual void * udata(void * data = nullptr) override;

public:
    virtual Err   init(char const * destination, int port = 0) override;
    virtual Err  start() override;
    virtual Err   stop() override;
    virtual Err  close() override;
    virtual Err cancel() override;
    virtual Err  write(char const * buffer, std::size_t size) override;

public:
    virtual void setWriteTimeout(uint64_t millisec) override;
    virtual Err       startTimer(uint64_t millisec) override;
    virtual void       stopTimer() override;
    virtual bool   isActiveTimer() override;

protected:
    virtual void     onUserTimer() override;
    virtual void onShutdownTimer() override;
    virtual void    onAsyncWrite() override;

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

