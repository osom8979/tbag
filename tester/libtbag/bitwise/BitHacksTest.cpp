/**
 * @file   BitHacksTest.cpp
 * @brief  BitHacks class tester.
 * @author zer0
 * @date   2018-11-09
 */

#include <gtest/gtest.h>
#include <libtbag/bitwise/BitHacks.hpp>

using namespace libtbag;
using namespace libtbag::bitwise;

TEST(BitHacksTest, isPowerOf2)
{
    ASSERT_TRUE(isPowerOf2(1));
    ASSERT_TRUE(isPowerOf2(2));
    ASSERT_TRUE(isPowerOf2(4));
    ASSERT_TRUE(isPowerOf2(8));
    ASSERT_TRUE(isPowerOf2(16));
    ASSERT_TRUE(isPowerOf2(32));
    ASSERT_TRUE(isPowerOf2(64));
    ASSERT_TRUE(isPowerOf2(128));

    ASSERT_FALSE(isPowerOf2(3));
    ASSERT_FALSE(isPowerOf2(5));
    ASSERT_FALSE(isPowerOf2(7));
    ASSERT_FALSE(isPowerOf2(10));
    ASSERT_FALSE(isPowerOf2(17));
    ASSERT_FALSE(isPowerOf2(30));
    ASSERT_FALSE(isPowerOf2(80));
    ASSERT_FALSE(isPowerOf2(140));
}

TEST(BitHacksTest, findMostSignificantBit)
{
    ASSERT_EQ(1, findMostSignificantBit(1));
    ASSERT_EQ(2, findMostSignificantBit(2));
    ASSERT_EQ(3, findMostSignificantBit(4));
    ASSERT_EQ(4, findMostSignificantBit(8));
    ASSERT_EQ(5, findMostSignificantBit(16));
    ASSERT_EQ(6, findMostSignificantBit(32));
    ASSERT_EQ(7, findMostSignificantBit(64));
    ASSERT_EQ(8, findMostSignificantBit(128));

    ASSERT_EQ(2, findMostSignificantBit(3));
    ASSERT_EQ(3, findMostSignificantBit(5));
    ASSERT_EQ(3, findMostSignificantBit(7));
    ASSERT_EQ(4, findMostSignificantBit(10));
    ASSERT_EQ(5, findMostSignificantBit(17));
    ASSERT_EQ(5, findMostSignificantBit(30));
    ASSERT_EQ(7, findMostSignificantBit(80));
    ASSERT_EQ(8, findMostSignificantBit(140));
}

