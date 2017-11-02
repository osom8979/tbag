/**
 * @file   PointTest.cpp
 * @brief  Path class tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/predef.hpp>
#include <libtbag/geometry/Point.hpp>

#include <atomic>
#include <iostream>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(PointTest, Constructor)
{
    int value = 1;

    Point p1;
    Point p2 = {value, 2};
    Point p3 = p2;
    Point p4_move = Point{3, 4};
    Point p4 = p4_move;

    p1 = Point{-1, -2};

    ASSERT_EQ(-1, p1.x);
    ASSERT_EQ(-2, p1.y);
    ASSERT_EQ( 1, p2.x);
    ASSERT_EQ( 2, p2.y);
    ASSERT_EQ( 1, p3.x);
    ASSERT_EQ( 2, p3.y);
    ASSERT_EQ( 3, p4.x);
    ASSERT_EQ( 4, p4.y);

    ASSERT_EQ(p3, p2);
    ASSERT_NE(p4, p3);
}

TEST(PointTest, MakePoint)
{
    int       v1 =  0;
    int const v2 =  1;
    int     & v3 = v1;

    Point p1_1 = Point( 0,  0);
    Point p1_2 = Point( 0, v1);
    Point p1_3 = Point( 0, v2);
    Point p1_4 = Point( 0, v3);

    Point p2_1 = Point(v1,  0);
    Point p2_2 = Point(v1, v1);
    Point p2_3 = Point(v1, v2);
    Point p2_4 = Point(v1, v3);

    Point const p3_1 = Point(v2,  0);
    Point const p3_2 = Point(v2, v1);
    Point const p3_3 = Point(v2, v2);
    Point const p3_4 = Point(v2, v3);

    Point const p4_1 = Point(v3,  0);
    Point const p4_2 = Point(v3, v1);
    Point const p4_3 = Point(v3, v2);
    Point const p4_4 = Point(v3, v3);

    REMOVE_UNUSED_VARIABLE(p1_1 == p1_2);
    REMOVE_UNUSED_VARIABLE(p1_3 == p1_4);
    REMOVE_UNUSED_VARIABLE(p2_1 == p2_2);
    REMOVE_UNUSED_VARIABLE(p2_3 == p2_4);
    REMOVE_UNUSED_VARIABLE(p3_1 == p3_2);
    REMOVE_UNUSED_VARIABLE(p3_3 == p3_4);
    REMOVE_UNUSED_VARIABLE(p4_1 == p4_2);
    REMOVE_UNUSED_VARIABLE(p4_3 == p4_4);
}

TEST(PointTest, Operators_1)
{
    Point       p1 = {0, 1};
    Point const p2 = {4, 5};

    Point p3 = p1 - p2;
    Point p4 = p1 + p2;

    ASSERT_EQ(p3.x, p1.x - p2.x);
    ASSERT_EQ(p3.y, p1.y - p2.y);
    ASSERT_EQ(p4.x, p1.x + p2.x);
    ASSERT_EQ(p4.y, p1.y + p2.y);

    Point p5 = Point(0, 1) - p2;
    Point p6 = Point(0, 1) + p2;

    Point p7 = p1 - Point(4, 5);
    Point p8 = p1 + Point(4, 5);

    ASSERT_EQ(p3, p5);
    ASSERT_EQ(p4, p6);

    ASSERT_EQ(p3, p7);
    ASSERT_EQ(p4, p8);

    // ----------
    // Size test.
    Size       s1 = Size(4, 5);
    Size const s2 = Size(4, 5);
    Size       s3 = {0,  0};
    Size const s4 = {8, 10};

    ASSERT_EQ(p3, p1 - s1);
    ASSERT_EQ(p4, p1 + s2);
    ASSERT_EQ(s3, s1 - s2);
    ASSERT_EQ(s4, s1 + s2);
}

TEST(PointTest, Operators_2)
{
    Point const p1 = {1, 2};
    Size  const s1 = {3, 4};

    Point p2;
    Size  s2;

    p2 = p1; p2 += p1;
    ASSERT_EQ(Point(2, 4), p2);
    p2 = p1; p2 -= p1;
    ASSERT_EQ(Point(0, 0), p2);
    p2 = p1; p2 += s1;
    ASSERT_EQ(Point(4, 6), p2);
    p2 = p1; p2 -= s1;
    ASSERT_EQ(Point(-2, -2), p2);
    s2 = s1; s2 += s1;
    ASSERT_EQ(Size(6, 8), s2);
    s2 = s1; s2 -= s1;
    ASSERT_EQ(Size(0, 0), s2);
}

TEST(PointTest, Equals)
{
    Point       p1 = {1, 2};
    Point const p2 = {1, 2};

    ASSERT_EQ(p1, p2);
    ASSERT_EQ(Point(1, 2), Point(1, 2));

    ASSERT_EQ(p1, Point(1, 2));
    ASSERT_EQ(p2, Point(1, 2));

    ASSERT_EQ(Point(1, 2), p1);
    ASSERT_EQ(Point(1, 2), p2);

    // ----------
    // Size test.
    Size       s1 = {1, 2};
    Size const s2 = {1, 2};
    ASSERT_EQ(s1, s2);
}

TEST(PointTest, String)
{
    Point p = {1, 2};
    std::cout << "Point: " << p << std::endl;

    Size s = {2, 3};
    std::cout << "Size: " << s << std::endl;
}

