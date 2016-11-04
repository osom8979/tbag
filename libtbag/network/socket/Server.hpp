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
class TBAG_API Server : public libtbag::network::socket::Tcp::Callback
{
public:
    using Tcp = libtbag::network::socket::Tcp;
    using EventLoop = libtbag::loop::UvEventLoop;
    using ReadErrorCode  = Tcp::ReadErrorCode;
    using WriteErrorCode = Tcp::WriteErrorCode;

    using ClientKey = std::size_t;
    using SharedClient = std::shared_ptr<Tcp>;
    using ClientMap = std::unordered_map<ClientKey, SharedClient>;

    inline static TBAG_CONSTEXPR ClientKey getErrorKey() TBAG_NOEXCEPT
    { return std::numeric_limits<ClientKey>::max(); }

private:
    EventLoop _loop;
    Tcp _tcp;

private:
    ClientMap _clients;

public:
    Server();
    ~Server();

public:
    bool run(std::string const & ip, int port);
    bool runIpv4(std::string const & ip, int port);
    bool runIpv6(std::string const & ip, int port);

public:
    bool read();
    void close();

    // client methods.
public:
    bool write(ClientKey id, char const * buffer, std::size_t length);
    bool close(ClientKey id);

protected:
    /**
     * @warning
     *  It should be used in @c onConnection callbacks.
     */
    ClientKey accept();

public:
    virtual void onConnection(int status) override {}
    virtual void onCloseTcp() override {}
    virtual void onCloseWrite() override {}
    virtual void onCloseConnect() override {}
    virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) override {}
    virtual void onWrite(WriteErrorCode code) override {}

private:
    // Don't use this callback.
    virtual void onConnect(int status) override {}
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__

