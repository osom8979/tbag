/**
 * @file   HttpServerClientTest.cpp
 * @brief  HttpServer and HttpClient class tester.
 * @author zer0
 * @date   2020-03-10
 */

#include <gtest/gtest.h>
#include <libtbag/http/HttpServer.hpp>
#include <libtbag/http/HttpClient.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <sstream>

using namespace libtbag;
using namespace libtbag::http;

TEST(HttpServerClientTest, DefaultHttp)
{
#if defined(DEMO_TCP_PORT)
    auto const port = DEMO_TCP_PORT;
#else
    auto const port = 8080;
#endif
    std::string const host = "0.0.0.0";
    std::string const bind_address = host + ":" + libtbag::string::toString(port);
    std::string const test_body = "__test__";
    std::string const test_path = "/test.html";

    std::stringstream req_ss;
    req_ss << "GET " << test_path << " HTTP/1.1\r\n"
           << "Host: " << bind_address << "\r\n"
           << "User-Agent: tester\r\n"
           << "Accept: */*\r\n\r\n";
    auto const test_request = req_ss.str();

    std::stringstream res_ss;
    res_ss << "HTTP/1.1 200 Ok\r\n"
           << "Content-Type: text/plain; charset=utf-8\r\n"
           << "Content-Length: " << test_body.size() << "\r\n"
           << "\r\n"
           << test_body;
    auto const test_response = res_ss.str();

    HttpServer::EventFunctional events;
    using _arg0 = HttpServer::EventFunctional::CallbackArg0;
    using _arg1 = HttpServer::EventFunctional::CallbackArg1;
    using _ret = HttpServer::EventFunctional::CallbackReturn;
    events.get_cb = [&](_arg0 server, _arg1 conn) -> _ret {
        return isSuccess(HttpServer::write(conn, test_response));
    };

    HttpServer::Options options;
    HttpServer::opt(options, HttpServer::OPT_LISTENING_PORTS, bind_address);

    HttpServer::RequestMap reqs;
    ASSERT_TRUE(HttpServer::req(reqs, test_path, events));

    HttpServer server;
    ASSERT_FALSE(server.isOpen());
    ASSERT_EQ(E_SUCCESS, server.open(options, reqs).code);
    ASSERT_TRUE(server.isOpen());

    HttpClient client;
    ASSERT_FALSE(client.isOpen());
    ASSERT_EQ(E_SUCCESS, client.open(host, port).code);
    ASSERT_TRUE(client.isOpen());

    ASSERT_EQ(E_SUCCESS, client.write(test_request));
    ASSERT_EQ(E_SUCCESS, client.wait());
    auto const response_result = client.read();
    ASSERT_EQ(E_SUCCESS, response_result.code);
    ASSERT_EQ(test_body, response_result.value);

    client.close();
    ASSERT_FALSE(client.isOpen());

    server.close();
    ASSERT_FALSE(server.isOpen());
}

