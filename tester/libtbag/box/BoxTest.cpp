/**
 * @file   BoxTest.cpp
 * @brief  Box class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/box/Box.hpp>

using namespace libtbag;
using namespace libtbag::box;

TEST(BoxTest, Default)
{
    Box b0;
    ASSERT_TRUE(b0);

    Box b1_move;
    ASSERT_TRUE(b1_move);
    auto const * b1_data = b1_move.get();
    Box b2 = std::move(b1_move);
    ASSERT_TRUE(b2);
    auto const * b2_data = b2.get();
    ASSERT_EQ(b1_data, b2_data);

    Box b3;
    Box b4 = b3;
    ASSERT_TRUE(b3);
    ASSERT_TRUE(b4);
    auto const * b3_data = b3.get();
    auto const * b4_data = b4.get();
    ASSERT_EQ(b3_data, b4_data);

    Box b5;
    ASSERT_TRUE(b5);
    Box b6(nullptr);
    ASSERT_FALSE(b6);
    b6 = b5;
    auto const * b5_data = b5.get();
    auto const * b6_data = b6.get();
    ASSERT_EQ(b5_data, b6_data);
}

TEST(BoxTest, Resize)
{
}

