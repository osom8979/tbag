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
#include <libtbag/macro/attributes.hpp>
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/loop/event/UvEventHandler.hpp>

#include <string>
#include <memory>

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
class TBAG_EXPORTS Server : public libtbag::loop::event::UvEventHandler
{
public:
    struct TcpPimpl;
    struct SockAddrPimpl;

    using UniqueTcp = std::unique_ptr<TcpPimpl>;
    using UniqueSockAddr = std::unique_ptr<SockAddrPimpl>;

    using Parent = libtbag::loop::event::UvEventHandler;
    using Loop   = libtbag::loop::UvEventLoop;

public:
    static constexpr int LISTEN_QUEUE_LIMIT = 128;

private:
    Loop _loop;
    UniqueTcp _server;
    UniqueSockAddr _sockaddr;

public:
    Server();
    virtual ~Server();

public:
    bool runIpv4(std::string const & ip, int port);

public:
    virtual void onAlloc(void * handle, size_t suggested_size, void * buf) override;
    virtual void onRead(void * stream, ssize_t nread, void const * buf) override;
    virtual void onWrite(void * req, int status) override;
    virtual void onConnection(void * server, int status) override;

};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_SERVER_HPP__

