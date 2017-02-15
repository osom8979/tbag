/**
 * @file   TcpClient.hpp
 * @brief  TcpClient class prototype.
 * @author zer0
 * @date   2016-12-29
 * @date   2017-02-15 (Apply BasicTcp class)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/network/BaseTcp.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * TcpClient class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 * @date   2017-02-15 (Apply BasicTcp class)
 */
class TBAG_API TcpClient : public TcpCallback
{
public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    Loop _loop;

private:
    SharedClient _client;
    SharedAsync  _async;

private:
    Mutex _async_mutex;

public:
    TcpClient();
    virtual ~TcpClient();

public:
    inline Loop       & atLoop()       TBAG_NOEXCEPT { return _loop; }
    inline Loop const & atLoop() const TBAG_NOEXCEPT { return _loop; }

    inline WeakClient getWeakClient() { return _client; }
    inline WeakAsync  getWeakAsync () { return _async;  }

public:
    bool initIpv4(std::string const & ip, int port);
    bool initIpv6(std::string const & ip, int port);
    bool init(std::string const & ip, int port);

public:
    bool run();

public:
    bool asyncClose();
    bool asyncWrite(char const * buffer, std::size_t size);

private:
    virtual void onConnect(BaseTcp & tcp, ConnectRequest & request, uerr code) override;

    virtual binf onAlloc(BaseTcp & tcp, std::size_t suggested_size) override;
    virtual void onWrite(BaseTcp & tcp, WriteRequest & request, uerr code) override;
    virtual void onClose(BaseTcp & tcp) override;

    virtual void onReadEof     (BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) override;
    virtual void onReadDatagram(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) override;
    virtual void onReadError   (BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) override;

    virtual void onAsyncWrite(BaseTcp & tcp, uerr code) override;

public:
    virtual void onClientConnect(ConnectRequest & request, uerr code);

    virtual binf onClientAlloc(std::size_t suggested_size);
    virtual void onClientWrite(WriteRequest & request, uerr code);

    virtual void onClientReadEof     (uerr code, char const * buffer, std::size_t size);
    virtual void onClientReadDatagram(uerr code, char const * buffer, std::size_t size);
    virtual void onClientReadError   (uerr code, char const * buffer, std::size_t size);

    virtual void onClientAsyncWrite(uerr code);

    virtual void onClientClose();
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPCLIENT_HPP__

