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

TEST(equationTest, getLinearEquationWithTwoPoint)
{
    auto e = getLinearEquationWithTwoPoint(1, 1, 2, 2);
    ASSERT_EQ(1, e.a);
    ASSERT_EQ(0, e.b);
}

TEST(equationTest, getIntersectionWithTwoLinearEquation)
{
    LinearEquation<int> e1{ 1, -1};
    LinearEquation<int> e2{-1,  1};
    auto p = getIntersectionWithTwoLinearEquation(e1, e2);
    ASSERT_EQ(1, p.x);
    ASSERT_EQ(0, p.y);
}

