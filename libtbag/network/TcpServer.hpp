/**
 * @file   TcpServer.hpp
 * @brief  TcpServer class prototype.
 * @author zer0
 * @date   2016-12-30
 * @date   2017-02-15 (Apply BasicTcp class)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/network/BaseTcp.hpp>

#include <string>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * TcpServer class prototype.
 *
 * @author zer0
 * @date   2016-12-30
 * @date   2017-02-15 (Apply BasicTcp class)
 */
class TBAG_API TcpServer : public TcpCallback
{
public:
    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    Loop _loop;

private:
    SharedServer _server;
    SharedAsync  _async;

private:
    Mutex _async_mutex;

public:
    TcpServer();
    virtual ~TcpServer();

public:
    inline Loop       & atLoop()       TBAG_NOEXCEPT { return _loop; }
    inline Loop const & atLoop() const TBAG_NOEXCEPT { return _loop; }

    inline WeakServer getWeakServer() { return _server; }
    inline WeakAsync  getWeakAsync () { return _async;  }

protected:
    WeakClient acceptedNewClient();

public:
    bool initIpv4(std::string const & ip, int port);
    bool initIpv6(std::string const & ip, int port);
    bool init(std::string const & ip, int port);

public:
    bool run();
    bool run(std::string const & ip, int port);

public:
    bool asyncClose();
    bool asyncCloseClient(ClientTcp & client);
    bool asyncWriteClient(ClientTcp & client, char const * buffer, std::size_t size);

private:
    virtual void onConnection(BaseTcp & tcp, uerr code) override;

    virtual binf onAlloc(BaseTcp & tcp, std::size_t suggested_size) override;
    virtual void onWrite(BaseTcp & tcp, WriteRequest & request, uerr code) override;
    virtual void onClose(BaseTcp & tcp) override;

    virtual void onReadEof     (BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) override;
    virtual void onReadDatagram(BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) override;
    virtual void onReadError   (BaseTcp & tcp, uerr code, char const * buffer, std::size_t size) override;

    virtual void onAsyncWrite(BaseTcp & tcp, uerr code) override;

public:
    virtual bool onNewConnection(uerr code, WeakClient client);

    virtual binf onClientAlloc(ClientTcp & client, std::size_t suggested_size);
    virtual void onClientWrite(ClientTcp & client, WriteRequest & request, uerr code);

    virtual void onClientReadEof     (ClientTcp & client, uerr code, char const * buffer, std::size_t size);
    virtual void onClientReadDatagram(ClientTcp & client, uerr code, char const * buffer, std::size_t size);
    virtual void onClientReadError   (ClientTcp & client, uerr code, char const * buffer, std::size_t size);

    virtual void onClientAsyncWrite(ClientTcp & client, uerr code);

    virtual void onServerClose(ServerTcp & server);
    virtual void onClientClose(ClientTcp & client);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPSERVER_HPP__

