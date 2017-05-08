/**
 * @file   NetworkTcpTest.cpp
 * @brief  NetworkTcp class tester.
 * @author zer0
 * @date   2017-05-06
 */

#include <gtest/gtest.h>
#include <libtbag/network/details/TcpServer.hpp>
#include <libtbag/network/details/TcpClient.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::details;

TEST(NetworkTcpTest, Timeout)
{
    uvpp::Loop loop_server;
    uvpp::Loop loop_client;

    auto server = Server::create(loop_server, Client::Type::TCP);
    auto client = Client::create(loop_client, Client::Type::TCP);

//    server->init("192.168.255.0", 9999);
//    client->init("192.168.255.0", 9999, 1000);
//
//    uvpp::uerr result_server;
//    uvpp::uerr result_client;
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
//    ASSERT_EQ(uvpp::uerr::UVPP_SUCCESS, result_server);
//    ASSERT_EQ(uvpp::uerr::UVPP_SUCCESS, result_client);
}

