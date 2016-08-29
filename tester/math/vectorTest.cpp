/**
 * @file   vectorTest.cpp
 * @brief  vector header tester.
 * @author zer0
 * @date   2016-08-29
 */

#include <gtest/gtest.h>
#include <libtbag/math/vector.hpp>
#include <libtbag/math/trigonometry.hpp>

using namespace libtbag;
using namespace libtbag::math;

TEST(vectorTest, getDotProduct)
{
    Vector2<double> v1{3, 0};
    Vector2<double> v2{3, 4};

    auto r1 = getDotProduct(v1, v2);
    auto r2 = getDotProductWithCosineResult(getScalar(v1), getScalar(v2), cosA(getScalar(v1), getScalar(v2)));

    ASSERT_NEAR(r1, r2, 0.001);
}

TEST(vectorTest, getCrossProduct)
{
    Vector3<int> v1{1, 3,  4};
    Vector3<int> v2{2, 7, -5};
    auto v = getCrossProduct(v1, v2);

    ASSERT_EQ(-43, v.x);
    ASSERT_EQ( 13, v.y);
    ASSERT_EQ(  1, v.z);
}

