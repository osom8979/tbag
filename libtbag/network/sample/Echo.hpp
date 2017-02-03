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
//#include <libtbag/network/TcpClient.hpp>
//#include <libtbag/network/TcpServer.hpp>
//#include <libtbag/network/DatagramAdapter.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace sample  {

///**
// * Echo server class prototype.
// *
// * @author zer0
// * @date   2016-11-04
// * @date   2016-11-07 (Refactoring this class)
// * @date   2016-12-23 (Move project: tester -> libtbag)
// */
//class TBAG_API EchoServer : public TcpServer
//{
//private:
//    int _write_count; ///< Write count.
//    int  _echo_count; ///< Max echo count.
//    bool    _massive; ///< Enable/Disable massive data.
//
//public:
//    EchoServer(int count = 3, bool massive = false);
//    virtual ~EchoServer();
//
//public:
//    virtual void onConnection(uerr code) override;
//    virtual void onClose() override;
//
//public:
//    virtual binf onClientAlloc(Client & client, std::size_t suggested_size) override;
//    virtual void onClientRead(Client & client, uerr code, char const * buffer, std::size_t size) override;
//    virtual void onClientWrite(Client & client, WriteRequest & request, uerr code) override;
//    virtual void onClientClose(Client & client) override;
//};
//
///**
// * Echo client class prototype.
// *
// * @author zer0
// * @date   2016-11-04
// * @date   2016-11-07 (Refactoring this class)
// * @date   2016-12-23 (Move project: tester -> libtbag)
// */
//class TBAG_API EchoClient : public TcpClient
//{
//private:
//    DatagramAdapter _datagram;
//    int _read_count;
//
//private:
//    int _debugging_count;
//
//public:
//    EchoClient();
//    virtual ~EchoClient();
//
//public:
//    virtual binf onAlloc(std::size_t suggested_size) override;
//    virtual void onConnect(ConnectRequest & request, uerr code) override;
//    virtual void onRead(uerr code, char const * buffer, std::size_t size) override;
//    virtual void onWrite(WriteRequest & request, uerr code) override;
//    virtual void onClose() override;
//};

char const * const TEST_ECHO_SERVER_ADDRESS =   "0.0.0.0";
char const * const TEST_ECHO_CLIENT_ADDRESS = "127.0.0.1";
int          const TEST_ECHO_PORT           =        9999;

TBAG_API int runEchoServer(std::string const & ip = TEST_ECHO_SERVER_ADDRESS, int port = TEST_ECHO_PORT);
TBAG_API int runEchoClient(std::string const & ip = TEST_ECHO_CLIENT_ADDRESS, int port = TEST_ECHO_PORT);

} // namespace sample
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_SAMPLE_ECHO_HPP__

