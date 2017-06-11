/**
 * @file   WebSocketParserTest.cpp
 * @brief  WebSocketParser class tester.
 * @author zer0
 * @date   2017-06-11
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/WebSocketParser.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(WebSocketParserTest, UpgradeWebsocketKey)
{
    std::string const TEST_ORIGINAL = "dGhlIHNhbXBsZSBub25jZQ==";
    std::string const TEST_RESULT   = "s3pPLMBiTxaQ9kYGzzhZRbK+xOo=";
    ASSERT_EQ(TEST_RESULT, upgradeWebsocketKey(TEST_ORIGINAL));
}

