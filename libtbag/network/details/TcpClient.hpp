/**
 * @file   TcpClient.hpp
 * @brief  TcpClient class prototype.
 * @author zer0
 * @date   2017-05-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/Client.hpp>
#include <libtbag/network/details/NetCommon.hpp>

#include <memory>
#include <atomic>
#include <chrono>
#include <mutex>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

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
    virtual void onShutdown(ShutdownRequest & request, uerr code) override;
    virtual void onConnect(ConnectRequest & request, uerr code) override;
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
    SharedClient   _client;
    SharedAsync    _async;
    SharedClose    _close;
    SharedShutdown _shutdown;

private:
    SafetyWriteAsync::SharedWriter _last_writer;
    mutable Mutex _mutex;

public:
    TcpClient(Loop & loop);
    virtual ~TcpClient();

public:
    // @formatter:off
    inline WeakClient   getClient  () { Guard g(_mutex); return WeakClient  (_client);   }
    inline WeakAsync    getAsync   () { Guard g(_mutex); return WeakAsync   (_async);    }
    inline WeakClose    getClose   () { Guard g(_mutex); return WeakClose   (_close);    }
    inline WeakShutdown getShutdown() { Guard g(_mutex); return WeakShutdown(_shutdown); }
    // @formatter:on

    inline bool isWriting() const
    { Guard g(_mutex); return static_cast<bool>(_last_writer) == false; }

private:
    void startTimeoutToShutdown(milliseconds const & millisec);
    void cancelTimeoutToShutdown();
    void startTimeoutToClose(milliseconds const & millisec);
    void cancelTimeoutToClose();

private:
    bool write(SafetyWriteAsync::SharedWriter writer, uint64_t millisec);

public:
    virtual Type getType() const override
    { return Type::TCP; }
    virtual Id getId() const override
    { return _client->id(); }

public:
    virtual bool init(String const & ip, int port = 0, uint64_t millisec = 0) override;

public:
    virtual bool  start() override;
    virtual bool   stop() override;
    virtual bool  close() override;
    virtual bool cancel() override;

public:
    virtual bool write(binf const * buffer, Size size, uint64_t millisec = 0) override;
    virtual bool write(char const * buffer, Size size, uint64_t millisec = 0) override;
};

/**
 * FunctionalTcpClient class prototype.
 *
 * @author zer0
 * @date   2017-05-08
 */
struct FunctionalTcpClient : public TcpClient
{
    // @formatter:off
    using uerr = NetCommon::uerr;
    using Size = NetCommon::Size;

    using OnConnect = std::function<void(uerr)>;
    using OnWrite   = std::function<void(uerr)>;
    using OnRead    = std::function<void(uerr, char const *, Size)>;
    using OnClose   = std::function<void()>;

    OnConnect connect_cb;
    OnWrite   write_cb;
    OnRead    read_cb;
    OnClose   close_cb;

    FunctionalTcpClient(Loop & loop) : TcpClient(loop)
    { /* EMPTY */ }
    virtual ~FunctionalTcpClient()
    { /* EMPTY */ }

    inline void setOnConnect(OnConnect const & cb) { connect_cb = cb; }
    inline void setOnWrite  (OnWrite   const & cb) { write_cb   = cb; }
    inline void setOnRead   (OnRead    const & cb) { read_cb    = cb; }
    inline void setOnClose  (OnClose   const & cb) { close_cb   = cb; }

    virtual void onConnect(uerr code) override
    { if (connect_cb) { connect_cb(code); } }
    virtual void onWrite(uerr code) override
    { if (write_cb) { write_cb(code); } }
    virtual void onRead(uerr code, char const * buffer, Size size) override
    { if (read_cb) { read_cb(code, buffer, size); } }
    virtual void onClose() override
    { if (close_cb) { close_cb(); } }
    // @formatter:on
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_TCPCLIENT_HPP__

