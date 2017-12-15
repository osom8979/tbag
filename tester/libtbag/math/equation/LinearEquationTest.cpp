/**
 * @file   LinearEquationTest.cpp
 * @brief  LinearEquation class tester.
 * @author zer0
 * @date   2017-12-15
 */

#include <gtest/gtest.h>
#include <libtbag/math/equation/LinearEquation.hpp>

using namespace libtbag;
using namespace libtbag::math;
using namespace libtbag::math::equation;

using IntegerLinearEquation = LinearEquation<int>;
using  DoubleLinearEquation = LinearEquation<double>;

TEST(LinearEquationTest, Default)
{
    IntegerLinearEquation e1;
    DoubleLinearEquation e2;
}

