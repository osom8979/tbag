/**
 * @file   NetworkTcpTest.cpp
 * @brief  NetworkTcp class tester.
 * @author zer0
 * @date   2017-05-06
 */

#include <gtest/gtest.h>
#include <libtbag/network/details/TcpServer.hpp>
#include <libtbag/network/details/TcpClient.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::details;

TEST(NetworkTcpTest, Timeout)
{
//    uvpp::Loop loop;
//    auto client = Client::create(loop, Client::Type::TCP);
//    client->init("192.168.255.0", 9999, 1000);
//    ASSERT_EQ(uvpp::uerr::UVPP_SUCCESS, loop.run());
}

