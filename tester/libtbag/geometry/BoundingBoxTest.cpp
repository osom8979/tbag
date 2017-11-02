/**
 * @file   BoundingBoxTest.cpp
 * @brief  BoundingBox tester.
 * @author zer0
 * @date   2017-09-20
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/BoundingBox.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(BoundingBoxTest, Default)
{
    /*
     *  0 |  10  20  30  40  50
     * ---+--------------------
     *    |
     * 10 |   *-----------+
     *    |   |       *   |
     * 20 |   |           *
     *    |   |           |
     * 30 |   |   *       |
     *    |   |           |
     * 40 |   +-*---------+
     *    |
     * 50 |
     */
    std::vector<Point> points;
    points.push_back(Point(10, 10));
    points.push_back(Point(30, 15));
    points.push_back(Point(40, 20));
    points.push_back(Point(20, 30));
    points.push_back(Point(15, 40));
    auto rect = getBoundingBox(points.data(), points.size());

    ASSERT_EQ(10, rect.x);
    ASSERT_EQ(10, rect.y);
    ASSERT_EQ(30, rect.width);
    ASSERT_EQ(30, rect.height);
}

