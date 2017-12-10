/**
 * @file   EquationTest.cpp
 * @brief  Equation tester.
 * @author zer0
 * @date   2016-08-26
 */

#include <gtest/gtest.h>
#include <libtbag/math/Equation.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(EquationTest, GetLinearEquationWithTwoPoint)
{
    auto e = getLinearEquationWithTwoPoint(1, 1, 2, 2);
    ASSERT_EQ(1, e.a);
    ASSERT_EQ(0, e.b);
}

TEST(EquationTest, IsParallelWithTwoLinearEquation)
{
    LinearEquation<double> e1{2.4, 1};
    LinearEquation<double> e2{2.4, 2};
    ASSERT_TRUE(isParallelWithTwoLinearEquation(e1, e2));
}

TEST(EquationTest, GetIntersectionWithTwoLinearEquation)
{
    LinearEquation<double> e1{ 2, 0};
    LinearEquation<double> e2{-2, 2};
    auto p = getIntersectionWithTwoLinearEquation(e1, e2);
    ASSERT_NEAR(0.5, p.x, 0.1);
    ASSERT_NEAR(1.0, p.y, 0.1);
}

TEST(EquationTest, IsCross)
{
    using namespace libtbag::geometry;

    Pointd const P11(108, 53);
    Pointd const P12(185, 144);
    Pointd const P21(113.5, 61.5);
    Pointd const P22(115.5, 61.5);
    Pointd cross;
    bool const RESULT = isCross<double, Pointd>(P11, P12, P21, P22, cross);
    ASSERT_TRUE(RESULT);
}

