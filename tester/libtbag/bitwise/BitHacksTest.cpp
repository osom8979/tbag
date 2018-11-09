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

