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

TEST(NetworkHttpTest, TcpHttpClient)
{
    log::SeverityGuard guard;

    HttpResponse response;
    auto result = http::requestWithSync("http://osom8979.github.io", 10000, response);
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(200, response.status);
}

TEST(NetworkHttpTest, TcpHttpServer)
{
    log::SeverityGuard guard;

    char const * const TEST_BODY = "<html><head><title>TEST</title></head><body><p>Hello, World!</p></body></html>";

    uvpp::Loop loop;
    HttpServer server(loop);

    int on_open    = 0;
    int on_request = 0;
    int on_close   = 0;

    server.init(details::ANY_IPV4, 0);
    int const SERVER_PORT = server.getPort();

    std::string request_url = "http://localhost:";
    request_url += std::to_string(SERVER_PORT);
    request_url += "/";

    std::cout << "Request URL: " << request_url << std::endl;

    server.setOnOpen([&](HttpServer::WeakClient node){
        std::cout << "Server.OnOpen()\n";
        ++on_open;
    });
    server.setOnRequest([&](Err code, HttpParser const & request, HttpBuilder & response, uint64_t & timeout){
        std::cout << "Server.OnRequest()\n";
        ++on_request;
        request.getMethodName();
        response.setStatus(200);
        response.setReason("OK");
        response.setBody(TEST_BODY);
        timeout = 10000;
    });
    server.setOnClose([&](HttpServer::WeakClient node){
        std::cout << "Server.OnClose()\n";
        ++on_close;
        server.close();
    });

    Err server_result = Err::E_UNKNOWN;
    Err client_result = Err::E_UNKNOWN;

    HttpResponse response;

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client_thread([&](){ client_result = http::requestWithSync(request_url, 10000, response); });

    client_thread.join();
    server_thread.join();

    ASSERT_STREQ(TEST_BODY, response.body.c_str());
    ASSERT_EQ(200, response.status);
}

