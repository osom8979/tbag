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
    using namespace uvpp;
    Loop loop;
    TcpHttpClient http(loop);

    Err result = Err::E_UNKNOWN;
    int status;

    if (http.init("osom8979.github.io", 80, 10000) == false) {
        std::cout << "Network unreachable.\n";
        return;
    }

    auto builder = HttpBuilder(1, 1).setMethod("GET").setUri("/")
            .insertHeader("Host", "osom8979.github.io")
            .insertHeader("User-Agent", "curl/7.51.0")
            .insertHeader("Accept", "*/*");

    http.setup(builder, [&](Err code, HttpParser const & response){
        result = code;
        status = response.getStatusCode();
        std::cout << response.getBody() << std::endl;
    }, TcpHttpClient::Millisec(10000));

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(200, status);

//    if (client.init(ip, port, 1000) == false) {
//        std::cout << "Network unreachable.\n";
//        return;
//    }
//    ASSERT_EQ(Err::E_SUCCESS, loop.run());
}

