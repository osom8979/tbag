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
#include <libtbag/filesystem/File.hpp>
#include <libtbag/crypto/X509.hpp>
#include <libtbag/util/TestUtils.hpp>

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
    HttpServer::opt(options, HttpServer::OPT_NUM_THREADS, 1);

    HttpServer::RequestMap reqs;
    ASSERT_TRUE(HttpServer::req(reqs, test_path, events));

    HttpServer server;
    ASSERT_FALSE(server.isOpen());
    auto const server_open_result = server.open(options, reqs);
    if (!server_open_result) {
        std::cerr << server_open_result.msg << std::endl;
    }
    ASSERT_EQ(E_SUCCESS, server_open_result.code);
    ASSERT_TRUE(server.isOpen());

    HttpClient client;
    ASSERT_FALSE(client.isOpen());
    auto const client_open_result = client.open(host, port);
    if (!client_open_result) {
        std::cerr << client_open_result.msg << std::endl;
    }
    ASSERT_EQ(E_SUCCESS, client_open_result.code);
    ASSERT_TRUE(client.isOpen());

    ASSERT_EQ(E_SUCCESS, client.write(test_request));
    auto const wait_result = client.wait();
    if (!wait_result) {
        std::cerr << wait_result.msg << std::endl;
    }
    ASSERT_EQ(E_SUCCESS, wait_result.code);

    auto const response_result = client.read();
    ASSERT_EQ(E_SUCCESS, response_result.code);
    ASSERT_EQ(test_body, response_result.value);

    client.close();
    ASSERT_FALSE(client.isOpen());

    server.close();
    ASSERT_FALSE(server.isOpen());
}

TEST(HttpServerClientTest, DefaultHttps)
{
    tttDir_Automatic();
    auto const ssl_cert_path = tttDir_Get() / "ssl_cert.pem";

    using namespace libtbag::filesystem;
    using namespace libtbag::crypto;
    ASSERT_EQ(E_SUCCESS, writeFile(ssl_cert_path, genX509PemToSingleText()));
    ASSERT_TRUE(ssl_cert_path.isRegularFile());

#if defined(DEMO_TCP_PORT)
    auto const port = DEMO_TCP_PORT;
#else
    auto const port = 8080;
#endif
    std::string const host = "0.0.0.0";
    std::string const bind_address = host + ":" + libtbag::string::toString(port);
    std::string const test_body = "__test_https__";
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
    HttpServer::opt(options, HttpServer::OPT_LISTENING_PORTS, bind_address + "s");
    HttpServer::opt(options, HttpServer::OPT_NUM_THREADS, 1);
    HttpServer::opt(options, HttpServer::OPT_SSL_CERTIFICATE, ssl_cert_path);

    HttpServer::RequestMap reqs;
    ASSERT_TRUE(HttpServer::req(reqs, test_path, events));

    HttpServer server;
    ASSERT_FALSE(server.isOpen());
    auto const server_open_result = server.open(options, reqs);
    if (!server_open_result) {
        std::cerr << server_open_result.msg << std::endl;
    }
    ASSERT_EQ(E_SUCCESS, server_open_result.code);
    ASSERT_TRUE(server.isOpen());

    HttpClient client;
    ASSERT_FALSE(client.isOpen());
    auto const client_open_result = client.open(host, port, true);
    if (!client_open_result) {
        std::cerr << client_open_result.msg << std::endl;
    }
    ASSERT_EQ(E_SUCCESS, client_open_result.code);
    ASSERT_TRUE(client.isOpen());

    ASSERT_EQ(E_SUCCESS, client.write(test_request));
    auto const wait_result = client.wait();
    if (!wait_result) {
        std::cerr << wait_result.msg << std::endl;
    }
    ASSERT_EQ(E_SUCCESS, wait_result.code);

    auto const response_result = client.read();
    ASSERT_EQ(E_SUCCESS, response_result.code);
    ASSERT_EQ(test_body, response_result.value);

    client.close();
    ASSERT_FALSE(client.isOpen());

    server.close();
    ASSERT_FALSE(server.isOpen());
}

