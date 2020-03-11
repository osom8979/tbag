/**
 * @file   HttpServerClientTest.cpp
 * @brief  HttpServer and HttpClient class tester.
 * @author zer0
 * @date   2020-03-10
 */

#include <gtest/gtest.h>
#include <libtbag/http/HttpServer.hpp>
#include <libtbag/http/HttpClient.hpp>

using namespace libtbag;
using namespace libtbag::http;

//TEST(HttpServerClientTest, HttpClient)
//{
//#if defined(DEMO_TCP_PORT)
//    auto const demo_port = DEMO_TCP_PORT;
//#else
//    auto const demo_port = 8080;
//#endif
//    HttpClient client;
//    ASSERT_FALSE(client.isOpen());
//    auto const result = client.open("localhost", 80);
//    if (!result) {
//        std::cout << result.msg << std::endl;
//    }
//    ASSERT_EQ(E_SUCCESS, result.code);
//    ASSERT_TRUE(client.isOpen());
//    client.close();
//    ASSERT_FALSE(client.isOpen());
//}

TEST(HttpServerClientTest, Http)
{
}

