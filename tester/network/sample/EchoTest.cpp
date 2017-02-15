/**
 * @file   EchoTest.cpp
 * @brief  Echo class tester.
 * @author zer0
 * @date   2017-02-15
 */

#include <gtest/gtest.h>
#include <libtbag/network/sample/Echo.hpp>

#include <thread>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::sample;

TEST(EchoTest, Default)
{
    EchoServer server(ECHO_COUNT);
    EchoClient client(ECHO_COUNT);

    bool server_result = false;
    bool client_result = false;

    std::thread server_thread([&](){
        server_result = server.run(TEST_ECHO_SERVER_ADDRESS, TEST_ECHO_PORT);
    });
    std::thread client_thread([&](){
        client_result = client.run(TEST_ECHO_CLIENT_ADDRESS, TEST_ECHO_PORT);
    });

    server_thread.join();
    client_thread.join();

    ASSERT_TRUE(server_result);
    ASSERT_TRUE(client_result);

    ASSERT_EQ(0, server.getCount());
    ASSERT_EQ(0, client.getCount());
}

