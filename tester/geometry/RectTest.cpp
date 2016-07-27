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

    ASSERT_EQ(r1.point.x, 1);
    ASSERT_EQ(r1.point.y, 2);
    ASSERT_EQ(r1.size.w, 3);
    ASSERT_EQ(r1.size.h, 4);

    ASSERT_EQ(r2.point.x, 1);
    ASSERT_EQ(r2.point.y, 2);
    ASSERT_EQ(r2.size.w, 3);
    ASSERT_EQ(r2.size.h, 4);

    ASSERT_EQ(r3.point.x, 1);
    ASSERT_EQ(r3.point.y, 2);
    ASSERT_EQ(r3.size.w, 3);
    ASSERT_EQ(r3.size.h, 4);

    ASSERT_EQ(r4.point.x, 5);
    ASSERT_EQ(r4.point.y, 6);
    ASSERT_EQ(r4.size.w, 7);
    ASSERT_EQ(r4.size.h, 8);

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

    ASSERT_EQ(getLeftTop(r)    , makePoint(10, 10));
    ASSERT_EQ(getRightTop(r)   , makePoint(20, 10));
    ASSERT_EQ(getLeftBottom(r) , makePoint(10, 20));
    ASSERT_EQ(getRightBottom(r), makePoint(20, 20));
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
    ASSERT_EQ(clip, predict);
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
    std::string estimated;

    estimated += __RECT_PREFIX_CHAR;
    estimated += __RECT_STREAM_BRACE_OPEN;
    estimated += "1";
    estimated += __RECT_STREAM_VALUE_SEPARATOR;
    estimated += "2";
    estimated += __RECT_STREAM_VALUE_SEPARATOR;
    estimated += "3";
    estimated += __RECT_STREAM_VALUE_SEPARATOR;
    estimated += "4";
    estimated += __RECT_STREAM_BRACE_CLOSE;

    ASSERT_EQ(toString(p), estimated);
}

////#include <atomic>
//TEST(RectTest, Atomic)
//{
//    std::atomic<Rect> r1;
//    r1.store(Rect{1, 1, 1, 1});
//
//    ASSERT_EQ(r1.load().x, 1);
//    ASSERT_EQ(r1.load().y, 1);
//    ASSERT_EQ(r1.load().w, 1);
//    ASSERT_EQ(r1.load().h, 1);
//
//    Rect r2 = r1;
//
//    ASSERT_EQ(r2.x, 1);
//    ASSERT_EQ(r2.y, 1);
//}

