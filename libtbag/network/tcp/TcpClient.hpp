/**
 * @file   TcpClient.hpp
 * @brief  TcpClient class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Client.hpp>

#include <memory>
#include <atomic>
#include <chrono>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace tcp     {

// Forward declaration.
class TcpRealClient;
class TcpClient;

/**
 * TcpRealClient class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpRealClient : public details::NetCommon, public uvpp::Tcp
{
public:
    using AtomicBool = std::atomic_bool;

private:
    TcpClient & _parent;

private:
    ConnectRequest _connect_req;

private:
    Buffer _buffer;

public:
    TcpRealClient(Loop & loop, TcpClient & parent);
    virtual ~TcpRealClient();

public:
    // @formatter:off
    inline ConnectRequest       & atConnectReq()       TBAG_NOEXCEPT { return _connect_req; }
    inline ConnectRequest const & atConnectReq() const TBAG_NOEXCEPT { return _connect_req; }
    // @formatter:on

public:
    bool init(String const & ip, int port);

public:
    virtual void onConnect(ConnectRequest & request, uerr code) override;
    virtual void onShutdown(ShutdownRequest & request, uerr code) override;
    virtual void onWrite(WriteRequest & request, uerr code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
    virtual void onClose() override;
};

/**
 * TcpClient class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API TcpClient : public Client
{
public:
    friend class TcpRealClient;

public:
    using SharedClient = std::shared_ptr<TcpRealClient>;
    using   WeakClient =   std::weak_ptr<TcpRealClient>;

private:
    SharedClient           _client;
    SharedSafetyWriteAsync _async;
    SharedTimeoutClose     _close;
    SharedTimeoutShutdown  _shutdown;

private:
    SafetyWriteAsync::SharedWriter _last_writer;
    mutable Mutex _mutex;

public:
    TcpClient(Loop & loop);
    virtual ~TcpClient();

public:
    inline WeakClient getClient()
    {
        Guard g(_mutex);
        return WeakClient(_client);
    }

    inline WeakSafetyWriteAsync getAsync()
    {
        Guard g(_mutex);
        return WeakSafetyWriteAsync(_async);
    }

    inline WeakTimeoutClose getClose()
    {
        Guard g(_mutex);
        return WeakTimeoutClose(_close);
    }

    inline WeakTimeoutShutdown getShutdown()
    {
        Guard g(_mutex);
        return WeakTimeoutShutdown(_shutdown);
    }

    inline bool isWriting() const
    {
        Guard g(_mutex);
        return static_cast<bool>(_last_writer);
    }

public:
    inline void lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.lock()))
    { _mutex.lock(); }
    inline void unlock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.unlock()))
    { _mutex.unlock(); }
    inline bool try_lock() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_mutex.try_lock()))
    { return _mutex.try_lock(); }

private:
    void startTimeoutShutdown(Milliseconds const & millisec);
    void startTimeoutClose(Milliseconds const & millisec);

    void cancelTimeoutShutdown();
    void cancelTimeoutClose();

private:
    bool write(SafetyWriteAsync::SharedWriter writer, uint64_t millisec);

private:
    void closeAll();

public:
    /** Obtain the TCP Network type. */
    virtual Type getType() const override
    { return Type::TCP; }

    /** Obtain the Tcp(client) handle id. */
    virtual Id getId() const override
    { return _client->id(); }

public:
    virtual bool init(String const & ip, int port = 0, uint64_t millisec = 0) override;

public:
    virtual bool  start() override;
    virtual bool   stop() override;
    virtual void  close() override;
    virtual void cancel() override;

public:
    virtual bool write(binf const * buffer, Size size, uint64_t millisec = 0) override;
    virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) override;
};

/**
 * FunctionalTcpClient typedef.
 *
 * @author zer0
 * @date   2017-05-08
 */
using FunctionalTcpClient = FunctionalClient<TcpClient>;

} // namespace tcp
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCP_TCPCLIENT_HPP__

