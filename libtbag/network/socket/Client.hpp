/**
 * @file   Client.hpp
 * @brief  Client class prototype.
 * @author zer0
 * @date   2016-10-14
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/socket/Tcp.hpp>
#include <libtbag/loop/UvEventLoop.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace socket  {

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2016-10-14
 */
class TBAG_API Client : public libtbag::network::socket::Tcp::Callback
{
public:
    using Tcp = libtbag::network::socket::Tcp;
    using EventLoop = libtbag::loop::UvEventLoop;
    using ReadErrorCode  = Tcp::ReadErrorCode;
    using WriteErrorCode = Tcp::WriteErrorCode;

private:
    EventLoop _loop;
    Tcp _tcp;

public:
    Client();
    virtual ~Client();

public:
    bool run(std::string const & ip, int port);
    bool runIpv4(std::string const & ip, int port);
    bool runIpv6(std::string const & ip, int port);

public:
    bool read();
    bool write(char const * buffer, std::size_t length);
    void close();

public:
    virtual void onConnect(int status) override {}
    virtual void onCloseTcp() override {}
    virtual void onCloseWrite() override {}
    virtual void onCloseConnect() override {}
    virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) override {}
    virtual void onWrite(WriteErrorCode code) override {}

private:
    // Don't use this callback.
    virtual void onConnection(int status) override {}

};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__

