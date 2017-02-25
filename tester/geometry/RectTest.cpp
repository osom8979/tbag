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

    ASSERT_EQ(makeRect(6, 8,  7,  8), r + p);
    ASSERT_EQ(makeRect(4, 4,  7,  8), r - p);
    ASSERT_EQ(makeRect(5, 6, 10, 12), r + s);
    ASSERT_EQ(makeRect(5, 6,  4,  4), r - s);

    Rect temp;
    temp = r; temp += p;
    ASSERT_EQ(makeRect(6, 8,  7,  8), temp);
    temp = r; temp -= p;
    ASSERT_EQ(makeRect(4, 4,  7,  8), temp);
    temp = r; temp += s;
    ASSERT_EQ(makeRect(5, 6, 10, 12), temp);
    temp = r; temp -= s;
    ASSERT_EQ(makeRect(5, 6,  4,  4), temp);
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

    ASSERT_EQ(makeRect(2, 2,  1,  1), r1 & r2);
    ASSERT_EQ(makeRect(1, 1,  3,  3), r1 | r2);
}

TEST(RectTest, Utility)
{
    Point p = {10, 20};
    Size  s = {30, 40};
    Rect  r = {p.x, p.y, s.width, s.height};
    int   a = s.width * s.height;

    ASSERT_EQ(p, getPoint(r));
    ASSERT_EQ(s, getSize (r));
    ASSERT_EQ(a, getArea (r));
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

    ASSERT_EQ(makePoint(10, 10),     getLeftTop(r));
    ASSERT_EQ(makePoint(20, 10),    getRightTop(r));
    ASSERT_EQ(makePoint(10, 20),  getLeftBottom(r));
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
    Rect r = {20, 20, 10, 10};

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
    ASSERT_TRUE(clipRect(Rect{20, 20, 20, 20}, Rect{30, 30, -20, -20}, &clip));

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
    ASSERT_FALSE(clipRect(Rect{10, 20, 10, 10}, Rect{30, 10, 10, 10}));
}

TEST(RectTest, String)
{
    Rect p = {1, 2, 3, 4};
    toString(p);
}

// Don't use std::atomic<Rect>
// undefined reference to `__atomic_load_16' in GCC 4.9 (Docker-Ubuntu)
//TEST(RectTest, Atomic)
//{
//    std::atomic<Rect> r1;
//    r1.store(Rect{1, 2, 3, 4});
//
//    ASSERT_EQ(1, r1.load().x);
//    ASSERT_EQ(2, r1.load().y);
//    ASSERT_EQ(3, r1.load().width);
//    ASSERT_EQ(4, r1.load().height);
//
//    Rect r2 = r1;
//
//    ASSERT_EQ(1, r2.x);
//    ASSERT_EQ(2, r2.y);
//    ASSERT_EQ(3, r2.width);
//    ASSERT_EQ(4, r2.height);
//}

