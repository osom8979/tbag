/**
 * @file   equationTest.cpp
 * @brief  equation header tester.
 * @author zer0
 * @date   2016-08-26
 */

#include <gtest/gtest.h>
#include <libtbag/math/equation.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(equationTest, Default)
{
    auto e = getLinearEquationWithTwoPoint(1, 1, 2, 2);
    ASSERT_EQ(1, e.a);
    ASSERT_EQ(0, e.b);
}

