/**
 * @file   PacketGeneratorTest.cpp
 * @brief  PacketGenerator class tester.
 * @author zer0
 * @date   2017-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/log/msg/PacketGenerator.hpp>

using namespace libtbag;
using namespace libtbag::log;
using namespace libtbag::log::msg;

TEST(PacketGeneratorTest, Default)
{
    PacketGenerator object;
    ASSERT_TRUE(true);
}

