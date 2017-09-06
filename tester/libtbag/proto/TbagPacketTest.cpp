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

TEST(TbagPacketTest, Version)
{
    util::Version const TEST_VERSION(1, 2, 3);
    ASSERT_TRUE(TEST_VERSION == toVersion(fromVersion(TEST_VERSION)));
}

TEST(TbagPacketTest, Point)
{
    geometry::Point const TEST_POINT = {1, 2};
    ASSERT_TRUE(TEST_POINT == toPoint(fromPoint(TEST_POINT)));
}

TEST(TbagPacketTest, Size)
{
    geometry::Size const TEST_SIZE = {3, 4};
    ASSERT_TRUE(TEST_SIZE == toSize(fromSize(TEST_SIZE)));
}

TEST(TbagPacketTest, Rect)
{
    geometry::Rect const TEST_RECT = {1, 2, 3, 4};
    ASSERT_TRUE(TEST_RECT == toRect(fromRect(TEST_RECT)));
}

