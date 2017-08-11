/**
 * @file   NetworkHttpTest.cpp
 * @brief  NetworkHttp class tester.
 * @author zer0
 * @date   2017-05-19
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>

#include <libtbag/log/Log.hpp>
#include <libtbag/network/http/HttpClient.hpp>
#include <libtbag/network/http/HttpServer.hpp>
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/WsClient.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <iostream>
#include <thread>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

using WeakClient = HttpServer::WeakClient;
using HttpPacket = HttpServer::HttpPacket;
using WsPacket   = HttpServer::WsPacket;

TEST(NetworkHttpTest, HttpClient)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);

    HttpResponse response;
    auto result = http::requestWithSync("http://osom8979.github.io", 10000, response);
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(200, response.status);
}

static bool runSimpleServerTest(HttpServer::StreamType type, std::string const & bind, std::string const & method)
{
    uvpp::Loop loop;
    FuncHttpServer server(loop, type);

    server.init(bind.c_str());

    std::string request_url = "http://localhost:";
    request_url += std::to_string(server.port());
    request_url += "/";

    int on_open    = 0;
    int on_request = 0;
    int on_close   = 0;

    server.setOnHttpOpen([&](WeakClient node){
        ++on_open;
    });
    server.setOnHttpRequest([&](WeakClient node, Err code, HttpPacket & packet){
        ++on_request;
        packet.response.setStatus(200);
        packet.response.setReason("OK");
        packet.response.setBody(packet.request.getMethodName());
    });
    server.setOnHttpClose([&](WeakClient node){
        ++on_close;
        server.close();
    });

    Err server_result = Err::E_UNKNOWN;
    Err client_result = Err::E_UNKNOWN;

    HttpResponse response;
    HttpRequest  request;
    request.method = method;

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client_thread([&](){
        if (HttpServer::StreamType::PIPE == type) {
            client_result = http::requestWithSync(type, bind, 0, Uri(request_url), request, 1000, response);
        } else {
            client_result = http::requestWithSync(request_url, request, 1000, response);
        }
    });

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

    if (response.getBody() != method) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Response body error({}).", method, response.getBody());
        return false;
    }

    if (on_open != 1 || on_request != 1 || on_close != 1) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Counter error({}/{}/{}).",
               method, on_open, on_request, on_close);
        return false;
    }

    return true;
}

TEST(NetworkHttpTest, TcpHttpServer)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);
    ASSERT_TRUE(runSimpleServerTest(HttpServer::StreamType::TCP, details::ANY_IPV4, "GET"));
    ASSERT_TRUE(runSimpleServerTest(HttpServer::StreamType::TCP, details::ANY_IPV4, "POST"));
}

TEST(NetworkHttpTest, PipeHttpServer)
{
#if defined(TBAG_PLATFORM_WINDOWS)
    char const * const PATH = "\\\\.\\pipe\\HTTP_TEST";
#else
    char const * const TEST_FILENAME = "http.sock";
    tttDir(true, true);
    auto const PATH = tttDirGet() / TEST_FILENAME;
#endif

    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);
    ASSERT_TRUE(runSimpleServerTest(HttpServer::StreamType::PIPE, PATH, "GET"));
    ASSERT_TRUE(runSimpleServerTest(HttpServer::StreamType::PIPE, PATH, "POST"));
}

TEST(NetworkHttpTest, RoutingServer)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);

    uvpp::Loop loop;
    FuncHttpServer server(loop);

    int on_open    = 0;
    int on_close   = 0;
    int on_request = 0;

    int on_request_doc = 0;
    int on_request_down_get  = 0;
    int on_request_down_post = 0;

    server.init(details::ANY_IPV4, 0);
    int const SERVER_PORT = server.port();

    std::string request_url = "http://localhost:";
    request_url += std::to_string(SERVER_PORT);
    request_url += "/";

    std::string const REQUEST_URL_DOC  = request_url + "Documents";
    std::string const REQUEST_URL_DOWN = request_url + "Downloads";

    std::cout << "Request URL: " << request_url << std::endl;

    server.setOnHttpOpen([&](WeakClient node){
        ++on_open;
    });

    server.setOnHttpRequest([&](WeakClient node, Err code, HttpPacket & packet){
        std::cout << "Server.OnRequest()\n";
        ++on_request;
        packet.response.setStatus(200);
        packet.response.setReason("OK");
        packet.response.setBody(packet.request.getMethodName() + packet.request.getUrl());
    });
    server.setRequest("/Documents", [&](WeakClient node, Err code, HttpPacket & packet){
        std::cout << "Server.OnRequest(/Documents)\n";
        ++on_request_doc;
        packet.response.setStatus(200);
        packet.response.setReason("OK");
        packet.response.setBody(packet.request.getMethodName() + packet.request.getUrl());
    });
    server.setRequest("GET", "/Downloads", [&](WeakClient node, Err code, HttpPacket & packet){
        std::cout << "Server.OnRequest([GET]/Downloads)\n";
        ++on_request_down_get;
        packet.response.setStatus(200);
        packet.response.setReason("OK");
        packet.response.setBody(packet.request.getMethodName() + packet.request.getUrl());
    });
    server.setRequest("POST", "/Downloads", [&](WeakClient node, Err code, HttpPacket & packet){
        std::cout << "Server.OnRequest([POST]/Downloads)\n";
        ++on_request_down_post;
        packet.response.setStatus(200);
        packet.response.setReason("OK");
        packet.response.setBody(packet.request.getMethodName() + packet.request.getUrl());
    });

    server.setOnHttpClose([&](WeakClient node){
        ++on_close;
        if (on_close == 5) {
            server.close();
        }
    });

    Err server_result = Err::E_UNKNOWN;
    Err client_result = Err::E_UNKNOWN;

    HttpResponse response;
    HttpResponse response_doc_get;
    HttpResponse response_doc_post;
    HttpResponse response_down_get;
    HttpResponse response_down_post;

    HttpRequest request_get;
    request_get.method = "GET";
    HttpRequest request_post;
    request_post.method = "POST";

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client1_thread([&](){ client_result = http::requestWithSync(request_url, 1000, response); });
    std::thread client2_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOC, request_get, 1000, response_doc_get); });
    std::thread client3_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOC, request_post, 1000, response_doc_post); });
    std::thread client4_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOWN, request_get, 1000, response_down_get); });
    std::thread client5_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOWN, request_post, 1000, response_down_post); });

    client1_thread.join();
    client2_thread.join();
    client3_thread.join();
    client4_thread.join();
    client5_thread.join();
    server_thread.join();

    ASSERT_EQ(200, response.status);
    ASSERT_EQ(200, response_doc_get.status);
    ASSERT_EQ(200, response_doc_post.status);
    ASSERT_EQ(200, response_down_get.status);
    ASSERT_EQ(200, response_down_post.status);

    ASSERT_EQ(5, on_open             );
    ASSERT_EQ(1, on_request          );
    ASSERT_EQ(2, on_request_doc      );
    ASSERT_EQ(1, on_request_down_get );
    ASSERT_EQ(1, on_request_down_post);
    ASSERT_EQ(5, on_close            );
}

TEST(NetworkHttpTest, WebSocketEcho)
{
    log::SeverityGuard guard;

    uvpp::Loop loop;
    FuncHttpServer server(loop);
    server.enableWebSocket();

    ASSERT_EQ(Err::E_SUCCESS, server.init(details::ANY_IPV4, 0));
    int const SERVER_PORT = server.port();
    ASSERT_LT(0, SERVER_PORT);
    std::cout << "WebSocket Server bind: ws://localhost:" << SERVER_PORT << "/" << std::endl;

    server.setOnWsOpen([&](WeakClient node, Err code, HttpPacket & packet){
        std::cout << "Server.OnWebSocketOpen(" << getErrName(code)
                  << ")\nRequest:\n" << packet.request.toDebugString()
                  << "\nResponse:\n" << packet.response.toResponseDebugString()
                  << std::endl;
    });
    server.setOnWsMessage([&](WeakClient node, OpCode op, char const * buffer, std::size_t size){
        server.writeText(node, std::string(buffer, buffer + size));
        std::cout << "Server.OnWebSocketMessage(" << getOpCodeName(op) << ")\n";
    });
    server.setOnHttpClose([&](WeakClient node){
        std::cout << "Server.OnClose\n";
    });

    FuncWsClient client(loop);
    HttpBuilder builder;
    Uri const URI(std::string("ws://localhost:") + std::to_string(SERVER_PORT));
    builder.setMethod(getHttpMethodName(HttpMethod::M_GET));
    builder.setUrl(URI.getRequestPath());
    builder.insertHeader(HEADER_HOST, URI.getHost());
    builder.insertHeader(HEADER_ORIGIN, URI.getHost());
    client.setup(builder);
    ASSERT_EQ(Err::E_SUCCESS, client.init("127.0.0.1", SERVER_PORT));

    int ws_open_counter = 0;
    int ws_message_counter = 0;
    int ws_error_counter = 0;
    int ws_close_counter = 0;

    std::string const TEST_TEXT = "ECHO MESSAGE";
    client.setOnWsOpen([&](HttpResponse const & response){
        ASSERT_EQ(Err::E_SUCCESS, client.writeText(TEST_TEXT));
        ws_open_counter++;
    });
    client.setOnWsMessage([&](OpCode op, char const * buffer, std::size_t size){
        ASSERT_EQ(OpCode::OC_TEXT_FRAME, op);
        ASSERT_EQ(TEST_TEXT, std::string(buffer, buffer + size));
        ASSERT_EQ(Err::E_SUCCESS, client.closeWebSocket());
        ws_message_counter++;
    });
    client.setOnWsError([&](Err code){
        ws_error_counter++;
    });
    client.setOnWsClose([&](uint16_t code, std::string const & reason){
        ASSERT_EQ(getWsStatusCodeNumber(WsStatusCode::WSSC_NORMAL_CLOSURE), code);
        ASSERT_EQ(std::string(getWsStatusCodeReason(WsStatusCode::WSSC_NORMAL_CLOSURE)), reason);
        ASSERT_EQ(Err::E_SUCCESS, server.close());
        ws_close_counter++;
    });

    ASSERT_EQ(Err::E_SUCCESS, loop.run());
    ASSERT_EQ(1, ws_open_counter);
    ASSERT_EQ(1, ws_message_counter);
    ASSERT_EQ(0, ws_error_counter);
    ASSERT_EQ(1, ws_close_counter);
}

TEST(NetworkHttpTest, MultipleWebSocketClients)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::WARNING_SEVERITY);

    uvpp::Loop loop;
    FuncHttpServer server(loop);
    server.enableWebSocket();

    ASSERT_EQ(Err::E_SUCCESS, server.init(details::ANY_IPV4, 0));
    int const SERVER_PORT = server.port();
    ASSERT_LT(0, SERVER_PORT);
    std::cout << "WebSocket Server bind: ws://localhost:" << SERVER_PORT << "/" << std::endl;

    int const TEST_CLIENT_COUNT =  90;
    int const TEST_ECHO_COUNT   = 120;
    int const TOTAL_WRITE_COUNT = TEST_CLIENT_COUNT * (TEST_ECHO_COUNT + 2/*HTTP_REQUEST&WS_CLOSE*/);

    int server_on_write_count       = 0;
    int server_on_ws_open_count     = 0;
    int server_on_ws_message_count  = 0;
    int server_on_http_close        = 0;

    server.setOnHttpWrite([&](WeakClient node, Err code){
        //std::cout << "server.setOnHttpWrite()" << std::endl;
        ++server_on_write_count;
    });
    server.setOnWsOpen([&](WeakClient node, Err code, HttpPacket & packet){
        //std::cout << "server.setOnWsOpen()" << std::endl;
        ++server_on_ws_open_count;
    });
    server.setOnWsMessage([&](WeakClient node, OpCode op, char const * buffer, std::size_t size){
        //std::cout << "server.setOnWsMessage()" << std::endl;
        ASSERT_EQ(OpCode::OC_TEXT_FRAME, op);
        ++server_on_ws_message_count;
        for (int i = 0; i < TEST_ECHO_COUNT; ++i) {
            server.writeText(node, std::string(buffer, buffer + size) + std::to_string(i));
        }
    });
    server.setOnHttpClose([&](WeakClient node){
        //std::cout << "server.setOnHttpClose()" << std::endl;
        ++server_on_http_close;
    });

    std::vector<std::shared_ptr<FuncWsClient> > clients;
    std::vector<int> ws_open_counter   (TEST_CLIENT_COUNT, 0);
    std::vector<int> ws_message_counter(TEST_CLIENT_COUNT, 0);
    std::vector<int> ws_error_counter  (TEST_CLIENT_COUNT, 0);
    std::vector<int> ws_close_counter  (TEST_CLIENT_COUNT, 0);
    int ws_total_close_counter = 0;

    for (int i = 0; i < TEST_CLIENT_COUNT; ++i) {
        std::shared_ptr<FuncWsClient> shared(new FuncWsClient(loop));
        clients.push_back(shared);
        auto & client = *shared;

        HttpBuilder builder;
        Uri const URI(std::string("ws://localhost:") + std::to_string(SERVER_PORT));
        builder.setMethod(getHttpMethodName(HttpMethod::M_GET));
        builder.setUrl(URI.getRequestPath());
        builder.insertHeader(HEADER_HOST, URI.getHost());
        builder.insertHeader(HEADER_ORIGIN, URI.getHost());
        client.setup(builder);
        ASSERT_EQ(Err::E_SUCCESS, client.init("127.0.0.1", SERVER_PORT));

        char const TEST_TEXT[] = "abcdefghijklmnopqrstuvwxyz";

        client.setOnWsOpen([&, i](HttpResponse const & response){
            //std::cout << "client.setOnWsOpen(" << i << ")" << std::endl;
            ASSERT_EQ(Err::E_SUCCESS, client.writeText(TEST_TEXT));
            ++(ws_open_counter.at(i));
        });
        client.setOnWsMessage([&, i](OpCode op, char const * buffer, std::size_t size){
            //std::cout << "client.setOnWsMessage(" << i << ")" << std::endl;
            ASSERT_EQ(OpCode::OC_TEXT_FRAME, op);
            ASSERT_EQ(std::string(TEST_TEXT) + std::to_string(ws_message_counter.at(i)), std::string(buffer, buffer + size));
            ++(ws_message_counter.at(i));
            if (ws_message_counter.at(i) >= TEST_ECHO_COUNT) {
                ASSERT_EQ(Err::E_SUCCESS, client.closeWebSocket());
            }
        });
        client.setOnWsError([&, i](Err code){
            //std::cout << "client.setOnWsError(" << i << ")" << std::endl;
            ++(ws_error_counter.at(i));
        });
        client.setOnWsClose([&, i](uint16_t code, std::string const & reason){
            //std::cout << "client.setOnWsClose(" << i << ")" << std::endl;
            ++(ws_close_counter.at(i));
            ++ws_total_close_counter;
            if (ws_total_close_counter >= TEST_CLIENT_COUNT) {
                server.close();
            }
        });
    }

    ASSERT_EQ(Err::E_SUCCESS, loop.run());

    ASSERT_EQ(TEST_CLIENT_COUNT, server_on_ws_open_count);
    ASSERT_EQ(TEST_CLIENT_COUNT, server_on_ws_message_count);
    ASSERT_EQ(TEST_CLIENT_COUNT, server_on_http_close);

    for (int i = 0; i < TEST_CLIENT_COUNT; ++i) {
        ASSERT_EQ(1, ws_open_counter[i]);
        ASSERT_EQ(TEST_ECHO_COUNT, ws_message_counter[i]);
        ASSERT_EQ(0, ws_error_counter[i]);
        ASSERT_EQ(1, ws_close_counter[i]);
    }
    ASSERT_EQ(TEST_CLIENT_COUNT, ws_total_close_counter);
    EXPECT_EQ(TOTAL_WRITE_COUNT, server_on_write_count);
}

