/**
 * @file   mathTest.cpp
 * @brief  math header tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/math/math.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(math_CommonTest, isFloatingPointEquals)
{
    ASSERT_TRUE(math::isFloatingPointEquals(0.015, 0.017, 0.1));
}

TEST(math_CommonTest, scale)
{
    ASSERT_EQ(math::scale(0, 100, -1, 1, 50), 0);
}

