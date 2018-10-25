/**
 * @file   TbagPacketTest.cpp
 * @brief  TbagPacket class tester.
 * @author zer0
 * @date   2018-10-24
 */

#include <gtest/gtest.h>
#include <libtbag/proto/TbagPacket.hpp>

using namespace libtbag;
using namespace libtbag::proto;

TEST(TbagPacketTest, Default)
{
    uint64_t const TEST_ID   = 10;
    int32_t  const TEST_TYPE = 20;
    int32_t  const TEST_CODE = 30;

    TbagPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.update(BUILD_BUFFER));
    ASSERT_EQ(TEST_ID, packet.id());
    ASSERT_EQ(TEST_TYPE, packet.type());
    ASSERT_EQ(TEST_CODE, packet.code());
    ASSERT_TRUE(packet.bags().empty());
}

TEST(TbagPacketTest, Assign)
{
    uint64_t const TEST_ID   = 1;
    int32_t  const TEST_TYPE = 2;
    int32_t  const TEST_CODE = 3;

    TbagPacket packet;
    ASSERT_EQ(Err::E_SUCCESS, packet.build(TEST_ID, TEST_TYPE, TEST_CODE));
    auto const BUILD_BUFFER1 = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER1.empty());

    ASSERT_EQ(Err::E_SUCCESS, packet.assign(BUILD_BUFFER1));
    auto const BUILD_BUFFER2 = packet.toBuffer();
    ASSERT_FALSE(BUILD_BUFFER2.empty());
    ASSERT_TRUE(std::equal(BUILD_BUFFER1.begin(), BUILD_BUFFER1.end(), BUILD_BUFFER2.begin(), BUILD_BUFFER2.end()));
}

