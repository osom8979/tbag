/**
 * @file   SimpleHttpServerTest.cpp
 * @brief  SimpleHttpServer class tester.
 * @author zer0
 * @date   2019-01-14
 * @date   2019-01-15 (Rename: SimpleHttpServer -> HttpServer)
 */

#include <gtest/gtest.h>
#include <libtbag/http/HttpServer.hpp>
#include <libtbag/http/SimpleHttpClient.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/log/Log.hpp>

//#define __PRINT_REQUEST_STRING
//#define __PRINT_RESPONSE_STRING

TBAG_CONSTEXPR static int _TEST_SERVER_PORT = 18080;

using namespace libtbag;
using namespace libtbag::http;

TEST(HttpServerTest, Default)
{
    std::string const TEST_BODY = "SUCCESS";

    HttpServer::Callbacks cbs;
    cbs.http_cb = [&](std::intptr_t id, HttpRequest const & request) -> HttpResponse {
#if defined(__PRINT_REQUEST_STRING)
        tDLogI("[SERVER] HTTP {}", request.toDebugRequestString());
#endif
        HttpResponse res;
        res.setHttpStatus(HttpStatus::SC_OK);
        res.setBodyString(TEST_BODY);
        return res;
    };

    libtbag::log::SeverityGuard guard;
    HttpServer server(libtbag::net::ANY_IPV4, _TEST_SERVER_PORT, cbs);

    auto const URL = std::string("http://localhost:") + std::to_string(_TEST_SERVER_PORT);
    auto const RESPONSE = requestGet(URL);
    ASSERT_EQ(200, RESPONSE.code);
    ASSERT_EQ(TEST_BODY, RESPONSE.getBodyString());

#if defined(__PRINT_RESPONSE_STRING)
    std::cout << RESPONSE.toDebugResponseString() << std::endl
              << "[Body]" << std::endl
              << RESPONSE.getBodyString() << std::endl;
#endif
}

TEST(HttpServerTest, Callbacks)
{
    int begin  = 0;
    int end    = 0;
    int accept = 0;
    int http   = 0;
    int close  = 0;
    int error  = 0;

    HttpServer::Callbacks cbs;
    cbs.begin_cb = [&](){
        ++begin;
    };
    cbs.end_cb = [&](){
        ++end;
    };
    cbs.accept_cb = [&](std::intptr_t id, std::string const & peer) -> bool {
        ++accept;
        return true;
    };
    cbs.http_cb = [&](std::intptr_t id, HttpRequest const & request) -> HttpResponse {
        ++http;
        return HttpResponse{};
    };
    cbs.close_cb = [&](std::intptr_t id){
        ++close;
    };
    cbs.error_cb = [&](std::intptr_t id, Err code){
        ++error;
    };

    libtbag::log::SeverityGuard guard;
    HttpServer server(libtbag::net::ANY_IPV4, _TEST_SERVER_PORT, cbs);

    auto const URL = std::string("http://localhost:") + std::to_string(_TEST_SERVER_PORT);
    auto const RESPONSE = requestGet(URL);
    ASSERT_EQ(200, RESPONSE.code);

    ASSERT_EQ(E_SUCCESS, server.exit());
    server.join();

    ASSERT_EQ(1, begin);
    ASSERT_EQ(1, end);
    ASSERT_EQ(1, accept);
    ASSERT_EQ(1, http);
    ASSERT_EQ(1, close);
    ASSERT_EQ(0, error);
}

TEST(HttpServerTest, WebSocket)
{
    std::string const TEST_BODY = "TEST";

    int server_begin    = 0;
    int server_end      = 0;
    int server_accept   = 0;
    int server_continue = 0;
    int server_switch   = 0;
    int server_message  = 0;
    int server_http     = 0;
    int server_close    = 0;
    int server_error    = 0;

    std::unique_ptr<HttpServer> server;
    HttpServer::Callbacks server_cbs;
    server_cbs.begin_cb = [&](){
        ++server_begin;
    };
    server_cbs.end_cb = [&](){
        ++server_end;
    };
    server_cbs.accept_cb = [&](std::intptr_t id, std::string const & peer) -> bool {
        ++server_accept;
        tDLogI("[SERVER] Accept node: {}", id);
        return true;
    };
    server_cbs.continue_cb = [&](std::intptr_t id){
        ++server_continue;
    };
    server_cbs.switch_cb = [&](std::intptr_t id, HttpRequest const & request) -> bool {
        ++server_switch;
        auto const CODE = server->writeWsResponse(id, request);
        tDLogI("[SERVER] Switching protocol: {}, Result: {}", id, CODE);
        return true;
    };
    server_cbs.message_cb = [&](std::intptr_t id, WsOpCode opcode, HttpBuffer const & payload){
        ++server_message;
        auto const CODE = server->writeText(id, payload.data(), payload.size());
        tDLogI("[SERVER] WebSocket write text: {}, Result: {}", id, CODE);
    };
    server_cbs.http_cb = [&](std::intptr_t id, HttpRequest const & request) -> HttpResponse {
        ++server_http;
        return HttpResponse{};
    };
    server_cbs.close_cb = [&](std::intptr_t id){
        ++server_close;
        tDLogI("[SERVER] Close node: {}", id);
    };
    server_cbs.error_cb = [&](std::intptr_t id, Err code){
        ++server_error;
    };

    int client_begin   = 0;
    int client_end     = 0;
    int client_http    = 0;
    int client_switch  = 0;
    int client_message = 0;
    int client_error   = 0;

    std::unique_ptr<HttpClient> client;
    HttpClient::Callbacks client_cbs;

    std::string response_payload;

    client_cbs.begin_cb = [&](){
        ++client_begin;
        auto const CODE = client->writeWsRequest();
        tDLogI("[CLIENT] WebSocket request code: {}", CODE);
    };
    client_cbs.end_cb = [&](){
        ++client_end;
    };
    client_cbs.http_cb = [&](HttpResponse const & response){
        ++client_http;
    };
    client_cbs.switch_cb = [&](HttpResponse const & response) -> bool {
        ++client_switch;
        auto const CODE = client->writeText(TEST_BODY);
        tDLogI("[CLIENT] WebSocket write text: {}", CODE);
        return true;
    };
    client_cbs.message_cb = [&](WsOpCode opcode, HttpBuffer const & payload){
        ++client_message;
        response_payload.assign(payload.begin(), payload.end());
        tDLogI("[CLIENT] WebSocket recv text: {}", response_payload);
        client->exit();
    };
    client_cbs.error_cb = [&](Err code){
        ++client_error;
    };

    libtbag::log::SeverityGuard guard;
    server = std::make_unique<HttpServer>(std::string("ws://0.0.0.0:") + std::to_string(_TEST_SERVER_PORT), server_cbs);
    client = std::make_unique<HttpClient>(std::string("ws://127.0.0.1:") + std::to_string(_TEST_SERVER_PORT), client_cbs);

    client->join();
    ASSERT_EQ(E_SUCCESS, server->exit());
    server->join();

    ASSERT_EQ(1, server_begin);
    ASSERT_EQ(1, server_end);
    ASSERT_EQ(1, server_accept);
    ASSERT_EQ(0, server_http);
    ASSERT_EQ(1, server_close);
    ASSERT_EQ(0, server_error);

    ASSERT_EQ(1, client_begin);
    ASSERT_EQ(1, client_end);
    ASSERT_EQ(0, client_http);
    ASSERT_EQ(1, client_switch);
    ASSERT_EQ(1, client_message);
    ASSERT_EQ(0, client_error);

    ASSERT_EQ(TEST_BODY, response_payload);
}

