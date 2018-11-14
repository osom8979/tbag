/**
 * @file   MsgPacketTest.cpp
 * @brief  MsgPacket class tester.
 * @author zer0
 * @date   2018-11-14
 */

#include <gtest/gtest.h>
#include <libtbag/proto/MsgPacket.hpp>

using namespace libtbag;
using namespace libtbag::proto;

TEST(MsgPacketTest, Default)
{
    MsgPacket::MqEvent const TEST_EVENT = MsgPacket::MqEvent::ME_MSG;
    std::string const TEST_VALUE = "TEST_MSG";

    MsgPacket::MqMsg msg1;
    msg1.event = TEST_EVENT;
    msg1.data.assign(TEST_VALUE.begin(), TEST_VALUE.end());

    MsgPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(msg1));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    MsgPacket::MqMsg msg2;
    std::size_t computed_size = 0;
    ASSERT_EQ(Err::E_SUCCESS, packet.parse(BUILD_BUFFER, &msg2, &computed_size));
    ASSERT_EQ(BUILD_BUFFER.size(), computed_size);
    ASSERT_EQ(TEST_EVENT, msg2.event);
    ASSERT_EQ(TEST_VALUE, std::string(msg2.data.begin(), msg2.data.end()));
}

