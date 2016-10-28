/**
 * @file   RealTest.cpp
 * @brief  Real header tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/math/Real.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(RealTest, IsFloatingPointEquals)
{
    ASSERT_TRUE(math::isFloatingPointEquals(0.015, 0.017, 0.1));
}

