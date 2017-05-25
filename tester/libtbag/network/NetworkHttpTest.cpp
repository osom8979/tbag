/**
 * @file   NetworkHttpTest.cpp
 * @brief  NetworkHttp class tester.
 * @author zer0
 * @date   2017-05-19
 */

#include <gtest/gtest.h>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <iostream>
#include <thread>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(NetworkHttpTest, HttpClient)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);

    HttpResponse response;
    auto result = http::requestWithSync("http://osom8979.github.io", 10000, response);
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(200, response.status);
}

TEST(NetworkHttpTest, GetMethod)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);

    uvpp::Loop loop;
    HttpServer server(loop);

    server.init(details::ANY_IPV4, 0);

    std::string request_url = "http://localhost:";
    request_url += std::to_string(server.getPort());
    request_url += "/";

    int on_open    = 0;
    int on_request = 0;
    int on_close   = 0;

    server.setOnOpen([&](HttpServer::WeakClient node){
        ++on_open;
    });
    server.setOnRequest([&](Err code, HttpParser const & request, HttpBuilder & response, uint64_t & timeout){
        ++on_request;
        response.setStatus(200);
        response.setReason("OK");
        response.setBody(request.getMethodName() + request.getUrl());
        timeout = 10000;
    });
    server.setOnClose([&](HttpServer::WeakClient node){
        ++on_close;
        server.close();
    });

    Err server_result = Err::E_UNKNOWN;
    Err client_result = Err::E_UNKNOWN;

    HttpResponse response;
    HttpRequest  request;
    request.method = "GET";

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client_thread([&](){ client_result = http::requestWithSync(request_url, 10000, response); });

    client_thread.join();
    server_thread.join();

    ASSERT_EQ(Err::E_SUCCESS, server_result);
    ASSERT_EQ(Err::E_SUCCESS, client_result);
    ASSERT_EQ(200, response.status);
}

