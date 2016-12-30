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
#include <libtbag/network/TcpLoop.hpp>
#include <libtbag/container/Pointer.hpp>

#include <unordered_map>
#include <vector>

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
class TBAG_API TcpServer : public TcpLoop
{
public:
    using CallableTcp = uv::ex::CallableTcp;

    using Buffer = std::vector<char>;
    using binf   = uv::binf;

public:
    /** Client class prototype. */
    struct Client : public CallableTcp, public CallableTcp::Callback
    {
        TcpServer & server;
        Buffer read_buffer;

        Client(TcpServer & s) : CallableTcp(s.atLoop(), this), server(s) { /* EMPTY. */ }
        ~Client() { /* EMPTY. */ }

        virtual binf onAlloc(std::size_t suggested_size) override;
        virtual void onRead(Err code, char const * buffer, std::size_t size) override;
        virtual void onWrite(WriteRequest & request, Err code) override;
        virtual void onClose() override;
    };

public:
    using SharedClient = std::shared_ptr<Client>;
    using WeakClient   = std::weak_ptr<Client>;
    using ClientKey    = container::Pointer<Client>;
    using ClientMap    = std::unordered_map<ClientKey, SharedClient, ClientKey::Hash, ClientKey::EqualTo>;

private:
    ClientMap _clients;

public:
    TcpServer();
    virtual ~TcpServer();

protected:
    SharedClient createClient();
    bool insertClient(SharedClient const & client);
    void eraseClient(Client & client);

protected:
    WeakClient createAcceptedClient();

public:
    bool initIpv4(std::string const & ip, int port);

public:
    virtual bool run(std::string const & ip, int port) override;

// Extension event methods.
public:
    virtual binf onClientAlloc(Client & client, std::size_t suggested_size);
    virtual void onClientRead(Client & client, Err code, char const * buffer, std::size_t size);
    virtual void onClientWrite(Client & client, WriteRequest & request, Err code);
    virtual void onClientClose(Client & client);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_TCPSERVER_HPP__

