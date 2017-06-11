/**
 * @file   WebSocketFrameTest.cpp
 * @brief  WebSocketFrame class tester.
 * @author zer0
 * @date   2017-06-11
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/WebSocketFrame.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(WebSocketFrameTest, Default)
{
    WebSocketFrame object;
    ASSERT_TRUE(true);
}

