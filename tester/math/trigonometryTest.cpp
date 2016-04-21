/**
 * @file   trigonometryTest.cpp
 * @brief  trigonometry header tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/math/trigonometry.hpp>
#include <libtbag/math/constant.hpp>

using namespace libtbag;
using namespace libtbag::trigonometry;

TEST(trigonometryTest, Default)
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
    float hypotenuse = MATH_CONSTANT_SQRT2; // h

    ASSERT_NEAR(sinA(opposite, hypotenuse), MATH_CONSTANT_SQRT1_2, 0.0001);
    ASSERT_NEAR(cosA(adjacent, hypotenuse), MATH_CONSTANT_SQRT1_2, 0.0001);
    ASSERT_NEAR(tanA(opposite, adjacent),                     1.0, 0.0001);
}

