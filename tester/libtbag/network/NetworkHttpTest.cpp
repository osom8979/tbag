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

static bool runSimpleServerTest(std::string const & method)
{
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
        timeout = 1000;
    });
    server.setOnClose([&](HttpServer::WeakClient node){
        ++on_close;
        server.close();
    });

    Err server_result = Err::E_UNKNOWN;
    Err client_result = Err::E_UNKNOWN;

    HttpResponse response;
    HttpRequest  request;
    request.method = method;

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client_thread([&](){ client_result = http::requestWithSync(request_url, request, 10000, response); });

    client_thread.join();
    server_thread.join();

    if (Err::E_SUCCESS != server_result) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Server result {} error", method, getErrName(server_result));
        return false;
    }
    if (Err::E_SUCCESS != client_result) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Client result {} error", method, getErrName(client_result));
        return false;
    }

    if (200 != response.status) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Response is not OK({}).", method, response.status);
        return false;
    }

    if (on_open != 1 || on_request != 1 || on_close != 1) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Counter error({}/{}/{}).",
               method, on_open, on_request, on_close);
        return false;
    }

    return true;
}

TEST(NetworkHttpTest, GetMethod)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);
    runSimpleServerTest("GET");
}

TEST(NetworkHttpTest, PostMethod)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);
    runSimpleServerTest("POST");
}

