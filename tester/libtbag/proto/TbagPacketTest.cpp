/**
 * @file   TbagPacketTest.cpp
 * @brief  TbagPacket class tester.
 * @author zer0
 * @date   2017-09-06
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

