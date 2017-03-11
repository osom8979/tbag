/**
 * @file   ScaleTest.cpp
 * @brief  Scale header tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/math/Scale.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(ScaleTest, Scale)
{
    ASSERT_EQ(0, math::scale(0, 100, -1, 1, 50));
}

