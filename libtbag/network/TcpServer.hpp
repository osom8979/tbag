/**
 * @file   TcpServer.hpp
 * @brief  TcpServer class prototype.
 * @author zer0
 * @date   2016-12-30
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

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Tcp.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <libtbag/network/DatagramAdapter.hpp>

#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * TcpServer class prototype.
 *
 * @author zer0
 * @date   2016-12-30
 */
class TBAG_API TcpServer : public Noncopyable
{
public:
    using binf = uvpp::binf;
    using uerr = uvpp::uerr;

    using Loop  = uvpp::Loop;
    using Tcp   = uvpp::Tcp;
    using Async = uvpp::Async;
    using Job   = Async::Job;
    using WriteRequest = uvpp::WriteRequest;

    using Buffer = std::vector<char>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

public:
    /** Server class prototype. */
    struct Server : public Tcp
    {
        friend class TcpServer;

        TcpServer * parent;

        Server(Loop & loop, TcpServer * server);
        virtual ~Server();

        virtual void onConnection(uerr code) override;
        virtual void onClose() override;
    };

    /** Client class prototype. */
    struct Client : public Tcp
    {
        friend class TcpServer;

        TcpServer * parent;

        std::atomic_bool write_ready;
        WriteRequest write_req;

        Buffer buffer;
        DatagramRead datagram;

        Client(Loop & loop, TcpServer * server);
        virtual ~Client();

        inline bool isReady() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(write_ready.load()))
        { return write_ready.load(); }

        uerr write(char const * buffer, std::size_t size);

        virtual binf onAlloc(std::size_t suggested_size) override;
        virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
        virtual void onWrite(WriteRequest & request, uerr code) override;
        virtual void onClose() override;
    };

public:
    using SharedServer = std::shared_ptr<Server>;
    using SharedClient = std::shared_ptr<Client>;
    using SharedAsync  = std::shared_ptr<Async>;

    using WeakServer = std::weak_ptr<Server>;
    using WeakClient = std::weak_ptr<Client>;
    using WeakAsync  = std::weak_ptr<Async>;

public:
    struct WriteJob : public Job
    {
        WeakClient client;
        Buffer buffer;
        Mutex  mutex;
        uerr   result;

        WriteJob(WeakClient c, char const * data, std::size_t size);
        virtual ~WriteJob();

        virtual void run(Async * handle) override;
    };

private:
    Loop _loop;

private:
    SharedServer _server;
    SharedAsync  _async;

public:
    TcpServer();
    ~TcpServer();

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

public:
    bool asyncClose();
    bool asyncWrite(Client & client, char const * buffer, std::size_t size);

public:
    virtual void onConnection(uerr code);
    virtual void onClose();

public:
    virtual binf onClientAlloc(Client & client, std::size_t suggested_size);
    virtual void onClientRead (Client & client, uerr code, char const * buffer, std::size_t size);
    virtual void onClientWrite(Client & client, WriteRequest & request, uerr code);
    virtual void onClientClose(Client & client);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPSERVER_HPP__

