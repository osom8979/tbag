/**
 * @file   NetworkHttpTest.cpp
 * @brief  NetworkHttp class tester.
 * @author zer0
 * @date   2017-05-19
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(NetworkHttpTest, TcpHttpClient)
{
    HttpResponse response;
    auto result = http::requestWithSync("http://osom8979.github.io", 10000, response);
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(200, response.status);
}

