/**
 * @file   SimpleHttpServerTest.cpp
 * @brief  SimpleHttpServer class tester.
 * @author zer0
 * @date   2019-01-14
 */

#include <gtest/gtest.h>
#include <libtbag/http/SimpleHttpServer.hpp>
#include <libtbag/http/SimpleHttpClient.hpp>
#include <libtbag/log/Log.hpp>

//#define __PRINT_REQUEST_STRING

TBAG_CONSTEXPR static int _TEST_SERVER_PORT = 18080;
TBAG_CONSTEXPR static char const * const _TEST_SERVER_HOST = "0.0.0.0";

using namespace libtbag;
using namespace libtbag::http;

TEST(SimpleHttpServerTest, Default)
{
    std::string const TEST_BODY = "SUCCESS";

    SimpleHttpServer::Callbacks cbs;
    cbs.http_cb = [&](std::intptr_t id, HttpRequest const & request) -> HttpResponse {
#if defined(__PRINT_REQUEST_STRING)
        std::cout << request.toDebugRequestString() << std::endl;
#endif
        HttpResponse res;
        res.setHttpStatus(HttpStatus::SC_OK);
        res.setBodyString(TEST_BODY);
        return res;
    };

    libtbag::log::SeverityGuard guard;
    SimpleHttpServer server(_TEST_SERVER_HOST, _TEST_SERVER_PORT, cbs);

    auto const RESPONSE = requestGet("http://localhost:18080");
    ASSERT_EQ(200, RESPONSE.code);
    ASSERT_EQ(TEST_BODY, RESPONSE.getBodyString());
};

