/**
 * @file   TrigonometryTest.cpp
 * @brief  Trigonometry header tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/math/Trigonometry.hpp>
#include <libtbag/math/Constant.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(TrigonometryTest, Default)
{
    /*
     *                   C
     *                   *
     *                  /|
     *  h: hypotenuse  / |
     *                /  | a: opposite
     *               /   |
     *            A *----* B: Right angle (90-degree)
     *          b: adjacent
     */

    float opposite   = 1; // a
    float adjacent   = 1; // b
    float hypotenuse = static_cast<float>(MATH_CONSTANT_SQRT2); // h

    ASSERT_NEAR(MATH_CONSTANT_SQRT1_2, sinA(opposite, hypotenuse), 0.0001);
    ASSERT_NEAR(MATH_CONSTANT_SQRT1_2, cosA(adjacent, hypotenuse), 0.0001);
    ASSERT_NEAR(                  1.0, tanA(opposite, adjacent),   0.0001);
}

