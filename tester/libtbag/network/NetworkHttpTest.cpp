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
#include <libtbag/network/http/HttpBuilder.hpp>
#include <libtbag/network/http/HttpProperty.hpp>
#include <libtbag/network/http/FunctionalHttpServer.hpp>
#include <libtbag/network/http/FunctionalWsClient.hpp>
#include <libtbag/network/http/SimpleHttpClient.hpp>
#include <libtbag/uvpp/func/FunctionalTimer.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <iostream>
#include <thread>
#include <atomic>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

using WeakClient = HttpServer::WeakClient;
using HttpPacket = HttpServer::HttpPacket;
using WsPacket   = HttpServer::WsPacket;

//#define MASSIVE_NETWORK_HTTP_TEST

TEST(NetworkHttpTest, HttpClient)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);

    common::HttpProperty response;
    auto result = http::requestWithSync("http://osom8979.github.io", response, 10000);
    ASSERT_EQ(Err::E_SUCCESS, result);
    ASSERT_EQ(200, response.getStatusCode());
}

static bool runSimpleServerTest(HttpServer::StreamType type, std::string const & bind, std::string const & method)
{
    uvpp::Loop loop;
    FuncHttpServer server(loop, type);

    server.init(bind.c_str());

    int on_open    = 0;
    int on_request = 0;
    int on_close   = 0;

    server.set_onHttpOpen([&](WeakClient node){
        ++on_open;
    });
    server.set_onHttpRequest([&](WeakClient node, Err code, HttpPacket & packet){
        ++on_request;
        packet.response.setStatus(200);
        packet.response.setReason("OK");
        packet.response.setBody(packet.request.getMethodName());
    });
    server.set_onHttpClose([&](WeakClient node){
        ++on_close;
        server.close();
    });

    Err server_result = Err::E_UNKNOWN;
    Err client_result = Err::E_UNKNOWN;

    common::HttpProperty response;
    common::HttpProperty request;
    request.setMethod(method);

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client_thread([&](){
        if (HttpServer::StreamType::PIPE == type) {
            client_result = http::requestWithSync(Uri(bind), request, response, 1000, type);
        } else {
            std::string request_url = "http://" + bind + ":";
            request_url += std::to_string(server.port());
            request_url += "/";

            client_result = http::requestWithSync(request_url, request, response, 1000);
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

    if (200 != response.getStatusCode()) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Response is not OK({}).", method, response.getStatusCode());
        return false;
    }

    if (response.getBodyString() != method) {
        tDLogA("NetworkHttpTest.runSimpleServerTest({}) Response body error({}).", method, response.getBodyString());
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

    server.set_onHttpOpen([&](WeakClient node){
        ++on_open;
    });

    server.set_onHttpRequest([&](WeakClient node, Err code, HttpPacket & packet){
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

    server.set_onHttpClose([&](WeakClient node){
        ++on_close;
        if (on_close == 5) {
            server.close();
        }
    });

    Err server_result = Err::E_UNKNOWN;
    Err client_result = Err::E_UNKNOWN;

    common::HttpProperty response;
    common::HttpProperty response_doc_get;
    common::HttpProperty response_doc_post;
    common::HttpProperty response_down_get;
    common::HttpProperty response_down_post;

    common::HttpProperty request_get;
    request_get.setMethod("GET");
    common::HttpProperty request_post;
    request_post.setMethod("POST");

    std::thread server_thread([&](){ server_result = loop.run(); });
    std::thread client1_thread([&](){ client_result = http::requestWithSync(request_url, response, 1000); });
    std::thread client2_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOC , request_get , response_doc_get  , 1000); });
    std::thread client3_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOC , request_post, response_doc_post , 1000); });
    std::thread client4_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOWN, request_get , response_down_get , 1000); });
    std::thread client5_thread([&](){ client_result = http::requestWithSync(REQUEST_URL_DOWN, request_post, response_down_post, 1000); });

    client1_thread.join();
    client2_thread.join();
    client3_thread.join();
    client4_thread.join();
    client5_thread.join();
    server_thread.join();

    ASSERT_EQ(200, response.getStatusCode());
    ASSERT_EQ(200, response_doc_get.getStatusCode());
    ASSERT_EQ(200, response_doc_post.getStatusCode());
    ASSERT_EQ(200, response_down_get.getStatusCode());
    ASSERT_EQ(200, response_down_post.getStatusCode());

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

    server.set_onWsOpen([&](WeakClient node, Err code, HttpPacket & packet){
        std::cout << "Server.OnWebSocketOpen(" << getErrName(code)
                  << ")\nRequest:\n" << packet.request.toDebugString()
                  << "\nResponse:\n" << packet.response.toResponseDebugString()
                  << std::endl;
    });
    server.set_onWsMessage([&](WeakClient node, WsOpCode op, char const * buffer, std::size_t size){
        server.writeText(node, std::string(buffer, buffer + size));
        std::cout << "Server.OnWebSocketMessage(" << getWsOpCodeName(op) << ")\n";
    });
    server.set_onHttpClose([&](WeakClient node){
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
    client.setOnWsOpen([&](http::HttpResponse const & response){
        ASSERT_EQ(Err::E_SUCCESS, client.writeText(TEST_TEXT));
        ws_open_counter++;
    });
    client.setOnWsMessage([&](WsOpCode op, char const * buffer, std::size_t size){
        ASSERT_EQ(WsOpCode::WSOC_TEXT_FRAME, op);
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

    uvpp::Loop server_loop;
    FuncHttpServer server(server_loop);
    server.enableWebSocket();

    ASSERT_EQ(Err::E_SUCCESS, server.init(details::ANY_IPV4, 0));
    int const SERVER_PORT = server.port();
    ASSERT_LT(0, SERVER_PORT);
    std::cout << "WebSocket Server bind: ws://localhost:" << SERVER_PORT << "/" << std::endl;

#if defined(MASSIVE_NETWORK_HTTP_TEST)
    int const TEST_CLIENT_COUNT   = 80;
    int const TEST_ECHO_COUNT     = 180;
    int const TEST_SERVER_TIMEOUT = 5 * 1000;
#define _MASSIVE_TEST_DATA "0123456789-ABCDEFGHIJKLMNOPQRSTUVWXYZ-abcdefghijklmnopqrstuvwxyz"
    char const TEST_TEXT[] = _MASSIVE_TEST_DATA _MASSIVE_TEST_DATA _MASSIVE_TEST_DATA _MASSIVE_TEST_DATA;
#undef _MASSIVE_TEST_DATA
#else
    int const TEST_CLIENT_COUNT   = 40;
    int const TEST_ECHO_COUNT     = 90;
    int const TEST_SERVER_TIMEOUT = 2 * 1000;
    char const TEST_TEXT[] = "abcdefghijklmnopqrstuvwxyz";
#endif
    int const TOTAL_WRITE_COUNT = TEST_CLIENT_COUNT * (TEST_ECHO_COUNT + 2/*HTTP_REQUEST&WS_CLOSE*/);

    int server_on_write_count       = 0;
    int server_on_ws_open_count     = 0;
    int server_on_ws_message_count  = 0;
    int server_on_http_close        = 0;
    Err server_loop_result = Err::E_UNKNOWN;

    bool server_timeout = false;

    auto timer = server_loop.newHandle<uvpp::func::FuncTimer>(server_loop);
    timer->setOnTimer([&](){
        server_timeout = true;
        server.close();
        timer->close();
    });
    timer->start(TEST_SERVER_TIMEOUT);

    server.set_onHttpWrite([&](WeakClient node, Err code){
        ++server_on_write_count;
    });
    server.set_onWsOpen([&](WeakClient node, Err code, HttpPacket & packet){
        ++server_on_ws_open_count;
    });
    server.set_onWsMessage([&](WeakClient node, WsOpCode op, char const * buffer, std::size_t size){
        if (op == WsOpCode::WSOC_TEXT_FRAME) {
            Err const write_code = server.writeText(node, std::string(buffer, buffer + size));
            if (isWsWriteSuccess(write_code)) {
                ++server_on_ws_message_count;
            }
        }
    });
    server.set_onHttpClose([&](WeakClient node){
        ++server_on_http_close;
        if (timer->isActive()) {
            timer->stop();
        }
        if (timer->isClosing() == false) {
            timer->close();
        }
    });

    std::thread server_thread([&](){
        server_loop_result = server_loop.run();
    });

    std::vector<uvpp::Loop>  client_loops(TEST_CLIENT_COUNT);
    std::vector<std::thread> client_threads(TEST_CLIENT_COUNT);
    std::vector<std::thread> client_write_threads(TEST_CLIENT_COUNT);

    using SharedFuncWsClient = std::shared_ptr<FuncWsClient>;
    std::vector<SharedFuncWsClient> clients;

    std::vector<Err> ws_loop_result    (TEST_CLIENT_COUNT, Err::E_UNKNOWN);
    std::vector<int> ws_open_counter   (TEST_CLIENT_COUNT, 0);
    std::vector<int> ws_message_counter(TEST_CLIENT_COUNT, 0);
    std::vector<int> ws_error_counter  (TEST_CLIENT_COUNT, 0);
    std::vector<int> ws_close_counter  (TEST_CLIENT_COUNT, 0);
    std::vector<int> ws_write_counter  (TEST_CLIENT_COUNT, 0);

    std::atomic_int ws_total_close_counter;
    ws_total_close_counter.store(0);

    for (int i = 0; i < TEST_CLIENT_COUNT; ++i) {
        clients.emplace_back(new FuncWsClient(client_loops[i]));
    }

    ASSERT_EQ(TEST_CLIENT_COUNT, clients.size());

    for (int i = 0; i < TEST_CLIENT_COUNT; ++i) {
        auto client = clients[i];
        ASSERT_TRUE(static_cast<bool>(client));

        HttpBuilder builder;
        Uri const URI(std::string("ws://localhost:") + std::to_string(SERVER_PORT));
        builder.setMethod(getHttpMethodName(HttpMethod::M_GET));
        builder.setUrl(URI.getRequestPath());
        builder.insertHeader(HEADER_HOST, URI.getHost());
        builder.insertHeader(HEADER_ORIGIN, URI.getHost());
        client->setup(builder);
        ASSERT_EQ(Err::E_SUCCESS, client->init("127.0.0.1", SERVER_PORT));

        client->setOnWsOpen([&, i](http::HttpResponse const & response){
            auto shared_client = clients[i];
            ASSERT_TRUE(static_cast<bool>(shared_client));
            ++(ws_open_counter.at(i));
            client_write_threads[i] = std::thread([&, i](){
                for (int echo_count = 0; echo_count < TEST_ECHO_COUNT; ++echo_count) {
                    Err write_code = clients[i]->writeText(TEST_TEXT);
                    if (isWsWriteSuccess(write_code)) {
                        ++(ws_write_counter.at(i));
                    } else {
                        tDLogE("client[{}]->writeText() {} error", i, getErrName(write_code));
                    }
                }
            });
        });
        client->setOnWsMessage([&, i](WsOpCode op, char const * buffer, std::size_t size){
            auto shared_client = clients[i];
            ASSERT_TRUE(static_cast<bool>(shared_client));
            ASSERT_EQ(WsOpCode::WSOC_TEXT_FRAME, op);
            ASSERT_EQ(std::string(TEST_TEXT), std::string(buffer, buffer + size));
            ++(ws_message_counter.at(i));
            if (ws_message_counter.at(i) >= TEST_ECHO_COUNT) {
                ASSERT_EQ(Err::E_SUCCESS, clients[i]->closeWebSocket());
            }
        });
        client->setOnWsError([&, i](Err code){
            auto shared_client = clients[i];
            ASSERT_TRUE(static_cast<bool>(shared_client));
            ++(ws_error_counter.at(i));
        });
        client->setOnWsClose([&, i](uint16_t code, std::string const & reason){
            auto shared_client = clients[i];
            ASSERT_TRUE(static_cast<bool>(shared_client));
            ++(ws_close_counter.at(i));
            ++ws_total_close_counter;
            if (ws_total_close_counter >= TEST_CLIENT_COUNT) {
                server.close();
            }
        });

        client_threads[i] = std::thread([&, i](){
            ws_loop_result[i] = client_loops[i].run();
        });
    }

    server_thread.join();
    for (int i = 0; i < TEST_CLIENT_COUNT; ++i) {
        client_threads[i].join();
        client_write_threads[i].join();
    }

    ASSERT_EQ(TEST_CLIENT_COUNT, server_on_ws_open_count);
    ASSERT_EQ(TEST_CLIENT_COUNT * TEST_ECHO_COUNT, server_on_ws_message_count);
    ASSERT_EQ(TEST_CLIENT_COUNT, server_on_http_close);

    for (int i = 0; i < TEST_CLIENT_COUNT; ++i) {
        ASSERT_EQ(Err::E_SUCCESS, ws_loop_result[i]);
        ASSERT_EQ(1, ws_open_counter[i]);
        ASSERT_EQ(TEST_ECHO_COUNT, ws_write_counter[i]);
        ASSERT_EQ(TEST_ECHO_COUNT, ws_message_counter[i]);
        ASSERT_EQ(0, ws_error_counter[i]);
        ASSERT_EQ(1, ws_close_counter[i]);
    }
    ASSERT_EQ(TEST_CLIENT_COUNT, ws_total_close_counter);
    EXPECT_EQ(TOTAL_WRITE_COUNT, server_on_write_count);
    EXPECT_FALSE(server_timeout);
}

