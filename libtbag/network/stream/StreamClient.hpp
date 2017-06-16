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
#include <libtbag/uvpp/func/FunctionalTimer.hpp>

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

    using ConnectRequest  = uvpp::ConnectRequest;
    using ShutdownRequest = uvpp::ShutdownRequest;
    using WriteRequest    = uvpp::WriteRequest;

public:
    using SharedClientBackend = std::shared_ptr<Stream>;
    using   WeakClientBackend =   std::weak_ptr<Stream>;

public:
    using       SafetyAsync = uvpp::ex::SafetyAsync;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using   WeakSafetyAsync = std::shared_ptr<SafetyAsync>;

public:
    using       FuncTimer = uvpp::func::FuncTimer;
    using SharedFuncTimer = std::shared_ptr<FuncTimer>;
    using   WeakFuncTimer = std::shared_ptr<FuncTimer>;

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

private:
    StreamType const STREAM_TYPE;
    bool _owner_async;

private:
    SharedClientBackend _client;
    SharedSafetyAsync   _async;

    SharedFuncTimer _close_timer;
    SharedFuncTimer _shutdown_timer;

private:
    mutable Mutex _mutex;

    struct {
        WriteStatus status;
        ConnectRequest  connect_req;
        WriteRequest    write_req;
        ShutdownRequest shutdown_req;
        Buffers buffers;
    } _writer;

public:
    StreamClient(Loop & loop, StreamType type);
    StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async, WriteReady const & ready);
    StreamClient(Loop & loop, StreamType type, SharedSafetyAsync async);
    virtual ~StreamClient();

public:
    WeakClientBackend getClient() { Guard g(_mutex); return WeakClientBackend(_client); }
    WeakSafetyAsync   getAsync () { Guard g(_mutex); return WeakSafetyAsync(_async); }

private:
    static Err startTimer(uvpp::Timer & timer, uint64_t millisec);
    static Err stopTimer (uvpp::Timer & timer);
    static char const * getWriteStatusName(WriteStatus status) TBAG_NOEXCEPT;

public:
    inline WriteStatus getWriteStatus() const TBAG_NOEXCEPT
    { return _writer.status; }
    inline char const * getWriteStatusName() const TBAG_NOEXCEPT
    { return getWriteStatusName(_writer.status); }

// ===============================
// === PROTECTED SECTION BEGIN ===
// [WARNING] Don't mutex guard in this protected section.
protected:
    bool _initInternalHandles();

    Err _startCloseTimer(uint64_t millisec);
    Err _startShutdownTimer(uint64_t millisec);

    void _stopCloseTimer();
    void _stopShutdownTimer();

    Err _shutdownWrite();
    Err _writeReal(binf const * buffer, std::size_t size);
    void _copyToWriteBuffer(binf const * buffer, std::size_t size);
    std::vector<binf> _getWriteBufferInfo();
    Err _autoWrite(binf const * buffer, std::size_t size, uint64_t millisec = 0);

    void _closeAll();
// === PROTECTED SECTION END ===
// =============================

private:
    void onAsyncWrite();

public:
    virtual Id          id   () const override;
    virtual std::string dest () const override;
    virtual int         port () const override;
    virtual void *      udata() override;

public:
    virtual Err  init  (char const * destination, int port = 0, uint64_t millisec = 0) override;
    virtual Err  start () override;
    virtual Err  stop  () override;
    virtual void close () override;
    virtual void cancel() override;
    virtual Err  write (binf const * buffer, std::size_t size, uint64_t millisec = 0) override;
    virtual Err  write (char const * buffer, std::size_t size, uint64_t millisec = 0) override;

// Event backend.
public:
    virtual void backConnect(Err code) override;
    virtual void backShutdown(Err code) override;
    virtual void backWrite(Err code) override;
    virtual void backRead(Err code, ReadPacket const & packet) override;
    virtual void backClose() override;

public:
    template <typename Predicated>
    void updateUserData(Predicated predicated)
    {
        Guard guard(_mutex);
        predicated(_client->getUserData());
    }
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

