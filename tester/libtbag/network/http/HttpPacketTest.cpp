/**
 * @file   HttpPacketTest.cpp
 * @brief  HttpPacket class tester.
 * @author zer0
 * @date   2017-10-02
 */

#include <gtest/gtest.h>
#include <libtbag/network/http/HttpPacket.hpp>

using namespace libtbag;
using namespace libtbag::network;
using namespace libtbag::network::http;

TEST(HttpPacketTest, Default)
{
    HttpPacket object;
    ASSERT_TRUE(true);
}

