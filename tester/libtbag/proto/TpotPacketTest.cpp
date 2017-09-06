/**
 * @file   TpotPacketTest.cpp
 * @brief  TpotPacket class tester.
 * @author zer0
 * @date   2017-09-06
 */

#include <gtest/gtest.h>
#include <libtbag/proto/TpotPacket.hpp>

using namespace libtbag;
using namespace libtbag::proto;

TEST(TpotPacketTest, Default)
{
    TpotPacket object;
    ASSERT_TRUE(true);
}

