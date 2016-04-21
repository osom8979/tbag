/**
 * @file   ColorTest.cpp
 * @brief  Color class tester.
 * @author zer0
 * @date   2016-04-21
 */

#include <gtest/gtest.h>
#include <libtbag/graphic/Color.hpp>

using namespace libtbag;
using namespace libtbag::graphic;

TEST(ColorTest, Constructor)
{
    Color c1;
    Color c2 = {0, 1, 2, 3};
    Color c3 = c2;
    Color c4_move = Color{4, 5, 6, 7};
    Color c4 = std::move(c4_move);

    c1 = Color{8, 9, 10, 11};

    ASSERT_EQ(c1.r, 8);
    ASSERT_EQ(c1.g, 9);
    ASSERT_EQ(c1.b, 10);
    ASSERT_EQ(c1.a, 11);

    ASSERT_EQ(c2, c3);
    ASSERT_NE(c3, c4);
}

TEST(ColorTest, makePoint)
{
    Color c1 = makeColor(4, 5, 6, 7);
    Color c2 = makeColor(4, 5, 6, 8);
    ASSERT_NE(c1, c2);
}

