/**
 * @file   RectTest.cpp
 * @brief  Rect class tester.
 * @author zer0
 * @date   2016-04-20
 */

#include <gtest/gtest.h>
#include <libtbag/geometry/Rect.hpp>

using namespace libtbag;
using namespace libtbag::geometry;

TEST(RectTest, Constructor)
{
    Rect r1;
    Rect r2 = { {1, 2}, {3, 4} };
    Rect r3 = r2;
    Rect r4_move = Rect{ {5, 6}, {7, 8} };
    Rect r4 = std::move(r4_move);

    r1 = r2;

    ASSERT_EQ(1, r1.point.x);
    ASSERT_EQ(2, r1.point.y);
    ASSERT_EQ(3, r1.size.w );
    ASSERT_EQ(4, r1.size.h );

    ASSERT_EQ(1, r2.point.x);
    ASSERT_EQ(2, r2.point.y);
    ASSERT_EQ(3, r2.size.w );
    ASSERT_EQ(4, r2.size.h );

    ASSERT_EQ(1, r3.point.x);
    ASSERT_EQ(2, r3.point.y);
    ASSERT_EQ(3, r3.size.w );
    ASSERT_EQ(4, r3.size.h );

    ASSERT_EQ(5, r4.point.x);
    ASSERT_EQ(6, r4.point.y);
    ASSERT_EQ(7, r4.size.w );
    ASSERT_EQ(8, r4.size.h );

    ASSERT_EQ(r1, r2);
    ASSERT_EQ(r2, r3);
    ASSERT_NE(r3, r4);
}

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
    Rect r = { {20, 20}, {-10, -10} };

    ASSERT_EQ(makePoint(10, 10), getLeftTop(r)    );
    ASSERT_EQ(makePoint(20, 10), getRightTop(r)   );
    ASSERT_EQ(makePoint(10, 20), getLeftBottom(r) );
    ASSERT_EQ(makePoint(20, 20), getRightBottom(r));
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
    Rect r = { {20, 20}, {10, 10} };

    Point p1 = {30, 25};
    Point p2 = {25, 25};
    ASSERT_TRUE(checkInside(r, p1));
    ASSERT_TRUE(checkInside(r, p2));

    Point p3 = {10, 25};
    Point p4 = {40, 25};
    Point p5 = {25, 15};
    Point p6 = {25, 35};
    ASSERT_FALSE(checkInside(r, p3));
    ASSERT_FALSE(checkInside(r, p4));
    ASSERT_FALSE(checkInside(r, p5));
    ASSERT_FALSE(checkInside(r, p6));
}

TEST(RectTest, clipRect_true)
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
    ASSERT_TRUE(clipRect(Rect{ {20, 20}, {20, 20} }, Rect{ {30, 30}, {-20, -20} }, &clip));

    Rect predict = { {20, 20}, {10, 10} };
    ASSERT_EQ(predict, clip);
}

TEST(RectTest, clipRect_false)
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
    ASSERT_FALSE(clipRect(Rect{ {10, 20}, {10, 10} }, Rect{ {30, 10}, {10, 10} }));
}

TEST(RectTest, String)
{
    Rect p = { {1, 2}, {3, 4} };
    toString(p);
}

