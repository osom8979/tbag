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
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/loop/event/UvHandler.hpp>
#include <libtbag/network/socket/Tcp.hpp>

#include <string>

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
class TBAG_API Server : public libtbag::Noncopyable
{
public:
    struct SocketPimpl;
    friend struct SocketPimpl;

public:
    using EventLoop    = libtbag::loop::UvEventLoop;
    using UniqueSocket = std::unique_ptr<SocketPimpl>;

private:
    EventLoop    _loop;
    UniqueSocket _socket;

public:
    Server();
    virtual ~Server();

public:
    bool run(std::string const & ip, int port);

//public:
//    Server();
//    virtual ~Server();
//
//public:
//    bool runIpv4(std::string const & ip, int port);
//
//protected:
//    virtual void onAlloc(void * handle, size_t suggested_size, void * buf) override;
//    virtual void onRead(void * stream, ssize_t nread, void const * buf) override;
//    virtual void onWrite(void * req, int status) override;
//    virtual void onConnection(void * server, int status) override;

};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__

