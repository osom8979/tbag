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

struct TcpServerTest : public TcpServer
{
    TcpServerTest(Loop & loop) : TcpServer(loop)
    {
        // EMPTY.
    }

    virtual bool onClientConnect(NodeInterface * node, uerr code) override
    {
        std::cout << "TcpServerTest::onClientConnect(" << uvpp::getErrorName(code) << ")\n";
        return true;
    }

    virtual void onClientWrite(NodeInterface * node, uerr code) override
    {
        std::cout << "TcpServerTest::onClientWrite(" << uvpp::getErrorName(code) << ")\n";
    }

    virtual void onClientRead(NodeInterface * node, uerr code, char const * buffer, Size size) override
    {
        std::cout << "TcpServerTest::onClientRead(" << uvpp::getErrorName(code) << ")\n";
    }

    virtual void onClientClose(NodeInterface * node) override
    {
        std::cout << "TcpServerTest::onClientClose()\n";
    }

    virtual void onServerClose() override
    {
        std::cout << "TcpServerTest::onServerClose()\n";
    }
};

struct TcpClientTest : public TcpClient
{
    TcpClientTest(Loop & loop) : TcpClient(loop)
    {
        // EMPTY.
    }

    virtual void onConnect(uerr code) override
    {
        std::cout << "TcpClientTest::onConnect(" << uvpp::getErrorName(code) << ")\n";
    }

    virtual void onWrite(uerr code) override
    {
        std::cout << "TcpClientTest::onWrite(" << uvpp::getErrorName(code) << ")\n";
    }

    virtual void onRead(uerr code, char const * buffer, Size size) override
    {
        std::cout << "TcpClientTest::onRead(" << uvpp::getErrorName(code) << ")\n";
    }

    virtual void onClose() override
    {
        std::cout << "TcpClientTest::onClose()\n";
    }
};

TEST(NetworkTcpTest, Timeout)
{
    uvpp::Loop loop_server;
    uvpp::Loop loop_client;

    TcpServerTest server(loop_server);
    TcpClientTest client(loop_client);

    int const TEST_PORT_NUMBER = 18000;

    server.init(ANY_IPV4, TEST_PORT_NUMBER);
    client.init(LOOPBACK_IPV4, TEST_PORT_NUMBER, 1000);

    uvpp::uerr result_server;
    uvpp::uerr result_client;

    std::thread thread_server([&](){
        result_server = loop_server.run();
    });
    std::thread thread_client([&](){
        result_client = loop_client.run();
    });

    thread_server.join();
    thread_client.join();

    ASSERT_EQ(uvpp::uerr::UVPP_SUCCESS, result_server);
    ASSERT_EQ(uvpp::uerr::UVPP_SUCCESS, result_client);
}

