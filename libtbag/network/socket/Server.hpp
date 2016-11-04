/**
 * @file   Server.hpp
 * @brief  Server class prototype.
 * @author zer0
 * @date   2016-10-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/socket/Tcp.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

#include <string>
#include <memory>
#include <unordered_map>
#include <limits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Server class prototype.
 *
 * @author zer0
 * @date   2016-10-14
 */
class TBAG_API Server : protected libtbag::network::socket::Tcp::Callback
{
public:
    using Tcp = libtbag::network::socket::Tcp;
    using EventLoop = libtbag::loop::UvEventLoop;
    using ReadErrorCode  = Tcp::ReadErrorCode;
    using WriteErrorCode = Tcp::WriteErrorCode;

public:
    class TBAG_API AcceptedClient : public    libtbag::network::socket::Tcp
                                  , protected libtbag::network::socket::Tcp::Callback
                                  , public    std::enable_shared_from_this<AcceptedClient>
    {
    private:
        Server & _parent;

    public:
        AcceptedClient(Server & parent);
        virtual ~AcceptedClient();

    public:
        virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) override;
        virtual void onWrite(WriteErrorCode code) override;
        virtual void onClose() override;

    private:
        // Don't use this callback.
        virtual void onConnection(int status) override {}
        virtual void onConnect(int status) override {}
    };

    using ClientKey    = std::size_t;
    using SharedClient = std::shared_ptr<AcceptedClient>;
    using WeakClient   = std::weak_ptr<AcceptedClient>;
    using ClientMap    = std::unordered_map<ClientKey, SharedClient>;

    inline static TBAG_CONSTEXPR ClientKey getErrorKey() TBAG_NOEXCEPT
    { return std::numeric_limits<ClientKey>::max(); }

private:
    EventLoop _loop;
    Tcp _tcp;

private:
    ClientMap _clients;

public:
    Server();
    virtual ~Server();

public:
    bool run(std::string const & ip, int port);
    bool runIpv4(std::string const & ip, int port);
    bool runIpv6(std::string const & ip, int port);
    void close();

// client methods.
public:
    inline WeakClient at(ClientKey id)
    {
        auto find_itr = _clients.find(id);
        if (find_itr != _clients.end()) {
            return WeakClient(find_itr->second);
        }
        return WeakClient();
    }

protected:
    /**
     * @warning
     *  It should be used in @c onConnection callbacks.
     */
    WeakClient accept();

// CLIENT EVENT.
public:
    virtual void onClientClose(WeakClient client) {}
    virtual void onClientRead(WeakClient client, ReadErrorCode code, char * buffer, std::size_t length) {}
    virtual void onClientWrite(WeakClient client, WriteErrorCode code) {}

// SERVER EVENT.
public:
    virtual void onConnection(int status) override {}
    virtual void onClose() override {}

private:
    // Don't use this callback.
    virtual void onConnect(int status) override {}
    virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) override {}
    virtual void onWrite(WriteErrorCode code) override {}
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__

