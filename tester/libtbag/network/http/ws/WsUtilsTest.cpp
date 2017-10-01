/**
 * @file   WsUtilsTest.cpp
 * @brief  WsUtils class tester.
 * @author zer0
 * @date   2017-10-01
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/ws/WsUtils.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;
using namespace libtbag::network::http::ws;

TEST(WsUtilsTest, UpgradeWebsocketKey)
{
    std::string const TEST_ORIGINAL = "dGhlIHNhbXBsZSBub25jZQ==";
    std::string const TEST_RESULT   = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
    ASSERT_EQ(TEST_RESULT, getUpgradeWebSocketKey(TEST_ORIGINAL));
}

