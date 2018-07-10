/**
 * @file   EuclideanTest.cpp
 * @brief  Euclidean class tester.
 * @author zer0
 * @date   2018-07-10
 */

#include <gtest/gtest.h>
#include <libtbag/math/Euclidean.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(EuclideanTest, GCD)
{
    ASSERT_EQ(24, gcd(192, 72));
}

TEST(EuclideanTest, LCM)
{
    ASSERT_EQ(576, lcm(192, 72));
}

