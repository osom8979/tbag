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
#include <libtbag/network/TcpClient.hpp>
#include <libtbag/network/TcpServer.hpp>
#include <libtbag/network/DatagramAdapter.hpp>

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
class TBAG_API EchoServer : public TcpServer
{
private:
    int _write_count; ///< Write count.
    int  _echo_count; ///< Max echo count.

public:
    EchoServer(int count = 3);
    virtual ~EchoServer();

public:
    virtual void onConnection(Err code) override;
    virtual void onClose() override;

public:
    virtual binf onClientAlloc(Client & client, std::size_t suggested_size) override;
    virtual void onClientRead(Client & client, Err code, char const * buffer, std::size_t size) override;
    virtual void onClientWrite(Client & client, WriteRequest & request, Err code) override;
    virtual void onClientClose(Client & client) override;
};

/**
 * Echo client class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 * @date   2016-12-23 (Move project: tester -> libtbag)
 */
class TBAG_API EchoClient : public TcpClient
{
private:
    DatagramAdapter _datagram;

public:
    EchoClient();
    virtual ~EchoClient();

public:
    virtual binf onAlloc(std::size_t suggested_size) override;
    virtual void onConnect(ConnectRequest & request, Err code) override;
    virtual void onRead(Err code, char const * buffer, std::size_t size) override;
    virtual void onWrite(WriteRequest & request, Err code) override;
    virtual void onClose() override;
};

TBAG_API int runEchoServer(std::string const & ip, int port);
TBAG_API int runEchoClient(std::string const & ip, int port);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_ECHO_HPP__

