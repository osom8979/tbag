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
#include <libtbag/log/Log.hpp>

#define __PRINT_REQUEST_STRING

TBAG_CONSTEXPR static int _TEST_SERVER_PORT = 18080;
TBAG_CONSTEXPR static char const * const _TEST_SERVER_HOST = "0.0.0.0";

using namespace libtbag;
using namespace libtbag::http;

TEST(HttpServerTest, Default)
{
    std::string const TEST_BODY = "SUCCESS";

    HttpServer::Callbacks cbs;
    cbs.http_cb = [&](std::intptr_t id, HttpRequest const & request) -> HttpResponse {
#if defined(__PRINT_REQUEST_STRING)
        std::cout << request.toDebugRequestString() << std::endl;
#endif
        HttpResponse res;
        res.setHttpStatus(HttpStatus::SC_OK);
        res.setBodyString(TEST_BODY);
        return res;
    };

//    libtbag::log::SeverityGuard guard;
    HttpServer server(_TEST_SERVER_HOST, _TEST_SERVER_PORT, cbs);

    auto const URL = std::string("http://localhost:") + std::to_string(_TEST_SERVER_PORT);
    auto const RESPONSE = requestGet(URL);
    ASSERT_EQ(200, RESPONSE.code);
    ASSERT_EQ(TEST_BODY, RESPONSE.getBodyString());
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
        HttpResponse res;
        res.setHttpStatus(HttpStatus::SC_OK);
        res.setBodyString("OK");
        return res;
    };
    cbs.close_cb = [&](std::intptr_t id){
        ++close;
    };
    cbs.error_cb = [&](std::intptr_t id, Err code){
        ++error;
    };

    libtbag::log::SeverityGuard guard;
    HttpServer server(_TEST_SERVER_HOST, _TEST_SERVER_PORT, cbs);

    auto const URL = std::string("http://localhost:") + std::to_string(_TEST_SERVER_PORT);
    auto const RESPONSE = requestGet(URL);
    ASSERT_EQ(200, RESPONSE.code);

    ASSERT_EQ(Err::E_SUCCESS, server.exit());
    server.join();

    ASSERT_EQ(1, begin);
    ASSERT_EQ(1, end);
    ASSERT_EQ(1, accept);
    ASSERT_EQ(1, http);
    ASSERT_EQ(1, close);
    ASSERT_EQ(0, error);
}

