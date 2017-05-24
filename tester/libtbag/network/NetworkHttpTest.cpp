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

TEST(NetworkHttpTest, HttpServer)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);

    char const * const TEST_BODY = "<html><head><title>TEST</title></head><body><p>Hello, World!</p></body></html>";

    uvpp::Loop loop;
    HttpServer server(loop);

    int on_open         = 0;
    int on_request      = 0;
    int on_request_doc  = 0;
    int on_request_down = 0;
    int on_close        = 0;

    server.init(details::ANY_IPV4, 0);
    int const SERVER_PORT = server.getPort();

    std::string request_url = "http://localhost:";
    request_url += std::to_string(SERVER_PORT);
    request_url += "/";

    std::string request_url_doc  = request_url + "Documents";
    std::string request_url_down = request_url + "Downloads";

    std::cout << "Request URL: " << request_url << std::endl;

    server.setOnOpen([&](HttpServer::WeakClient node){
        std::cout << "Server.OnOpen()\n";
        ++on_open;
    });

    server.setOnRequest([&](Err code, HttpParser const & request, HttpBuilder & response, uint64_t & timeout){
        std::cout << "Server.OnRequest()\n";
        ++on_request;
        response.setStatus(200);
        response.setReason("OK");
        response.setBody(TEST_BODY);
        timeout = 10000;
    });

    server.setOnRequest("/Documents", [&](Err code, HttpParser const & request, HttpBuilder & response, uint64_t & timeout){
        std::cout << "Server.OnRequest(/Documents)\n";
        ++on_request_doc;
        response.setStatus(200);
        response.setReason("OK");
        response.setBody(TEST_BODY);
        timeout = 10000;
    });
    server.setOnRequest("/Downloads", [&](Err code, HttpParser const & request, HttpBuilder & response, uint64_t & timeout){
        std::cout << "Server.OnRequest(/Downloads)\n";
        ++on_request_down;
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

    HttpResponse response1;
    HttpResponse response2;
    HttpResponse response3;

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client1_thread([&](){ client_result = http::requestWithSync(request_url     , 10000, response1); });
    std::thread client2_thread([&](){ client_result = http::requestWithSync(request_url_doc , 10000, response2); });
    std::thread client3_thread([&](){ client_result = http::requestWithSync(request_url_down, 10000, response3); });

    client1_thread.join();
    client2_thread.join();
    client3_thread.join();
    server_thread.join();

    ASSERT_STREQ(TEST_BODY, response1.body.c_str());
    ASSERT_EQ(200, response1.status);

    ASSERT_STREQ(TEST_BODY, response2.body.c_str());
    ASSERT_EQ(200, response2.status);

    ASSERT_STREQ(TEST_BODY, response3.body.c_str());
    ASSERT_EQ(200, response3.status);

    ASSERT_EQ(3, on_open        );
    ASSERT_EQ(1, on_request     );
    ASSERT_EQ(1, on_request_doc );
    ASSERT_EQ(1, on_request_down);
    ASSERT_EQ(3, on_close       );
}

