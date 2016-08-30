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

TEST(vectorTest, getRelativePositionOfPointToLine)
{
    /*
     *    |
     *   2|       * p2
     *    |
     *   1|   *------>*
     *    |   P0      p1
     *   -+------------------
     *    |   1   3   5
     */
    geometry::BasePoint<int> p1{1, 1};
    geometry::BasePoint<int> p2{5, 1};
    geometry::BasePoint<int> p3{3, 2};
    ASSERT_LT(0, getRelativePositionOfPointToLine(p1, p2, p3));

    /*
     *    |
     *   2|   *------>*
     *    |   P0      p1
     *   1|       * p2
     *    |
     *   -+------------------
     *    |   1   3   5
     */
    p1 = geometry::makePoint(1, 2);
    p2 = geometry::makePoint(5, 2);
    p3 = geometry::makePoint(3, 1);
    ASSERT_GT(0, getRelativePositionOfPointToLine(p1, p2, p3));

    /*
     *    |       p2
     *   1|   *---*-->*
     *    |   P0      p1
     *   -+------------------
     *    |   1   3   5
     */
    p1 = geometry::makePoint(1, 1);
    p2 = geometry::makePoint(5, 1);
    p3 = geometry::makePoint(3, 1);
    ASSERT_EQ(0, getRelativePositionOfPointToLine(p1, p2, p3));
}

