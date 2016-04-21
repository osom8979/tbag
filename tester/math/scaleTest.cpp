/**
 * @file   scaleTest.cpp
 * @brief  scale header tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/math/scale.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(scaleTest, scale)
{
    ASSERT_EQ(math::scale(0, 100, -1, 1, 50), 0);
}

