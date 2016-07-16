/**
 * @file   MsgPacketTest.cpp
 * @brief  MsgPacket class tester.
 * @author zer0
 * @date   2016-07-16
 */

#include <gtest/gtest.h>
#include <libtbag/log/details/MsgPacket.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::details;

TEST(MsgPacketTest, Operator)
{
    MsgPacket packet;

    packet << 'c';
    ASSERT_STREQ(packet.getStringPointer(), "c");
    packet.clearString();

    packet << 100;
    ASSERT_STREQ(packet.getStringPointer(), "100");
    packet.clearString();

    packet << "TEST";
    ASSERT_STREQ(packet.getStringPointer(), "TEST");
    packet << std::string("TEMP");
    ASSERT_STREQ(packet.getStringPointer(), "TESTTEMP");
    packet.clearString();

    int test = 0;
    packet << &test;
    ASSERT_GT(std::stoul(packet.getStringPointer()), 0U);
    packet.clearString();
}

