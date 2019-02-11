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
    Color c2 = {0, 1, 2};
    Color c3 = c2;

    c1 = Color{8, 9, 10};

    ASSERT_EQ( 8, c1.r);
    ASSERT_EQ( 9, c1.g);
    ASSERT_EQ(10, c1.b);
    ASSERT_EQ(c2, c3);
}

