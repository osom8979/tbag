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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/loop/UvEventLoop.hpp>
#include <libtbag/loop/event/UvHandler.hpp>

#include <string>
#include <memory>

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
class TBAG_API Client : public libtbag::Noncopyable
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
    Client();
    virtual ~Client();

public:
    bool run(std::string const & ip, int port);

//protected:
//    //virtual void onAlloc(void * handle, size_t suggested_size, void * buf) override;
//    //virtual void onRead(void * stream, ssize_t nread, void const * buf) override;
//    virtual void onWrite(void * req, int status) override;
//    virtual void onConnect(void * req, int status) override;
};

} // namespace socket
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SOCKET_CLIENT_HPP__

