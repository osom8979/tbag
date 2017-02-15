/**
 * @file   Echo.hpp
 * @brief  Echo class prototype.
 * @author zer0
 * @date   2016-12-23
 * @date   2017-02-15 (Apply BasicTcp class)
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

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

TBAG_CONSTEXPR char const * const ECHO_MESSAGE = "TBAG_ECHO_MESSAGE";
TBAG_CONSTEXPR int const ECHO_COUNT = 5;

/**
 * Echo server class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 * @date   2016-12-23 (Move project: tester -> libtbag)
 * @date   2017-02-15 (Apply BasicTcp class)
 */
class TBAG_API EchoServer : public TcpServer
{
private:
    int _count; ///< Max echo count.

public:
    EchoServer(int count);
    virtual ~EchoServer();

public:
    virtual bool onNewConnection(uerr code, WeakClient client) override;
    virtual void onClientWrite(ClientTcp & client, WriteRequest & request, uerr code) override;
    virtual void onClientReadDatagram(ClientTcp & client, uerr code, char const * buffer, std::size_t size) override;
};

/**
 * Echo client class prototype.
 *
 * @author zer0
 * @date   2016-11-04
 * @date   2016-11-07 (Refactoring this class)
 * @date   2016-12-23 (Move project: tester -> libtbag)
 * @date   2017-02-15 (Apply BasicTcp class)
 */
class TBAG_API EchoClient : public TcpClient
{
private:
    int _count;

public:
    EchoClient(int count);
    virtual ~EchoClient();

public:
    virtual void onClientConnect(ConnectRequest & request, uerr code) override;
    virtual void onClientReadDatagram(uerr code, char const * buffer, std::size_t size) override;
};

char const * const TEST_ECHO_SERVER_ADDRESS =   "0.0.0.0";
char const * const TEST_ECHO_CLIENT_ADDRESS = "127.0.0.1";
int          const TEST_ECHO_PORT           =       19999;

TBAG_API int runEchoServer(std::string const & ip = TEST_ECHO_SERVER_ADDRESS, int port = TEST_ECHO_PORT);
TBAG_API int runEchoClient(std::string const & ip = TEST_ECHO_CLIENT_ADDRESS, int port = TEST_ECHO_PORT);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_ECHO_HPP__

