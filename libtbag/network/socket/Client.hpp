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

//class TBAG_API Client : protected libtbag::network::socket::Tcp::Callback
//{
//public:
//    using Tcp = libtbag::network::socket::Tcp;
//    using EventLoop = libtbag::loop::UvEventLoop;
//    using ReadErrorCode  = Tcp::ReadErrorCode;
//    using WriteErrorCode = Tcp::WriteErrorCode;
//
//private:
//    EventLoop _loop;
//    Tcp _tcp;
//
//public:
//    Client();
//    virtual ~Client();
//
//public:
//    bool run(std::string const & ip, int port);
//    bool runIpv4(std::string const & ip, int port);
//    bool runIpv6(std::string const & ip, int port);
//
//public:
//    bool read();
//    bool write(char const * buffer, std::size_t length);
//    void close();
//
//public:
//    virtual void onConnect(int status) override {}
//    virtual void onClose() override {}
//    virtual void onRead(ReadErrorCode code, char * buffer, std::size_t length) override {}
//    virtual void onWrite(WriteErrorCode code) override {}
//
//private:
//    // Don't use this callback.
//    virtual void onConnection(int status) override {}
//};

/**
 * Client class prototype.
 *
 * @author zer0
 * @date   2016-10-14
 * @date   2016-11-07 (Refactoring this class)
 */
class TBAG_API Client : public Noncopyable
{
public:
    Client();
    ~Client();

public:
    bool run(std::string const & address, int port);
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__

