/**
 * @file   LogMsgPacketTest.cpp
 * @brief  LogMsgPacket class tester.
 * @author zer0
 * @date   2016-07-16
 * @date   2016-11-14 (MsgPacket -> LogMsgPacket)
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/LogMsgPacket.hpp>

#include <iostream>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(LogMsgPacketTest, Operator)
{
    LogMsgPacket packet;

    packet << 100 << 'c';
    ASSERT_STREQ("100c", packet.c_str());
    packet.clear();

    packet << "TEST";
    ASSERT_STREQ("TEST", packet.c_str());
    packet << std::string("TEMP");
    ASSERT_STREQ("TESTTEMP", packet.c_str());
    packet.clear();

    int test = 0;
    packet << &test;
    ASSERT_LT(0U, std::stoull(std::string(packet.c_str())));
    packet.clear();
}
