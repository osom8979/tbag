/**
 * @file   realTest.cpp
 * @brief  real header tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/math/real.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(realTest, isFloatingPointEquals)
{
    ASSERT_TRUE(math::isFloatingPointEquals(0.015, 0.017, 0.1));
}

