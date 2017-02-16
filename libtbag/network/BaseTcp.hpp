/**
 * @file   BaseTcp.hpp
 * @brief  BaseTcp class prototype.
 * @author zer0
 * @date   2017-02-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_BASETCP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_BASETCP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <libtbag/network/DatagramAdapter.hpp>

#include <vector>
#include <atomic>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/** Client/Server type. */
enum class CsType
{
    SERVER,
    CLIENT,
};

// Forward declaration.
struct TcpCallback;
class  ServerTcp;
class  ClientTcp;

/**
 * BaseTcp class prototype.
 *
 * @author zer0
 * @date   2017-02-15
 */
class TBAG_API BaseTcp : public uvpp::Tcp
{
public:
    using Loop = uvpp::Loop;

private:
    CsType _cstype;
    TcpCallback * _callback;

public:
    BaseTcp(Loop & loop, CsType type, TcpCallback * c) : uvpp::Tcp(loop), _cstype(type), _callback(c)
    { /* EMPTY. */ }
    virtual ~BaseTcp()
    { /* EMPTY. */ }

public:
    inline CsType getCsType() const TBAG_NOEXCEPT
    { return _cstype; }
    inline TcpCallback * getCallback() TBAG_NOEXCEPT
    { return _callback; }
};

/**
 * Tcp class callback.
 *
 * @author zer0
 * @date   2017-02-15
 */
struct TBAG_API TcpCallback
{
    // @formatter:off
    using binf = uvpp::binf;
    using uerr = uvpp::uerr;

    using Loop  = uvpp::Loop;
    using Tcp   = uvpp::Tcp;
    using Async = uvpp::Async;
    using Job   = Async::Job;

    using ConnectRequest  = uvpp::ConnectRequest;
    using ShutdownRequest = uvpp::ShutdownRequest;
    using WriteRequest    = uvpp::WriteRequest;

    using SharedServer = std::shared_ptr<ServerTcp>;
    using SharedClient = std::shared_ptr<ClientTcp>;
    using SharedAsync  = std::shared_ptr<Async>;

    using WeakServer = std::weak_ptr<ServerTcp>;
    using WeakClient = std::weak_ptr<ClientTcp>;
    using WeakAsync  = std::weak_ptr<Async>;

    // Event of Tcp.
    virtual void onConnect(BaseTcp & tcp, ConnectRequest & request, uerr code) { /* EMPTY. */ }

    // Event of Stream.
    virtual void onShutdown  (BaseTcp & tcp, ShutdownRequest & request, uerr code)             { /* EMPTY. */   }
    virtual void onConnection(BaseTcp & tcp, uerr code)                                        { /* EMPTY. */   }
    virtual binf onAlloc     (BaseTcp & tcp, std::size_t suggested_size)                       { return binf(); }
    virtual bool onRead      (BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) { return false;  }
    virtual void onWrite     (BaseTcp & tcp, WriteRequest & request, uerr code)                { /* EMPTY. */   }

    // Event of Handle.
    virtual void onClose(BaseTcp & tcp)             { /* EMPTY. */ }
    virtual void onWalk (BaseTcp & tcp, void * arg) { /* EMPTY. */ }

    // Event of read extension.
    virtual void onReadEof     (BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onReadDatagram(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) { /* EMPTY. */ }
    virtual void onReadError   (BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) { /* EMPTY. */ }

    // Event of write extension.
    virtual void onAsyncWrite(BaseTcp & tcp, uerr code) { /* EMPTY. */ }
    // @formatter:on
};

/**
 * Client tcp class prototype.
 *
 * @author zer0
 * @date   2017-02-15
 */
class TBAG_API ClientTcp : public BaseTcp
{
public:
    using binf = uvpp::binf;
    using uerr = uvpp::uerr;

    using Buffer     = std::vector<char>;
    using AtomicBool = std::atomic_bool;

    using Loop = uvpp::Loop;
    using Tcp  = uvpp::Tcp;

    using ConnectRequest = uvpp::ConnectRequest;
    using WriteRequest   = uvpp::WriteRequest;

    using Datagram = DatagramAdapter;

private:
    ConnectRequest _connect_request;
    AtomicBool     _connect_ready;

private:
    WriteRequest _write_request;
    AtomicBool   _write_ready;

private:
    Buffer    _read_buffer;
    Datagram  _datagram;

public:
    ClientTcp(Loop & loop, TcpCallback * callback);
    virtual ~ClientTcp();

public:
    inline bool isReady() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_write_ready.load()))
    { return _write_ready.load(); }

    inline Buffer       & atReadBuffer()       TBAG_NOEXCEPT { return _read_buffer; }
    inline Buffer const & atReadBuffer() const TBAG_NOEXCEPT { return _read_buffer; }

    inline Datagram       & atDatagram()       TBAG_NOEXCEPT { return _datagram; }
    inline Datagram const & atDatagram() const TBAG_NOEXCEPT { return _datagram; }

public:
    uerr connect(sockaddr const * address);
    uerr write(char const * buffer, std::size_t size);

public:
    bool pushWriteBuffer(char const * buffer, std::size_t size);
    uerr writeWithPushedBuffer();

public:
    virtual void onConnect(ConnectRequest & request, uerr code) override;
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
    virtual void onWrite(WriteRequest & request, uerr code) override;
    virtual void onClose() override;
};

/**
 * Server tcp class prototype.
 *
 * @author zer0
 * @date   2017-02-15
 */
class TBAG_API ServerTcp : public BaseTcp
{
public:
    using binf = uvpp::binf;
    using uerr = uvpp::uerr;

    using Loop = uvpp::Loop;
    using Tcp  = uvpp::Tcp;

public:
    ServerTcp(Loop & loop, TcpCallback * callback);
    virtual ~ServerTcp();

public:
    virtual void onConnection(uerr code) override;
    virtual void onClose() override;
};

/**
 * Write async job class prototype.
 *
 * @author zer0
 * @date   2017-02-15
 */
class TBAG_API WriteJob : public uvpp::Async::Job
{
public:
    using binf    = uvpp::binf;
    using uerr    = uvpp::uerr;
    using Async   = uvpp::Async;
    using WeakTcp = std::weak_ptr<BaseTcp>;

private:
    WeakTcp _tcp;

public:
    WriteJob(WeakTcp tcp);
    virtual ~WriteJob();

public:
    virtual void run(Async * handle) override;
};

/**
 * Close async job class prototype.
 *
 * @author zer0
 * @date   2017-02-15
 */
class TBAG_API CloseJob : public uvpp::Async::Job
{
public:
    using binf    = uvpp::binf;
    using uerr    = uvpp::uerr;
    using Async   = uvpp::Async;
    using WeakTcp = std::weak_ptr<BaseTcp>;

private:
    WeakTcp _tcp;

public:
    CloseJob(WeakTcp tcp);
    virtual ~CloseJob();

public:
    virtual void run(Async * handle) override;
};

/**
 * CloseSelf async job class prototype.
 *
 * @author zer0
 * @date   2017-02-15
 */
class TBAG_API CloseSelfJob : public uvpp::Async::Job
{
public:
    using binf  = uvpp::binf;
    using uerr  = uvpp::uerr;
    using Async = uvpp::Async;

public:
    CloseSelfJob();
    virtual ~CloseSelfJob();

public:
    virtual void run(Async * handle) override;
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_BASETCP_HPP__

