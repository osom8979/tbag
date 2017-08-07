/**
 * @file   WsFrameBufferTest.cpp
 * @brief  WsFrameBuffer class tester.
 * @author zer0
 * @date   2017-08-07
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/WsFrameBuffer.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(WsFrameBufferTest, Default)
{
    WsFrameBuffer object;
    ASSERT_TRUE(true);
}

