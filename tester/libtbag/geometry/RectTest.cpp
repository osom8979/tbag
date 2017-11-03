/**
 * @file   RectTest.cpp
 * @brief  Rect class tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Rect.hpp>

#include <atomic>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(RectTest, Constructor)
{
    Rect r1;
    Rect r2 = {1, 2, 3, 4};
    Rect r3 = r2;
    Rect r4_move = Rect{5, 6, 7, 8};
    Rect r4 = std::move(r4_move);

    r1 = r2;

    ASSERT_EQ(1, r1.x);
    ASSERT_EQ(2, r1.y);
    ASSERT_EQ(3, r1.width);
    ASSERT_EQ(4, r1.height);

    ASSERT_EQ(1, r2.x);
    ASSERT_EQ(2, r2.y);
    ASSERT_EQ(3, r2.width);
    ASSERT_EQ(4, r2.height);

    ASSERT_EQ(1, r3.x);
    ASSERT_EQ(2, r3.y);
    ASSERT_EQ(3, r3.width);
    ASSERT_EQ(4, r3.height);

    ASSERT_EQ(5, r4.x);
    ASSERT_EQ(6, r4.y);
    ASSERT_EQ(7, r4.width);
    ASSERT_EQ(8, r4.height);

    ASSERT_EQ(r1, r2);
    ASSERT_EQ(r2, r3);
    ASSERT_NE(r3, r4);
}

TEST(RectTest, Operators_1)
{
    Point p = {1, 2};
    Size  s = {3, 4};
    Rect  r = {5, 6, 7, 8};

    ASSERT_EQ(Rect(6, 8,  7,  8), r + p);
    ASSERT_EQ(Rect(4, 4,  7,  8), r - p);
    ASSERT_EQ(Rect(5, 6, 10, 12), r + s);
    ASSERT_EQ(Rect(5, 6,  4,  4), r - s);

    Rect temp;
    temp = r; temp += p;
    ASSERT_EQ(Rect(6, 8,  7,  8), temp);
    temp = r; temp -= p;
    ASSERT_EQ(Rect(4, 4,  7,  8), temp);
    temp = r; temp += s;
    ASSERT_EQ(Rect(5, 6, 10, 12), temp);
    temp = r; temp -= s;
    ASSERT_EQ(Rect(5, 6,  4,  4), temp);
}

TEST(RectTest, Operators_2)
{
    /*
     * 0 |  1  2  3  4  5
     * --+--------------------
     *   |
     * 1 |  +-----+
     *   |  |     |
     * 2 |  |  +-----+
     *   |  |  |  |  |
     * 3 |  +- |--+  |
     *   |     |     |
     * 4 |     +-----+
     *   |
     * 5 |
     */
    Rect r1 = {1, 1, 2, 2};
    Rect r2 = {2, 2, 2, 2};

    ASSERT_EQ(Rect(2, 2, 1, 1), r1 & r2);
    ASSERT_EQ(Rect(1, 1, 3, 3), r1 | r2);
}

TEST(RectTest, Utility)
{
    Point p = {10, 20};
    Size  s = {30, 40};
    Rect  r = {p.x, p.y, s.width, s.height};
    int   a = s.width * s.height;

    ASSERT_EQ(p, r.point());
    ASSERT_EQ(s, r.size ());
    ASSERT_EQ(a, r.area ());
};

TEST(RectTest, Methods)
{
    /*
     *  0 |  10  20
     * ---+--------
     *    |
     * 10 |   +---+
     *    |   |   |
     * 20 |   +---+
     */
    Rect r = {20, 20, -10, -10};

    ASSERT_EQ(Point(10, 10), r.getLeftTop());
    ASSERT_EQ(Point(20, 10), r.getRightTop());
    ASSERT_EQ(Point(10, 20), r.getLeftBottom());
    ASSERT_EQ(Point(20, 20), r.getRightBottom());
}

TEST(RectTest, CheckInside)
{
    /*
     *  0 |  10  20  30
     * ---+------------
     *    |
     * 10 |
     *    |
     * 20 |       +---+
     *    |       |   |
     * 30 |       +---+
     */
    Rect r = {20, 20, 10, 10};

    Point p1 = {30, 25};
    Point p2 = {25, 25};
    ASSERT_TRUE(r.contains(p1));
    ASSERT_TRUE(r.contains(p2));
    ASSERT_TRUE(r.contains(Rect(p1, p2)));

    Point p3 = {10, 25};
    Point p4 = {40, 25};
    Point p5 = {25, 15};
    Point p6 = {25, 35};
    ASSERT_FALSE(r.contains(p3));
    ASSERT_FALSE(r.contains(p4));
    ASSERT_FALSE(r.contains(p5));
    ASSERT_FALSE(r.contains(p6));
    ASSERT_FALSE(r.contains(Rect(p1, p3)));
    ASSERT_FALSE(r.contains(Rect(p1, p4)));
    ASSERT_FALSE(r.contains(Rect(p1, p5)));
    ASSERT_FALSE(r.contains(Rect(p1, p6)));
    ASSERT_FALSE(r.contains(Rect(p2, p3)));
    ASSERT_FALSE(r.contains(Rect(p2, p4)));
    ASSERT_FALSE(r.contains(Rect(p2, p5)));
    ASSERT_FALSE(r.contains(Rect(p2, p6)));
}

TEST(RectTest, ClipRect_True)
{
    /*
     *  0 |  10  20  30  40  50
     * ---+--------------------
     *    |
     * 10 |   +-------+
     *    |   |       |
     * 20 |   |   +-------+
     *    |   |   |   |   |
     * 30 |   +---|---+   |
     *    |       |       |
     * 40 |       +-------+
     *    |
     * 50 |
     */
    Rect clip;
    ASSERT_TRUE(Rect(20, 20, 20, 20).clip(Rect(30, 30, -20, -20), &clip));

    Rect predict = {20, 20, 10, 10};
    ASSERT_EQ(predict, clip);
}

TEST(RectTest, ClipRect_False)
{
    /*
     *  0 |  10  20  30  40  50
     * ---+--------------------
     *    |
     * 10 |           +---+
     *    |           |   |
     * 20 |   +---+   +---+
     *    |   |   |
     * 30 |   +---+
     *    |
     * 40 |
     */
    ASSERT_FALSE(Rect(10, 20, 10, 10).clip(Rect(30, 10, 10, 10)));
}

TEST(RectTest, AbsoluteRect)
{
    /*
     *  0 |  10  20
     * ---+--------
     *    |
     * 10 |   +---+
     *    |   |   |
     * 20 |   +---+ <-(x,y)
     */
    Rect r = {20, 20, -10, -10};
    ASSERT_EQ(Rect(10, 10, 10, 10), r.absolute());
    ASSERT_EQ(Rect(10, 10, 10, 10), Rect(10, 10, 10, 10).absolute());
}

TEST(RectTest, String)
{
    Rect p = {1, 2, 3, 4};
    ASSERT_FALSE(p.toString().empty());
}

