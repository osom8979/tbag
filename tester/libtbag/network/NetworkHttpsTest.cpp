/**
 * @file   NetworkHttpsTest.cpp
 * @brief  NetworkHttps class tester.
 * @author zer0
 * @date   2018-02-04
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>

#include <libtbag/log/Log.hpp>
#include <libtbag/network/http/HttpsClient.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(NetworkHttpsTest, HttpsClient)
{
    log::SeverityGuard guard(log::TBAG_DEFAULT_LOGGER_NAME, log::INFO_SEVERITY);

    //HttpResponse response;
    //auto result = http::requestWithSync("http://osom8979.github.io", response, 10000);
    //ASSERT_EQ(Err::E_SUCCESS, result);
    //ASSERT_EQ(200, response.code);
}

