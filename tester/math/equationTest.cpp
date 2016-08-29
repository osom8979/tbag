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

TEST(equationTest, isParallelWithTwoLinearEquation)
{
    LinearEquation<double> e1{2.4, 1};
    LinearEquation<double> e2{2.4, 2};
    ASSERT_TRUE(isParallelWithTwoLinearEquation(e1, e2));
}

TEST(equationTest, getIntersectionWithTwoLinearEquation)
{
    LinearEquation<double> e1{ 2, 0};
    LinearEquation<double> e2{-2, 2};
    auto p = getIntersectionWithTwoLinearEquation(e1, e2);
    ASSERT_NEAR(0.5, p.x, 0.1);
    ASSERT_NEAR(1.0, p.y, 0.1);
}

