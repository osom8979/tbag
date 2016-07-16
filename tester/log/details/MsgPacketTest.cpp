/**
 * @file   MsgPacketTest.cpp
 * @brief  MsgPacket class tester.
 * @author zer0
 * @date   2016-07-16
 */

#include <gtest/gtest.h>
#include <libtbag/log/details/MsgPacket.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::details;

TEST(MsgPacketTest, Operator)
{
    MsgPacket packet;

    packet << 100 << 'c';
    ASSERT_STREQ(packet.getStringPointer(), "100c");
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

    packet.format("{}", 50);
    ASSERT_EQ(packet.getString(), "50");
    packet.clearString();
}

