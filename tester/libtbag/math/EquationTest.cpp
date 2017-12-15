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

// ---------------
namespace __impl {
// ---------------

inline bool testIsCross(double p11x, double p11y, double p12x, double p12y, double p21x, double p21y, double p22x, double p22y)
{
    using namespace libtbag::geometry;
    Pointd const P11(p11x, p11y);
    Pointd const P12(p12x, p12y);
    Pointd const P21(p21x, p21y);
    Pointd const P22(p22x, p22y);
    Pointd cross;
    return isCross<double, Pointd>(P11, P12, P21, P22, cross);
}

// ------------------
} // namespace __impl
// ------------------

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
    ASSERT_TRUE(__impl::testIsCross(108,  53, 185, 144, 113.5, 61.5, 115.5, 61.5));
    ASSERT_TRUE(__impl::testIsCross(201, 140, 113,  42, 135.5, 67.5, 137  , 67.5));
}

