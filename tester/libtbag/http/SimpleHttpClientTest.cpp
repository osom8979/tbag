/**
 * @file   SimpleHttpClientTest.cpp
 * @brief  SimpleHttpClient class tester.
 * @author zer0
 * @date   2019-01-14
 */

#include <gtest/gtest.h>
#include <libtbag/http/SimpleHttpClient.hpp>
#include <libtbag/log/Log.hpp>

//#define __PRINT_RESPONSE_STRING

using namespace libtbag;
using namespace libtbag::http;

TEST(SimpleHttpClientTest, HttpRequest)
{
    libtbag::log::SeverityGuard guard;
    auto const RESPONSE = requestGet("http://osom8979.github.io");
    ASSERT_EQ(200, RESPONSE.code);

#if defined(__PRINT_RESPONSE_STRING)
    std::cout << RESPONSE.toDebugResponseString() << std::endl
              << "[Body]" << std::endl
              << RESPONSE.getBodyString() << std::endl;
#endif
}

TEST(SimpleHttpClientTest, HttpsRequest)
{
    libtbag::log::SeverityGuard guard;
    auto const RESPONSE = requestGet("https://osom8979.github.io");
    ASSERT_EQ(200, RESPONSE.code);

#if defined(__PRINT_RESPONSE_STRING)
    std::cout << RESPONSE.toDebugResponseString() << std::endl
              << "[Body]" << std::endl
              << RESPONSE.getBodyString() << std::endl;
#endif
}

