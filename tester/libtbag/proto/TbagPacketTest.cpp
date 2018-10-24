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
    TbagPacket object;
    ASSERT_TRUE(true);
}

