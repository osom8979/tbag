/**
 * @file   Echo.hpp
 * @brief  Echo class prototype.
 * @author zer0
 * @date   2016-12-23
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_ECHO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_ECHO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/network/socket/Server.hpp>
#include <libtbag/network/socket/Client.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

/**
 * Echo server class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 * @date   2016-12-23 (Move project: tester -> libtbag)
 */
class TBAG_API EchoServer : public socket::Server, public socket::Server::EventCallback
{
private:
    int _count; ///< Max echo count.

public:
    EchoServer(int count = 5);
    virtual ~EchoServer();

public:
    virtual bool onConnection(ClientKey key, int status) override;
    virtual void onClose() override;
    virtual void onCloseClient(ClientKey key) override;
    virtual void onRead(ClientKey from, Code code, char * buffer, std::size_t size) override;
    virtual void onWrite(ClientKey to, Code code) override;
};

/**
 * Echo client class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 * @date   2016-12-23 (Move project: tester -> libtbag)
 */
class TBAG_API EchoClient : public socket::Client, public socket::Client::EventCallback
{
public:
    EchoClient(Loop & loop);
    virtual ~EchoClient();

public:
    virtual void onConnect(Err code) override;
    virtual void onClose() override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;
    virtual void onWrite(Err code) override;
};

TBAG_API int runEchoServer(std::string const & ip, int port);
TBAG_API int runEchoClient(std::string const & ip, int port);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_ECHO_HPP__

