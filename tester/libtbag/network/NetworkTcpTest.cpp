/**
 * @file   NetworkTcpTest.cpp
 * @brief  NetworkTcp class tester.
 * @author zer0
 * @date   2017-05-06
 */

#include <gtest/gtest.h>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/details/TcpServer.hpp>
#include <libtbag/network/details/TcpClient.hpp>

#include <thread>
#include <iostream>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::details;

TEST(NetworkTcpTest, ClientTimeout)
{
    using namespace uvpp;
    Loop loop;
    FunctionalTcpClient client(loop);

    if (client.init("8.8.8.8", 9999, 50) == false) {
        std::cout << "Network unreachable.\n";
        return;
    }

    int connect = 0;
    int close = 0;

    uerr connect_result = uerr::UVPP_UNKNOWN;
    uerr result = uerr::UVPP_UNKNOWN;

    client.setOnConnect([&](uerr code){
        connect_result = code;
        connect++;
    });
    client.setOnClose([&](){
        close++;
    });

    std::thread thread([&](){
        result = loop.run();
    });

    thread.join();

    ASSERT_EQ(uerr::UVPP_ECANCELED, connect_result);
    ASSERT_EQ(uerr::UVPP_SUCCESS, result);
    ASSERT_EQ(1, connect);
    ASSERT_EQ(1, close);
}

TEST(NetworkTcpTest, Connection)
{
//    using namespace uvpp;
//    Loop loop_server;
//    Loop loop_client;
//
//    FunctionalTcpServer server(loop_server);
//    FunctionalTcpClient client(loop_client);
//
//    server.setOnConnection([&](uerr code){
//        std::cout << "server/onConnection()\n";
//        while (true) {
//            // waiting.
//        }
//    });
//    client.setOnConnect([&](uerr code){
//        std::cout << "client/onConnect()\n";
//    });
//    client.setOnClose([&](){
//        std::cout << "client/onClose()\n";
//    });
//
//    int const TEST_PORT_NUMBER = 18000;
//
//    server.init(ANY_IPV4, TEST_PORT_NUMBER);
//    client.init(LOOPBACK_IPV4, TEST_PORT_NUMBER, 10);
//
//    uerr result_server;
//    uerr result_client;
//
//    std::thread thread_server([&](){
//        result_server = loop_server.run();
//    });
//    std::thread thread_client([&](){
//        result_client = loop_client.run();
//    });
//
//    thread_server.join();
//    thread_client.join();
//
//    ASSERT_EQ(uerr::UVPP_SUCCESS, result_server);
//    ASSERT_EQ(uerr::UVPP_SUCCESS, result_client);
}

